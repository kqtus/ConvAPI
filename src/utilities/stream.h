#pragma once
#include "utilities_pch.h"
#include <string>
#include <cstdlib>

enum class EStreamType
{
	BINARY,
	TEXT
};

template<EStreamType stream_type>
class stream
{
public:
	stream();
	~stream();

	virtual bool Open(const char* file_name) = 0;
	virtual bool Open(const wchar_t* file_name) = 0;
	virtual void Close();

	void Seek(size_t pos, size_t offset);
	size_t Tell() const;
	bool Eof() const;

	std::wstring GetFileName() const;

protected:
	FILE* m_File;
	std::wstring m_FileName;
};

template<EStreamType stream_type>
inline stream<stream_type>::stream()
	: m_File(nullptr)
	, m_FileName(L"")
{
}

template<EStreamType stream_type>
inline stream<stream_type>::~stream()
{
	//fclose(m_File);
}

template<EStreamType stream_type>
inline void stream<stream_type>::Close()
{
	fclose(m_File);
}

template<EStreamType stream_type>
inline void stream<stream_type>::Seek(size_t offset, size_t origin)
{
	fseek(m_File, offset, origin);
}

template<EStreamType stream_type>
inline size_t stream<stream_type>::Tell() const
{
	return ftell(m_File);
}

template<EStreamType stream_type>
inline bool stream<stream_type>::Eof() const
{
	return feof(m_File);
}

template<EStreamType stream_type>
inline std::wstring stream<stream_type>::GetFileName() const
{
	return m_FileName;
}

template<EStreamType stream_type>
class in_stream : public stream<stream_type>
{
public:
	bool Open(const char* file_name) override;
	bool Open(const wchar_t* file_name) override;

	template<class T>
	T Read();
};

template<EStreamType stream_type>
inline bool in_stream<stream_type>::Open(const char* file_name)
{
	mbtowc((wchar_t*)m_FileName.c_str(), file_name, strlen(file_name));

	if (m_File)
	{
		fclose(m_File);
	}

	m_File = fopen(file_name, [&]() -> const char*
	{
		switch (stream_type)
		{
		case EStreamType::BINARY:
			return "rb";
		case EStreamType::TEXT:
			return "r";
		}
	}());

	return m_File != nullptr;
}

template<EStreamType stream_type>
inline bool in_stream<stream_type>::Open(const wchar_t* file_name)
{
	m_FileName = file_name;
	if (m_File)
	{
		fclose(m_File);
	}

	m_File = _wfopen(file_name, [&]() -> const wchar_t*
	{
		switch (stream_type)
		{
		case EStreamType::BINARY:
			return L"rb";
		case EStreamType::TEXT:
			return L"r";
		}
	}());

	return m_File != nullptr;
}

#define _READ_STREAM(strm, T) \
[&] { \
	T ret = 0; fread((void*)&ret, sizeof(T), 1, strm); \
	return ret; \
}();

#define _PRIMITIVE_STREAM_READ_METHOD(ret_type) \
template<> \
template<> \
inline ret_type in_stream<EStreamType::BINARY>::Read() { return _READ_STREAM(m_File, ret_type); }

_PRIMITIVE_STREAM_READ_METHOD(int8_t);
_PRIMITIVE_STREAM_READ_METHOD(uint8_t);
_PRIMITIVE_STREAM_READ_METHOD(int16_t);
_PRIMITIVE_STREAM_READ_METHOD(uint16_t);
_PRIMITIVE_STREAM_READ_METHOD(int32_t);
_PRIMITIVE_STREAM_READ_METHOD(uint32_t);
_PRIMITIVE_STREAM_READ_METHOD(int64_t);
_PRIMITIVE_STREAM_READ_METHOD(uint64_t);
_PRIMITIVE_STREAM_READ_METHOD(float_t);
_PRIMITIVE_STREAM_READ_METHOD(double_t);

template<>
template<>
inline uint8_t in_stream<EStreamType::TEXT>::Read() { return _READ_STREAM(m_File, uint8_t); }

