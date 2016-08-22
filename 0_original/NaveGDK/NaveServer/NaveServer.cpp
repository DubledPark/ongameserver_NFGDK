#include "NaveServer.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

#include <vector>
#include <Nave/NFStringUtil.h>

std::vector<Nave::StringA> g_AdapterInfo;

void UpdateAdapterInfo()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{
			if(strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") != 0)
				g_AdapterInfo.push_back(pAdapter->IpAddressList.IpAddress.String);

/*
			printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
			printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
			printf("\tAdapter Addr: \t%ld\n", pAdapter->Address);
			printf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

			printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			printf("\t***\n");
			if (pAdapter->DhcpEnabled) 
			{
				printf("\tDHCP Enabled: Yes\n");
				printf("\t\tDHCP Server: \t%s\n", pAdapter->DhcpServer.IpAddress.String);
				printf("\tLease Obtained: %ld\n", pAdapter->LeaseObtained);
			}
			else
				printf("\tDHCP Enabled: No\n");

			if (pAdapter->HaveWins) 
			{
				printf("\tHave Wins: Yes\n");
				printf("\t\tPrimary Wins Server: \t%s\n", pAdapter->PrimaryWinsServer.IpAddress.String);
				printf("\t\tSecondary Wins Server: \t%s\n", pAdapter->SecondaryWinsServer.IpAddress.String);
			}
			else
				printf("\tHave Wins: No\n");
*/
			pAdapter = pAdapter->Next;
		}
	}
/*	else 
	{
	  printf("Call to GetAdaptersInfo failed.\n");
	}
*/
	free(pAdapterInfo);
}

BOOL GetLocalIP(WCHAR* pLocalIP_Out, BOOL bClosedIP)
{
	if(g_AdapterInfo.empty())
		UpdateAdapterInfo();

	// 고정아이피가 있으면 고정아이피 리턴
	// 고정아이피가 없고 가상아이피만 존재하면 가상아이피 리턴
	int isize = g_AdapterInfo.size();
	for(int i = 0; i < isize; ++i)
	{
		wcscpy(pLocalIP_Out, Nave::ToUnicode(g_AdapterInfo[i]).c_str());

		if(bClosedIP)	// 내부아이피를 얻어와라.
		{
			if(wcsncmp(pLocalIP_Out, L"0", 1) == 0)
				continue;
			else if(wcsncmp(pLocalIP_Out, L"192", 3) == 0)
				return TRUE;
			else if(wcsncmp(pLocalIP_Out, L"127", 3) == 0)
				return TRUE;
		}
		else			// 외부 아이피를 얻어와라
		{
			if(wcsncmp(pLocalIP_Out, L"0", 1) == 0)
				continue;
			else if(wcsncmp(pLocalIP_Out, L"192", 3) == 0)
				continue;
			else if(wcsncmp(pLocalIP_Out, L"127", 3) == 0)
				continue;

			return TRUE;
		}
	}

	if(!bClosedIP)
		return GetLocalIP(pLocalIP_Out, TRUE);

	return FALSE;
}
