#include "Nave.h"
#include <crtdbg.h>
#include <malloc.h> 
#include <time.h>
#include <tlhelp32.h>
#include <excpt.h>

#pragma comment (lib, "dbghelp")

#include "NFException.h"
#include <sstream>

#include "NFFilePtr.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4127)
#pragma warning(disable:4535) // must be compiled with: /EHa

/*
// 기본 에러메시지 박스창을 띄우지 않게 하기 위한창.
SetErrorMode(UINT uMode)

SEM_FAILCRITICALERRORS
“응용프로그램 오류”라고 하는 회색창(-_-)을 보여주지 않는다.

SEM_NOGPFAULTERRORBOX
NOGPFAULT 란다. NO General Protection FAULT 를 줄여놓은것이다. 일반 보호오류가 발생해도 회색창을 보여주지 않는다.

SEM_NOOPENFILEERRORBOX
혹시 본적이 있는지 모르겠지만, 파일을 오픈할 때 없는 파일을 오픈하면 오류가 발생한다. 회색이지만(-_-) “응용프로그램 오류” 같은 창이 아닌 메시지 박스가 뜬다(‘빈 파일에 엑세스할 수 없습니다.’라던가? 그 비슷한 메시지를 보여줬던 것 같다). 그걸 안보이게 해준다.
*/


/*
	Release 모드에서는 예외가 작동하지 않는다.
	예외를 작동하기 위해선 옵션의 최적화 모드를 /Od 로 만들어야 한다.
	/Od가 아닌 디폴트값 /O2 에서도 예외가 가능한 방법을 찾자.
*/
namespace Nave { 

	NFException::MiniDumpTypes NFException::s_eMiniDumpType = NFException::NormalDump;
	BOOL NFException::s_bExceptionSaveDump = FALSE;
	WCHAR NFException::s_szDefaultDumpFileName[128]=L"Nave";
	WCHAR NFException::s_szLogBuffer[MAX_LOG_BUFFER]=L"";
	LPTOP_LEVEL_EXCEPTION_FILTER NFException::s_lpPreviousFilter = NULL;

	BOOL IsNT()
	{	
		OSVERSIONINFO vi = { sizeof(vi)};	
		::GetVersionEx(&vi);
		return vi.dwPlatformId == VER_PLATFORM_WIN32_NT;
	}

	HANDLE SymGetProcessHandle()
	{
		if (IsNT())
			return GetCurrentProcess();
		else
			return (HANDLE)GetCurrentProcessId();
	}

	VOID NFException::MiniDump(LPCWSTR filename, PEXCEPTION_POINTERS ep)
	{
		HANDLE process=SymGetProcessHandle();
		DWORD processId=GetCurrentProcessId();
		MINIDUMP_EXCEPTION_INFORMATION ei;
		ei.ClientPointers=FALSE;
		ei.ThreadId=GetCurrentThreadId();
		ei.ExceptionPointers=ep;

		HANDLE file=CreateFileW(filename, GENERIC_WRITE, FILE_SHARE_READ
			, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file==INVALID_HANDLE_VALUE) return;

		_MINIDUMP_TYPE type=MiniDumpNormal;
		switch(NFException::s_eMiniDumpType)
		{
			case NFException::HeapDump: type=MiniDumpWithFullMemory; break;
		};

		MiniDumpWriteDump(process, processId, file, type, &ei, NULL, NULL);

		CloseHandle(file);
	}

	LPCWSTR NFException::GetExceptionString(DWORD Code)
	{
		switch(Code)
		{
		case EXCEPTION_ACCESS_VIOLATION:		 return L"EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_BREAKPOINT:				 return L"EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:	 return L"EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_SINGLE_STEP:				 return L"EXCEPTION_SINGLE_STEP";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:	 return L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_FLT_DENORMAL_OPERAND:	 return L"EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:		 return L"EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:		 return L"EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:	 return L"EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:			 return L"EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:			 return L"EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:			 return L"EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:		 return L"EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:			 return L"EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_PRIV_INSTRUCTION:		 return L"EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return L"EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_STACK_OVERFLOW:			 return L"EXCEPTION_STACK_OVERFLOW";
		}
		return L"INVALID";
	}

