/** 
 *  @file		NFFactory.h
 *  @brief		Ŭ���������� �ڵ�ȭ ���ִ� ClassFactory
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <map>
#include <string>

namespace Nave {

	typedef std::string FactoryType;

	/** 
	 *  @class        IFactoryItem
	 *  @brief        NFFactory���� ����ϴ� Ŭ���� ���� ��������� �ʴ´�.
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
		/// ���� �Լ�
		virtual Base* Create() const = 0;
	};

	/** 
	 *  @class        NFFactoryCreator
	 *  @brief        NFFactory�� Ŭ������ ����ϱ� ���� ��ü�̴�.
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
		/// ���� �Լ�
		virtual Base* Create() const {return new Product;}
	};

	/** 
	 *  @class        NFFactory
	 *  @brief        ��ü�� ������ �ڵ�ȭ���ִ� Ŭ�����̴�.
	 *  @remarks      NFFactory<NFObject> factory;											\r\n
	 *                // ���																\r\n
	 *                factory.Register("NFView" ,new NFFactoryCreator<NFView, NFObject>);		\r\n
	 *                factory.Register("NFControl" ,new NFFactoryCreator<NFControl, NFObject>);	\r\n
	 *                // ���																\r\n	
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

		/// Factory�� ��ϵ� ��� ��ü�� �����մϴ�.
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
		 * @brief		Factory�� ����� Ŭ���� ��ü�� ����մϴ�.
		 * @param type		��ü�� �̸�
		 * @param pCreator	��ü ������ NFCreator�� ����
		 * @return		��������
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
		 * @brief		Factory�� ��ϵ� ��ü�� �����մϴ�.
		 * @param type	�����Ұ�ü�� �̸�
		 * @return		������ ��ü ������
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
		/// Factory���� ����� Creator ��ü �����
		CreatorMap m_mapCreator;
	};
}