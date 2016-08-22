/** 
 *  @file		NFRandom.h
 *  @brief		���� ������
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include "NFPseudoRandom.h"

namespace Nave { 

	/** 
	 *  @class        NFRandomInt
	 *  @brief		  int�� ���� Ŭ����
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
		/// NFRandomInt ������
		NFRandomInt();
		// NFRandomInt ������
		NFRandomInt(unsigned long seed);
		/// Random Seed�� �����մϴ�.
		void SetSeed(unsigned long seed);

		/// generates a int number
		virtual unsigned int Next();
		/// generates a int number [0~max]
		virtual unsigned int Next(unsigned int excludedMax);
		/// generates a int number [min~max]
		virtual unsigned int Next(unsigned int includedMin, unsigned int excludedMax);

	private:
		/// ���� ��ü
		Private::NFPseudoRandomInt random;

		/// unable to copy
		NFRandomInt(const NFRandomInt& );
		/// operator =
		void operator =(const NFRandomInt&);
	};

	/** 
	 *  @class        NFRandomDouble
	 *  @brief        double�� ���� Ŭ����
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
		/// NFRandomDouble ������
		NFRandomDouble();
		/// NFRandomDouble ������
		NFRandomDouble(unsigned long seed);

		/// Random Seed�� �����մϴ�.
		void SetSeed(unsigned long seed);

		/// generates a float number [0, 1]
		virtual double Next(); 

	private:
		/// ���� ��ü
		Private::NFPseudoRandomFloat	random;

		/// unable to copy
		NFRandomDouble(const NFRandomDouble& );
		/// operator =
		void operator =(const NFRandomDouble&);
	};

}