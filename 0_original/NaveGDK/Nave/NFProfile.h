/** 
 *  @file		NFProfile.h
 *  @brief		프로파일링 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFProfile
	 *  @brief        어플리케이션의 프로파일링에 사용됩니다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFProfile
	{
	public:
		/**
		 * @brief	NFProfile 생성자
		 * @param strID 프로파일링 이름
		 */
		NFProfile(WCHAR* strID);

		/// NFProfile 소멸자
		~NFProfile(void);

	public:
		/**
		 * @brief	버퍼에 프로파일링 정보를 출력합니다.
		 * @param strBuffer		출력 버퍼
		 * @param iMaxBuffer	버퍼 크기
		 */
		static void		PrintProfile(WCHAR* strBuffer, INT iMaxBuffer);
		/// 프로파일링을 종료합니다.
		static void		EndProfile();

		/// 프로파일링 변수를 초기화합니다.
		static void		ProfileInit();

		/**
		 * @brief	새로운 프로파일링샘플을 추가합니다.
		 * @param name 프로파일링샘플이름
		 */
		static void		ProfileBegin( WCHAR* name );
		/**
		 * @brief	프로파일링 샘플을 종료합니다.
		 * @param name 프로파일링샘플이름
		 */
		static void		ProfileEnd( WCHAR* name );

	private:
		/**
		 * @brief	프로파일링 타임을 얻어옵니다
		 * @return  시간값
		 */
		static float	GetTime();

		/// 프로파일링 출력함수
		static void		StoreProfileInHistory( WCHAR* name, float percent );
		/// 프로파일링 출력함수
		static void		GetProfileFromHistory( WCHAR* name, float* ave, float* min, float* max );
		/// 프로파일링 출력함수
		static void		PrintString(WCHAR* tBuf, WCHAR* sBuf, LONG& index);

	private:
		enum 
		{
			NUM_PROFILE_SAMPLES = 50	/// 프로파일링에서 사용할 최대 샘플수
		};

		typedef struct 
		{
		   bool			bValid;					///Whether this data is valid
		   unsigned int iProfileInstances;      ///# of times ProfileBegin called
		   int			iOpenProfiles;			///# of times ProfileBegin w/o ProfileEnd
		   WCHAR		szName[256];			///Name of sample
		   float		fStartTime;				///The current open profile start time
		   float		fAccumulator;			///All samples this frame added together
		   float		fChildrenSampleTime;	///Time taken by all children
		   unsigned int iNumParents;            ///Number of profile parents
		} ProfileSample;

		typedef struct 
		{
		   bool			bValid;				///Whether the data is valid
		   WCHAR		szName[256];		///Name of the sample
		   float		fAve;				///Average time per frame (percentage)
		   float		fMin;				///Minimum time per frame (percentage)
		   float		fMax;				///Maximum time per frame (percentage)
		} ProfileSampleHistory;

		/// 프로파일링 샘플 
		static ProfileSample			s_stSamples[NUM_PROFILE_SAMPLES];
		/// 프로파일링 히스토리
		static ProfileSampleHistory		s_stHistory[NUM_PROFILE_SAMPLES];
		/// 시작된 시간
		static float					s_fStartProfile;
		/// 종료시간
		static float					s_fEndProfile;
		/// 초기화된 시간
		static float					s_fFrequency;
		/// 프로파일링 초기화 상태
		static BOOL						s_bInitProfile;
		/// 프로파일링이 종료되었다.
		static BOOL						s_bEndProfile;
		/// 프로파일링 기본아이디
		static WCHAR					s_strID[256];
	};
} 

#ifdef _DEBUG
/// DEBUG 모드일때 프로파일링을 Enable 합니다.
#define _ENABLE_PROFILE_
#endif

#ifdef _ENABLE_PROFILE_
/// 프로파일링을 선언합니다.
#define PROFILE(ID) Nave::NFProfile enable(ID)
/// 프로파일을 종료합니다.
#define PROFILE_END() Nave::NFProfile::EndProfile();
/// 프로파일에 새로운 객체를 시도합니다.
#define PROFILE_PUSH(ID) Nave::NFProfile::ProfileBegin(ID)
/// 해당 객체의 프로파일을 종료합니다.
#define PROFILE_POP(ID) Nave::NFProfile::ProfileEnd(ID)
/// 버퍼에 프로파일 정보를 출력합니다.
#define PROFILE_PRINT(Buff, Len) Nave::NFProfile::PrintProfile(Buff, Len)
#else
/// 프로파일링을 선언합니다.
#define PROFILE(ID) 
/// 프로파일을 종료합니다.
#define PROFILE_END()
/// 프로파일에 새로운 객체를 시도합니다.
#define PROFILE_PUSH(ID)
/// 해당 객체의 프로파일을 종료합니다.
#define PROFILE_POP(ID)
/// 버퍼에 프로파일 정보를 출력합니다.
#define PROFILE_PRINT(Buff, Len)
#endif
