/** 
 *  @file		NFException.h
 *  @brief		Exception 관리 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <string>
#include <ostream>
#include <DbgHelp.h>

/// @warning __FUNCTION__은 비표준. Microsoft 환경에서만 사용 가능하다
#ifdef _MSC_VER
	/// 예외를 편리하게 처리한도록 도와주는 매크로
#define EXCEPTION( num, desc ) \
	throw( Nave::NFException( num, desc, __FUNCTION__, __FILE__, __LINE__ ) )
#else
	/// 예외를 편리하게 처리한도록 도와주는 매크로
#define EXCEPTION( num, desc, func ) \	
	throw( Nave::NFException( num, desc, func, __FILE__, __LINE__ ) )
#endif

namespace Nave {

	/** 
	 *  @class        NFException
	 *  @brief        예외가 발생했을대 예외를 처리하기 위한 클래스
	 *  @remarks      자동 예외 사용법														\r\n
	 *                NFException::EnableUnhandledExceptioinFilter(true);					\r\n
	 *                NFException::EnableSaveDump(true);									\r\n
	 *																						\r\n
	 *                수동 예외 사용법														\r\n
	 *                try																	\r\n
	 *                {																		\r\n
	 *		            EXCEPTION( ERR_INIT_FAILED, "System init failed. What the hell!" );	\r\n
	 *                }																		\r\n
	 *                catch( Nave::NFException& ex )										\r\n
	 *                {																		\r\n
	 *					// 에러 처리 코드. 아래는 에러 명세 문자열을 화면에 출력하는 예		\r\n
	 *					cout << ex.GetFullDescription() << endl;							\r\n
	 *                }																		\r\n
	 *                catch( ... )															\r\n
	 *                {																		\r\n
	 *					// 기타. 다른 예외 처리. 필요하지 않을 수도 있음					\r\n
	 *                }																		\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFException
	{
	public:
		/// 길이값 계산 Define값
		#define CHECK_LENGTH_RETURN(length, total_length, retval) \
			if(length < 0) { return retval; } else { total_length += length; }

		enum {
			SHIFT_NUM = 4,				/// 로그 출력시 오프셋 SHIFT 값
			BYTES_PER_LINE = 16, 		/// 로그 출력시 사용할 값

			MAX_BUFFER =1024,			/// 로그 출력시 사용할 버퍼크기
			MAX_LOG_BUFFER = 65535		/// 로그 출력시 사용할 64-kb log buffer
		};

		enum MiniDumpTypes 
		{ 
			NormalDump=0,				/// 일반 덤프
			HeapDump					/// 최대 덤프
		};

		/// Call Stack 출력용 구조체
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
		 * @brief	예외의 에러문자열 구하기
		 * @param Code 예외 코드
		 * @return	에러코드의 문자열
		 */
		static LPCWSTR		GetExceptionString(DWORD Code);

		/**
		 * @brief	사용자 예외 덤프를 사용합니다.
		 * @param enable	사용여부
		 * @remarks     EnableUnhandledExceptioinFilter 이걸 사용할경우 
		 *
		 *				try~catch로 묶이지 않은 곳에서 예외가 발생할때 프로그램이 특정행동을
		 *
		 *				하고 종료될수 있다록 정의할수있다.
		 *
		 *				EnableSaveDump 이 정보에 의해 조용히 죽을주, dmp를 남기고 죽을지 
		 *
		 *				정의한다.
		 *
		 *				현재는 덤프를 남기는 역활만 한다.
		 *
		 *				나중에 죽으면서 특정 어플리케이션을 샐행 하도록 처리할수 있다.
		 *
		 */
		static VOID				EnableUnhandledExceptioinFilter(BOOL enable = TRUE);

		/**
		 * @brief		덤프 타입을 설정합니다.
		 * @param type	덤프 타입
		 */
		static VOID				SetMiniDumpType(MiniDumpTypes type);

		/**	
		 * @brief		예외 발생시 덤프를 저장할지 지정합니다.
		 * @param enable 저장여부
		 */
		static VOID				EnableSaveDump(BOOL enable = TRUE);

		/**
		 * @brief		덤프파일을 생성시 사용할 프로젝트 이름
		 * @param strDumpName	덤프파일명
		 */
		static VOID				SetDumpProjectName(LPCWSTR strDumpName) { wcscpy_s(s_szDefaultDumpFileName, strDumpName); }

		/**
		 * @brief		강제로 Dump파일을 생성합니다.
		 * @param filename		덤프 파일명
		 */
		static VOID				MiniDump(LPCWSTR filename);

		/**
		 * @brief		예외가 발생하여 Dump를 생성합니다.
		 * @param filename		덤프 파일명
		 * @param ep			예외 정보
		 */
		static VOID				MiniDump(LPCWSTR filename, PEXCEPTION_POINTERS ep);

		/// 로그 출력 관련 : 프로그램 이름을 출력합니다.
		static VOID				SetProgramName(WCHAR* pszOutBuffer, int nBufferSize, WCHAR* pszProgramName);

		/// 로그 출력 관련 : 예외 리포트를 출력합니다.
		static VOID				WriteExceptionReport(FILE* fp, PEXCEPTION_POINTERS pExceptionInfo);
		/// 로그 출력 관련 : 어플리케이션 어드레스를 출력합니다.
	    static BOOL				GetLogicalAddress(PVOID addr, LPWSTR szModule, unsigned long len, unsigned long& section, unsigned long& offset );
		/// 로그 출력 관련 : 기본 정보를 출력합니다.
		static int				WriteBasicInfo(WCHAR* szBuffer_Out, const int nBufferSize, PEXCEPTION_RECORD pExceptionRecord);
		/// 로그 출력 관련 : 레지스트리 정보를 출력합니다.
		static int				WriteRegistersInfo(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext);
		/// 로그 출력 관련 : 스택 정보를 출력합니다.
		static int				WriteStackDetails(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext, BOOL bWriteVariables, BOOL& bHasSymbol_Out, const int nStackDepth = 256);
		/// 로그 출력 관련 : 메모리 정보를 출력합니다.
		static int				WriteMemoryDump(WCHAR* szBuffer_Out, const int nBufferSize, PCONTEXT pContext, unsigned int nMaxIPDump = 16, unsigned int nMaxStackDump = 1024);
		/// 로그 출력 관련 : 버퍼에 해당 정보를 기록합니다.
		static int				Dump(WCHAR* szBuffer_Out, const int nBufferSize, DWORD64 pData, unsigned long dwSize, BOOL bAlign);
		/// 로그 출력 관련 : 스택 출력시 사용합니다.
		static BOOL	CALLBACK	EnumerateSymbolsCallback(PSYMBOL_INFO,ULONG, PVOID);
		/// 로그 출력 관련 : EnumerateSymbolsCallback에서 사용합니다.
	    static BOOL				FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME* sf, WCHAR* pszBuffer, unsigned int nBufferSize);
		/// 로그 출력 관련 : 버퍼에 해당 정보를 기록합니다.
		static int				FormatOutputValue(WCHAR* pszCurrBuffer, 
									const int nBufferSize, BasicType basicType, DWORD64 length, PVOID pAddress);

		/// 로그 출력 관련 : 버퍼에 해당 정보를 기록합니다.
		static int				DumpTypeIndex(WCHAR* szBuffer_Out, 
									const int nBufferSize, DWORD64 modBase, unsigned long dwTypeIndex, 
									unsigned int nestingLevel, DWORD_PTR offset, BOOL & bHandled);

		/// 로그 출력 관련 : BasicType를 구합니다.
	    static BasicType		GetBasicType(unsigned long typeIndex, DWORD64 modBase);
		///////////////////////////////////////////////

	public:
		/// 로그가 저장될 로그 버퍼
		static WCHAR			s_szLogBuffer[MAX_LOG_BUFFER];
		/// 기본 덤프 파일
		static WCHAR			s_szDefaultDumpFileName[128];
		/// 덤프 기록타입.
		static MiniDumpTypes	s_eMiniDumpType;
		/// 덤프 로그의 파일 기록여부
		static BOOL				s_bExceptionSaveDump;

		/// 사용자 덤프시 사용하는 변수 
		static LPTOP_LEVEL_EXCEPTION_FILTER s_lpPreviousFilter;

	public:
		/**
		 * @brief	예외 객체를 생성한다
		 * @param number		에러(예외) 번호
		 * @param description	에러 설명
		 * @param source		에러 발생 함수/위치
		 * @return 
		 */
		NFException( int number, const Nave::String& description, const Nave::String& source );

		/**
		 * @brief	좀 더 상세한 정보를 갖는 예외를 생성하는 생성자
		 * @param number		에러(예외) 번호
		 * @param description	에러 설명
		 * @param source		에러 발생 함수/위치
		 * @param file			에러 발생 파일명
		 * @param line			에러 발생 줄 번호
		 */
		NFException( int number, const Nave::String& description, const Nave::String& source, const WCHAR* file, int line );

		/**
		 * @brief	복사 생성자
		 * @param rhs		복사할 원본 NFException 객체
		 */
		NFException( const NFException& rhs );

		/**
		 * @brief	대입 연산자 정의
		 * @param rhs		복사할 원본 NFException 객체
		 */
		void operator =( const NFException& rhs );

		/**
		 * @brief	에러에 대한 전체 서술을 반환한다. 이것은 예외의 모든 정보를
		 *          일정 형식의 문자열 폼으로 만들어 반환한다. 이 문자열은 세부
		 *          정보를 파악하기 보다는 단순히 스트림에 출력하기 위한 용도로
		 *          사용되어야 적합하다.
		 * @return  예외 전체를 서술하는 문장을 담은 문자열. 
		 * @warning 출력용도로만 사용 권장
		 */
		const Nave::String GetFullDescription() const;

		/**
		 * @brief	에러 코드(번호)를 반환한다.
		 * @return  에러 코드 번호
		 */
		int GetNumber() const  { return m_iNumber; }

		/**
		 * @brief	예외 발생 파일의 이름을 반환한다
		 * @return	예외가 발생하는 파일의 이름
		 */
		const Nave::String& GetFile() const  { return m_szFile; }

		/**
		 * @brief	예외 발생 줄 번호를 반환한다.
		 * @return	줄 번호
		 */
		int GetLine() const  { return m_iLine; }

		/**
		 * @brief	예외의 Description 필드의 문자열만을 반환한다
		 * @return  Description필드 값
		 */
		const Nave::String& GetDescription() const  { return m_szDescription; }

	private:
		/// 에러(예외) 발생 줄 번호
		int m_iLine;

		/// 에러 번호
		int m_iNumber;

		/// 에러 설명
		Nave::String  m_szDescription;

		/// 에러 발생 함수, 위치 확인용 Identifier
		Nave::String  m_szSource;

		/// 에러 발생 파일명
		Nave::String  m_szFile;

	};
}
