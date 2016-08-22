/** 
 *  @file		NFMD5.h
 *  @brief		MD5 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFMD5
	 *  @brief        MD5를 구합니다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFMD5
	{
	public:
		/// NFMD5 생성자
		NFMD5(void);
		/// NFMD5 소멸자
		~NFMD5(void);

	public:
		/**
		 * @brief	MD5값을 구합니다.
		 * @param key	MD5값을 구할 정보
		 * @param size	key의 사이즈
		 * @return	MD5 스트링
		 */
		Nave::String GetMD5( CHAR* key, INT size );

	private:
		typedef struct 
		{
			UINT state[4];        /// state (ABCD) 
			UINT count[2];        /// number of bits, modulo 2^64 (lsb first) 
			BYTE buffer[64];      /// input buffer 
		} MD5_CTX;

	private:
		VOID MD5Init( MD5_CTX* ctx );
		VOID MD5Update( MD5_CTX* ctx, BYTE* input, UINT inputlen );
		VOID MD5Final( BYTE* digest, MD5_CTX* ctx );
	};

} 