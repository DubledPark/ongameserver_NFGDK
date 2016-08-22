/** 
 *  @file		NFDeltaTimer.h
 *  @brief		Ÿ�̸� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <windows.h>

namespace Nave {

	/** 
	 *  @class        NFDeltaTimer
	 *  @brief        �ð��� �ð� ������ �ð� ���� �������� Ÿ�̸� ��ƾ
	 *  @remarks      NFDeltaTimer<float> timer(0, 5);			\r\n
	 *                float dt = timer.GetTimeDelta();			\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	template <typename TPrecision> class NFDeltaTimer 
	{
	public:
		/// NFDeltaTimer ������
		NFDeltaTimer() 
		{
			INT64 largo;
			this->m_tMinDelta=0;
			this->m_tMaxDelta=1;
			QueryPerformanceFrequency((LARGE_INTEGER*)&largo);

			m_tFreq=TPrecision(largo);	
		}

		/**
		 * @brief	NFDeltaTimer ������
		 * @param m_tMinDelta �ּҰ�
		 * @param m_tMaxDelta �ִ밪
		 */
		NFDeltaTimer(TPrecision m_tMinDelta, TPrecision m_tMaxDelta) 
		{
			INT64 largo;
			this->m_tMinDelta=m_tMinDelta;
			this->m_tMaxDelta=m_tMaxDelta;
			QueryPerformanceFrequency((LARGE_INTEGER*)&largo);

			m_tFreq=TPrecision(largo);	
		}

		/// NFDeltaTimer �Ҹ���
		~NFDeltaTimer()
		{
		}

		/**
		 * @brief	�� ���� �ð��� ���ɴϴ�.
		 * @return  ����� �ð���
		 * @warning	
		 */
		TPrecision GetAbsoluteTime() 
		{
			__int64 largo;
			QueryPerformanceCounter((LARGE_INTEGER*)&largo);
			return TPrecision((TPrecision)largo / m_tFreq);
		}

		/**
		 * @brief	���� �ð��� ���ɴϴ�.
		 * @return  ����� �ð��� 
		 * @warning	
		 */
		TPrecision GetTimeDelta() 
		{
			__int64 largo;
			QueryPerformanceCounter((LARGE_INTEGER*)&largo);
			__int64 diff= largo-m_lLastTime;
			m_lLastTime=largo;
			TPrecision dif=TPrecision((TPrecision)diff / m_tFreq);
			if ( dif > m_tMaxDelta) dif=m_tMaxDelta;
			if ( dif < m_tMinDelta) dif=m_tMinDelta;
			return dif;
		}

	private:
		/// ���� ������ NFDeltaTimer�� ������ �ð�
		TPrecision m_tFreq;
		/// �ּ� �����̰�
		TPrecision m_tMinDelta;
		/// �ִ� �����̰�
		TPrecision m_tMaxDelta;
		/// ������ �ð�
		__int64 m_lLastTime;
	};

}
