// NFZipper.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <Nave/Nave.h>

#include <Nave/NFMD5.h>
#include <Nave/NFFilePtr.h>
#include <Nave/NFStringUtil.h>

#include <SDK/GZip/zlib.h>
#pragma comment(lib,"zlib.lib")

bool GetMD5(char* inData, int len, char* outMD5)
{
	Nave::NFMD5 md5;

	Nave::String str = md5.GetMD5(inData, len);

	strcpy(outMD5, Nave::ToASCII(str).c_str());

	return true;
}

int main(int argc, CHAR* argv[])
{
	if(argc != 4)
	{
		printf("NFZipper.exe [/e:/d] in:filename out:filename \n");
		printf("/e : Encrypt\n");
		printf("/d : Decrypt\n");
		return 0;
	}

	int iType = 0;
	char input[512] = {0, };
	char output[512] = {0, };


	for(int i = 1; i < argc; ++i)
	{
		if(strnicmp(argv[i], "/e", 2) == 0)	// ��ȣ
			iType = 0;
		else if(strnicmp(argv[i], "/d", 2) == 0)	// ��ȣ
			iType = 1;
		else if(strnicmp(argv[i], "in:", 3) == 0)	
			strcpy(input, &argv[i][3]);
		else if(strnicmp(argv[i], "out:", 4) == 0)	
			strcpy(output, &argv[i][4]);
	}

	///////////////////////////////////////////////////////////////////
	// in ������ �ε��մϴ�.
	char* fileDat = NULL;
	char* outDat = NULL;
	long len;
	long datalen = 0;

	// ������ MD5�� �����մϴ�.
	char szMD5[40];
	memset(szMD5, 0, sizeof(szMD5));

	if(strlen(input) > 0)
	{
		Nave::NFFilePtrA fp(input, "rb");

		if(iType == 0)
		{
			len = fp.Length();
			fileDat = new char[len];

			fread(fileDat, len, 1, fp);
			GetMD5(fileDat, len, szMD5);

			outDat = new char[len*2];
			datalen = 0;
		}
		else if(iType == 1)
		{
			fread(&len, sizeof(long), 1, fp);
			fread(&datalen, sizeof(long), 1, fp); 
			fread(szMD5, 32, 1, fp);

			fileDat = new char[datalen];
			fread(fileDat, datalen, 1, fp);

			outDat = new char[len*2];
		}
	}

	///////////////////////////////////////////////////////////////////
	if(iType == 0)
	{
		// ������ ���ڵ� �մϴ�.
		uLongf destLen = len*2;	// destLen�� ���̰� �����Ǿ��־�� zip ���̺귯���� ���� �۵��Ѵ�.
		compress2((Bytef*)outDat, &destLen, (Bytef*)fileDat, len, 5);
		outDat[destLen] = 0;

		Nave::NFFilePtrA fp(output, "wb");
		fwrite(&len, sizeof(long), 1, fp); 

		datalen = (long)destLen;
		fwrite(&datalen, sizeof(long), 1, fp); 

		fwrite(szMD5, 32, 1, fp);

		// MD5�� �̿��� ��ȣȭ�� �մϴ�.
		for(long i = 0; i < datalen; ++i)
		{
			outDat[i] = outDat[i]^szMD5[i%32];
		}

		fwrite(outDat, datalen, 1, fp);
	}
	else if(iType == 1)
	{
		// MD5�� �̿��� ��ȣȭ�� �մϴ�.
		for(long i = 0; i < datalen; ++i)
		{
			fileDat[i] = fileDat[i]^szMD5[i%32];
		}

		// ������ ���ڵ� �մϴ�.
		uLongf destLen = len*2;	// destLen�� ���̰� �����Ǿ��־�� zip ���̺귯���� ���� �۵��Ѵ�.
		uncompress((Bytef*)outDat, &destLen, (Bytef*)fileDat, datalen);
		outDat[destLen]=0;

		Nave::NFFilePtrA fp(output, "wb");
		fwrite(outDat, (long)destLen, 1, fp);
	}

	if(fileDat)
		delete []fileDat;

	if(outDat)
		delete []outDat;

	return 0;
}

