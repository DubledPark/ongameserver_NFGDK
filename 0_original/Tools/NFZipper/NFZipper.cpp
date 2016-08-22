// NFZipper.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
		if(strnicmp(argv[i], "/e", 2) == 0)	// 암호
			iType = 0;
		else if(strnicmp(argv[i], "/d", 2) == 0)	// 암호
			iType = 1;
		else if(strnicmp(argv[i], "in:", 3) == 0)	
			strcpy(input, &argv[i][3]);
		else if(strnicmp(argv[i], "out:", 4) == 0)	
			strcpy(output, &argv[i][4]);
	}

	///////////////////////////////////////////////////////////////////
	// in 파일을 로딩합니다.
	char* fileDat = NULL;
	char* outDat = NULL;
	long len;
	long datalen = 0;

	// 파일의 MD5를 생성합니다.
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
		// 파일을 인코딩 합니다.
		uLongf destLen = len*2;	// destLen의 길이가 지정되어있어야 zip 라이브러리가 정상 작동한다.
		compress2((Bytef*)outDat, &destLen, (Bytef*)fileDat, len, 5);
		outDat[destLen] = 0;

		Nave::NFFilePtrA fp(output, "wb");
		fwrite(&len, sizeof(long), 1, fp); 

		datalen = (long)destLen;
		fwrite(&datalen, sizeof(long), 1, fp); 

		fwrite(szMD5, 32, 1, fp);

		// MD5를 이용해 암호화를 합니다.
		for(long i = 0; i < datalen; ++i)
		{
			outDat[i] = outDat[i]^szMD5[i%32];
		}

		fwrite(outDat, datalen, 1, fp);
	}
	else if(iType == 1)
	{
		// MD5를 이용해 암호화를 합니다.
		for(long i = 0; i < datalen; ++i)
		{
			fileDat[i] = fileDat[i]^szMD5[i%32];
		}

		// 파일을 디코딩 합니다.
		uLongf destLen = len*2;	// destLen의 길이가 지정되어있어야 zip 라이브러리가 정상 작동한다.
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

