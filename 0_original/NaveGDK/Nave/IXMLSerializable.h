/** 
 *  @file		IXMLSerializable.h
 *  @brief		XMLSerializable Interface
 *  @remarks	XML�� struct, list, map, vector ����� ��ü�� �����ϱ� ���� Interface Ŭ���� 
 *  @author		������(edith2580@gmail.com)
 *  @date		2009-04-02
 */

namespace Nave {

	/// XMLStreamWriter Interface
	class IXMLStreamWriter;
	/// XMLStreamReader Interface
	class IXMLStreamReader;

	/** 
	 *  @class        ISerializable
	 *  @brief        Serializable Interface
	 *  @remarks      
	 *                
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	class ISerializable 
	{ 
	public:	
		/**
		 * @brief		Serialize 
		 * @param out	Write ����
		 */
		virtual void Serialize(IXMLStreamWriter& out) const=0;
		/**
		 * @brief		Deserialize
		 * @param in	Read ����
		 */
		virtual void Deserialize(IXMLStreamReader& in)=0;
		virtual ~ISerializable() {}
	};

	/** 
	 *  @class        ArraySerialize
	 *  @brief        ArraySerialize Helper
	 *  @warning      Write �ϱ� ���� ��ü�̱� ������ Deserialize�� ������� �ʴ´�
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class ArraySerialize : public ISerializable 
	{
	public:
		/**
		 * @brief	ArraySerialize ������ list,vector���� ���
		 * @param pBuffer	����
		 * @param pSize		������
		 */
		ArraySerialize(const T* pBuffer, unsigned int pSize) : buffer(pBuffer), size(pSize) 
		{
		}

		/**
		 * @brief	Write
		 * @param out Write ����
		 */
		virtual void Serialize(IXMLStreamWriter& out) const 
		{
			out.Begin(L"array");
			for (unsigned int i=0; i<size; ++i) 
			{
				out.Write(L"item",buffer[i]);
			}
		}

		virtual void Deserialize(IXMLStreamReader&) 
		{ 
			assert (FALSE); 
		}

	private:
		const T* buffer;
		unsigned int size;
	};

	/** 
	 *  @class        ArrayDeserialize
	 *  @brief        ArrayDeserialize Helper
	 *  @warning      Read �ϱ� ���� ��ü�̱� ������ Serialize�� ������� �ʴ´�
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class ArrayDeserialize : public ISerializable 
	{
	public:
		/**
		 * @brief	ArrayDeserialize ������ list,vector���� ���
		 * @param pBuffer	����
		 * @param pSize		������
		 */
		ArrayDeserialize(T* pBuffer, unsigned int pSize) : buffer(pBuffer), size(pSize) 
		{
		}

		virtual void Serialize(IXMLStreamWriter&) const 
		{
			assert(FALSE);
		}

		/**
		 * @brief	Read
		 * @param in Read ����
		 */
		virtual void Deserialize(IXMLStreamReader& in) 
		{
			in.Begin(L"array");
			size_t fsize=in.GetCount(L"item");

			if (fsize >= size) 
			{
				if ( fsize != size) 
					throw Exception((L"static array being deserialized has smaller size on file"));
			} 
			else 
			{
				throw Exception((L"static array being deserialized has smaller size on file"));
			}

			for (unsigned int i=0; i<size; ++i) 
			{
				T value;
				in.Read(L"item",value);
				buffer[i]=value;
			}
		}

	private:
		T* buffer;
		unsigned int size;
	};

	/** 
	 *  @class        ContainerSerialize
	 *  @brief        ContainerSerialize Helper
	 *  @warning      Write �ϱ� ���� ��ü�̱� ������ Deserialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class ContainerSerialize : public ISerializable 
	{
	public:
		/**
		 * @brief	ContainerSerialize ������
		 * @param pContainer �����̳� Ÿ��
		 */
		ContainerSerialize<T>(const T& pContainer) : container(pContainer) 
		{
		}

		/**
		 * @brief	Write 
		 * @param out Write ����
		 */
		virtual void Serialize(IXMLStreamWriter& out) const 
		{
			out.Begin(L"array");
			int counter=0;

			for (T::const_iterator it=container.begin(); it != container.end(); ++it, ++counter) 
			{
				out.Write(L"item", *it);
			}
		}

		virtual void Deserialize(IXMLStreamReader&) 
		{ 
			assert (FALSE); 
		}

	private:
		const T& container;
	};

	/** 
	 *  @class        ContainerDeserialize
	 *  @brief        ContainerDeserialize Helper
	 *  @warning      Read �ϱ� ���� ��ü�̱� ������ Serialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class ContainerDeserialize : public ISerializable 
	{
	public:
		/**
		 * @brief	ContainerDeserialize ������
		 * @param pContainer �����̳� Ÿ��
		 */
		ContainerDeserialize<T>(T& pContainer) : container(pContainer) 
		{
		}

		virtual void Serialize(IXMLStreamWriter&) const 
		{ 
			assert (FALSE); 
		}

		/**
		 * @brief	Read
		 * @param in Read ����
		 */
		virtual void Deserialize(IXMLStreamReader& in) 
		{ 
			in.Begin(L"array");
			size_t size=in.GetCount(L"item");

			for (size_t i=0; i<size; ++i) 
			{
				T::value_type value;
				in.Read(L"item",value);
				container.push_back(value);
			}
		}

	private:
		T& container;
	};

	/** 
	 *  @class        Pair
	 *  @brief        Pair Helper
	 *  @warning	  key, value Ÿ���� ���� ��ü�� ��� ex) std::map      
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T1, typename T2> class Pair 
	{
	public:
		Pair<T1,T2>(const T1& key, const T2& value) : Value(key,value) 
		{
		}

		Pair<T1,T2>() 
		{
		}

		/**
		 * @brief	Pair ��ü�� �����Ѵ�.
		 * @param out Write ��ü
		 */
		virtual void Serialize(IXMLStreamWriter& out) const 
		{
			out.Begin(L"pair");
			out.Write(L"key", Value.first);
			out.Write(L"value", Value.second);

		}

		/**
		 * @brief	Pair ��ü�� �ε��Ѵ�.
		 * @param in Read ��ü
		 */
		virtual void Deserialize(IXMLStreamReader& in) 
		{
			in.Begin(L"pair");
			in.Read(L"key", Value.first);
			in.Read(L"value", Value.second);
		}

	public:
		std::pair<T1,T2> Value;
	};

	/** 
	 *  @class        KeyedContainerSerialize
	 *  @brief        KeyedContainerSerialize Helper
	 *  @warning      Write �ϱ� ���� ��ü�̱� ������ Deserialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class KeyedContainerSerialize : public ISerializable 
	{
	public:
		/**
		 * @brief	KeyedContainerSerialize ������
		 * @param pContainer �����̳� Ÿ��
		 */
		KeyedContainerSerialize<T>(const T& pContainer) : container(pContainer) 
		{
		}

		/**
		 * @brief	Write 
		 * @param out Write ����
		 */
		virtual void Serialize(IXMLStreamWriter& out) const 
		{
			out.Begin(L"associative_array");
			int counter=0;

			for (T::const_iterator it=container.begin(); it != container.end(); ++it, ++counter) 
			{
				out.Write(L"item", Pair<T::key_type,T::mapped_type>(it->first, it->second));
			}
		}

		virtual void Deserialize(IXMLStreamReader&)
		{ 
			assert (FALSE); 
		}

	private:
		const T& container;
	};

	/** 
	 *  @class        KeyedContainerDeserialize
	 *  @brief        KeyedContainerDeserialize Helper
	 *  @warning      Read �ϱ� ���� ��ü�̱� ������ Serialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	 */
	template <typename T> class KeyedContainerDeserialize : public ISerializable 
	{
	public:
		/**
		 * @brief	KeyedContainerDeserialize ������
		 * @param pContainer �����̳� Ÿ��
		 */
		KeyedContainerDeserialize<T>(T& pContainer) : container(pContainer) 
		{
		}

		virtual void Serialize(IXMLStreamWriter&) const 
		{ 
			assert (FALSE); 
		}

		/**
		 * @brief	Read
		 * @param in Read ����
		 */
		virtual void Deserialize(IXMLStreamReader& in) 
		{
			in.Begin(L"associative_array");
			size_t size=in.GetCount(L"item");

			for (size_t i=0; i<size; ++i) 
			{
				Pair<T::key_type,T::mapped_type> pair;
				in.Read(L"item",pair);
				container.insert(pair.Value);
			}

		}

	private:
		T& container;
	};

	/**
	 *  @class		WriterWrapper
	 *  @brief		WriterWrapper Helper
	 *  @warning    Write �ϱ� ���� ��ü�̱� ������ Deserialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	*/
	template <typename T> struct WriterWrapper : ISerializable 
	{
	public:		
		WriterWrapper<T>(T& value) : Value(value) 
		{
		}

		virtual void Serialize(IXMLStreamWriter& out) const 
		{ 
			Value.Serialize(out); 
		}
		
		virtual void Deserialize(IXMLStreamReader& ) 
		{ 
			assert(FALSE);
		}

	public:
		T& Value;
	};

	/**
	 *  @class		ReaderWrapper
	 *  @brief		ReaderWrapper Helper
	 *  @warning    Read �ϱ� ���� ��ü�̱� ������ Serialize�� ������� �ʴ´�.
	 *  @par          
	 *  @author  Edith
	 *  @date    2009-04-03
	*/
	template <typename T> struct ReaderWrapper : ISerializable 
	{
	public:		
		ReaderWrapper<T>(T& value) : Value(value) 
		{
		}

		virtual void Serialize(IXMLStreamWriter&) const 
		{
			assert(FALSE); 
		}
		
		virtual void Deserialize(IXMLStreamReader& in) 
		{
			Value.Deserialize(in);
		}

	public:
		T& Value;
	};

}

