#pragma once
#include "../../utilities/string_tokenizer.h"

namespace rw
{
	namespace rs
	{
		#define CLR_AND_RET_ON_FALSE(expr)	\
		if (!expr)							\
		{									\
			Clear();						\
			return false;					\
		}

		bool ide_obj_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, object_count));

			float dd;
			for (int i = 0; i < object_count; ++i)
			{
				CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + i, dd));
				draw_dists.push_back(dd);
			}
				
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + object_count, flags));

			return true;
		}

		std::string ide_obj_entry::ToString() const
		{
			return "";
		}

		void ide_obj_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			object_count = 0;
			draw_dists.clear();
			flags = 0;
		}

		bool ide_tobj_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, object_count));

			float dd;
			for (int i = 0; i < object_count; ++i)
			{
				CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + i, dd));
				draw_dists.push_back(dd);
			}

			CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + object_count + 0, flags));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + object_count + 1, time_on));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4 + object_count + 2, time_off));

			return true;
		}

		std::string ide_tobj_entry::ToString() const
		{
			return "";
		}

		void ide_tobj_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			object_count = 0;
			draw_dists.clear();
			flags = 0;
			time_on = 0;
			time_off = 0;
		}

		bool ide_anim_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, draw_dist));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, flags));

			return true;
		}

		std::string ide_anim_entry::ToString() const
		{
			return "";
		}

		void ide_anim_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			draw_dist = 0.f;
			flags = 0;
		}

		bool ide_peds_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			// GTA III
			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, threat));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, animation_type));

			// #TODO: Read hex
			//CLR_AND_RET_ON_FALSE(tokenizer.Get(5, veh_class));

			// GTA VC
			if (tokenizer.GetTokensCount() > 6)
			{
				CLR_AND_RET_ON_FALSE(tokenizer.Get(6, start_animation));
				CLR_AND_RET_ON_FALSE(tokenizer.Get(7, radio1));
				CLR_AND_RET_ON_FALSE(tokenizer.Get(8, radio2));
			}

			return true;
		}

		std::string ide_peds_entry::ToString() const
		{
			return "";
		}

		void ide_peds_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			threat = "";
			behavior = "";
			animation_type = "";
			veh_class = 0;
			start_animation = "";
			radio1 = 0;
			radio2 = 0;
		}

		bool ide_weap_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, anim_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, unk1));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(5, draw_dist));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(6, unk2));

			return true;
		}

		std::string ide_weap_entry::ToString() const
		{
			return "";
		}

		void ide_weap_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			anim_name = "";
			unk1 = "";
			draw_dist = 0.f;
			unk2 = "";
		}

		bool ide_cars_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ide_cars_entry::ToString() const
		{
			return "";
		}

		bool ide_hier_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, unk1));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, unk2));

			return true;
		}

		std::string ide_hier_entry::ToString() const
		{
			return "";
		}

		void ide_hier_entry::Clear()
		{
			id = 0;
			model_name = "";
			txd_name = "";
			unk1 = "";
			unk2 = 0.f;
		}

		bool ide_txdp_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, txd_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, parent_txd_name));

			return true;
		}

		std::string ide_txdp_entry::ToString() const
		{
			return "";
		}

		void ide_txdp_entry::Clear()
		{
			txd_name = "";
			parent_txd_name = "";
		}

		bool ide_2dfx_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ide_2dfx_entry::ToString() const
		{
			return "";
		}

		#undef CLR_AND_RET_ON_FALSE

		EIdeSectionType::TYPE EIdeSectionType::FromString(const std::string& str)
		{
			#define MAKE_CASE(cmp_str, cmp_type) if (str == cmp_str) return cmp_type;

			MAKE_CASE("objs", EIdeSectionType::OBJS);
			MAKE_CASE("tobj", EIdeSectionType::TOBJ);
			MAKE_CASE("anim", EIdeSectionType::ANIM);
			MAKE_CASE("peds", EIdeSectionType::PEDS);
			MAKE_CASE("weap", EIdeSectionType::WEAP);
			MAKE_CASE("cars", EIdeSectionType::CARS);
			MAKE_CASE("hier", EIdeSectionType::HIER);
			MAKE_CASE("txdp", EIdeSectionType::TXDP);
			MAKE_CASE("2dfx", EIdeSectionType::_2DFX);

			#undef MAKE_CASE

			return EIdeSectionType::_UNKNOWN;
		}

		std::string EIdeSectionType::ToString(TYPE type)
		{
			switch (type)
			{
			case EIdeSectionType::OBJS: return "objs";
			case EIdeSectionType::TOBJ: return "tobj";
			case EIdeSectionType::ANIM: return "anim";
			case EIdeSectionType::PEDS: return "peds";
			case EIdeSectionType::WEAP: return "weap";
			case EIdeSectionType::CARS: return "cars";
			case EIdeSectionType::HIER: return "hier";
			case EIdeSectionType::TXDP: return "txdp";
			case EIdeSectionType::_2DFX: return "2dfx";
			default: return "";
			}
		}

		bool item_definitions::Read(in_stream<EStreamType::TEXT>& stream)
		{
			auto is_skippable_line = [](const std::string& str)
			{
				if (str.empty())
					return true;
				if (str == "\n")
					return true;
				if (str.size() > 0 && str[0] == '#')
					return true;

				return false;
			};

			auto is_section_scope_def = [](const std::string& str)
			{
				return EIdeSectionType::FromString(str) != EIdeSectionType::_UNKNOWN;
			};

			auto is_section_scope_end = [](const std::string& str)
			{
				return str == "end";
			};

			EIdeSectionType::TYPE cur_section = EIdeSectionType::_UNKNOWN;

			while (!stream.Eof())
			{
				std::string line;
				READ_LINE(stream, line);

				if (is_skippable_line(line))
					continue;

				if (is_section_scope_def(line))
				{
					cur_section = EIdeSectionType::FromString(line);
					continue;
				}

				if (is_section_scope_end(line))
				{
					cur_section = EIdeSectionType::_UNKNOWN;
					continue;
				}
					
				#define MAKE_CASE(sect_type, TObj, dest_vec)	\
				case sect_type:									\
				{												\
					TObj obj;									\
					if (obj.FromString(line))					\
						dest_vec.push_back(obj);				\
					break;										\
				}
				
				switch (cur_section)
				{
					MAKE_CASE(EIdeSectionType::OBJS, ide_obj_entry, m_ObjEntries);
					MAKE_CASE(EIdeSectionType::TOBJ, ide_tobj_entry, m_TobjEntries);
					MAKE_CASE(EIdeSectionType::ANIM, ide_anim_entry, m_AnimEntries);
					MAKE_CASE(EIdeSectionType::PEDS, ide_peds_entry, m_PedsEntries);
					MAKE_CASE(EIdeSectionType::WEAP, ide_weap_entry, m_WeapEntries);
					MAKE_CASE(EIdeSectionType::CARS, ide_cars_entry, m_CarsEntries);
					MAKE_CASE(EIdeSectionType::HIER, ide_hier_entry, m_HierEntries);
					MAKE_CASE(EIdeSectionType::TXDP, ide_txdp_entry, m_TxdpEntries);
					MAKE_CASE(EIdeSectionType::_2DFX, ide_2dfx_entry, m_2dfxEntries);
				}

				#undef MAKE_CASE
			}

			return true;
		}

		bool item_definitions::Write(out_stream<EStreamType::TEXT>& stream)
		{
			return false;
		}

		bool item_placements::Read(in_stream<EStreamType::TEXT>& stream)
		{
			return false;
		}

		bool item_placements::Write(out_stream<EStreamType::TEXT>& stream)
		{
			return false;
		}
	}
}
