/** 
 *  @file		NFProfile.h
 *  @brief		�������ϸ� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFProfile
	 *  @brief        ���ø����̼��� �������ϸ��� ���˴ϴ�.
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
		 * @brief	NFProfile ������
		 * @param strID �������ϸ� �̸�
		 */
		NFProfile(WCHAR* strID);

		/// NFProfile �Ҹ���
		~NFProfile(void);

	public:
		/**
		 * @brief	���ۿ� �������ϸ� ������ ����մϴ�.
		 * @param strBuffer		��� ����
		 * @param iMaxBuffer	���� ũ��
		 */
		static void		PrintProfile(WCHAR* strBuffer, INT iMaxBuffer);
		/// �������ϸ��� �����մϴ�.
		static void		EndProfile();

		/// �������ϸ� ������ �ʱ�ȭ�մϴ�.
		static void		ProfileInit();

		/**
		 * @brief	���ο� �������ϸ������� �߰��մϴ�.
		 * @param name �������ϸ������̸�
		 */
		static void		ProfileBegin( WCHAR* name );
		/**
		 * @brief	�������ϸ� ������ �����մϴ�.
		 * @param name �������ϸ������̸�
		 */
		static void		ProfileEnd( WCHAR* name );

	private:
		/**
		 * @brief	�������ϸ� Ÿ���� ���ɴϴ�
		 * @return  �ð���
		 */
		static float	GetTime();

		/// �������ϸ� ����Լ�
		static void		StoreProfileInHistory( WCHAR* name, float percent );
		/// �������ϸ� ����Լ�
		static void		GetProfileFromHistory( WCHAR* name, float* ave, float* min, float* max );
		/// �������ϸ� ����Լ�
		static void		PrintString(WCHAR* tBuf, WCHAR* sBuf, LONG& index);

	private:
		enum 
		{
			NUM_PROFILE_SAMPLES = 50	/// �������ϸ����� ����� �ִ� ���ü�
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

		/// �������ϸ� ���� 
		static ProfileSample			s_stSamples[NUM_PROFILE_SAMPLES];
		/// �������ϸ� �����丮
		static ProfileSampleHistory		s_stHistory[NUM_PROFILE_SAMPLES];
		/// ���۵� �ð�
		static float					s_fStartProfile;
		/// ����ð�
		static float					s_fEndProfile;
		/// �ʱ�ȭ�� �ð�
		static float					s_fFrequency;
		/// �������ϸ� �ʱ�ȭ ����
		static BOOL						s_bInitProfile;
		/// �������ϸ��� ����Ǿ���.
		static BOOL						s_bEndProfile;
		/// �������ϸ� �⺻���̵�
		static WCHAR					s_strID[256];
	};
} 

#ifdef _DEBUG
/// DEBUG ����϶� �������ϸ��� Enable �մϴ�.
#define _ENABLE_PROFILE_
#endif

#ifdef _ENABLE_PROFILE_
/// �������ϸ��� �����մϴ�.
#define PROFILE(ID) Nave::NFProfile enable(ID)
/// ���������� �����մϴ�.
#define PROFILE_END() Nave::NFProfile::EndProfile();
/// �������Ͽ� ���ο� ��ü�� �õ��մϴ�.
#define PROFILE_PUSH(ID) Nave::NFProfile::ProfileBegin(ID)
/// �ش� ��ü�� ���������� �����մϴ�.
#define PROFILE_POP(ID) Nave::NFProfile::ProfileEnd(ID)
/// ���ۿ� �������� ������ ����մϴ�.
#define PROFILE_PRINT(Buff, Len) Nave::NFProfile::PrintProfile(Buff, Len)
#else
/// �������ϸ��� �����մϴ�.
#define PROFILE(ID) 
/// ���������� �����մϴ�.
#define PROFILE_END()
/// �������Ͽ� ���ο� ��ü�� �õ��մϴ�.
#define PROFILE_PUSH(ID)
/// �ش� ��ü�� ���������� �����մϴ�.
#define PROFILE_POP(ID)
/// ���ۿ� �������� ������ ����մϴ�.
#define PROFILE_PRINT(Buff, Len)
#endif
