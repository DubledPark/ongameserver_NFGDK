#include "NaveNet.h"
#include "NFPacket.h"

#include <GZip/zlib.h>
#pragma comment(lib,"zlib.lib")

namespace NaveNet { 

	NFPacket::NFPacket()
	{
		Init();
	}

	NFPacket::~NFPacket()
	{
	}

	VOID NFPacket::Init()
	{
		ZeroMemory(&m_Header, sizeof(PACKETHEADER));
		ZeroMemory(m_Packet, sizeof(m_Packet));
	}

	UINT NFPacket::EncryptPacket()	
	{ 
		// 체크섬 생성순서.
		// 1. 헤더 생성 (사이즈및 커맨드 설정.)
		// 2. 암호화 및 압축
		// 3. 체크섬 생성.
		m_Header.CheckSum = ((m_Header.Command+m_Header.Size+m_Packet[0]+m_Packet[1])^0xA6F69E23)^0x62F2EA02;

#ifdef ENCRYPT
		// 사이즈가 100바이트 이상이면 압축하고 체크섬 설정
		if(m_Header.Size >= 100)
		{
			m_Header.CheckSum = m_Header.CheckSum|0xF0000000;
			// 압축하기
			// m_Header.Size 사이즈 재조정
			uLongf destLen = DEF_PACKETSIZE;
			int srcLen = GetSize();
			char srcBuff[DEF_PACKETSIZE];
			memcpy(srcBuff, m_Packet, srcLen);
			compress2((Bytef*)m_Packet, &destLen, (Bytef*)srcBuff, srcLen, 5);
			m_Packet[destLen]=0;
			SetSize((USHORT)destLen);
		}
		else 
			m_Header.CheckSum = m_Header.CheckSum&0x0FFFFFFF;
#endif
		return m_Header.Size;
	}

	VOID NFPacket::DecryptPacket()
	{
#ifdef ENCRYPT
		if(m_Header.CheckSum&0xF0000000)
		{
			// 압축풀기
			// m_Header.Size 사이즈 재조정
			uLongf destLen = DEF_PACKETSIZE;
			int srcLen = GetSize();
			char srcBuff[DEF_PACKETSIZE];
			memcpy(srcBuff, m_Packet, srcLen);
			uncompress((Bytef*)m_Packet, &destLen, (Bytef*)srcBuff, srcLen);
			m_Packet[destLen]=0;
			SetSize((USHORT)destLen);
		}
		// 체크섬 조정
		m_Header.CheckSum = m_Header.CheckSum&0x0FFFFFFF;
#endif
	}

	BOOL NFPacket::IsAliveChecksum()	
	{ 
		DWORD CheckSum = ((m_Header.Command+m_Header.Size+m_Packet[0]+m_Packet[1])^0xA6F69E23)^0x62F2EA02;
#ifdef ENCRYPT
		CheckSum = CheckSum&0x0FFFFFFF;
#endif

		if(m_Header.CheckSum == CheckSum)
			return TRUE;

		return FALSE; 
	}

}