#pragma once
#include "../formats_pch.h"

#include "../../utilities/interfaces.h"
#include "../../utilities/stream.h"

#include <map>
#include <string>
#include <vector>

namespace xml
{
	class attribute : public common::ITexWriteable
	{
		CONVERTIBLE_ENTITY
	public:
		attribute();
		attribute(const std::string& key, const std::string& value);

		void SetKey(const std::string& key);
		std::string GetKey() const;

		void SetValue(const std::string& value);
		std::string GetValue() const;

		bool Write(out_stream<EStreamType::TEXT>& stream) override;
	
	private:
		std::string key;
		std::string value;
	};

	class node : public std::map<std::string, std::string>, public common::ITexWriteable
	{
		CONVERTIBLE_ENTITY
	public:
		node();
		node(const std::string& name, int indent = 0);
		node(const std::string& name, const std::string& value, int indent = 0);

		void SetName(const std::string& name);
		std::string GetName() const;

		void AddChild(node child);
		node GetChild(const size_t& pos) const;

		bool HasChildren() const;
		bool HasValue() const;

		bool Write(out_stream<EStreamType::TEXT>& stream) override;

		static const char* DEFAULT_NODE_NAME;
		static const char* EMPTY_VALUE;
	private:
		std::string GetIndentStr() const;
		void UpdateIndentsForSubnodes();

		std::string name;
		std::string value;
		int indent;
		std::vector<node> subnodes;
	};
}