// Sample01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

/*
	NaveGDK 의 기본기능 테스트.
*/


#include "stdafx.h"

#include <Nave/Nave.h>				/// NaveGDK 기본헤더
#include <Nave/NFProfile.h>			/// 프로파일러 테스트
#include <Nave/NFTokenizer.h>		/// 토큰
#include <Nave/NFChecksum.h>		/// 체크섬
#include <Nave/NFRandom.h>			/// 랜덤
#include <Nave/NFSpline.h>			
#include <Nave/NFMD5.h>
#include <Nave/NFLog.h>				/// 로그
#include <Nave/NFLogManager.h>

#include <Nave/NFObject.h>			/// NFObject
#include <Nave/NFFactory.h>			/// Factory 패턴
#include <Nave/NFAutoreleasePool.h>	/// NFObject의 AutoRelease Pool 테스트

#include <Nave/NFFilePtr.h>			/// FILE* 자동화

//#include <NaveFramework/NFInputManager.h>

#include "Config.h"

#include <Nave/NFXMLScript.h>		/// XML 관련

#include <NaveFramework/NFStringTable.h>

using namespace Nave;


// NFObject를 이용한 AutoRelease 테스트
class UIMemory : public NFObject
{
	char	m_Data[2048];
public:
	UIMemory()
	{
	}
	~UIMemory()
	{
	}
};


// NFObject를 이용한 AutoRelease 테스트 및 NFFactory를 이용한 생성테스트
class UIFactoryItem : public NFObject
{
public:
	UIFactoryItem()
	{
		LOG_IMPORTANT((L"Init UIFactoryItem : %d", GetHashCode()));
	}

	~UIFactoryItem()
	{
		LOG_IMPORTANT((L"Destory UIFactoryItem : %d", GetHashCode()));
	}

    virtual const char* ToString() { return "UIFactoryItem"; }
};

class UIFactoryItem2 : public NFObject
{
public:
	UIFactoryItem2()
	{
		LOG_IMPORTANT((L"Init UIFactoryItem2 : %d", GetHashCode()));
	}

	~UIFactoryItem2()
	{
		LOG_IMPORTANT((L"Destory UIFactoryItem2 : %d", GetHashCode()));
	}

    virtual const char* ToString() { return "UIFactoryItem2"; }
};

/// 스마트 포인터 테스트
class UISmartTest
{
	int m_idx;
public:
	UISmartTest() : m_idx(0)
	{
		LOG_IMPORTANT((L"Init UISmartTest : %d", m_idx))
	}
	UISmartTest(int idx)
	{
		m_idx = idx;
		LOG_IMPORTANT((L"Init UISmartTest : %d", m_idx))
	}

	~UISmartTest()
	{
		LOG_IMPORTANT((L"Destory UISmartTest : %d", m_idx));
	}
};




