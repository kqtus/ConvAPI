#include "string_tokenizer.h"

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
