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
		// üũ�� ��������.
		// 1. ��� ���� (������� Ŀ�ǵ� ����.)
		// 2. ��ȣȭ �� ����
		// 3. üũ�� ����.
		m_Header.CheckSum = ((m_Header.Command+m_Header.Size+m_Packet[0]+m_Packet[1])^0xA6F69E23)^0x62F2EA02;

#ifdef ENCRYPT
		// ����� 100����Ʈ �̻��̸� �����ϰ� üũ�� ����
		if(m_Header.Size >= 100)
		{
			m_Header.CheckSum = m_Header.CheckSum|0xF0000000;
			// �����ϱ�
			// m_Header.Size ������ ������
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
			// ����Ǯ��
			// m_Header.Size ������ ������
			uLongf destLen = DEF_PACKETSIZE;
			int srcLen = GetSize();
			char srcBuff[DEF_PACKETSIZE];
			memcpy(srcBuff, m_Packet, srcLen);
			uncompress((Bytef*)m_Packet, &destLen, (Bytef*)srcBuff, srcLen);
			m_Packet[destLen]=0;
			SetSize((USHORT)destLen);
		}
		// üũ�� ����
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