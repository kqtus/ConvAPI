#pragma once
#include <vector>
#include "rwcore.h"

namespace rw
{
	namespace rs
	{
		using TInStream = in_stream<EStreamType::BINARY>;
		using TOutStream = out_stream<EStreamType::BINARY>;

		enum class EArchiveVer
		{
			VER1,
			VER2
		};

		template<EArchiveVer version>
		class archive_entry_header
			: public common::IBinReadable
			, public common::IBinWriteable
		{
			CONVERTIBLE_ENTITY
			friend class archive<version>;
		protected:
			uint32_t offset;
			union
			{
				struct
				{
					uint32_t streaming_size;
				} ver1;
				struct
				{
					uint16_t streaming_size;
					uint16_t size_in_archive;
				} ver2;
			};
			uint8_t* file_name;

		public:
			archive_entry_header();
			~archive_entry_header();

			bool Read(TInStream& stream) override;
			bool Write(TOutStream& stream) override;

			size_t GetHeaderSize() const;

			const int FILE_NAME_SIZE = 24;
		};

		template<EArchiveVer version>
		class archive
			: public common::IBinReadable
			, public common::IBinWriteable
		{
			CONVERTIBLE_ENTITY
			using THeadersVec = std::vector<archive_entry_header<version>*>;
		
		private:
			TInStream m_InputStream;
			uint32_t m_EntryCount;
			THeadersVec m_EntryHeaders;
			uint8_t* m_Label;

		public:
			archive();
			~archive();

			bool Read(TInStream& stream) override;
			bool Write(TOutStream& stream) override;
		};

		template<EArchiveVer version>
		bool archive_entry_header<version>::Read(TInStream& stream)
		{
			READ_VAR(stream, offset);
			READ_VAR(stream, ver1.streaming_size);
			INIT_ARR(file_name, FILE_NAME_SIZE);
			READ_ARR(stream, file_name, FILE_NAME_SIZE);
		}

		template<EArchiveVer version>
		bool archive_entry_header<version>::Write(TOutStream& stream)
		{
			WRITE_VAR(stream, offset);
			WRITE_VAR(stream, ver1.streaming_size);
			WRITE_ARR(stream, file_name, FILE_NAME_SIZE);
		}

		template<EArchiveVer version>
		size_t archive_entry_header<version>::GetHeaderSize() const
		{
			return sizeof(offset) + sizeof(ver1.streaming_size) + FILE_NAME_SIZE;
		}

		template<>
		bool archive<EArchiveVer::VER1>::Read(TInStream& stream)
		{
			m_InputStream = stream;
			// Open .dir file and read headers
			return false;
		}

		template<>
		bool archive<EArchiveVer::VER1>::Write(TOutStream& stream)
		{
			// Open .dir file and write headers
			return false;
		}

		template<>
		bool archive<EArchiveVer::VER2>::Read(TInStream& stream)
		{
			m_InputStream = stream;

			INIT_ARR(m_Label, 4);
			READ_ARR(stream, m_Label, 4);
			READ_VAR(stream, m_EntryCount);

			for (int i = 0; i < m_EntryCount; i++)
			{
				auto entry_hd = new archive_entry_header<EArchiveVer::VER2>();
				entry_hd->Read(stream);
				m_EntryHeaders.push_back(entry_hd);
			}

			return true;
		}

		template<>
		bool archive<EArchiveVer::VER2>::Write(TOutStream& stream)
		{
			WRITE_ARR(stream, m_Label, 4);
			WRITE_VAR(stream, m_EntryCount);

			for (auto& entry_hd : m_EntryHeaders)
			{
				entry_hd->Write(stream);
			}

			// #TODO: Write files from m_InputStream
			return true;
		}
	}
}