/** 
 *  @file		NFSpline.h
 *  @brief		곡선 계산 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 
	

	/** 
	 *  @class        NFSpline
	 *  @brief        점과 점사이의 중간값 구하는 클래스
	 *  @remarks      
	 *				p[0] ~ p[1] ~ p[2] ~ p[3]		\r\n
	 *												\r\n
	 *				float3 p[4];					\r\n
	 *				p[0] = float3(x1, y1, z1);		\r\n
	 *				p[1] = float3(x2, y2, z2);		\r\n
	 *				p[2] = float3(x3, y3, z3);		\r\n
	 *  			p[3] = float3(x4, y4, z4);		\r\n
	 *												\r\n
	 *  			dt = 0~1;  p[0] = 0, p[3] = 1;	\r\n
	 *												\r\n
	 *				float3 v = Nave::NFSpline::GetCatmullSplineValue(p[0], p[1], p[2], p[3], dt);
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFSpline 
	{
	public:
		template <typename T, typename S>
		static T GetBezierSplineValue(const T& a, const T& b, const T& c, const T& d, const S& time)
		{
			S time2 = time * time;
			S time3 = time * time2;

			S tp1 = 1 - time;
			S tp2 = tp1 * tp1;
			return a * tp2 * tp1 + b * 3 * tp2 * time + c * 3 * tp1 * time2 + d * time3;
		}

		template <typename T, typename S>
		static T GetCatmullSplineValue(const T& a, const T& b, const T& c, const T& d, const S& time)
		{
			S time2 = time * time;
			S time3 = time * time2;

			T a5 = a * (S)-0.5;
			T d5 = d * (S)0.5;
			return (T)(time3 * (a5 + b*1.5 - c*1.5 + d5) + time2 * (a - b*2.5 + c*2 - d5) + time * (a5 + c*0.5) + b);
		}

		template <typename T, typename S>
		static T GetBiCubicSplineValue(const T& a, const T& b, const T& c, const T& d, const S& time)
		{
			S time2 = time * time;
			S time3 = time * time2;

			T s1 = c - a;
			T s2 = d - b;
			return time3 * (2*b - 2*c + s1 + s2) + time2 * (-3*b + 3*c - 2*s1 - s2) + time * s1 + b;
		}

		template <typename T, typename S>
		static T GetBSplineValue(const T& a, const T& b, const T& c, const T& d, const S& time)
		{
			S time2 = time * time;
			S time3 = time * time2;

			T c3 = c * 3;
			T a3 = a * 3;
			return (time3 * (-a + 3*b - c3 + d) + time2 * (a3 - 6*b + c3) + time * (-a3 + c3) + (a + 4*b + c)) / 6;
		}
	};

}  