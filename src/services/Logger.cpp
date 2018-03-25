#pragma once
#include "Logger.h"

#include <chrono>
#include <ctime>

CLogger::CLogger(const std::string& file_path)
	: m_LogFilePath(file_path)
	, m_LogStream(nullptr)
{
}

CLogger::~CLogger()
{
}

bool CLogger::Init()
{
	m_LogStream = new out_stream<EStreamType::TEXT>();

	if (!m_LogStream->Open(m_LogFilePath))
	{
		m_LogStream = nullptr;
		return false;
	}

	WriteInitLogInfo();
	return true;
}

void CLogger::WriteLine(const ELogKind& log_kind, const std::string& message)
{
	WriteEntryInfo(log_kind);
	Write(message);
	WriteEndl();
}

void CLogger::WriteInitLogInfo()
{
	WRITE_CSTR(m_LogStream, "----------------------------\n");
	WRITE_CSTR(m_LogStream, "--- Log\n");
	WriteEndl();
}

void CLogger::WriteEntryInfo(const ELogKind& log_kind)
{
	Write("[");
	Write(LogKindLabels[log_kind]);
	Write(" | ");
	WriteCurrentTime();
	Write("] ");
}

void CLogger::WriteCurrentTime()
{
	auto cur_time_point = std::chrono::system_clock::now();
	auto cur_time = std::chrono::system_clock::to_time_t(cur_time_point);
	WRITE_CSTR(m_LogStream, std::ctime(&cur_time));
}

void CLogger::WriteEndl()
{
	Write("\n");
}

void CLogger::Write(const std::string& token)
{
	WRITE_STR(m_LogStream, token);
}
