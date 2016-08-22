/** 
 *  @file		NFFactory.h
 *  @brief		클래스생성을 자동화 해주는 ClassFactory
 *  @remarks	
 *  @author		강동명(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <map>
#include <string>

namespace Nave {

	typedef std::string FactoryType;

	/** 
	 *  @class        IFactoryItem
	 *  @brief        NFFactory에서 사용하는 클래스 직접 사용하지는 않는다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	template<class Base>
	class IFactoryItem
	{
	public:
		virtual ~IFactoryItem() {}
		/// 생성 함수
		virtual Base* Create() const = 0;
	};

	/** 
	 *  @class        NFFactoryCreator
	 *  @brief        NFFactory에 클래스를 등록하기 위한 객체이다.
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	template<class Product, class Base>
	class NFFactoryCreator : public IFactoryItem<Base>
	{
	public:
		/// 생성 함수
		virtual Base* Create() const {return new Product;}
	};

	/** 
	 *  @class        NFFactory
	 *  @brief        객체의 생성을 자동화해주는 클래스이다.
	 *  @remarks      NFFactory<NFObject> factory;											\r\n
	 *                // 등록																\r\n
	 *                factory.Register("NFView" ,new NFFactoryCreator<NFView, NFObject>);		\r\n
	 *                factory.Register("NFControl" ,new NFFactoryCreator<NFControl, NFObject>);	\r\n
	 *                // 사용																\r\n	
	 *                NFObject * pEntity1 = factory.Create("NFView");						\r\n
	 *                assert(!stricmp(pEntity1->ToString(), "NFView"));						\r\n
	 *																						\r\n
	 *                NFObject * pEntity2 = factory.Create("NFControl");					\r\n
	 *                assert(!stricmp(pEntity2->ToString(), "NFControl"));					\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-04
	 */
	template<class Base>
	class NFFactory
	{
	public:
		NFFactory()
		{
		}

		~NFFactory()
		{
			DeleteAllObject();		
		}

		/// Factory에 등록된 모든 객체를 삭제합니다.
		void DeleteAllObject()
		{
			CreatorMap::iterator obj = m_mapCreator.begin();
			CreatorMap::iterator end = m_mapCreator.end();

			while(obj != end)
			{
				IFactoryItem<Base> * pCreator = (*obj).second;
				(*obj).second = NULL;

				if(pCreator)
					delete pCreator;

				++obj;
			}

			m_mapCreator.clear();
		}

		/**
		 * @brief		Factory에 사용할 클래스 객체를 등록합니다.
		 * @param type		객체의 이름
		 * @param pCreator	객체 생성자 NFCreator로 생성
		 * @return		성공여부
		 */
		bool NFFactory<Base>::Register(FactoryType type, IFactoryItem<Base>* pCreator)
		{
			CreatorMap::iterator it = m_mapCreator.find(type);
			if (it != m_mapCreator.end()) {
				delete pCreator;
				return false;
			}
			m_mapCreator[type] = pCreator;
			return true;
		}

		/**
		 * @brief		Factory에 등록된 객체를 생성합니다.
		 * @param type	생성할객체의 이름
		 * @return		생성된 객체 포인터
		 */
		Base * NFFactory<Base>::Create(FactoryType type)
		{
			CreatorMap::iterator it = m_mapCreator.find(type);
			if (it == m_mapCreator.end()) 
				return NULL;

			IFactoryItem<Base> * pCreator = (*it).second;
			return pCreator->Create();
		}

	private:
		typedef std::map<FactoryType, IFactoryItem<Base>* > CreatorMap;
		/// Factory에서 사용할 Creator 객체 저장소
		CreatorMap m_mapCreator;
	};
}