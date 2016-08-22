/** 
 *  @file		NFThread.h
 *  @brief		Auto Thread ���� Ŭ����
 *  @remarks	
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

#pragma once

#include <windows.h>

namespace Nave { 

	/** 
	 *  @class        NFThread
	 *  @brief        ������ ��ü Ŭ����
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-05
	 */
	class NFThread
	{
	public:
		NFThread() : m_hThreadHandle(INVALID_HANDLE_VALUE) 
		{ 
		}
		
		~NFThread() 
		{ 
		}

		typedef unsigned int(__stdcall *LPThreadFunc)(void*);
		/// ������ �Լ�������
		static inline unsigned int __stdcall ThreadFunc(void* pArg);

		/// ������ �ڵ��� �����Ѵ�.
		inline HANDLE GetHandle() { return m_hThreadHandle; }
		/// ������ �ڵ��� �����Ѵ�.
		inline void SetHandle(HANDLE hHandle) { m_hThreadHandle = hHandle; }

		/// ���� ���� �Ǵ� ������ �ִ´�.
		virtual unsigned int Run() = 0;		
		/// ������ ���� �� �ִ� ��ƾ�� �ִ´�.
		virtual BOOL End() = 0;				

	private:
		/// �������ڵ�
		HANDLE	m_hThreadHandle;

//		friend class NFThreadManager;
	};

	/// ������ ȣ���Լ�
	inline unsigned int __stdcall NFThread::ThreadFunc(void* pArg)
	{
		return static_cast<NFThread*>(pArg)->Run();
	}
}