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

		enum class EColVer
		{
			COLL,
			COL2,
			COL3
		};
	}

	// IMG Archive
	namespace rs
	{


		template<EArchiveVer version>
		class archive;

		template<EArchiveVer version>
		class archive_entry_header
			: public common::IBinReadable
			, public common::IBinWriteable
		{
			CONVERTIBLE_ENTITY
			friend class archive<version>;
		public:
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
			//archive_entry_header();
			//~archive_entry_header();

			bool Read(TInStream& stream) override;
			bool Write(TOutStream& stream) override;

			size_t GetHeaderSize() const;

			const static int FILE_NAME_SIZE = 24;
			const static int SECTOR_SIZE = 2048;
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
			//archive();
			//~archive();

			bool Read(TInStream& stream) override;
			bool Write(TOutStream& stream) override;
			const THeadersVec& GetEntryHeaders() const;

			bool GetFileStream(TInStream& stream, std::string file_name);
		};
		
		template<EArchiveVer version>
		inline const std::vector<archive_entry_header<version>*>& archive<version>::GetEntryHeaders() const
		{
			return m_EntryHeaders;
		}
		
		template<EArchiveVer version>
		bool archive_entry_header<version>::Read(TInStream& stream)
		{
			READ_VAR(stream, offset);
			READ_VAR(stream, ver1.streaming_size);
			INIT_ARR(file_name, FILE_NAME_SIZE);
			READ_ARR(stream, file_name, FILE_NAME_SIZE);
			return true;
		}

		template<EArchiveVer version>
		bool archive_entry_header<version>::Write(TOutStream& stream)
		{
			WRITE_VAR(stream, offset);
			WRITE_VAR(stream, ver1.streaming_size);
			WRITE_ARR(stream, file_name, FILE_NAME_SIZE);
			return true;
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
			
			std::string dir_fname = stream.GetFileName();
			dir_fname.erase(dir_fname.find_last_of("."));
			dir_fname.append(".dir");

			TInStream dir_stream;
			if (dir_stream.Open(dir_fname.c_str()))
			{
				while (!dir_stream.Eof())
				{
					auto header = new archive_entry_header<EArchiveVer::VER1>();
					
					header->Read(dir_stream);
					this->m_EntryHeaders.push_back(header);
				}
				dir_stream.Close();
				m_EntryCount = m_EntryHeaders.size();
				return true;
			}
			
			return false;
		}

		template<>
		bool archive<EArchiveVer::VER1>::Write(TOutStream& stream)
		{
			WRITE_ARR(stream, m_Label, 4);
			WRITE_VAR(stream, m_EntryCount);

			for (auto& entry_hd : m_EntryHeaders)
			{
				entry_hd->Write(stream);
			}

			// #TODO: Write files to stream
			return true;
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

			// #TODO: Write files to stream
			return true;
		}
	
		template<EArchiveVer version>
		bool archive<version>::GetFileStream(TInStream& stream, std::string file_name)
		{
			for (auto& eh : m_EntryHeaders)
			{
				if (strcmp((char*)eh->file_name, file_name.c_str()) == 0)
				{
					stream = m_InputStream;
					stream.Seek(eh->offset * archive_entry_header<version>::SECTOR_SIZE, 0);
					return true;
				}
			}
			return false;
		}
	}

	// COL Archive
	namespace rs
	{
		class col_header
			: public common::IBinReadable
			, public common::IBinWriteable
		{
			CONVERTIBLE_ENTITY

		};
	}
}