	VOID NFException::SetProgramName(WCHAR* pszOutBuffer, int nBufferSize, WCHAR* pszProgramName)
	{
		if(0 == pszProgramName)
		{
			WCHAR szDrive[MAX_PATH], szDir[MAX_PATH], szFilename[MAX_PATH], szExt[MAX_PATH];

			// Figure out what the report file will be named, and store it away
			GetModuleFileNameW(0, pszOutBuffer, nBufferSize);

			LPWSTR pszDot = pszOutBuffer;
		    
			// Look for the '.' before the "EXE" extension. Replace '.' to '\0'
			if((pszDot = wcsrchr( pszDot, L'.'))) 
			{
				*pszDot = 0;
			}

			_wsplitpath(pszOutBuffer, szDrive, szDir, szFilename, szExt);
			wcsncpy(pszOutBuffer, szFilename, nBufferSize);
		}
		else
		{
			wcsncpy(pszOutBuffer, pszProgramName, nBufferSize);
		}
	}

	BOOL NFException::GetLogicalAddress(PVOID addr, LPWSTR szModule, 
											 unsigned long len, 
											 unsigned long& section, 
											 unsigned long& offset )
	{
		MEMORY_BASIC_INFORMATION mbi;

		if(!VirtualQuery(addr, &mbi, sizeof(mbi)))
		{
			return FALSE;
		}

		PVOID hMod = mbi.AllocationBase;
		if(NULL == hMod) 
		{
			return FALSE;
		}

		if(!GetModuleFileNameW((HMODULE) hMod, szModule, len))
		{
			return FALSE;
		}

		// Point to the DOS header in memory
		PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER) hMod;

		// From the DOS header, find the NT (PE) header
		PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(((DWORD64) hMod) + pDosHdr->e_lfanew);

		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

		DWORD64 rva = (DWORD64)addr - (DWORD64)hMod; // RVA is offset from module load address

		// Iterate through the section table, looking for the one that encompasses
		// the linear address.
		for( unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++ ) 
		{
			unsigned long sectionStart = pSection->VirtualAddress;
			unsigned long sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

			// Is the address in this section???
			if ( (rva >= sectionStart) && (rva <= sectionEnd) ) 
			{
				// Yes, address is in the section.  Calculate section and offset,
				// and store in the "section" & "offset" params, which were
				// passed by reference.
				section = i+1;
				offset = (unsigned long) (rva - sectionStart);
				return TRUE;
			}
		}

