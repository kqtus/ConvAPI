#pragma once
#include "utilities_pch.h"

class stream
{
public:
	virtual bool Open(const char* file_name) = 0;
	virtual void Close();

protected:
	FILE* m_File;
};

inline void stream::Close()
{
	fclose(m_File);
}

class in_stream : public stream
{
public:
	bool Open(const char* file_name) override;
	void Seek(size_t pos, size_t offset);
	size_t Tell() const;

	template<class T>
	T Read();
};

inline bool in_stream::Open(const char* file_name)
{
	m_File = fopen(file_name, (const char*)"rb");
	return m_File != nullptr;
}

inline void in_stream::Seek(size_t offset, size_t origin)
{
	fseek(m_File, offset, origin);
}

inline size_t in_stream::Tell() const
{
	return ftell(m_File);
}

#define _READ_STREAM(strm, T) \
[&] { \
	T ret = 0; fread((void*)&ret, sizeof(T), 1, strm); \
	return ret; \
}();

#define _GEN_STREAM_READ_METHOD(ret_type) \
template<> \
inline ret_type in_stream::Read() { return _READ_STREAM(m_File, ret_type); }

_GEN_STREAM_READ_METHOD(int8_t);
_GEN_STREAM_READ_METHOD(uint8_t);
_GEN_STREAM_READ_METHOD(int16_t);
_GEN_STREAM_READ_METHOD(uint16_t);
_GEN_STREAM_READ_METHOD(int32_t);
_GEN_STREAM_READ_METHOD(uint32_t);
_GEN_STREAM_READ_METHOD(int64_t);
_GEN_STREAM_READ_METHOD(uint64_t);
_GEN_STREAM_READ_METHOD(float_t);
_GEN_STREAM_READ_METHOD(double_t);

#define READ_VAR(strm, variable) variable = strm.Read<std::remove_reference<decltype(variable)>::type>();

#define READ_ARR(strm, arr, size) \
INIT_ARR(arr, size) \
for (int i = 0; i < size; i++) READ_VAR(strm, arr[i]);

class out_stream : public stream
{
public:
	bool Open(const char* file_name) override;

	template<class T>
	void Write(T* data);

	template<class T>
	void Write(T* data, size_t size);
};

inline bool out_stream::Open(const char* file_name)
{
	return fopen_s(&m_File, file_name, "wb+");
}

#define _WRITE_STREAM(strm, data_to_write, T) \
[&] { \
	fwrite(data_to_write, sizeof(T), 1, strm); \
}();

#define _GEN_STREAM_WRITE_METHOD(T) \
template<> \
inline void out_stream::Write(T* data) { _WRITE_STREAM(m_File, data, T); }

_GEN_STREAM_WRITE_METHOD(int8_t);
_GEN_STREAM_WRITE_METHOD(uint8_t);
_GEN_STREAM_WRITE_METHOD(int16_t);
_GEN_STREAM_WRITE_METHOD(uint16_t);
_GEN_STREAM_WRITE_METHOD(int32_t);
_GEN_STREAM_WRITE_METHOD(uint32_t);
_GEN_STREAM_WRITE_METHOD(int64_t);
_GEN_STREAM_WRITE_METHOD(uint64_t);
_GEN_STREAM_WRITE_METHOD(float_t);
_GEN_STREAM_WRITE_METHOD(double_t);

template<>
inline void out_stream::Write(const void* data, size_t size)
{
	fwrite(data, size, 1, m_File);
}