int _tmain(int argc, _TCHAR* argv[])
{
	// wprintf 를 사용하기 위해서
	// 로그를 콘솔에 출력하기 위해서는 아래와 같이 로케일 설정을 해줘야한다.
	setlocale(LC_ALL, "Korean");

	// 로그 설정.
	NFLog::SetLogPrint(TRUE);
//	NFLog::SetLogLimit(NFLog::Proxy::Info);
//	NFLog::EnableLogFile(TRUE);
//	NFLog::SetLogDetail(TRUE);

	// 예외처리
	// 자체 함수 NFException 사용
	NFException::EnableUnhandledExceptioinFilter(true);
	// Dump 저장
	NFException::EnableSaveDump(true);


	LOG_IMPORTANT((L"-----> Test Start"));

	// 프로파일러 실행
	PROFILE(L"TestConsol");

	WCHAR strBuff[1024];
	WCHAR strTemp[1024];

	/////////////////////////////////////////////////////////
	// Token 테스트
	/////////////////////////////////////////////////////////
	if(1)
	{
		String tempStr = L"01|02|03|04|05|06|07|08|09|10|11|12";

		NFTokenizer strtok = NFTokenizer(tempStr, L"|");

		LOG_IMPORTANT((L"Number Of Tokens: %d", strtok.CountTokens()));
		LOG_IMPORTANT((L"String: %s", strtok.RemainingString().c_str()));

		int cnt = strtok.CountTokens();
		String finalString = L"";

		LOG_IMPORTANT((L"-----> Token Test"));

		for(int i = 0; i < cnt; i++)
		{
			PROFILE_PUSH(L"Token");

			String tempStr = L"";

			LOG_IMPORTANT((L"Guid=%d", GetGUID()));

			LOG_IMPORTANT((L"Token[%d] ----> [%s]", i, strtok.NextToken().c_str()));
			LOG_IMPORTANT((L"Token Count : %d", strtok.CountTokens()));

			finalString += tempStr;

			PROFILE_POP(L"Token");
		}

		LOG_IMPORTANT((L"Final String : %s", finalString.c_str()));
	}

	LOG_IMPORTANT((L"-----> Util Test"));

	if(1)
	{
		////////////////////////////////////////////////////////////////////////
		// 모듈 파일명
		////////////////////////////////////////////////////////////////////////
		GetProgramName(strBuff, 1024);
		LOG_IMPORTANT((strBuff));

		GetProgramDir(strBuff, 1024 );
		LOG_IMPORTANT((strBuff));

		wcscpy(strTemp, strBuff);
		GetDir(strTemp, strBuff);
		LOG_IMPORTANT((strBuff));

		////////////////////////////////////////////////////////////////////////
		// 체크섬 구하기
		////////////////////////////////////////////////////////////////////////
		DWORD dwCrc32 = 0;
		NFChecksum::FromFileWin32(argv[0], dwCrc32);
		LOG_IMPORTANT((L"Win32 :    0x%x", dwCrc32));
		dwCrc32 = 0;
		NFChecksum::FromFileFilemap(argv[0], dwCrc32);
		LOG_IMPORTANT((L"Filemap :  0x%x", dwCrc32));
		dwCrc32 = 0;
		NFChecksum::FromFileAssembly(argv[0], dwCrc32);
		LOG_IMPORTANT((L"Assembly : 0x%x", dwCrc32));

		/////////////////////////////////////////////////////////////////////////
		// MD5 체크
		/////////////////////////////////////////////////////////////////////////
		NFMD5	md5;
		NFFilePtrW fp(argv[0], L"rb");
		long ta = fp.Length();
		CHAR* pmdData = new CHAR[ta];

		fseek(fp, 0, SEEK_SET);
		fread(pmdData, ta, 1, fp);

		String str = md5.GetMD5(pmdData, ta);
		LOG_IMPORTANT((L"MD5 : 0x%s", str.c_str()));

		delete pmdData;
		fclose(fp);
	}

	LOG_IMPORTANT((L"-----> SmartPtr Test"));
	if(1)
	{
		////////////////////////////////////////////////////////////////////////
		// 스마트 포인트 사용
		////////////////////////////////////////////////////////////////////////
		UISmartTest test;

		NFSmartPtr<UISmartTest> pView = new UISmartTest(1);
		pView = new UISmartTest(2);
	}

	/////////////////////////////////////////////////////////
	// Autorelease 및 NFFactory 사용법
	/////////////////////////////////////////////////////////
	LOG_IMPORTANT((L"-----> Factory Test && AutoreleasePool"));

	NFAutoreleasePool* pool = new NFAutoreleasePool();

	if(1)
	{
		////////////////////////////////////////////////////////////////////////
		// NFFactory 사용법
		////////////////////////////////////////////////////////////////////////
		// 정의
		NFFactory<NFObject> factory;
		// 등록
		factory.Register("UIFactoryItem", new NFFactoryCreator<UIFactoryItem, NFObject>);    
		factory.Register("UIFactoryItem2", new NFFactoryCreator<UIFactoryItem2, NFObject>);

		// 사용
		// AutoReleasePool 을 테스트 하기 위해서
		NFObject* pEntity1 = factory.Create("UIFactoryItem");
		assert(!stricmp(pEntity1->ToString(), "UIFactoryItem"));
		LOG_IMPORTANT((L"UIFactoryItem : %d", pEntity1->GetHashCode()));

		NFObject* pEntity2 = factory.Create("UIFactoryItem2");
		assert(!stricmp(pEntity2->ToString(), "UIFactoryItem2"));
		LOG_IMPORTANT((L"UIFactoryItem2 : %d", pEntity2->GetHashCode()));

		// NFObject를 상속받았기 때문에 NFAutoreleasePool에 자동으로 등록된다. Pool이 삭제되면 삭제됨.
	}

	/////////////////////////////////////////////////////////
	// 오토릴리즈 속도 테스트
	// 속도 테스트 NFAutoreleasePool 을 썼을때와 안썼을때 1만개의 객체를 생성삭제할때의 속도차
	// 결과는 마지막 프로파일러에서 출력된다.
	/////////////////////////////////////////////////////////
	PROFILE_PUSH(L"Autorelease");
	std::list<NFObject*> list;
	for(int xx = 0; xx < 10000; ++xx)
	{
		PROFILE_PUSH(L"NewObject");

		NFObject* p = new UIMemory;

		list.push_back(p);

//		LOG_IMPORTANT((L"New Object %d (0x%x) 번을 생성하였습니다.", xx, p));

		PROFILE_POP(L"NewObject");
	}
	LOG_IMPORTANT((L"Autorelease Pool : %d", pool->GetCount()));
	_DELETE(pool);

	PROFILE_POP(L"Autorelease");

	/////////////////////////////////////////////////////////
	// GetCatmullSplineValue 
	// 곡선을 그리기 위한 함수 샘플
	/////////////////////////////////////////////////////////
	if(1)
	{
		float3 p[4];
		p[0] = float3(0.0f, 0.0f, 0.0f);
		p[1] = float3(10.0f, 10.0f, 10.0f);
		p[2] = float3(20.0f, 20.0f, 20.0f);
		p[3] = float3(20.0f, 20.0f, 20.0f);

		float dt = 0.5f;

		float3 v = NFSpline::GetCatmullSplineValue(p[0], p[1], p[2], p[3], dt);
	}

	LOG_IMPORTANT((L"-----> Random Test"));

	if(1)
	{
		////////////////////////////////////////////////////////////////////////
		// Random 사용법
		////////////////////////////////////////////////////////////////////////
		NFRandomInt foo;
		for(int i = 0; i < 10; ++i)
		{
			LOG_IMPORTANT((L"Random%d : %d, %d", i, foo.Next(100), foo.Next(10,1000)));
		}

		/// 10만번 랜덤을 돌려서 랜덤값이 나중에 어떻게 분배가 됐는지 확인
		int rand;
		int RandCnt[100];
		ZeroMemory(RandCnt, sizeof(int)*100);

		PROFILE_PUSH(L"Random");
		int iCount = 10000000;
		for(int i = 0; i < iCount; ++i)
		{
			rand = foo.Next(100);
			RandCnt[rand]++;
		}
		PROFILE_POP(L"Random");

		int iMin = 0;
		int iMax = 0;
		for(int i = 0; i < 100; ++i)
		{
			if(RandCnt[i] == 0)
				continue;

			LOG_IMPORTANT((L"%03d : %d %f", i, RandCnt[i], (float)RandCnt[i]/iCount));
		}
	}

	/////////////////////////////////////////////////////////
	// XML Test 법
	/////////////////////////////////////////////////////////
	LOG_IMPORTANT((L"-----> XML Test"));
	if(1)
	{
		CConfig config;
		config.Load(L"ConfigSample.xml");

		LOG_IMPORTANT((L"Config Title : %s", config.GetTitle()));

		config.Save(L"test.xml");

		/// NFXMLScript 테스트 UIScript를 위해 제작된 샘플 일부 (미완성)
		NFXMLScript script;
		script.Load(L"uiscript.xml");
		Nave::String title;			
		script.GetHeader()->GetString(L"Title", title);
		/// uiscript.xml 에 있는 타이틀 없음 이란 문구가 출력되어야함.
		LOG_IMPORTANT((L"UIScript.xml Title : %s", title.c_str()));



		// 로드된 uiscript.xml 에서 다음과 같이 값을 수정.
		script.GetHeader()->SetItem(L"X", 200);
		script.GetHeader()->SetItem(L"Y", 200);
		script.GetHeader()->SetItem(L"Width", 320);
		script.GetHeader()->SetItem(L"Height", 240);
		script.GetHeader()->SetItem(L"Title", L"UIDialogTest");
		script.GetHeader()->SetItem(L"SkinFile", L"Config/Dialog1.skin");
		script.GetHeader()->SetItem(L"SkinID", 0);
		script.GetHeader()->SetItem(L"Visible", L"TRUE");

		// 새로운 버튼을 추가함.
		CRect rt(0, 0, 100, 100);
		CPoint pt(50, 50);
		float3 f3(0.1f, 0.2f, 0.3f);

		NFXMLScript::SCRIPTITEM item;
		item.SetItem(L"TYPE", L"BUTTON");
		item.SetItem(L"ID", 1001);
		item.SetItem(L"X", 0);
		item.SetItem(L"Y", 0);
		item.SetItem(L"Width", 200);
		item.SetItem(L"Height", 200);
		item.SetItem(L"Text", L"버튼2번");
		item.SetItem(L"SkinID", 1);
		item.SetItem(L"CRect", rt);
		item.SetItem(L"CPoint", pt);
		item.SetItem(L"float3", f3);

		script.AddAttribute(&item);

		// 저장. uitest.xml로 저장함.
		script.Save(L"uitest.xml");

	}
/*
	// NFInputManager Test
	LOG_IMPORTANT((L"-----> NFInputManager"));
	if(1)
	{
		NaveFramework::NFInputManager* pInput = new NaveFramework::NFInputManager();

		pInput->AddKeyMap(L"Event_LEFT",	KEY_LEFT   ,KEY_NONE  ,EKS_DOWN  ,0   , 10.000000);
		pInput->AddKeyMap(L"Event_RIGHT",	KEY_RIGHT  ,KEY_NONE  ,EKS_DOWN  ,1   ,-10.000000);
		pInput->AddKeyMap(L"Event_UP",		KEY_UP     ,KEY_NONE  ,EKS_DOWN  ,2   , 10.000000);
		pInput->AddKeyMap(L"Event_DOWN",	KEY_DOWN   ,KEY_NONE  ,EKS_DOWN  ,3   ,-10.000000);
		pInput->AddKeyMap(L"Event_JUMPS",	KEY_SPACE  ,KEY_NONE  ,EKS_DOWN  ,4   , 31.000000);
		pInput->AddKeyMap(L"Event_JUMPP",	KEY_SPACE  ,KEY_NONE  ,EKS_PRESS ,5   , 32.000000);
		pInput->AddKeyMap(L"Event_JUMPE",	KEY_SPACE  ,KEY_NONE  ,EKS_UP    ,6   , 33.000000);
		pInput->AddKeyMap(L"Event_ATTACK",	KEY_Z      ,KEY_NONE  ,EKS_UP    ,7   , 50.000000);
		pInput->AddKeyMap(L"Event_CHAGE",	KEY_Z      ,KEY_LALT  ,EKS_PRESS ,8   , 51.000000);
		pInput->AddKeyMap(L"Event_DOUBLE",	KEY_Z      ,KEY_X     ,EKS_PRESS ,9   ,100.000000);
		pInput->AddKeyMap(L"Event_ALT",		KEY_LALT   ,KEY_NONE  ,EKS_DOWN  ,10  ,101.000000);

		pInput->SaveInputMap();

		_DELETE(pInput);
	}
*/

	/// NFStringTable 테스트
	NaveFramework::NFStringTable::LoadSTable(L"Main", L"test.stb");

	Nave::String str = NaveFramework::NFStringTable::FindString(L"Main", L"ID_NONE1");
	LOG_IMPORTANT((L"String Table : %s", str.c_str()));

	NaveFramework::NFStringTable::ReleaseAll();


	// 프로파일 종료
	PROFILE_END();

	// 프로파일 출력
	LOG_IMPORTANT((L"-----> Profile Print"));
	PROFILE_PRINT(strBuff, 1024);
	LOG_IMPORTANT((strBuff));

	LOG_IMPORTANT((L"-----> Test End"));

	// 로그저장
	NFLog::CloseLog();

	LOG_IMPORTANT((L"아무키나 누르세요.."));
	getchar();

	return 0;
}