		return FALSE;   // Should never get here!
	}

	NFException::BasicType NFException::GetBasicType(unsigned long typeIndex, DWORD64 modBase)
	{
		BasicType basicType;

		if(SymGetTypeInfo(GetCurrentProcess(), 
			modBase, typeIndex, TI_GET_BASETYPE, &basicType))
		{
			return basicType;
		}

		// Get the real "TypeId" of the child.  We need this for the
		// SymGetTypeInfo( TI_GET_TYPEID ) call below.
		unsigned long typeId = 0;

		if(SymGetTypeInfo(GetCurrentProcess(), 
			modBase, typeIndex, TI_GET_TYPEID, &typeId)) 
		{
			if(SymGetTypeInfo( GetCurrentProcess(), 
				modBase, typeId, TI_GET_BASETYPE, &basicType))
			{
				return basicType;
			}
		}

		return btNoType;
	}

	int NFException::FormatOutputValue(WCHAR* szBuffer_Out, 
											const int nBufferSize,
											BasicType basicType, 
											DWORD64 length, PVOID pAddress)
	{
		int nLength = 0;
		int nTotalLength = 0;

		// Format appropriately (assuming it's a 1, 2, or 4 bytes (!!!)
		if(length == 1)
		{
			nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %X", *(PBYTE)pAddress);
		}
		else if(length == 2)
		{
			nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %X", *(PWORD)pAddress);
		}
		else if(length == 4) 
		{
			if(basicType == btFloat) 
			{
				nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %f", *(PFLOAT)pAddress);
			}
			else if(basicType == btChar) 
			{
				if(!IsBadStringPtrW(*(LPWSTR*)pAddress, 32)) 
				{
					nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = \"%.31s\"", *(PDWORD)pAddress);
				}
				else
				{
					nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %X", *(PDWORD)pAddress);
				}
			} 
			else
			{
				nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %X", *(PDWORD)pAddress);
			}
		}
		else if (length == 8) 
		{
			if(basicType == btFloat)
			{
				nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = %lf", *(double *)pAddress );
			}
			else
			{	
				WCHAR szAddress[64] = L"";
				_ui64tow(*(DWORD64*)pAddress, szAddress, 16);

				nLength = _snwprintf(szBuffer_Out, nBufferSize, L" = 0x%s", szAddress);
			}
		}

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		return nTotalLength;
	}

	int NFException::DumpTypeIndex(WCHAR* szBuffer_Out, 
										const int nBufferSize,
										DWORD64 modBase, 
										unsigned long dwTypeIndex, 
										unsigned int nestingLevel, 
										DWORD_PTR offset, 
										BOOL & bHandled)
	{
		bHandled = FALSE;

		int nTotalLength = 0;
		int nLength = 0;

		// Get the name of the symbol.  This will either be a Type name (if a UDT), or the structure member name.
		WCHAR* pwszTypeName = 0;

		if(SymGetTypeInfo(GetCurrentProcess(), 
			modBase, dwTypeIndex, TI_GET_SYMNAME, &pwszTypeName))
		{
			nLength += _snwprintf(szBuffer_Out, nBufferSize, L" %ls", pwszTypeName);
			LocalFree(pwszTypeName);

			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		}

		// Determine how many children this type has.
		unsigned long dwChildrenCount = 0;
		SymGetTypeInfo(GetCurrentProcess(), modBase, 
			dwTypeIndex, TI_GET_CHILDRENCOUNT, &dwChildrenCount);

		if(!dwChildrenCount)     // If no children, we're done
		{
			return nTotalLength;
		}

		// Prepare to get an array of "TypeIds", representing each of the children.
		// SymGetTypeInfo(TI_FINDCHILDREN) expects more memory than just a
		// TI_FINDCHILDREN_PARAMS struct has.  Use derivation to accomplish this.
		struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS 
		{
			ULONG   MoreChildIds[1024];
			FINDCHILDREN() { Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]); }
		} children;

		children.Count = dwChildrenCount;
		children.Start = 0;

		// Get the array of TypeIds, one for each child type
		if(!SymGetTypeInfo(GetCurrentProcess(), 
			modBase, dwTypeIndex, TI_FINDCHILDREN, &children))
		{
			return nTotalLength;
		}

		// Append a line feed
		nLength += _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n");
		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		// Iterate through each of the children
		for(unsigned int nChildrenCount = 0; nChildrenCount < dwChildrenCount; ++nChildrenCount) 
		{
			// Add appropriate indentation level (since this routine is recursive)
			for( unsigned int j = 0; j <= nestingLevel+1; j++ )
			{
				szBuffer_Out += swprintf( szBuffer_Out, L"\t" );
			}

			// Recurse for each of the child types
			BOOL bHandled2;

			nLength = DumpTypeIndex(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, 
				modBase, children.ChildId[nChildrenCount], nestingLevel+1, offset, bHandled2);
			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

			// If the child wasn't a UDT, format it appropriately
			if(!bHandled2)
			{
				// Get the offset of the child member, relative to its parent
				unsigned long dwMemberOffset = 0;
				SymGetTypeInfo( GetCurrentProcess(), 
					modBase, children.ChildId[nChildrenCount], TI_GET_OFFSET, &dwMemberOffset);

				// Get the real "TypeId" of the child.  We need this for the SymGetTypeInfo( TI_GET_TYPEID ) call below.
				unsigned long typeId = 0;
				SymGetTypeInfo( GetCurrentProcess(), 
					modBase, children.ChildId[nChildrenCount], TI_GET_TYPEID, &typeId);

				// Get the size of the child member
				ULONG64 length = 0;
				SymGetTypeInfo(GetCurrentProcess(), modBase, 
					typeId, TI_GET_LENGTH, &length);

				// Calculate the address of the member
				DWORD_PTR dwFinalOffset = offset + dwMemberOffset;

				BasicType basicType = GetBasicType(children.ChildId[nChildrenCount], modBase );

				nLength = FormatOutputValue(szBuffer_Out + nTotalLength, 
					nBufferSize - nTotalLength, basicType, length, (PVOID)dwFinalOffset);
				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

				nLength = _snwprintf(szBuffer_Out + nTotalLength, 
					nBufferSize - nTotalLength, L"\r\n");

				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
			}
		}

		bHandled = TRUE;
		return nTotalLength;
	}

	int NFException::FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME* sf, WCHAR* pszBuffer, unsigned int nBufferSize)
	{
		int nLength = 0;
		int nTotalLength = 0;

		// If it's a function, don't do anything.
		if(pSym->Tag == SymTagFunction)
		{
			return 0;
		}

		// Indicate if the variable is a local or parameter
		if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER)
		{
			nLength = _snwprintf(pszBuffer, nBufferSize, L"Parameter ");
			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		}
		else if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL)
		{
			nLength = _snwprintf(pszBuffer, nBufferSize, L"Local ");
			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		}

		// Emit the variable name
		nLength = _snwprintf(pszBuffer + nTotalLength, nBufferSize - nTotalLength, L"\'%s\'", pSym->Name);
		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

	    
		DWORD_PTR pVariable = 0;    // Will point to the variable's data in memory

		if(pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE) 
		{
			pVariable = sf->AddrFrame.Offset;
			pVariable += (DWORD_PTR)pSym->Address;
		}
		else if (pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER) 
		{
			// Don't try to report register variable
			return 0;
		}
		else 
		{
			// It must be a global variable
			pVariable = (DWORD_PTR)pSym->Address;
		}

		// Determine if the variable is a user defined type (UDT).  IF so, bHandled
		// will return TRUE.
		BOOL bHandled;

		nLength = DumpTypeIndex(pszBuffer + nTotalLength, nBufferSize - nTotalLength,
			pSym->ModBase, pSym->TypeIndex, 0, pVariable, bHandled);

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
	    
		if(!bHandled) 
		{
			// The symbol wasn't a UDT, so do basic, stupid formatting of the
			// variable.  Based on the size, we're assuming it's a WCHAR, WORD, or
			// unsigned long.
			BasicType basicType = GetBasicType(pSym->TypeIndex, pSym->ModBase);

			nLength = FormatOutputValue(pszBuffer + nTotalLength, nBufferSize - nTotalLength,
				basicType, pSym->Size, (PVOID) pVariable);

			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		}

		return nTotalLength;
	}

	BOOL CALLBACK NFException::EnumerateSymbolsCallback(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
	{
		const int MAX_NAME_BUFFER = 3072;
		WCHAR szBuffer[MAX_NAME_BUFFER];

		SymEnumSymbols_UserData* lpSymbol_UserData = 
			reinterpret_cast<SymEnumSymbols_UserData*>(UserContext);

		__try 
		{
			if(0 < FormatSymbolValue(pSymInfo, lpSymbol_UserData->m_lpsf, szBuffer, MAX_NAME_BUFFER))
			{
				lpSymbol_UserData->m_nWritten_Out = _snwprintf(
					lpSymbol_UserData->m_szBuffer_Out, 
					lpSymbol_UserData->m_nBufferSize_In,
					L"\t%s\r\n", szBuffer);
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			lpSymbol_UserData->m_nWritten_Out = _snwprintf(
				lpSymbol_UserData->m_szBuffer_Out, 
				lpSymbol_UserData->m_nBufferSize_In,			
				L"punting on symbol %s\r\n", pSymInfo->Name);
		}

		return TRUE;
	}

	int NFException::WriteBasicInfo(WCHAR* szBuffer_Out, const int nBufferSize, PEXCEPTION_RECORD pExceptionRecord)
	{
		WCHAR szProgramName[MAX_PATH];
		WCHAR szFileName[MAX_PATH];
		WCHAR szUserName[MAX_PATH];
		WCHAR szComputerName[MAX_PATH];
		WCHAR szModuleName[MAX_PATH];

		unsigned long dwUserLen = MAX_PATH;
		unsigned long dwComputerLen = MAX_PATH;
		unsigned long dwSection = 0;
		unsigned long dwOffset = 0;
		
		SetProgramName(szProgramName, MAX_PATH, 0);
		GetModuleFileNameW((HMODULE)0, szFileName, MAX_PATH);	
		GetUserNameW(szUserName, &dwUserLen);
		GetComputerNameW(szComputerName, &dwComputerLen);

		// Print information about the type of fault and where the fault occured    
		GetLogicalAddress(pExceptionRecord->ExceptionAddress, 
			szModuleName, sizeof(szModuleName), dwSection, dwOffset);

		return _snwprintf(szBuffer_Out, nBufferSize, 
			L"------------------------------------------------------------------------------\r\n"
			L"    Basic Information\r\n"
			L"------------------------------------------------------------------------------\r\n"
			L"\r\n"
			L"Program Name : %s\r\n"
			L"EXE          : %s\r\n"
			L"User         : %s\r\n"
			L"Computer     : %s\r\n"
			L"\r\n"
			L"Program      : %s\r\n"
			L"Exception    : %08x (%s)\r\n"
			L"Fault Address: %08x %02X:%08x\r\n\r\n\r\n",

			szProgramName,
			szFileName,
			szUserName,
			szComputerName,
			szModuleName,
			pExceptionRecord->ExceptionCode,
			GetExceptionString(pExceptionRecord->ExceptionCode),
			pExceptionRecord->ExceptionAddress,
			dwSection,
			dwOffset);
	}

	int NFException::WriteRegistersInfo(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext)
	{
		return _snwprintf(szBuffer_Out, nBufferSize,
		    
			L"------------------------------------------------------------------------------\r\n"
			L"    x86 Registers\r\n"
			L"------------------------------------------------------------------------------\r\n"
			L"\r\n"
			L"EAX=%08x  EBX=%08x  ECX=%08x  EDX=%08x\r\n"
			L"ESI=%08x  EDI=%08x  EBP=%08x\r\n"
			L"DS =%04X      ES=%04X       FS=%04X       GS:%04X\r\n"
			L"CS:EIP=%04X:%08x\r\n"
			L"SS:ESP=%04X:%08x\r\n"
			L"Flags=%08x\r\n"
			L"\r\n\r\n",

			pContext->Eax, pContext->Ebx, pContext->Ecx, pContext->Edx,
			pContext->Esi, pContext->Edi, pContext->Ebp, 
			pContext->SegDs, pContext->SegEs, pContext->SegFs, pContext->SegGs,
			pContext->SegCs, pContext->Eip,
			pContext->SegSs, pContext->Esp,
			pContext->EFlags);
	}

	int NFException::WriteStackDetails(WCHAR* szBuffer_Out, 
										const int nBufferSize, 
										PCONTEXT pContext, 
										BOOL bWriteVariables, /* TRUE if local/params should be output */
										BOOL& bHasSymbol_Out,
										const int nStackDepth)  
	{
		int nTotalLength = 0;
		int nLength = _snwprintf(szBuffer_Out, nBufferSize, 
			L"------------------------------------------------------------------------------\r\n"
			L"    Call Stack (%s)\r\n"
			L"------------------------------------------------------------------------------\r\n"
			L"\r\n" 
			L"Address   Frame     Function                        SourceFile\r\n", 
			bWriteVariables ? L"Detail" : L"Short");

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		unsigned long dwMachineType = 0;
	    
		// Could use SymSetOptions here to add the SYMOPT_DEFERRED_LOADS flag
		STACKFRAME sf;
		memset( &sf, 0, sizeof(sf) );

		// Initialize the STACKFRAME structure for the first call.  This is only
		// necessary for Intel CPUs, and isn't mentioned in the documentation.
		sf.AddrPC.Offset       = pContext->Eip;
		sf.AddrPC.Mode         = AddrModeFlat;
		sf.AddrStack.Offset    = pContext->Esp;
		sf.AddrStack.Mode      = AddrModeFlat;
		sf.AddrFrame.Offset    = pContext->Ebp;
		sf.AddrFrame.Mode      = AddrModeFlat;

		dwMachineType = IMAGE_FILE_MACHINE_I386;

		const int MAX_SYMBOL_NAME_LEN = 2048;
		BYTE symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYMBOL_NAME_LEN];
		ZeroMemory(symbolBuffer, sizeof(SYMBOL_INFO) + MAX_SYMBOL_NAME_LEN);
		WCHAR szModule[MAX_PATH] = L"";
		IMAGEHLP_LINE lineInfo;
		IMAGEHLP_STACK_FRAME imagehlpStackFrame;
		Nave::String strTemp;

		SymEnumSymbols_UserData symbol_userdata;

		for(int nCount = 0; nCount < nStackDepth; ++nCount)
		{
			if(0 == sf.AddrFrame.Offset || 
				!StackWalk(dwMachineType, GetCurrentProcess(), GetCurrentThread(), &sf, pContext, 0, SymFunctionTableAccess, SymGetModuleBase, 0))
			{
				break;
			}

			nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, 
				L"%08x  %08x  ", sf.AddrPC.Offset, sf.AddrFrame.Offset);

			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

			// Get the name of the function for this stack frame entry
			PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
			pSymbol->SizeOfStruct = sizeof(symbolBuffer);
			pSymbol->MaxNameLen = MAX_SYMBOL_NAME_LEN;
	                        
			DWORD64 symDisplacement = 0;    // Displacement of the input address,
											// relative to the start of the symbol

			if(SymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &symDisplacement, pSymbol))
			{
				int iSize = MultiByteToWideChar(CP_ACP,0,pSymbol->Name,-1,NULL,0);
				std::wstring wstr(iSize-1, 0);
				::MultiByteToWideChar(CP_ACP, 0, pSymbol->Name, -1, &wstr[0], iSize);

				swprintf(szModule, L"%-s+0x%p", wstr.c_str(), (void*)symDisplacement);

				nLength = _snwprintf(szBuffer_Out + nTotalLength, 
					nBufferSize - nTotalLength, 
					L"%-30s", szModule);				
			} 
			else 
			{
				// No symbol found.  Print out the logical address instead.				
				unsigned long section = 0, offset = 0;

				GetLogicalAddress((PVOID)(DWORD64)sf.AddrPC.Offset, szModule, sizeof(szModule), section, offset);

				nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, 
					L"%04X:%08x                   %s", section, offset, szModule);
			}

			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

			// Get the source line for this stack frame entry

			memset(&lineInfo, 0, sizeof(IMAGEHLP_LINE));
			lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			unsigned long dwLineDisplacement = 0;
			if(SymGetLineFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo))
			{
				int iSize = MultiByteToWideChar(CP_ACP,0,lineInfo.FileName,-1,NULL,0);
				std::wstring wstr(iSize-1, 0);
				::MultiByteToWideChar(CP_ACP, 0, lineInfo.FileName, -1, &wstr[0], iSize);

				nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, 
					L"  %s line %u",wstr.c_str(),lineInfo.LineNumber); 
				
				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
				
				if(bHasSymbol_Out == FALSE)
				{
					bHasSymbol_Out = TRUE;
				}
			}

			nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n"); 
			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

			// Write out the variables, if desired
			if(bWriteVariables) 
			{
				// Use SymSetContext to get just the locals/params for this frame				
				memset(&imagehlpStackFrame, 0, sizeof(IMAGEHLP_STACK_FRAME));

				imagehlpStackFrame.InstructionOffset = sf.AddrPC.Offset;

				SymSetContext(GetCurrentProcess(), &imagehlpStackFrame, 0);

				// Enumerate the locals/parameters
				symbol_userdata.m_lpsf			= &sf;
				symbol_userdata.m_szBuffer_Out	= szBuffer_Out + nTotalLength;
				symbol_userdata.m_nWritten_Out	= 0;
				symbol_userdata.m_nBufferSize_In = nBufferSize - nTotalLength;

				SymEnumSymbols(GetCurrentProcess(), 0, 0, EnumerateSymbolsCallback, &symbol_userdata);

				CHECK_LENGTH_RETURN(symbol_userdata.m_nWritten_Out, nTotalLength, -1);					

				nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n"); 
				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
			}
		}
		
		nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n\r\n");
		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		return nTotalLength;
	}

	int NFException::WriteMemoryDump(WCHAR* szBuffer_Out, 
										  const int nBufferSize, 
										  PCONTEXT pContext,
										  unsigned int nMaxIPDump,
										  unsigned int nMaxStackDump)
	{
		int nTotalLength = 0;
		int nLength = _snwprintf(szBuffer_Out, nBufferSize,
	        
			L"------------------------------------------------------------------------------\r\n"
			L"    Memory Dump\r\n"
			L"------------------------------------------------------------------------------\r\n"
			L"\r\n" 
			L"Code: %d bytes starting at (EIP = %08lX)\r\n", nMaxIPDump, pContext->Eip);

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		nLength = Dump(szBuffer_Out + nTotalLength, 
			nBufferSize - nTotalLength, 
			pContext->Eip, nMaxIPDump, FALSE);

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		
		
		nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength,
			L"\r\nStack: %d bytes starting at (ESP = %08lX)\r\n", nMaxStackDump, pContext->Esp);

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		
		nLength = Dump(szBuffer_Out + nTotalLength, 
			nBufferSize - nTotalLength, 
			pContext->Esp, nMaxStackDump, TRUE);

		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		
		nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n\r\n");
		CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

		return nTotalLength;
	}

	int NFException::Dump(WCHAR* szBuffer_Out, const int nBufferSize, DWORD64 dw64Offset, unsigned long dwSize, BOOL bAlign )
	{
		int nTotalLength = 0;
		int nLength = 0;

		__try
		{
			unsigned long dwLoc = 0, dwILoc = 0, dwX = 0;
			
			LPBYTE pOut = (LPBYTE) dw64Offset;
			
			if(bAlign == TRUE)
			{
				pOut = (LPBYTE) ((dw64Offset >> SHIFT_NUM) << SHIFT_NUM);
			}

			for( dwLoc = 0; dwLoc < dwSize; dwLoc += 16, pOut += BYTES_PER_LINE ) 
			{
				LPBYTE pLine = pOut;
				
				nLength = _snwprintf(szBuffer_Out + nTotalLength, 
					nBufferSize - nTotalLength,
					L"%08lX: " , (DWORD64) pOut);

				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

				for(dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++)
				{
					if(dwX == (BYTES_PER_LINE / 2))
					{
						nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"   ");
					}
					if( dwILoc++ > dwSize ) 
					{
						nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"?? ");
					}
					else 
					{
						nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"%02X ", *(pLine++));
					}

					CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
				}

				pLine = pOut;

				nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L" ");

				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);

				for( dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++ )
				{
					if( dwILoc++ > dwSize )
					{
						nLength = _snwprintf(szBuffer_Out + nTotalLength, 
							nBufferSize - nTotalLength, L" ");				
					} 
					else
					{
						nLength = _snwprintf(szBuffer_Out + nTotalLength, 
							nBufferSize - nTotalLength, L"%c", isprint(*pLine) ? *pLine : L'.');

						pLine++;
					}

					CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
				}

				nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, L"\r\n");

				CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nLength = _snwprintf(szBuffer_Out + nTotalLength, nBufferSize - nTotalLength, 
				L"\r\n Dump failed by NFException : %d - %s\r\n",
				GetExceptionCode(), GetExceptionString(GetExceptionCode()));

			CHECK_LENGTH_RETURN(nLength, nTotalLength, -1);
		}

		return nTotalLength;
	}

	VOID NFException::WriteExceptionReport(FILE* fp, PEXCEPTION_POINTERS pExceptionInfo)
	{
		if(0 == fp)
			return;

		fwprintf(fp, L"==============================================================================\r\n\r\n");

		if(WriteBasicInfo(s_szLogBuffer, MAX_LOG_BUFFER, pExceptionInfo->ExceptionRecord) < 0)
		{
			_snwprintf(s_szLogBuffer, MAX_LOG_BUFFER, L"Failed to write basic info.\r\n\r\n");
		}

		fputws(s_szLogBuffer, fp);
		fflush(fp);

		if(WriteRegistersInfo(s_szLogBuffer, MAX_LOG_BUFFER, pExceptionInfo->ContextRecord) < 0)
		{
			_snwprintf(s_szLogBuffer, MAX_LOG_BUFFER, L"Failed to write registers info.\r\n\r\n");
		}

		fputws(s_szLogBuffer, fp);
		fflush(fp);

		if(!SymInitialize(GetCurrentProcess(), 0, TRUE))
		{        
			return;
		}

		CONTEXT	tempContext;
		tempContext = *(pExceptionInfo->ContextRecord);

		BOOL bHasSymbol = FALSE;

		if(WriteStackDetails(s_szLogBuffer, MAX_LOG_BUFFER, &tempContext, FALSE, bHasSymbol) < 0)
		{
			_snwprintf(s_szLogBuffer, MAX_LOG_BUFFER, L"Failed to write stack details.\r\n\r\n");
		}

		fputws(s_szLogBuffer, fp);
		fflush(fp);

		SymCleanup(GetCurrentProcess());

		fputws(L"\r\n\r\n\r\n", fp);

		if(WriteMemoryDump(s_szLogBuffer, MAX_LOG_BUFFER, pExceptionInfo->ContextRecord) < 0)
		{
			_snwprintf(s_szLogBuffer, MAX_LOG_BUFFER, L"Failed to write memory dump.\r\n\r\n");
		}

		fputws(s_szLogBuffer, fp);
		fflush(fp);
	}


	LONG __stdcall UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo) 
	{
		if(NFException::s_bExceptionSaveDump)
		{
			// 예외를 호출하기 전에 미니 덤프파일을 생성한다.
			WCHAR dumpfile[512];

			SYSTEMTIME	st;
			GetLocalTime(&st);

			swprintf(dumpfile, L"%s_%04d%02d%02d-%02d%02d%02d.dmp",
				NFException::s_szDefaultDumpFileName, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			// 이 덤프 생성루틴을 Execption 에서 옵션으로 변경할수 있도록 변경.
			// 미니덤프 생성.
			NFException::MiniDump(dumpfile, pExceptionInfo);

			// 기타 정보 출력
			swprintf(dumpfile, L"%s_%04d%02d%02d-%02d%02d%02d.txt",
				NFException::s_szDefaultDumpFileName, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			NFFilePtrW fp(dumpfile, L"wt");

			NFException::WriteExceptionReport(fp, pExceptionInfo);
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}

	VOID NFException::SetMiniDumpType(MiniDumpTypes type)
	{
		NFException::s_eMiniDumpType=type;
	}

	VOID NFException::EnableSaveDump(BOOL enable)
	{
		NFException::s_bExceptionSaveDump = enable;
	}

	VOID NFException::EnableUnhandledExceptioinFilter(BOOL enable)
	{
		if(s_lpPreviousFilter)
		{
			::SetUnhandledExceptionFilter(s_lpPreviousFilter);
			s_lpPreviousFilter = NULL;
		}

		// try~catch 에 정의 되지 않은 예외오류를 검사하기 위한 
		// 오류처리루틴이다.
		if(enable)
			s_lpPreviousFilter = ::SetUnhandledExceptionFilter(UnhandledExceptionFilter);
		else
		{
			::SetUnhandledExceptionFilter(s_lpPreviousFilter);
			s_lpPreviousFilter = NULL;
		}
	}

	VOID NFException::MiniDump(LPCWSTR filename)
	{
		HANDLE process=SymGetProcessHandle();
		DWORD processId=GetCurrentProcessId();
		MINIDUMP_EXCEPTION_INFORMATION ei;
//		PEXCEPTION_POINTERS ep = GetExceptionInformation();
		ei.ClientPointers = false;
		ei.ThreadId = GetCurrentThreadId();
		ei.ExceptionPointers = NULL;

		HANDLE file=CreateFileW(filename, GENERIC_WRITE, FILE_SHARE_READ
			, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file==INVALID_HANDLE_VALUE) return;

		_MINIDUMP_TYPE type=MiniDumpNormal;
		switch(NFException::s_eMiniDumpType)
		{
			case HeapDump: type=MiniDumpWithFullMemory; break;
		};

		MiniDumpWriteDump(process, processId, file, type, NULL, NULL, NULL);
		CloseHandle(file);
	}


	NFException::NFException( int number, const Nave::String& description, const Nave::String& source )
						: m_iLine(0), m_iNumber(number), m_szDescription(description), m_szSource(source)
	{
	}

	NFException::NFException( int number, const Nave::String& description, const Nave::String& source, const WCHAR* file, int line )
					: m_iLine(line), m_iNumber(number), m_szDescription(description), m_szSource(source)
	{
		m_szFile = file;
	}

	NFException::NFException( const NFException& rhs )
		: m_iLine( rhs.m_iLine ), m_iNumber( rhs.m_iNumber ),
		m_szDescription( rhs.m_szDescription ), m_szSource( rhs.m_szSource ),
		m_szFile( rhs.m_szFile )
	{
	}


	void NFException::operator =( const NFException& rhs )
	{
		m_szDescription = rhs.m_szDescription;
		m_iNumber       = rhs.m_iNumber;
		m_szSource      = rhs.m_szSource;
		m_szFile        = rhs.m_szFile;
		m_iLine         = rhs.m_iLine;
	}


	const Nave::String NFException::GetFullDescription() const
	{
		std::wstringstream desc;

		desc << L"An exception has been thrown!\n\n"
			L"-----------------------------------\nDetails:\n"
			L"-----------------------------------\n"
			L"Error No.  : " << m_iNumber;
			

		if( m_iLine > 0 )
		{
			desc << L"\nFile       : " << m_szFile;
			desc << L"\nLine       : " << m_iLine;
		}

		desc << L"\nFunction   : " << m_szSource
			<< L"\nDescription: " << m_szDescription;

		return desc.str();
	}



}

#pragma warning(pop)
