#pragma once
#include "../../utilities/string_tokenizer.h"

namespace rw
{
	namespace rs
	{
		namespace
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

			auto is_ide_section_scope_def = [](const std::string& str)
			{
				return EIdeSectionType::FromString(str) != EIdeSectionType::_UNKNOWN;
			};

			auto is_ipl_section_scope_def = [](const std::string& str)
			{
				return EIplSectionType::FromString(str) != EIplSectionType::_UNKNOWN;
			};

			auto is_section_scope_end = [](const std::string& str)
			{
				return str == "end";
			};

			#define CLR_AND_RET_ON_FALSE(expr)	\
			if (!expr)							\
			{									\
				Clear();						\
				return false;					\
			}

			#define MAKE_SECTION_CASE(sect_type, TObj, dest_vec)	\
			case sect_type:											\
			{														\
				TObj obj;											\
				if (obj.FromString(line))							\
					dest_vec.push_back(obj);						\
				break;												\
			}

			#define MAKE_STR_TO_ENUM_CASE(cmp_str, cmp_type) if (str == cmp_str) return cmp_type;
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

		void ide_cars_entry::Clear()
		{

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

		void ide_2dfx_entry::Clear()
		{

		}

		EIdeSectionType::TYPE EIdeSectionType::FromString(const std::string& str)
		{
			MAKE_STR_TO_ENUM_CASE("objs", EIdeSectionType::OBJS);
			MAKE_STR_TO_ENUM_CASE("tobj", EIdeSectionType::TOBJ);
			MAKE_STR_TO_ENUM_CASE("anim", EIdeSectionType::ANIM);
			MAKE_STR_TO_ENUM_CASE("peds", EIdeSectionType::PEDS);
			MAKE_STR_TO_ENUM_CASE("weap", EIdeSectionType::WEAP);
			MAKE_STR_TO_ENUM_CASE("cars", EIdeSectionType::CARS);
			MAKE_STR_TO_ENUM_CASE("hier", EIdeSectionType::HIER);
			MAKE_STR_TO_ENUM_CASE("txdp", EIdeSectionType::TXDP);
			MAKE_STR_TO_ENUM_CASE("2dfx", EIdeSectionType::_2DFX);

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

		EIplSectionType::TYPE EIplSectionType::FromString(const std::string& str)
		{
			MAKE_STR_TO_ENUM_CASE("inst", EIplSectionType::INST);
			MAKE_STR_TO_ENUM_CASE("cull", EIplSectionType::CULL);
			MAKE_STR_TO_ENUM_CASE("grge", EIplSectionType::GRGE);
			MAKE_STR_TO_ENUM_CASE("enex", EIplSectionType::ENEX);
			MAKE_STR_TO_ENUM_CASE("pick", EIplSectionType::PICK);
			MAKE_STR_TO_ENUM_CASE("cars", EIplSectionType::CARS);
			MAKE_STR_TO_ENUM_CASE("occl", EIplSectionType::OCCL);
			MAKE_STR_TO_ENUM_CASE("jump", EIplSectionType::JUMP);

			return EIplSectionType::_UNKNOWN;
		}

		std::string EIplSectionType::ToString(TYPE type)
		{
			switch (type)
			{
			case EIplSectionType::INST: return "inst";
			case EIplSectionType::CULL: return "cull";
			case EIplSectionType::GRGE: return "grge";
			case EIplSectionType::ENEX: return "enex";
			case EIplSectionType::PICK: return "pick";
			case EIplSectionType::CARS: return "cars";
			case EIplSectionType::OCCL: return "occl";
			case EIplSectionType::JUMP: return "jump";
			default: return "";
			}
		}

		bool item_definitions::Read(in_stream<EStreamType::TEXT>& stream)
		{
			EIdeSectionType::TYPE cur_section = EIdeSectionType::_UNKNOWN;

			while (!stream.Eof())
			{
				std::string line;
				READ_LINE(stream, line);

				if (is_skippable_line(line))
					continue;

				if (is_ide_section_scope_def(line))
				{
					cur_section = EIdeSectionType::FromString(line);
					continue;
				}

				if (is_section_scope_end(line))
				{
					cur_section = EIdeSectionType::_UNKNOWN;
					continue;
				}
					
				switch (cur_section)
				{
					MAKE_SECTION_CASE(EIdeSectionType::OBJS, ide_obj_entry, m_ObjEntries);
					MAKE_SECTION_CASE(EIdeSectionType::TOBJ, ide_tobj_entry, m_TobjEntries);
					MAKE_SECTION_CASE(EIdeSectionType::ANIM, ide_anim_entry, m_AnimEntries);
					MAKE_SECTION_CASE(EIdeSectionType::PEDS, ide_peds_entry, m_PedsEntries);
					MAKE_SECTION_CASE(EIdeSectionType::WEAP, ide_weap_entry, m_WeapEntries);
					MAKE_SECTION_CASE(EIdeSectionType::CARS, ide_cars_entry, m_CarsEntries);
					MAKE_SECTION_CASE(EIdeSectionType::HIER, ide_hier_entry, m_HierEntries);
					MAKE_SECTION_CASE(EIdeSectionType::TXDP, ide_txdp_entry, m_TxdpEntries);
					MAKE_SECTION_CASE(EIdeSectionType::_2DFX, ide_2dfx_entry, m_2dfxEntries);
				}
			}

			return true;
		}

		bool item_definitions::Write(out_stream<EStreamType::TEXT>& stream)
		{
			return false;
		}

		bool ipl_inst_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, id));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, model_name));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, interior));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, pos_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, pos_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(5, pos_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(6, scale_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(7, scale_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(8, scale_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(9, rot_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(10, rot_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(11, rot_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(12, rot_w));

			return true;
		}

		std::string ipl_inst_entry::ToString() const
		{
			return "";
		}

		void ipl_inst_entry::Clear()
		{
			id = 0;
			model_name = "";
			interior = 0;
			pos_x = 0.f;
			pos_y = 0.f;
			pos_z = 0.f;
			scale_x = 0.f;
			scale_y = 0.f;
			scale_z = 0.f;
			rot_x = 0.f;
			rot_y = 0.f;
			rot_z = 0.f;
			rot_w = 0.f;
		}

		bool ipl_cull_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, center_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, center_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, center_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, lower_left_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, lower_left_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(5, lower_left_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(6, upper_right_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(7, upper_right_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(8, upper_right_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(9, flags));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(10, unk));

			return true;
		}

		std::string ipl_cull_entry::ToString() const
		{
			return "";
		}

		void ipl_cull_entry::Clear()
		{
			center_x = 0.f;
			center_y = 0.f;
			center_z = 0.f;
			lower_left_x = 0.f;
			lower_left_y = 0.f;
			lower_left_z = 0.f;
			upper_right_x = 0.f;
			upper_right_y = 0.f;
			upper_right_z = 0.f;
			flags = 0;
			unk = 0;
		}

		bool ipl_grge_entry::FromString(const std::string& str)
		{
			str_tokenizer tokenizer;
			if (!tokenizer.Tokenize(str))
				return false;

			CLR_AND_RET_ON_FALSE(tokenizer.Get(0, pos_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(1, pos_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(2, pos_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(3, line_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(4, line_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(5, cube_x));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(6, cube_y));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(7, cube_z));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(8, door_type));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(9, garage_type));
			CLR_AND_RET_ON_FALSE(tokenizer.Get(10, name));

			return true;
		}

		std::string ipl_grge_entry::ToString() const
		{
			return "";
		}

		void ipl_grge_entry::Clear()
		{
			pos_x = 0.f;
			pos_y = 0.f;
			pos_z = 0.f;
			line_x = 0.f;
			line_y = 0.f;
			cube_x = 0.f;
			cube_y = 0.f;
			cube_z = 0.f;
			door_type = 0;
			garage_type = 0;
			name = "";
		}

		bool ipl_enex_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ipl_enex_entry::ToString() const
		{
			return "";
		}

		void ipl_enex_entry::Clear()
		{

		}

		bool ipl_pick_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ipl_pick_entry::ToString() const
		{
			return "";
		}

		void ipl_pick_entry::Clear()
		{

		}

		bool ipl_cars_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ipl_cars_entry::ToString() const
		{
			return "";
		}

		void ipl_cars_entry::Clear()
		{

		}

		bool ipl_occl_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ipl_occl_entry::ToString() const
		{
			return "";
		}

		void ipl_occl_entry::Clear()
		{

		}

		bool ipl_jump_entry::FromString(const std::string& str)
		{
			return false;
		}

		std::string ipl_jump_entry::ToString() const
		{
			return "";
		}

		void ipl_jump_entry::Clear()
		{

		}

		bool item_placements::Read(in_stream<EStreamType::TEXT>& stream)
		{
			EIplSectionType::TYPE cur_section = EIplSectionType::_UNKNOWN;

			while (!stream.Eof())
			{
				std::string line;
				READ_LINE(stream, line);

				if (is_skippable_line(line))
					continue;

				if (is_ipl_section_scope_def(line))
				{
					cur_section = EIplSectionType::FromString(line);
					continue;
				}

				if (is_section_scope_end(line))
				{
					cur_section = EIplSectionType::_UNKNOWN;
					continue;
				}

				switch (cur_section)
				{
					MAKE_SECTION_CASE(EIplSectionType::INST, ipl_inst_entry, m_InstEntries);
					MAKE_SECTION_CASE(EIplSectionType::CULL, ipl_cull_entry, m_CullEntries);
					MAKE_SECTION_CASE(EIplSectionType::GRGE, ipl_grge_entry, m_GrgeEntries);
					MAKE_SECTION_CASE(EIplSectionType::ENEX, ipl_enex_entry, m_EnexEntries);
					MAKE_SECTION_CASE(EIplSectionType::PICK, ipl_pick_entry, m_PickEntries);
					MAKE_SECTION_CASE(EIplSectionType::CARS, ipl_cars_entry, m_CarsEntries);
					MAKE_SECTION_CASE(EIplSectionType::OCCL, ipl_occl_entry, m_OcclEntries);
					MAKE_SECTION_CASE(EIplSectionType::JUMP, ipl_jump_entry, m_JumpEntries);
				}
			}

			return true;
		}

		bool item_placements::Write(out_stream<EStreamType::TEXT>& stream)
		{
			return false;
		}

		#undef CLR_AND_RET_ON_FALSE
		#undef MAKE_SECTION_CASE
		#undef MAKE_STR_TO_ENUM_CASE
	}
}

