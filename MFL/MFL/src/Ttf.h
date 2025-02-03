#pragma once

#include <inttypes.h>
#include <unordered_map>
#include <vector>
#include <filesystem>

#include <Scriptorium/Scriptorium.h>

namespace MFL
{
	using u2be = uint16_t;
	using s2be = int16_t;

	using u4be = uint32_t;
	using s4be = int32_t;

	using u8be = uint64_t;
	using s8be = int64_t;

	struct Fixed
	{
		u2be major;
		u2be minor;

		float GetFloat() const;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Fixed_2_14
	{
		uint16_t value;

		float GetFloat() const;

		void Parse(Scriptorium::Reader& reader);
	};

	struct DirTableEntry
	{
		std::string tag;
		u4be checksum;
		u4be offset;
		u4be length;

		void Parse(Scriptorium::Reader& reader);
	};

	struct OffsetTable
	{
		Fixed sfnt_version;
		u2be num_tables;
		u2be search_range;
		u2be entry_selector;
		u2be range_shift;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Format20
	{
		u2be number_of_glyphs;
		u2be glyph_name_index;

		void Parse(Scriptorium::Reader& reader);
	};

	struct SimpleGlyphFlags
	{
		union
		{
			struct
			{
				uint8_t on_curve : 1;
				uint8_t x_short_vector : 1;
				uint8_t y_short_vector : 1;
				uint8_t repeat : 1;
				uint8_t x_is_same : 1;
				uint8_t y_is_same : 1;
				uint8_t reserved : 2;
			};

			uint8_t value;
		};
	};

	struct CompoundGlyphFlags
	{
		union
		{
			struct
			{
				uint8_t arg_1_and_2_are_words : 1;
				uint8_t args_are_xy_values : 1;
				uint8_t round_xy_to_grid : 1;
				uint8_t we_have_a_scale : 1;
				uint8_t reserved_4 : 1;
				uint8_t more_components : 1;
				uint8_t we_have_an_x_and_y_scale : 1;
				uint8_t we_have_a_two_by_two : 1;
				uint8_t we_have_instructions : 1;
				uint8_t use_my_metrics : 1;
				uint8_t overlap_compound : 1;
				uint8_t scaled_component_offset : 1;
				uint8_t unscaled_component_offset : 1;
				uint8_t reserved_13 : 1;
				uint8_t reserved_14 : 1;
				uint8_t reserved_15 : 1;
			};

			uint16_t value;
		};
	};

	struct GlyfPoint
	{
		int16_t x = 0;
		int16_t y = 0;

		bool on_curve = true;
	};

	struct SimpleGlyph
	{
		std::vector<u2be> end_pts_of_contours;
		std::vector<SimpleGlyphFlags> flags;
		u2be instruction_length;
		std::vector<uint8_t> instructions;

		std::vector<GlyfPoint> coordinates;
	};

	struct CompoundGlyph
	{
		CompoundGlyphFlags flags;
		uint16_t glyph_index;

		double x_offset;
		double y_offset;

		double x_scale;
		double y_scale;
	};

	struct MaxpVersion10Body
	{
		u2be max_points;
		u2be max_contours;
		u2be max_composite_points;
		u2be max_composite_contours;
		u2be max_zones;
		u2be max_twilight_points;
		u2be max_storage;
		u2be max_function_defs;
		u2be max_instruction_defs;
		u2be max_stack_elements;
		u2be max_size_of_instructions;
		u2be max_component_elements;
		u2be max_component_depth;
	};

	enum class EncodingRecordPlatformID
	{
		Unicode = 0,
		Macintosh = 1,
		ISO = 2, // Deprecated
		Windows = 3,
		Custom = 4
	};

	struct EncodingRecord
	{
		EncodingRecordPlatformID platform_id;
		u2be encoding_id;
		u4be subtable_offset;
	};

	struct LongHorMetric
	{
		u2be advanceWidth;
		s2be leftSideBearing;
	};

	struct Head
	{
		Fixed version;
		Fixed font_revision;
		u4be checksum_adjustment;
		u4be magic_number;
		u2be flags;
		u2be units_per_em;
		u8be created;
		u8be modified;
		s2be x_min;
		s2be y_min;
		s2be x_max;
		s2be y_max;
		u2be mac_style;
		u2be lowest_rec_ppem;
		s2be font_direction_hint;
		s2be index_to_loc_format;
		s2be glyph_data_format;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Cvt
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Prep
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Kern
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Hhea
	{
		Fixed version;
		s2be ascender;
		s2be descender;
		s2be line_gap;
		u2be advance_width_max;
		s2be min_left_side_bearing;
		s2be min_right_side_bearing;
		s2be x_max_extend;
		s2be caret_slope_rise;
		s2be caret_slope_run;
		uint8_t reserved[10];
		s2be metric_data_format;
		u2be number_of_hmetrics;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Hmtx
	{
		std::vector<LongHorMetric>	hor_metrics;
		std::vector<s2be>			leftSideBearing;

		void Parse(Scriptorium::Reader& reader, u2be num_glyphs, u2be number_of_hmetrics);
	};

	struct Post
	{
		Fixed format;
		Fixed italic_angle;
		s2be underline_position;
		s2be underline_thichness;
		u4be is_fixed_pitch;
		u4be min_mem_type42;
		u4be max_mem_type42;
		u4be min_mem_type1;
		u4be max_mem_type1;
		Format20 format20;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Os2
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Name
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Maxp
	{
		Fixed table_version_number;
		u2be num_glyphs;
		MaxpVersion10Body version10_body;

		void Parse(Scriptorium::Reader& reader);
	};

	struct Glyf
	{
		s2be number_of_contours;
		s2be x_min;
		s2be y_min;
		s2be x_max;
		s2be y_max;
		SimpleGlyph value;

		std::vector<CompoundGlyph> compounds;

		void ParseSimpleGlyf(Scriptorium::Reader& reader);
		void ParseCompoundGlyf(Scriptorium::Reader& reader);

		void Parse(Scriptorium::Reader& reader);
	};

	struct Fpgm
	{
		void Parse(Scriptorium::Reader& reader);
	};

	struct Cmap
	{
		u2be version;
		u2be num_tables;
		std::vector<EncodingRecord> encoding_records;

		uint32_t operator[](uint32_t char_code) const;

		void Parse(Scriptorium::Reader& reader);

	private:
		std::unordered_map<uint32_t, uint32_t> map;
	};

	struct Loca
	{
		std::vector<uint32_t> offsets;
	};

	class TTF
	{
	public:
		explicit TTF(const std::filesystem::path& filename);
		explicit TTF(const uint8_t* buffer);

		OffsetTable offsetTable;

		Head	head;
		Cvt		cvt;
		Prep	prep;
		Kern	kern;
		Hhea	hhea;
		Hmtx	hmtx;
		Post	post;
		Os2		os2;
		Name	name;
		Maxp	maxp;
		Fpgm	fpgm;
		Cmap	cmap;
		Loca	loca;

		std::vector<Glyf> 	glyfs;

	public:
		struct GlyphMetrics
		{
			uint32_t Advance = 0;
			uint32_t LeftSizeBearing = 0;
		};

		const Glyf& GetGlyfByUnicode(uint32_t unicode) const;
		const GlyphMetrics GetGlyfMetricsByUnicode(uint32_t unicode) const;

	private:
		void Parse(Scriptorium::Reader& reader);

	private:
		std::unordered_map<std::string, size_t> m_TableLocation;
	};
}


