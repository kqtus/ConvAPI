#include "rwbase.h"

#define DECODE_IF_EQ(x, cmp) if (x == cmp) { return #cmp; }

std::string rw::Decode(const rw::RpGeometryFlag& flag)
{
	DECODE_IF_EQ(flag, rwVENDORID_CORE);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONTK);
	DECODE_IF_EQ(flag, rwVENDORID_REDLINERACER);
	DECODE_IF_EQ(flag, rwVENDORID_CSLRD);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONINT);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONWORLD);
	DECODE_IF_EQ(flag, rwVENDORID_BETA);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONRM);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONRWA);
	DECODE_IF_EQ(flag, rwVENDORID_CRITERIONRWP);
	return "UNKNOWN";
}

std::string rw::Decode(const rw::RwCorePluginID& flag)
{
	DECODE_IF_EQ(flag, rwID_NAOBJECT);
	DECODE_IF_EQ(flag, rwID_STRUCT);
	DECODE_IF_EQ(flag, rwID_STRING);
	DECODE_IF_EQ(flag, rwID_EXTENSION);
	DECODE_IF_EQ(flag, rwID_CAMERA);
	DECODE_IF_EQ(flag, rwID_TEXTURE);
	DECODE_IF_EQ(flag, rwID_MATERIAL);
	DECODE_IF_EQ(flag, rwID_MATLIST);
	DECODE_IF_EQ(flag, rwID_ATOMICSECT);
	DECODE_IF_EQ(flag, rwID_PLANESECT);
	DECODE_IF_EQ(flag, rwID_WORLD);
	DECODE_IF_EQ(flag, rwID_SPLINE);
	DECODE_IF_EQ(flag, rwID_MATRIX);
	DECODE_IF_EQ(flag, rwID_FRAMELIST);
	DECODE_IF_EQ(flag, rwID_GEOMETRY);
	DECODE_IF_EQ(flag, rwID_CLUMP);
	DECODE_IF_EQ(flag, rwID_LIGHT);
	DECODE_IF_EQ(flag, rwID_UNICODESTRING);
	DECODE_IF_EQ(flag, rwID_ATOMIC);
	DECODE_IF_EQ(flag, rwID_TEXTURENATIVE);
	DECODE_IF_EQ(flag, rwID_TEXDICTIONARY);
	DECODE_IF_EQ(flag, rwID_ANIMDATABASE);
	DECODE_IF_EQ(flag, rwID_IMAGE);
	DECODE_IF_EQ(flag, rwID_SKINANIMATION);
	DECODE_IF_EQ(flag, rwID_GEOMETRYLIST);
	DECODE_IF_EQ(flag, rwID_ANIMANIMATION);
	DECODE_IF_EQ(flag, rwID_HANIMANIMATION);
	DECODE_IF_EQ(flag, rwID_TEAM);
	DECODE_IF_EQ(flag, rwID_CROWD);
	DECODE_IF_EQ(flag, rwID_DMORPHANIMATION);
	DECODE_IF_EQ(flag, rwID_RIGHTTORENDER);
	DECODE_IF_EQ(flag, rwID_MTEFFECTNATIVE);
	DECODE_IF_EQ(flag, rwID_MTEFFECTDICT);
	DECODE_IF_EQ(flag, rwID_TEAMDICTIONARY);
	DECODE_IF_EQ(flag, rwID_PITEXDICTIONARY);
	DECODE_IF_EQ(flag, rwID_TOC);
	DECODE_IF_EQ(flag, rwID_PRTSTDGLOBALDATA);
	DECODE_IF_EQ(flag, rwID_ALTPIPE);
	DECODE_IF_EQ(flag, rwID_PIPEDS);
	DECODE_IF_EQ(flag, rwID_PATCHMESH);
	DECODE_IF_EQ(flag, rwID_CHUNKGROUPSTART);
	DECODE_IF_EQ(flag, rwID_CHUNKGROUPEND);
	DECODE_IF_EQ(flag, rwID_UVANIMDICT);
	DECODE_IF_EQ(flag, rwID_COLLTREE);
	DECODE_IF_EQ(flag, rwID_ENVIRONMENT);

	// -- Plugins
	DECODE_IF_EQ(flag, rwID_MORPHPLG);
	DECODE_IF_EQ(flag, rwID_BINMESHPLG);
	DECODE_IF_EQ(flag, rwID_SKYMIPMAPVALPLG);
	DECODE_IF_EQ(flag, rwID_HANIMPLG);
	DECODE_IF_EQ(flag, rwID_FRAMEPLG);
	// --

	DECODE_IF_EQ(flag, rwID_COREPLUGINIDMAX);
	return "UNKNOWN";
}

std::string rw::Decode(const rw::RwPluginVendor& flag)
{
	DECODE_IF_EQ(flag, rpGEOMETRYTRISTRIP);
	DECODE_IF_EQ(flag, rpGEOMETRYPOSITIONS);
	DECODE_IF_EQ(flag, rpGEOMETRYTEXTURED);
	DECODE_IF_EQ(flag, rpGEOMETRYPRELIT);
	DECODE_IF_EQ(flag, rpGEOMETRYNORMALS);
	DECODE_IF_EQ(flag, rpGEOMETRYLIGHT);
	DECODE_IF_EQ(flag, rpGEOMETRYMODULATEMATERIALCOLOR);
	DECODE_IF_EQ(flag, rpGEOMETRYTEXTURED2);
	DECODE_IF_EQ(flag, rpGEOMETRYNATIVE);
	DECODE_IF_EQ(flag, rpGEOMETRYNATIVEINSTANCE);
	DECODE_IF_EQ(flag, rpGEOMETRYFLAGSMASK);
	DECODE_IF_EQ(flag, rpGEOMETRYNATIVEFLAGSMASK);
	return "UNKNOWN";
}

uint32_t rw::chunk_base::DecodeVersion() const
{
	if (version & 0xFFFF0000)
	{
		return (version >> 14 & 0x3FF00) + 0x30000 | (version >> 16 & 0x3F);
	}	
	return version << 8;
}

bool rw::chunk_base::Read(in_stream<EStreamType::BINARY>& stream)
{
	READ_VAR(stream, type);
	READ_VAR(stream, size);
	READ_VAR(stream, version);
	return true;
}

bool rw::string::Read(in_stream<EStreamType::BINARY>& stream)
{
	chunk_base::Read(stream);

	this->resize(chunk_base::size);
	for (int i = 0; i < chunk_base::size; i++)
	{
		READ_VAR(stream, this->operator[](i));
	}

	return true;
}