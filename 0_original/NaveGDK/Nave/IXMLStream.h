/** 
 *  @file		IXMLStream.h
 *  @brief		XMLStream Interface
 *  @remarks	XML�� ����Ÿ�� �����ϱ� ���� Interface Ŭ����
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */
#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>

#include "IXMLSerializable.h"

namespace Nave { 

	#pragma warning( push )
	#pragma warning( disable:4512 ) 

	/** 
	 *  @class        IXMLStreamWriter
	 *  @brief        XMLStreamWriter Interface
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class IXMLStreamWriter
	{
	public:
		virtual ~IXMLStreamWriter() {}

		/// ���ο� XML �ε带 �����մϴ�.
		virtual void NewNode(const WCHAR* name)=0;
		/// XML�ε带 �����մϴ�.
		virtual void Begin(const WCHAR* type)=0;
		/// ���� �ε带 �����մϴ�.
		virtual void End()=0; 
		/// signed short Write
		virtual void Write(const WCHAR* name, signed short value)=0;
		/// unsigned short Write
		virtual void Write(const WCHAR* name, unsigned short value)=0;
		/// int Write
		virtual void Write(const WCHAR* name, int value)=0;
		/// unsigned int Write
		virtual void Write(const WCHAR* name, unsigned int value)=0;
		/// int Write
		virtual void Write(const WCHAR* name, long value)=0;
		/// unsigned int Write
		virtual void Write(const WCHAR* name, unsigned long value)=0;
		/// double Write
		virtual void Write(const WCHAR* name, double value)=0;
		/// float Write
		virtual void Write(const WCHAR* name, float value)=0;
		/// float2 Write
		virtual void Write(const WCHAR* name, const float2& value)=0;
		/// float3 Write
		virtual void Write(const WCHAR* name, const float3& value)=0;
		/// float4 Write
		virtual void Write(const WCHAR* name, const float4& value)=0;
		/// unicode string Write
		virtual void Write(const WCHAR* name, const WCHAR* buffer)=0;
		/// multibyte string Write
		virtual void Write(const WCHAR* name, const CHAR* buffer)=0;
		/// ISerializable Write
		virtual void Write(const WCHAR* name, const ISerializable& value)=0;
		/// std::string Write
		virtual void Write(const WCHAR* name, const std::string& value)
		{ 
			Write(name,value.c_str());
		}
		/// std::wstring Write
		virtual void Write(const WCHAR* name, const std::wstring& value) 
		{ 
			Write(name,value.c_str());
		}

		/// template type Write
		template <typename T> void Write(const WCHAR* name,const T& value) 
		{
			WriterWrapper<const T> helper(value);
			Write(name,static_cast<const ISerializable&>(helper));
		}

		/// Array type Write
		template <typename T> void WriteArray(const WCHAR* name, const T* array, unsigned int size) 
		{
			ArraySerialize<T> helper(array,size);
			Write(name,helper);
		}

		/**
		 * @brief	write-methods for the std::vector containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T> void Write(const WCHAR* name, const std::vector<T>& value)
		{ 
			writeContainer(name,value);	
		}

		/**
		 * @brief	write-methods for the std::list containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T> void Write(const WCHAR* name, const std::list<T>& value) 
		{ 
			writeContainer(name,value);	
		}

		/**
		 * @brief	write-methods for the std::map containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T1, typename T2> void Write(const WCHAR* name, const std::map<T1,T2>& value) 
		{ 
			writeKeyedContainer(name,value);	
		}

	private:
		/**
		 * @brief	write-methods for the containers type
		 * @param name name
		 * @param container container
		 */
		template <typename T> void writeContainer(const WCHAR* name, const T& container) 
		{
			ContainerSerialize<T> helper(container);
			Write(name,helper);
		}

