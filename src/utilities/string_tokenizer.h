#pragma once
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

str_tokenizer& str_tokenizer::SetDelimiters(const std::vector<char>& delimiters)
{
	m_Delimiters = delimiters;
	return *this;
}

bool str_tokenizer::Tokenize(const std::string& str)
{
	m_Tokens.clear();
	std::string cur_token;

	for (auto c : str)
	{
		if (IsDelim(c))
		{
			AddToken(cur_token);
			cur_token = "";
			continue;
		}

		cur_token += c;
	}

	AddToken(cur_token);
	return true;
}

size_t str_tokenizer::GetTokensCount() const
{
	return m_Tokens.size();
}

#define _MAKE_PRIMITIVE_GET(type, conv_func)			\
template<>												\
bool str_tokenizer::Get(size_t index, type& out) const	\
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

void str_tokenizer::GetTokens(std::vector<std::string>& tokens) const
{
	for (auto& token : m_Tokens)
		tokens.push_back(token);
}

bool str_tokenizer::IsDelim(char c) const
{
	for (auto delim : m_Delimiters)
		if (delim == c)
			return true;

	return false;
}

void str_tokenizer::PostprocToken(std::string& token) const
{
	auto is_whitespace = [](char c)
	{
		return c == ' ' || c == '\n' || c == '\t';
	};

	auto rev_and_pop_whitespaces = [&]
	{
		std::reverse(token.begin(), token.end());

		while (is_whitespace(token.back()))
			token.pop_back();
	};

	rev_and_pop_whitespaces();
	rev_and_pop_whitespaces();
}

void str_tokenizer::AddToken(const std::string& token)
{
	m_Tokens.push_back(token);
	PostprocToken(m_Tokens.back());
}