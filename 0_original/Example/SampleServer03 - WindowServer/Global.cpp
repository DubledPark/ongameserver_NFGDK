#include "Global.h"

ServerCtrl*								g_Server = NULL;
NaveServer::NFConnectionManager			g_UManager;
Config									g_Config;
int										g_iProcessPacket = 0;

void UpdateInfomation()
{
	if(g_Server)
		g_Server->UpdateInfo();
}
