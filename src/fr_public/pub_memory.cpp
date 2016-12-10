/**********************************************************
 *  \!file pub_memory.cpp
 *  \!brief
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "pub_memory.h"

#include <string.h>

using namespace std;

namespace Universal{
	BinaryMemory::BinaryMemory()
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{ ; }
	BinaryMemory::BinaryMemory(const void *_buffer, int64 _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{ 
		setBuffer(_buffer, _size);
	}
	BinaryMemory::BinaryMemory(const Byte *_buffer, int64 _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		int i = sizeof(Byte);
		setBuffer((void*)_buffer, _size * sizeof(Byte));
	}
	BinaryMemory::BinaryMemory(const char *_buffer, int64 _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		setBuffer((void*)_buffer, _size * sizeof(char));
	}
	BinaryMemory::BinaryMemory(const uint16 *_buffer, int64 _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		setBuffer((void*)_buffer, _size * sizeof(uint16));
	}
	BinaryMemory::BinaryMemory(const uint64 *_buffer, int64 _size)
		:m_Buffer(NULL),
		 m_MaxLimit(0xFFFFFFFF),
		 m_CurBufferSize(0),
		 m_MaxBufferSize(0)
	{
		setBuffer((void*)_buffer, _size * sizeof(uint64));
	}

	BinaryMemory::~BinaryMemory(){
		if(m_Buffer){
			free(m_Buffer);
			m_Buffer = NULL;
		}
	}

	BinaryMemory::BinaryMemory(const BinaryMemory &ref){
		if(m_Buffer != ref.getBuffer()){
			m_Buffer = NULL;
			m_CurBufferSize = 0;
			m_MaxBufferSize = 0;
		}
		setBuffer(ref.getBuffer(), ref.getBufferSize());
	}

	BinaryMemory& BinaryMemory::operator=(const BinaryMemory &ref){
		setBuffer(ref.getBuffer(), ref.getBufferSize());
		return *this;
	}

	BinaryMemory& BinaryMemory::operator+(const BinaryMemory &ref){
		addBuffer(ref.getBuffer(), ref.getBufferSize());
		return *this;
	}

	void BinaryMemory::setBuffer(const void *buffer, int64 size){
		if(buffer == NULL || size <= 0 || m_Buffer == buffer || size > m_MaxLimit){
			return;
		}
		m_CurBufferSize = size;
		if(m_Buffer){
			if(size > m_MaxBufferSize){
				m_MaxBufferSize = size;

				free(m_Buffer);
				m_Buffer = malloc(m_MaxBufferSize);
			}
		}
		else{
			m_MaxBufferSize = size;
			m_Buffer = malloc(m_MaxBufferSize);
		}
		memset(m_Buffer, 0, m_MaxBufferSize);
		memcpy(m_Buffer, buffer, m_CurBufferSize);
	}

	void BinaryMemory::addBuffer(const BinaryMemory &ref){
		addBuffer(ref.getBuffer(), ref.getBufferSize());
	}
	void BinaryMemory::addBuffer(const void *buffer, int64 size){
		if(buffer == NULL || size <= 0 || m_Buffer == buffer || size + m_CurBufferSize > m_MaxLimit){
			return;
		}

		if(m_Buffer){
			if(m_CurBufferSize + size > m_MaxBufferSize){
				void* pBufferTmp = malloc(m_MaxBufferSize);
				memcpy(pBufferTmp, m_Buffer, m_CurBufferSize);

				m_MaxBufferSize = (size + m_CurBufferSize) * 2;

				free(m_Buffer);
				m_Buffer = malloc(m_MaxBufferSize);
				memset(m_Buffer, 0, m_MaxBufferSize);
				memcpy(m_Buffer, pBufferTmp, m_CurBufferSize);

				free(pBufferTmp);
			}
		}
		else{
			m_CurBufferSize = 0;
			m_MaxBufferSize = size;
			m_Buffer = malloc(m_MaxBufferSize);
		}
		void* pAddPos = (char*)m_Buffer + m_CurBufferSize;
		memcpy(pAddPos, buffer, size);
		m_CurBufferSize += size;
	}

	void BinaryMemory::delBuffer(int64 start, int64 length){
		if(m_CurBufferSize > start && length > 0){
			length = length < (m_CurBufferSize - start) ? length : m_CurBufferSize - start;

			void* pMoveBuffer(NULL);
			int64 movePos = start + length;
			int64 moveLength = m_CurBufferSize - movePos;

			if(moveLength != 0){
				pMoveBuffer = malloc(moveLength);
				memcpy(pMoveBuffer, (char*)m_Buffer + movePos, moveLength);
			}

			memset((char*)m_Buffer + start, 0, m_CurBufferSize - start);

			if(pMoveBuffer != NULL){
				memcpy((char*)m_Buffer + start, pMoveBuffer, moveLength);
				free(pMoveBuffer); pMoveBuffer = NULL;
			}
			
			m_CurBufferSize -= length;
		}
	}

	void BinaryMemory::clearBuffer(){
		memset(m_Buffer, 0, m_CurBufferSize);
		m_CurBufferSize = 0;
	}

	void BinaryMemory::reserve(int64 size){
		if(m_MaxBufferSize < size &&  size <= m_MaxLimit){
			void* pSaveBuffer(NULL);
			if(m_CurBufferSize != 0){
				pSaveBuffer= (void*)malloc(m_CurBufferSize);
				memcpy(pSaveBuffer, m_Buffer, m_CurBufferSize);
			}
			if(m_Buffer != NULL){
				free(m_Buffer); m_Buffer = NULL;
			}

			m_Buffer = malloc(size);
			m_MaxBufferSize = size;
			memset(m_Buffer, 0, m_MaxBufferSize);

			if(pSaveBuffer != NULL && m_Buffer != NULL){
				memcpy(m_Buffer, pSaveBuffer, m_CurBufferSize);
				free(pSaveBuffer); pSaveBuffer = NULL;
			}
		}
	}

	void BinaryMemory::print(string expand)const{
		if(m_Buffer == NULL){
			return;
		}

		Byte* msg = (Byte*)m_Buffer;
		int64 index(0);
		ostringstream ostr;
		ostr << expand << " buffer is [";
		while(index < m_CurBufferSize){
			ostr << (uint16)(unsigned char)msg[index] << "|";
			++index;
		}
		ostr << "]";
		DEBUG_D(ostr.str());
	}

}

