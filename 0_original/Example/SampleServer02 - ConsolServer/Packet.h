#pragma once

#include <Nave/Nave.h>

enum PACKETLIST
{
	NONE = 0,
	IRC = 1,
	TEST,
	MaxPacket,
};

typedef struct PKIRC
{					
	CHAR	Key[64];
	CHAR	Message[512];
}*LPPKIRC;

typedef struct PKTEST
{
	unsigned int	TestKey;

	CHAR	Message[512];
}*LPPKTEST;