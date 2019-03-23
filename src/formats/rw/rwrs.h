#ifndef _RWRS
#define _RWRS

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
			_Unknown,
			VER1,
			VER2
		};

		enum class EColVer
		{
			_Unknown,
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

			bool GetFileStream(TInStream& stream, std::string file_name) const;
		};
		
		template<EArchiveVer version>
		inline inline const std::vector<archive_entry_header<version>*>& archive<version>::GetEntryHeaders() const
		{
			return m_EntryHeaders;
		}
		
		template<EArchiveVer version>
		inline bool archive_entry_header<version>::Read(TInStream& stream)
		{
			READ_VAR(stream, offset);
			READ_VAR(stream, ver1.streaming_size);
			INIT_ARR(file_name, FILE_NAME_SIZE);
			READ_ARR(stream, file_name, FILE_NAME_SIZE);
			return true;
		}

		template<EArchiveVer version>
		inline bool archive_entry_header<version>::Write(TOutStream& stream)
		{
			WRITE_VAR(stream, offset);
			WRITE_VAR(stream, ver1.streaming_size);
			WRITE_ARR(stream, file_name, FILE_NAME_SIZE);
			return true;
		}

		template<EArchiveVer version>
		inline size_t archive_entry_header<version>::GetHeaderSize() const
		{
			return sizeof(offset) + sizeof(ver1.streaming_size) + FILE_NAME_SIZE;
		}

		template<>
		inline bool archive<EArchiveVer::VER1>::Read(TInStream& stream)
		{
			m_InputStream = stream;
			
			std::wstring dir_fname = stream.GetFileName();
			dir_fname.erase(dir_fname.find_last_of(L"."));
			dir_fname.append(L".dir");

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
		inline bool archive<EArchiveVer::VER1>::Write(TOutStream& stream)
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
		inline bool archive<EArchiveVer::VER2>::Read(TInStream& stream)
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
		inline bool archive<EArchiveVer::VER2>::Write(TOutStream& stream)
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
		inline bool archive<version>::GetFileStream(TInStream& stream, std::string file_name) const
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

	namespace rs
	{
		struct EIdeSectionType
		{
			enum TYPE
			{
				_UNKNOWN,
				OBJS,
				TOBJ,
				ANIM,
				PEDS,
				WEAP,
				CARS,
				HIER,
				TXDP,
				_2DFX,
			};

			static TYPE FromString(const std::string& str);
			static std::string ToString(TYPE type);
		};

		struct EIplSectionType
		{
			enum TYPE
			{
				_UNKNOWN,
				INST,
				CULL,
				GRGE,
				ENEX,
				PICK,
				CARS,
				OCCL,
				JUMP,
			};

			static TYPE FromString(const std::string& str);
			static std::string ToString(TYPE type);
		};

		struct IEntry
		{
			virtual bool FromString(const std::string& str) = 0;
			virtual std::string ToString() const = 0;
			virtual void Clear() = 0;
		};

		struct ide_obj_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			unsigned int		id;
			std::string			model_name;
			std::string			txd_name;
			int					object_count;
			std::vector<float>	draw_dists;
			int					flags;
		};

		struct ide_tobj_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			unsigned int		id;
			std::string			model_name;
			std::string			txd_name;
			int					object_count;
			std::vector<float>	draw_dists;
			int					flags;
			int					time_on;
			int					time_off;
		};

		struct ide_anim_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			unsigned int	id;
			std::string		model_name;
			std::string		txd_name;
			std::string		anim_name;
			float			draw_dist;
			int				flags;
		};
		
		struct ide_peds_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			// GTA III
			unsigned int	id;
			std::string		model_name;
			std::string		txd_name;
			std::string		threat;
			std::string		behavior;
			std::string		animation_type;
			unsigned int	veh_class; // read in hex!
			// + GTA VC
			std::string		start_animation;
			unsigned int	radio1;
			unsigned int	radio2;
		};

		struct ide_weap_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			unsigned int	id;
			std::string		model_name;
			std::string		txd_name;
			std::string		anim_name;
			std::string		unk1;
			float			draw_dist;
			std::string		unk2;
		};

		struct ide_cars_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		struct ide_hier_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			unsigned int	id;
			std::string		model_name;
			std::string		txd_name;
			std::string		unk1;
			float			unk2; // normally 2000.0
		};

		struct ide_txdp_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			std::string		txd_name;
			std::string		parent_txd_name;
		};

		struct ide_2dfx_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		class item_definitions
			: public common::ITexReadable
			, public common::ITexWriteable
		{
		public:
			virtual bool Read(in_stream<EStreamType::TEXT>& stream) override;
			virtual bool Write(out_stream<EStreamType::TEXT>& stream) override;

			const std::vector<ide_obj_entry>& GetObjEntries() const;
			const std::vector<ide_tobj_entry>& GetTobjEntries() const;
			const std::vector<ide_anim_entry>& GetAnimEntries() const;
			const std::vector<ide_peds_entry>& GetPedsEntries() const;
			const std::vector<ide_weap_entry>& GetWeapEntries() const;
			const std::vector<ide_cars_entry>& GetCarsEntries() const;
			const std::vector<ide_hier_entry>& GetHierEntries() const;
			const std::vector<ide_txdp_entry>& GetTxdpEntries() const;
			const std::vector<ide_2dfx_entry>& Get2dfxEntries() const;

		protected:
			std::vector<ide_obj_entry> m_ObjEntries;
			std::vector<ide_tobj_entry> m_TobjEntries;
			std::vector<ide_anim_entry> m_AnimEntries;
			std::vector<ide_peds_entry> m_PedsEntries;
			std::vector<ide_weap_entry> m_WeapEntries;
			std::vector<ide_cars_entry> m_CarsEntries;
			std::vector<ide_hier_entry> m_HierEntries;
			std::vector<ide_txdp_entry> m_TxdpEntries;
			std::vector<ide_2dfx_entry> m_2dfxEntries;
		};

		struct ipl_inst_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			// GTA VC 
			unsigned int	id;
			std::string		model_name;
			unsigned int	interior;
			float			pos_x;
			float			pos_y;
			float			pos_z;
			float			scale_x;
			float			scale_y;
			float			scale_z;
			float			rot_x;
			float			rot_y;
			float			rot_z;
			float			rot_w;
		};

		struct ipl_cull_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			// GTA III and VC
			float			center_x;
			float			center_y;
			float			center_z;
			float			lower_left_x;
			float			lower_left_y;
			float			lower_left_z;
			float			upper_right_x;
			float			upper_right_y;
			float			upper_right_z;
			unsigned int	flags;
			unsigned int	unk;
		};

		struct ipl_grge_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;

			// GTA SA
			float			pos_x;
			float			pos_y;
			float			pos_z;
			float			line_x;
			float			line_y;
			float			cube_x;
			float			cube_y;
			float			cube_z;
			unsigned int	door_type;
			unsigned int	garage_type;
			std::string		name;
		};

		struct ipl_enex_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};


		struct ipl_pick_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		struct ipl_cars_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		struct ipl_occl_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		struct ipl_jump_entry : public IEntry
		{
			virtual bool FromString(const std::string& str) override;
			virtual std::string ToString() const override;
			virtual void Clear() override;
		};

		class item_placements
			: public common::ITexReadable
			, public common::ITexWriteable
		{
		public:
			virtual bool Read(in_stream<EStreamType::TEXT>& stream) override;
			virtual bool Write(out_stream<EStreamType::TEXT>& stream) override;
		
			const std::vector<ipl_inst_entry>& GetInstEntries() const;
			const std::vector<ipl_cull_entry>& GetCullEntries() const;
			const std::vector<ipl_grge_entry>& GetGrgeEntries() const;
			const std::vector<ipl_enex_entry>& GetEnexEntries() const;
			const std::vector<ipl_pick_entry>& GetPickEntries() const;
			const std::vector<ipl_cars_entry>& GetCarsEntries() const;
			const std::vector<ipl_occl_entry>& GetOcclEntries() const;
			const std::vector<ipl_jump_entry>& GetJumpEntries() const;

		protected:
			std::vector<ipl_inst_entry> m_InstEntries;
			std::vector<ipl_cull_entry> m_CullEntries;
			std::vector<ipl_grge_entry> m_GrgeEntries;
			std::vector<ipl_enex_entry> m_EnexEntries;
			std::vector<ipl_pick_entry> m_PickEntries;
			std::vector<ipl_cars_entry> m_CarsEntries;
			std::vector<ipl_occl_entry> m_OcclEntries;
			std::vector<ipl_jump_entry> m_JumpEntries;
		};
	}
}

#endif

