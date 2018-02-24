#pragma once

struct rgba
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

template<class T>
struct vec2
{
	T x, y;
};

template<class T>
struct vec3 : public vec2<T>
{
	T z;
};

template<class T>
struct vec4 : public vec3<T>
{
	T w;
};

template<int dim_size, class TValue>
class sq_mat
{
private:
	TValue** buf;

public:
	sq_mat()
	{
		buf = new TValue*[dim_size];
		for (int i = 0; i < dim_size; i++)
		{
			buf[i] = new TValue[dim_size];
			// #TODO Add memset for zeroing memory
		}
	}

	~sq_mat()
	{
		for (int i = 0; i < dim_size; i++)
		{
			delete[] buf[i];
		}
		delete[] buf;
	}

	TValue*& operator[](size_t index)
	{
		return buf[index];
	}
};

template<class TValue>
using mat3x3 = sq_mat<3, TValue>;

template<class TValue>
using mat4x4 = sq_mat<4, TValue>;