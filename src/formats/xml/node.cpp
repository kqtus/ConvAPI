#include "node.h"

xml::attribute::attribute()
{
}

xml::attribute::attribute(const std::string& key, const std::string& value)
	: key(key)
	, value(value)
{
}

void xml::attribute::SetKey(const std::string& key)
{
	this->key = key;
}

std::string xml::attribute::GetKey() const
{
	return this->key;
}

void xml::attribute::SetValue(const std::string& value)
{
	this->value = value;
}

std::string xml::attribute::GetValue() const
{
	return this->value;
}

bool xml::attribute::Write(out_stream<EStreamType::TEXT>& stream)
{
	// #TODO: Implement writing
	return false;
}

const char* xml::node::DEFAULT_NODE_NAME = "xml_node";
const char* EMPTY_VALUE = "";

xml::node::node()
	: name(DEFAULT_NODE_NAME)
	, indent(0)
	, map()
{
}

xml::node::node(const std::string& name, int indent)
	: node(name, "")
{
}

xml::node::node(const std::string& name, const std::string& value, int indent)
	: name(name)
	, value(value)
	, indent(indent)
	, map()
{
}

void xml::node::SetName(const std::string& name)
{
	this->name = name;
}

std::string xml::node::GetName() const
{
	return this->name;
}

void xml::node::AddChild(node* child)
{
	subnodes.push_back(child);
}

xml::node* xml::node::GetChild(const size_t& pos) const
{
	return subnodes[pos];
}

bool xml::node::HasChildren() const
{
	return !subnodes.empty();
}

bool xml::node::HasValue() const
{
	return !value.empty();
}

std::string xml::node::GetIndentStr() const
{
	std::string indent_str;
	for (int i = 0; i < indent * 4; i++)
	{
		indent_str += " ";
	}
	return indent_str;
}

void xml::node::UpdateIndentsForSubnodes()
{
	for (auto& child : subnodes)
	{
		child->indent = this->indent + 1;
		child->UpdateIndentsForSubnodes();
	}
}

bool xml::node::Write(out_stream<EStreamType::TEXT>& stream)
{
	if (stream.Tell() == 0)
	{
		xml::node::WriteDeclaration(stream);
	}

	WRITE_STR(stream, (GetIndentStr() + "<"));
	WRITE_STR(stream, name);

	for (auto attribute : *this)
	{
		WRITE_CSTR(stream, " ");
		WRITE_STR(stream, attribute.first);
		WRITE_CSTR(stream, "=\"");
		WRITE_STR(stream, attribute.second);
		WRITE_CSTR(stream, "\"");
	}

	if (!HasChildren() && !HasValue())
	{
		WRITE_CSTR(stream, "/>\n");
	}
	else
	{
		WRITE_CSTR(stream, ">");

		if (HasValue())
		{
			WRITE_STR(stream, value);
		}
		else
		{
			WRITE_CSTR(stream, "\n");
			for (auto child : subnodes)
			{
				child->Write(stream);
			}
			WRITE_STR(stream, GetIndentStr());
		}
		WRITE_STR(stream, ("</" + name + ">\n"));
	}
	
	return true;
}

xml::node xml::node::Declaration()
{
	node decl_node("xml");
	decl_node["version"] = "1.0";
	decl_node["encoding"] = "UTF-8";
	decl_node["standalone"] = "no";
	return decl_node;
}

bool xml::node::WriteDeclaration(out_stream<EStreamType::TEXT>& stream)
{
	auto decl_node = xml::node::Declaration();
	WRITE_CSTR(stream, "<?");
	WRITE_STR(stream, decl_node.name);

	for (auto attribute : decl_node)
	{
		WRITE_CSTR(stream, " ");
		WRITE_STR(stream, attribute.first);
		WRITE_CSTR(stream, "=\"");
		WRITE_STR(stream, attribute.second);
		WRITE_CSTR(stream, "\"");
	}

	WRITE_CSTR(stream, "?>\n");
	return true;
}