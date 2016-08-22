/** 
 *  @file		NFDeltaTimer.h
 *  @brief		타이머 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <windows.h>

namespace Nave {

	/** 
	 *  @class        NFDeltaTimer
	 *  @brief        시간과 시간 사이의 시간 차를 내기위한 타이머 루틴
	 *  @remarks      NFDeltaTimer<float> timer(0, 5);			\r\n
	 *                float dt = timer.GetTimeDelta();			\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	template <typename TPrecision> class NFDeltaTimer 
	{
	public:
		/// NFDeltaTimer 생성자
		NFDeltaTimer() 
		{
			INT64 largo;
			this->m_tMinDelta=0;
			this->m_tMaxDelta=1;
			QueryPerformanceFrequency((LARGE_INTEGER*)&largo);

			m_tFreq=TPrecision(largo);	
		}

		/**
		 * @brief	NFDeltaTimer 생성자
		 * @param m_tMinDelta 최소값
		 * @param m_tMaxDelta 최대값
		 */
		NFDeltaTimer(TPrecision m_tMinDelta, TPrecision m_tMaxDelta) 
		{
			INT64 largo;
			this->m_tMinDelta=m_tMinDelta;
			this->m_tMaxDelta=m_tMaxDelta;
			QueryPerformanceFrequency((LARGE_INTEGER*)&largo);

			m_tFreq=TPrecision(largo);	
		}

		/// NFDeltaTimer 소멸자
		~NFDeltaTimer()
		{
		}

		/**
		 * @brief	총 진행 시간을 얻어옵니다.
		 * @return  진행된 시간값
		 * @warning	
		 */
		TPrecision GetAbsoluteTime() 
		{
			__int64 largo;
			QueryPerformanceCounter((LARGE_INTEGER*)&largo);
			return TPrecision((TPrecision)largo / m_tFreq);
		}

		/**
		 * @brief	진행 시간을 얻어옵니다.
		 * @return  진행된 시간값 
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
		/// 최초 설정값 NFDeltaTimer가 생성된 시간
		TPrecision m_tFreq;
		/// 최소 딜레이값
		TPrecision m_tMinDelta;
		/// 최대 딜레이값
		TPrecision m_tMaxDelta;
		/// 마지막 시간
		__int64 m_lLastTime;
	};

}
