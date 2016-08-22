/** 
 *  @file		NFRandom.h
 *  @brief		랜덤 생성기
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "NFPseudoRandom.h"

namespace Nave { 

	/** 
	 *  @class        NFRandomInt
	 *  @brief		  int형 랜덤 클래스
	 *  @remarks      Nave::NFRandomInt foo;			\r\n
	 *  			  foo.Next()						\r\n
	 *  			  foo.Next(50);						\r\n
	 *  		 	  foo.Next(10,4300);				\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFRandomInt
	{
	public:
		/// NFRandomInt 생성자
		NFRandomInt();
		// NFRandomInt 생성자
		NFRandomInt(unsigned long seed);
		/// Random Seed를 설정합니다.
		void SetSeed(unsigned long seed);

		/// generates a int number
		virtual unsigned int Next();
		/// generates a int number [0~max]
		virtual unsigned int Next(unsigned int excludedMax);
		/// generates a int number [min~max]
		virtual unsigned int Next(unsigned int includedMin, unsigned int excludedMax);

	private:
		/// 랜덤 객체
		Private::NFPseudoRandomInt random;

		/// unable to copy
		NFRandomInt(const NFRandomInt& );
		/// operator =
		void operator =(const NFRandomInt&);
	};

	/** 
	 *  @class        NFRandomDouble
	 *  @brief        double형 랜덤 클래스
	 *  @remarks      Nave::NFRandomDouble foo;			\r\n
	 *  			  foo.Next()	// 0~1				\r\n
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFRandomDouble
	{
	public:
		/// NFRandomDouble 생성자
		NFRandomDouble();
		/// NFRandomDouble 생성자
		NFRandomDouble(unsigned long seed);

		/// Random Seed를 설정합니다.
		void SetSeed(unsigned long seed);

		/// generates a float number [0, 1]
		virtual double Next(); 

	private:
		/// 랜덤 객체
		Private::NFPseudoRandomFloat	random;

		/// unable to copy
		NFRandomDouble(const NFRandomDouble& );
		/// operator =
		void operator =(const NFRandomDouble&);
	};

}