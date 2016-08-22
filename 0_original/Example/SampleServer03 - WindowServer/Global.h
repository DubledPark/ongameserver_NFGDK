#pragma once

#include <NaveServer/NaveServer.h>
#include <NaveServer/NFServerCtrl.h>
#include <NaveServer/NFConnectionManager.h>
#include <NaveServer/NFConnection.h>
#include <NaveServer/NFConnection.h>
#include <NaveNet/NaveNet.h>
#include <NaveNet/NFPacket.h>
#include <Nave/NFSync.h>

#include "TestConnection.h"
#include "ServerCtrl.h"
#include "Config.h"

extern	ServerCtrl*								g_Server;
extern	NaveServer::NFConnectionManager			g_UManager;
extern	Config									g_Config;
extern	int										g_iProcessPacket;

void UpdateInfomation();