#define READ_VAR(strm, variable) variable = strm.Read<std::remove_reference<decltype(variable)>::type>();

#define READ_ARR(strm, arr, size) \
INIT_ARR(arr, size) \
for (int i = 0; i < size; i++) READ_VAR(strm, arr[i]);

#define READ_LINE(strm, str)			\
{										\
	uint8_t c;							\
	do { READ_VAR(strm, c); str += c; } \
	while (c != '\n' && c != '\0');		\
}

template<EStreamType stream_type>
class out_stream : public stream<stream_type>
{
public:
	bool Open(const char* file_name) override;
	bool Open(const wchar_t* file_name) override;

	template<class T>
	void Write(T* data);

	template<class T>
	void Write(T* data, size_t size);
};

template<EStreamType stream_type>
inline bool out_stream<stream_type>::Open(const char* file_name)
{
	mbtowc((wchar_t*)m_FileName.c_str(), file_name, strlen(file_name));

	if (m_File)
	{
		fclose(m_File);
	}

	m_File = fopen(file_name, [&]() -> const char*
	{
		switch (stream_type)
		{
		case EStreamType::BINARY:
			return "wb+";
		case EStreamType::TEXT:
			return "w+";
		}
	}());
	return m_File != nullptr;
}

template<EStreamType stream_type>
inline bool out_stream<stream_type>::Open(const wchar_t* file_name)
{
	m_FileName = file_name;
	if (m_File)
	{
		fclose(m_File);
	}

	m_File = _wfopen(file_name, [&]() -> const wchar_t*
	{
		switch (stream_type)
		{
		case EStreamType::BINARY:
			return L"wb+";
		case EStreamType::TEXT:
			return L"w+";
		}
	}());
	return m_File != nullptr;
}
#define _WRITE_STREAM(strm, data_to_write, T) \
[&] { \
	fwrite(data_to_write, sizeof(T), 1, strm); \
}();

#define _PRIMITIVE_STREAM_WRITE_METHOD(T) \
template<> \
template<> \
inline void out_stream<EStreamType::BINARY>::Write(T* data) { _WRITE_STREAM(m_File, data, T); }

_PRIMITIVE_STREAM_WRITE_METHOD(int8_t);
_PRIMITIVE_STREAM_WRITE_METHOD(uint8_t);
_PRIMITIVE_STREAM_WRITE_METHOD(int16_t);
_PRIMITIVE_STREAM_WRITE_METHOD(uint16_t);
_PRIMITIVE_STREAM_WRITE_METHOD(int32_t);
_PRIMITIVE_STREAM_WRITE_METHOD(uint32_t);
_PRIMITIVE_STREAM_WRITE_METHOD(int64_t);
_PRIMITIVE_STREAM_WRITE_METHOD(uint64_t);
_PRIMITIVE_STREAM_WRITE_METHOD(float_t);
_PRIMITIVE_STREAM_WRITE_METHOD(double_t);

#define WRITE_VAR(strm, variable) strm.Write(&variable);

#define WRITE_ARR(strm, arr, size) \
for (int i = 0; i < size; i++) WRITE_VAR(strm, arr[i]);


template<>
template<>
inline void out_stream<EStreamType::TEXT>::Write(const void* data, size_t size)
{
	fwrite(data, size, 1, m_File);
}

template<>
template<>
inline void out_stream<EStreamType::BINARY>::Write(const void* data, size_t size)
{
	fwrite(data, size, 1, m_File);
}

template<>
template<>
inline void out_stream<EStreamType::TEXT>::Write(std::string* data)
{
	fwrite(&((*data)[0]), data->size(), sizeof(char), m_File);
}

template<>
template<>
inline void out_stream<EStreamType::BINARY>::Write(std::string* data)
{
	fwrite(&((*data)[0]), data->size(), sizeof(char), m_File);
}

#define WRITE_STR(strm, x) strm.Write(&((std::string)x));
#define WRITE_CSTR(strm, x) strm.Write(static_cast<const void*>(x), strlen(x));

