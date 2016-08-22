/** 
 *  @file		NFImageManager.h
 *  @brief		NFImage를 관리하는 메니져 클래스
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-05
 */

#pragma once

#include <map>
#include <Nave/NFSingleton.h>
#include <Nave/NFSync.h>
#include "NFImage.h"

namespace Nave2D {

	/** 
	 *  @class        NFImageManager
	 *  @brief        Image를 로드하고 관리한다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFImageManager : public Nave::NFSingleton<NFImageManager>
	{
	public:
		/// NFImageManager 생성자
		NFImageManager(void);
		/// NFImageManager 소멸자
		~NFImageManager(void);

	public:
		/// 모든 객체를 삭제합니다.
		VOID		ReleaseAll();
		/**
		 * @brief	이미지를 검색합니다.
		 * @param dwID	이미지아이디
		 * @return	이미지포인터
		 */
		NFImage*	FindImage(DWORD dwID);

		/**
		 * @brief	새로운 이미지를 로드합니다.
		 * @param dwID	이미지아이디
		 * @param ScriptFile	스크립트 이름
		 * @param ImageFile		이미지 이름
		 * @return	이미지포인터
		 */
		NFImage*	AddFromFile(DWORD dwID, const WCHAR* ImageFile);
		/**
		 * @brief	새로운 이미지를 로드합니다.
		 * @param dwID			이미지아이디
		 * @param TextureID		텍스쳐인식아이디
		 * @param ScriptPoint	스크립트 포인터
		 * @param ScriptLen		스크립트 길이
		 * @param FilePoint		텍스쳐 포인터
		 * @param FileLen		텍스쳐 길이
		 * @return  이미지포인터
		 */
		NFImage*	AddFromMemory(DWORD dwID, const WCHAR* TextureID, char* FilePoint, DWORD FileLen);

	private:
		/**
		 * @brief	이미지 객체를 생성합니다.
		 * @param dwID	이미지 아이디
		 * @return  이미지포인터
		 */
		NFImage*		CreateImage(DWORD dwID);

	private:
		/// 이미지 맵
		std::map<DWORD, NFImage*>					m_ImageMap;
		/// 이미지 이터레이터
		typedef std::map<DWORD, NFImage*>::iterator	ImageIter;

		/// Sync 객체
		Nave::NFSync								m_Lock;

	};

}