#ifndef _STR_TOKENIZER
#define _STR_TOKENIZER

#include <vector>
#include <algorithm>
#include <string>

class str_tokenizer
{
public:
	str_tokenizer& SetDelimiters(const std::vector<char>& delimiters);

	bool Tokenize(const std::string& str);

	size_t GetTokensCount() const;

	template<class TOut>
	bool Get(size_t index, TOut& out) const;

	void GetTokens(std::vector<std::string>& tokens) const;

protected:
	bool IsDelim(char c) const;
	void PostprocToken(std::string& token) const;
	void AddToken(const std::string& token);

	std::vector<std::string> m_Tokens;
	std::vector<char> m_Delimiters = { ',' };
};


#define _MAKE_PRIMITIVE_GET(type, conv_func)			\
template<>												\
inline bool str_tokenizer::Get(size_t index, type& out) const	\
{														\
	if (index >= m_Tokens.size())						\
		return false;									\
														\
	auto token = m_Tokens.at(index);					\
	try													\
	{													\
		out = conv_func(token);							\
	}													\
	catch(...) { return false; }						\
	return true;										\
}

#define _STR_NOP(str) str

_MAKE_PRIMITIVE_GET(std::string,		_STR_NOP);
//_MAKE_PRIMITIVE_GET(char,				std::stoi);
//_MAKE_PRIMITIVE_GET(unsigned char,	std::stoul);
_MAKE_PRIMITIVE_GET(int,				std::stoi);
_MAKE_PRIMITIVE_GET(unsigned int,		std::stoul);
_MAKE_PRIMITIVE_GET(long,				std::stol);
_MAKE_PRIMITIVE_GET(unsigned long,		std::stoul);
_MAKE_PRIMITIVE_GET(float,				std::stof);
_MAKE_PRIMITIVE_GET(double,				std::stod);
_MAKE_PRIMITIVE_GET(long long,			std::stoll);
_MAKE_PRIMITIVE_GET(unsigned long long, std::stoull);

#undef _STR_NOP
#undef _MAKE_PRIMITIVE_GET

#endif