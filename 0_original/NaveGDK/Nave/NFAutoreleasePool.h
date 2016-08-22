/** 
 *  @file		NFAutoreleasePool.h
 *  @brief		AutoRelease Pool 
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <list>

namespace Nave { 
	/** 
	 *  @class        NFAutoreleasePool
	 *  @brief        NFObject�� ��ӹ��� ��ü�� �ڵ����� Release �ϱ� ���� Ŭ����
	 *  @remarks      Nave::NFAutoreleasePool pool = new Nave::NFAutoreleasePool();	\r\n
	 * 				  NFObject* pControl = new Nave::NFObject;						\r\n
	 * 				  pControl = new Nave::UIControl;								\r\n
	 * 				  _DELETE(pool);												\r\n
	 *                
	 *  @warning	NFObject�� ��ӹ��� ���� ��ü�� �� Ŭ�������� �������� ���Ѵ�.	\r\n
	 * 				1. NFObject�� ��ӹ��� ��ü�� NFSmartPtr�� ����� ��� �����	\r\n
	 * 				   ���� �浹�� ���� �ִ�. AutoreleasePool�� �����Ǳ�����		\r\n
	 * 				   NFSmartPtr ��ü�� �ʱ�ȭ �Ǹ� �̻��̾���.					\r\n
	 *																				\r\n
	 * 				2. AutoreleasePool �� ����Ұ�� �ӵ����ϰ� �ִ�.				\r\n
	 * 				   2048�� �޸𸮸� ���� class �� 1���� ����������				\r\n
	 * 				   AutoreleasePool�� ����ϸ� 1������ ��ü ������ 2.6��			\r\n
	 * 				   ������ AutoreleasePool�� ������� ������ 1���� ������		\r\n
	 * 				   1.6�ʷ� �ӵ��� Ȯ���� ���ϰ� �߻��ϰ� �ִ�.					\r\n
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class NFAutoreleasePool : public NFSingleton<NFAutoreleasePool>
	{
	public:
		/// NFAutoreleasePool ������
		NFAutoreleasePool();
		/// NFAutoreleasePool �Ҹ���
		~NFAutoreleasePool();

	public:
		/**
		 * @brief	Pool�� ����Ѵ�.
		 * @param id	�������̵�
		 * @param p		�޸� ������
		 * @param size	������
		 * @warning		NFObject�� new ���۷����Ϳ��� �ڵ����� ȣ���ϴ°����� ���� ȣ���ϴ°� �ƴϴ�
		 */
		void	Register(DWORD id, const void* p, DWORD size);

		/**
		 * @brief	Pool���� �����Ѵ�.
		 * @param id	�������̵�
		 * @param size	������
		 * @warning		NFObject�� delete ���۷����Ϳ��� �ڵ����� ȣ���ϴ°����� ���� ȣ���ϴ°� �ƴϴ�
		 */
		void	UnRegister(DWORD id, DWORD size);

		/**
		 * @brief	Pool�� ��ϵ� �������� ������ ���Ѵ�.
		 * @return  �� ������ ����
		 */
		const INT		GetCount() const;

	private:
		/// ������ �޸��� �ѻ�����
		DWORD				m_dwSize;
		/// AutoreleasePool�� ��뿩��
		BOOL				m_bRun;

		/// ������ ��ü�� ����Ʈ
		std::list<DWORD>	m_AutoPool;

		/// ���ͷ�����
		typedef std::list<DWORD>::iterator	IterAutoPool;
		/// const ���ͷ�����
		typedef std::list<DWORD>::const_iterator	ConstIterAutoPool; 
	};

}