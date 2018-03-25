#pragma once
#include <string>
#include "utilities/stream.h"
#include <map>

enum class ELogKind
{
	WARNING,
	ERROR,
	INFO
};

std::map<ELogKind, std::string> LogKindLabels
{
	{ ELogKind::WARNING, "WARNING" },
	{ ELogKind::ERROR, "ERROR" },
	{ ELogKind::INFO, "INFO" }
};

class CLogger
{
public:
	CLogger(const std::string& file_path);
	~CLogger();

	bool Init();
	void WriteLine(const ELogKind& log_kind, const std::string& message);

protected:
	void WriteInitLogInfo();
	void WriteEntryInfo(const ELogKind& log_kind);
	void WriteCurrentTime();
	void WriteEndl();
	void Write(const std::string& token);
	

	std::string m_LogFilePath;
	out_stream<EStreamType::TEXT>* m_LogStream;
};