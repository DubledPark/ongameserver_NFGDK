/** 
 *  @file		NFException.h
 *  @brief		Exception ���� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <string>
#include <ostream>
#include <DbgHelp.h>

/// @warning __FUNCTION__�� ��ǥ��. Microsoft ȯ�濡���� ��� �����ϴ�
#ifdef _MSC_VER
	/// ���ܸ� ���ϰ� ó���ѵ��� �����ִ� ��ũ��
#define EXCEPTION( num, desc ) \
	throw( Nave::NFException( num, desc, __FUNCTION__, __FILE__, __LINE__ ) )
#else
	/// ���ܸ� ���ϰ� ó���ѵ��� �����ִ� ��ũ��
#define EXCEPTION( num, desc, func ) \	
	throw( Nave::NFException( num, desc, func, __FILE__, __LINE__ ) )
#endif

namespace Nave {

	/** 
	 *  @class        NFException
	 *  @brief        ���ܰ� �߻������� ���ܸ� ó���ϱ� ���� Ŭ����
	 *  @remarks      �ڵ� ���� ����														\r\n
	 *                NFException::EnableUnhandledExceptioinFilter(true);					\r\n
	 *                NFException::EnableSaveDump(true);									\r\n
	 *																						\r\n
	 *                ���� ���� ����														\r\n
	 *                try																	\r\n
	 *                {																		\r\n
	 *		            EXCEPTION( ERR_INIT_FAILED, "System init failed. What the hell!" );	\r\n
	 *                }																		\r\n
	 *                catch( Nave::NFException& ex )										\r\n
	 *                {																		\r\n
	 *					// ���� ó�� �ڵ�. �Ʒ��� ���� �� ���ڿ��� ȭ�鿡 ����ϴ� ��		\r\n
	 *					cout << ex.GetFullDescription() << endl;							\r\n
	 *                }																		\r\n
	 *                catch( ... )															\r\n
	 *                {																		\r\n
	 *					// ��Ÿ. �ٸ� ���� ó��. �ʿ����� ���� ���� ����					\r\n
	 *                }																		\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFException
	{
	public:
		/// ���̰� ��� Define��
		#define CHECK_LENGTH_RETURN(length, total_length, retval) \
			if(length < 0) { return retval; } else { total_length += length; }

		enum {
			SHIFT_NUM = 4,				/// �α� ��½� ������ SHIFT ��
			BYTES_PER_LINE = 16, 		/// �α� ��½� ����� ��

			MAX_BUFFER =1024,			/// �α� ��½� ����� ����ũ��
			MAX_LOG_BUFFER = 65535		/// �α� ��½� ����� 64-kb log buffer
		};

		enum MiniDumpTypes 
		{ 
			NormalDump=0,				/// �Ϲ� ����
			HeapDump					/// �ִ� ����
		};

		/// Call Stack ��¿� ����ü
		struct SymEnumSymbols_UserData
		{
			STACKFRAME* m_lpsf;
			WCHAR*		m_szBuffer_Out;
			int			m_nBufferSize_In;
			int			m_nWritten_Out;
		};

		/// Stolen from CVCONST.H in the DIA 2.0 SDK
		enum BasicType 
		{
			btNoType	= 0,
			btVoid		= 1,
			btChar		= 2,
			btWChar		= 3,
			btInt		= 6,
			btUInt		= 7,
			btFloat		= 8,
			btBCD		= 9,
			btBool		= 10,
			btLong		= 13,
			btULong		= 14,
			btCurrency	= 25,
			btDate		= 26,
			btVariant	= 27,
			btComplex	= 28,
			btBit		= 29,
			btBSTR		= 30,
			btHresult	= 31
		};

		/// stolen from dia sdk 2.0 (cvconst.h)
		enum SymTagEnum
		{
			SymTagNull,
			SymTagExe,
			SymTagCompiland,
			SymTagCompilandDetails,
			SymTagCompilandEnv,
			SymTagFunction,
			SymTagBlock,
			SymTagData,
			SymTagAnnotation,
			SymTagLabel,
			SymTagPublicSymbol,
			SymTagUDT,
			SymTagEnum,
			SymTagFunctionType,
			SymTagPointerType,
			SymTagArrayType,
			SymTagBaseType,
			SymTagTypedef,
			SymTagBaseClass,
			SymTagFriend,
			SymTagFunctionArgType,
			SymTagFuncDebugStart,
			SymTagFuncDebugEnd,
			SymTagUsingNamespace,
			SymTagVTableShape,
			SymTagVTable,
			SymTagCustom,
			SymTagThunk,
			SymTagCustomType,
			SymTagManagedType,
			SymTagDimension,
			SymTagMax
		};

		/**
		 * @brief	������ �������ڿ� ���ϱ�
		 * @param Code ���� �ڵ�
		 * @return	�����ڵ��� ���ڿ�
		 */
		static LPCWSTR		GetExceptionString(DWORD Code);

		/**
		 * @brief	����� ���� ������ ����մϴ�.
		 * @param enable	��뿩��
		 * @remarks     EnableUnhandledExceptioinFilter �̰� ����Ұ�� 
		 *
		 *				try~catch�� ������ ���� ������ ���ܰ� �߻��Ҷ� ���α׷��� Ư���ൿ��
		 *
		 *				�ϰ� ����ɼ� �ִٷ� �����Ҽ��ִ�.
		 *
		 *				EnableSaveDump �� ������ ���� ������ ������, dmp�� ����� ������ 
		 *
		 *				�����Ѵ�.
		 *
		 *				����� ������ ����� ��Ȱ�� �Ѵ�.
		 *
		 *				���߿� �����鼭 Ư�� ���ø����̼��� ���� �ϵ��� ó���Ҽ� �ִ�.
		 *
		 */
		static VOID				EnableUnhandledExceptioinFilter(BOOL enable = TRUE);

		/**
		 * @brief		���� Ÿ���� �����մϴ�.
		 * @param type	���� Ÿ��
		 */
		static VOID				SetMiniDumpType(MiniDumpTypes type);

		/**	
		 * @brief		���� �߻��� ������ �������� �����մϴ�.
		 * @param enable ���忩��
		 */
		static VOID				EnableSaveDump(BOOL enable = TRUE);

		/**
		 * @brief		���������� ������ ����� ������Ʈ �̸�
		 * @param strDumpName	�������ϸ�
		 */
		static VOID				SetDumpProjectName(LPCWSTR strDumpName) { wcscpy_s(s_szDefaultDumpFileName, strDumpName); }

		/**
		 * @brief		������ Dump������ �����մϴ�.
		 * @param filename		���� ���ϸ�
		 */
		static VOID				MiniDump(LPCWSTR filename);

		/**
		 * @brief		���ܰ� �߻��Ͽ� Dump�� �����մϴ�.
		 * @param filename		���� ���ϸ�
		 * @param ep			���� ����
		 */
		static VOID				MiniDump(LPCWSTR filename, PEXCEPTION_POINTERS ep);

		/// �α� ��� ���� : ���α׷� �̸��� ����մϴ�.
		static VOID				SetProgramName(WCHAR* pszOutBuffer, int nBufferSize, WCHAR* pszProgramName);

		/// �α� ��� ���� : ���� ����Ʈ�� ����մϴ�.
		static VOID				WriteExceptionReport(FILE* fp, PEXCEPTION_POINTERS pExceptionInfo);
		/// �α� ��� ���� : ���ø����̼� ��巹���� ����մϴ�.
	    static BOOL				GetLogicalAddress(PVOID addr, LPWSTR szModule, unsigned long len, unsigned long& section, unsigned long& offset );
		/// �α� ��� ���� : �⺻ ������ ����մϴ�.
		static int				WriteBasicInfo(WCHAR* szBuffer_Out, const int nBufferSize, PEXCEPTION_RECORD pExceptionRecord);
		/// �α� ��� ���� : ������Ʈ�� ������ ����մϴ�.
		static int				WriteRegistersInfo(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext);
		/// �α� ��� ���� : ���� ������ ����մϴ�.
		static int				WriteStackDetails(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext, BOOL bWriteVariables, BOOL& bHasSymbol_Out, const int nStackDepth = 256);
		/// �α� ��� ���� : �޸� ������ ����մϴ�.
		static int				WriteMemoryDump(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext, unsigned int nMaxIPDump = 16, unsigned int nMaxStackDump = 1024);
		/// �α� ��� ���� : ���ۿ� �ش� ������ ����մϴ�.
		static int				Dump(WCHAR* szBuffer_Out, const int nBufferSize, DWORD64 pData, unsigned long dwSize, BOOL bAlign);
		/// �α� ��� ���� : ���� ��½� ����մϴ�.
		static BOOL	CALLBACK	EnumerateSymbolsCallback(PSYMBOL_INFO,ULONG, PVOID);
		/// �α� ��� ���� : EnumerateSymbolsCallback���� ����մϴ�.
	    static BOOL				FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME* sf, WCHAR* pszBuffer, unsigned int nBufferSize);
		/// �α� ��� ���� : ���ۿ� �ش� ������ ����մϴ�.
		static int				FormatOutputValue(WCHAR* pszCurrBuffer, 
									const int nBufferSize, BasicType basicType, DWORD64 length, PVOID pAddress);

		/// �α� ��� ���� : ���ۿ� �ش� ������ ����մϴ�.
		static int				DumpTypeIndex(WCHAR* szBuffer_Out, 
									const int nBufferSize, DWORD64 modBase, unsigned long dwTypeIndex, 
									unsigned int nestingLevel, DWORD_PTR offset, BOOL & bHandled);

		/// �α� ��� ���� : BasicType�� ���մϴ�.
	    static BasicType		GetBasicType(unsigned long typeIndex, DWORD64 modBase);
		///////////////////////////////////////////////

	public:
		/// �αװ� ����� �α� ����
		static WCHAR			s_szLogBuffer[MAX_LOG_BUFFER];
		/// �⺻ ���� ����
		static WCHAR			s_szDefaultDumpFileName[128];
		/// ���� ���Ÿ��.
		static MiniDumpTypes	s_eMiniDumpType;
		/// ���� �α��� ���� ��Ͽ���
		static BOOL				s_bExceptionSaveDump;

		/// ����� ������ ����ϴ� ���� 
		static LPTOP_LEVEL_EXCEPTION_FILTER s_lpPreviousFilter;

	public:
		/**
		 * @brief	���� ��ü�� �����Ѵ�
		 * @param number		����(����) ��ȣ
		 * @param description	���� ����
		 * @param source		���� �߻� �Լ�/��ġ
		 * @return 
		 */
		NFException( int number, const Nave::String& description, const Nave::String& source );

		/**
		 * @brief	�� �� ���� ������ ���� ���ܸ� �����ϴ� ������
		 * @param number		����(����) ��ȣ
		 * @param description	���� ����
		 * @param source		���� �߻� �Լ�/��ġ
		 * @param file			���� �߻� ���ϸ�
		 * @param line			���� �߻� �� ��ȣ
		 */
		NFException( int number, const Nave::String& description, const Nave::String& source, const WCHAR* file, int line );

		/**
		 * @brief	���� ������
		 * @param rhs		������ ���� NFException ��ü
		 */
		NFException( const NFException& rhs );

		/**
		 * @brief	���� ������ ����
		 * @param rhs		������ ���� NFException ��ü
		 */
		void operator =( const NFException& rhs );

		/**
		 * @brief	������ ���� ��ü ������ ��ȯ�Ѵ�. �̰��� ������ ��� ������
		 *          ���� ������ ���ڿ� ������ ����� ��ȯ�Ѵ�. �� ���ڿ��� ����
		 *          ������ �ľ��ϱ� ���ٴ� �ܼ��� ��Ʈ���� ����ϱ� ���� �뵵��
		 *          ���Ǿ�� �����ϴ�.
		 * @return  ���� ��ü�� �����ϴ� ������ ���� ���ڿ�. 
		 * @warning ��¿뵵�θ� ��� ����
		 */
		const Nave::String GetFullDescription() const;

		/**
		 * @brief	���� �ڵ�(��ȣ)�� ��ȯ�Ѵ�.
		 * @return  ���� �ڵ� ��ȣ
		 */
		int GetNumber() const  { return m_iNumber; }

		/**
		 * @brief	���� �߻� ������ �̸��� ��ȯ�Ѵ�
		 * @return	���ܰ� �߻��ϴ� ������ �̸�
		 */
		const Nave::String& GetFile() const  { return m_szFile; }

		/**
		 * @brief	���� �߻� �� ��ȣ�� ��ȯ�Ѵ�.
		 * @return	�� ��ȣ
		 */
		int GetLine() const  { return m_iLine; }

		/**
		 * @brief	������ Description �ʵ��� ���ڿ����� ��ȯ�Ѵ�
		 * @return  Description�ʵ� ��
		 */
		const Nave::String& GetDescription() const  { return m_szDescription; }

	private:
		/// ����(����) �߻� �� ��ȣ
		int m_iLine;

		/// ���� ��ȣ
		int m_iNumber;

		/// ���� ����
		Nave::String  m_szDescription;

		/// ���� �߻� �Լ�, ��ġ Ȯ�ο� Identifier
		Nave::String  m_szSource;

		/// ���� �߻� ���ϸ�
		Nave::String  m_szFile;

	};
}
