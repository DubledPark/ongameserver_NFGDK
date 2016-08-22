/** 
 *  @file		NFMD5.h
 *  @brief		MD5 Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave { 

	/** 
	 *  @class        NFMD5
	 *  @brief        MD5�� ���մϴ�.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	class NFMD5
	{
	public:
		/// NFMD5 ������
		NFMD5(void);
		/// NFMD5 �Ҹ���
		~NFMD5(void);

	public:
		/**
		 * @brief	MD5���� ���մϴ�.
		 * @param key	MD5���� ���� ����
		 * @param size	key�� ������
		 * @return	MD5 ��Ʈ��
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