		/**
		 * @brief	write-methods for the key containers type
		 * @param name name
		 * @param container container 
		 */
		template <typename T> void writeKeyedContainer(const WCHAR* name, const T& container) 
		{
			KeyedContainerSerialize<T> helper(container);
			Write(name,helper);
		}
	};

	/**
	 @class		IXMLStreamReader
	 @author	������(edith2580@gmail.com)
	 @date		2009-03-03
	 @brief		IXMLStreamReader Interface
	*/
	class IXMLStreamReader
	{
	public:
		virtual ~IXMLStreamReader() {}

		/// �ش� name�� �����ϴ��� Ȯ��
		virtual BOOL Exists(const WCHAR* name) const=0;
		/// �ش� name�� ChileNode�� ����
		virtual size_t GetCount(const WCHAR* name) const=0;
		/// ��带 �˻��Ѵ�.
		virtual BOOL FindNode(const WCHAR* name)=0;
		/// XML�ε带 �����մϴ�.
		virtual BOOL Begin(const WCHAR* type)=0;
		/// XML�ε带 �����մϴ�.
		virtual void End()=0;
		/// signed short Read
		virtual void Read(const WCHAR* name, signed short& value)=0;
		/// unsigned short Read
		virtual void Read(const WCHAR* name, unsigned short& value)=0;
		/// int Read
		virtual void Read(const WCHAR* name, int& value)=0;
		/// unsigned int Read
		virtual void Read(const WCHAR* name, unsigned int& value)=0;
		/// int Read
		virtual void Read(const WCHAR* name, long& value)=0;
		/// unsigned int Read
		virtual void Read(const WCHAR* name, unsigned long& value)=0;
		/// double Read
		virtual void Read(const WCHAR* name, double& value)=0;
		/// float Read
		virtual void Read(const WCHAR* name, float& value)=0;
		/// float2 Read
		virtual void Read(const WCHAR* name, float2& value)=0;
		/// float3 Read
		virtual void Read(const WCHAR* name, float3& value)=0;
		/// float4 Read
		virtual void Read(const WCHAR* name, float4& value)=0;
		/// unicode string Read
		virtual unsigned int Read(const WCHAR* name, WCHAR* buffer, unsigned int size)=0;
		/// multibyte string Read
		virtual unsigned int Read(const WCHAR* name, CHAR* buffer, unsigned int size)=0;
		/// ISerializable Read
		virtual void Read(const WCHAR* name, ISerializable& value)=0;

		/// std::string Read
		virtual void Read(const WCHAR* name, std::string& value)
		{
			CHAR tmp[MAXSTRING];
			int readed=Read(name,tmp,MAXSTRING);
			assert(readed < MAXSTRING);
			value=tmp;
		}

		/// std::wstring Read
		virtual void Read(const WCHAR* name, std::wstring& value)
		{
			WCHAR tmp[MAXSTRING];
			int readed=Read(name,tmp,MAXSTRING);
			assert(readed < MAXSTRING);
			value=tmp;
		}

		/// template type Read
		template <typename T> void Read(const WCHAR* name,T& value) 
		{
			ReaderWrapper<T> helper(value);

			Read(name,static_cast<ISerializable&>(helper));
		}

		/// Array type Read
		template <typename T> void ReadArray(const WCHAR* name,T* array, unsigned int size) 
		{
			if ( GetType() == Type::Flat) 
			{
				PrivateFlat::ArrayDeserialize<T> helper(array,size);
				Read(name,helper);
			} 
			else 
			{
				ArrayDeserialize<T> helper(array,size);
				Read(name,helper);
			}
		}

		/**
		 * @brief	read-methods for the std::vector containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T> void Read(const WCHAR* name, std::vector<T>& value) 
		{ 
			readContainer(name,value);	
		}

		/**
		 * @brief	read-methods for the std::list containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T> void Read(const WCHAR* name, std::list<T>& value) 
		{ 
			readContainer(name,value);	
		}

		/**
		 * @brief	read-methods for the std::map containers
		 * @param name name
		 * @param value  value
		 */
		template <typename T1, typename T2> void Read(const WCHAR* name, std::map<T1,T2>& value) 
		{ 
			readKeyedContainer(name,value); 
		}

		/**
		 * @brief	read-methods for the template containers
		 * @param name name
		 * @param value value
		 * @param defaultValue defaultValue
		 */
		template <typename T> void Read(const WCHAR* name, T& value, const T& defaultValue) 
		{
			if (Exists(name)) 
			{
				Read(name,value);
			} 
			else 
			{
				value=defaultValue;
			}
		}

	private:
		enum 
		{ 
			MAXSTRING = 1024 /// �ִ� String ����
		};

		/**
		 * @brief	read-methods for the containers type
		 * @param name name
		 * @param container container 
		 */
		template <typename T> void readContainer(const WCHAR* name, T& container) 
		{
			ContainerDeserialize<T> helper(container);
			Read(name,helper);
		}

		/**
		 * @brief	read-methods for the key containers type
		 * @param name name
		 * @param container container 
		 */
		template <typename T> void readKeyedContainer(const WCHAR* name, T& container) 
		{
			KeyedContainerDeserialize<T> helper(container);
			Read(name,helper);
		}
	};

	#pragma warning( pop ) // recover warning state

}