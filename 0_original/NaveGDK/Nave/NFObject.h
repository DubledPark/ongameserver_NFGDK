/** 
 *  @file		NFObject.h
 *  @brief		Object Super Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

namespace Nave {

	/** 
	 *  @class       NFObject 
	 *  @brief		 Object�� Super Ŭ����
	 *  @remarks     AutoreleasePool���� �����Ǵ� ��ü�� �����ϱ�	\r\n
	 *               ���ؼ��� NFObject�� ��ӹ����� �ȴ�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFObject
	{
	public:
		/// NFObject ������
		NFObject();
		/// NFObject �Ҹ���
		virtual ~NFObject();

	public:
		/// new ���۷����� Ŭ���� 
		static void * operator new(size_t size);
		/// delete ���۷����� Ŭ���� 
		static void operator delete(void *p, size_t size);

		/// ��ü�� ������ �����Ѵ� ���� Ŭ���� �̸��� ������ ���ڿ��� �����Ѵ�.
	    virtual const char* ToString() { return "NFObject"; }

		/// ������Ʈ�� ���� ���̵��̴�.
		inline DWORD GetHashCode() { return m_dwHash; }

		/// ���� ��ü���� (Hash�ڵ带 ������ ���Ѵ�)
        virtual bool Equals(NFObject& obj);

		/// �ΰ��� ��ü�� ���� ��ü���� (Hash�ڵ带 ������ ���Ѵ�)
		static bool Equals(NFObject& objA, NFObject& objB);

	private:
		/// Object�� ���� Hash�ڵ� (�޸𸮰� ������ �޸�������)
		DWORD	m_dwHash;
	};
}

