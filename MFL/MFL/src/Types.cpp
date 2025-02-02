#include "Types.h"

#include <assert.h>

namespace MFL
{
	float Fixed::GetFloat() const
	{
		return (float)((uint32_t)(major << 16) | (uint32_t)(minor)) / (float)(1 << 16);
	}

	void Fixed::Parse(Scriptorium::Reader& reader)
	{
		major = reader.ReadUInt16();
		minor = reader.ReadUInt16();
	}

	float Fixed_2_14::GetFloat() const
	{
		return ((float)value / (float)(1 << 14));
	}

	void Fixed_2_14::Parse(Scriptorium::Reader& reader)
	{
		value = reader.ReadUInt16();
	}

	void OffsetTable::Parse(Scriptorium::Reader& reader)
	{
		sfnt_version.Parse(reader);

		num_tables		= reader.ReadUInt16();
		search_range	= reader.ReadUInt16();
		entry_selector	= reader.ReadUInt16();
		range_shift		= reader.ReadUInt16();
	}

	DirTableEntry::DirTableEntry(Scriptorium::Reader& reader)
	{
		Parse(reader);
	}

	void DirTableEntry::Parse(Scriptorium::Reader& reader)
	{
		tag			= reader.ReadString(4);
		checksum	= reader.ReadUInt32();
		offset		= reader.ReadUInt32();
		length		= reader.ReadUInt32();
	}

	void Format20::Parse(Scriptorium::Reader& reader)
	{

	}

	void Head::Parse(Scriptorium::Reader& reader)
	{
		version.Parse(reader);
		font_revision.Parse(reader);

		checksum_adjustment	= reader.ReadUInt32();
		magic_number		= reader.ReadUInt32();
		flags				= reader.ReadUInt16();
		units_per_em		= reader.ReadUInt16();
		created				= reader.ReadUInt64();
		modified			= reader.ReadUInt64();
		x_min				= reader.ReadInt16();
		y_min				= reader.ReadInt16();
		x_max				= reader.ReadInt16();
		y_max				= reader.ReadInt16();
		mac_style			= reader.ReadUInt16();
		lowest_rec_ppem		= reader.ReadUInt16();
		font_direction_hint	= reader.ReadInt16();
		index_to_loc_format	= reader.ReadInt16();
		glyph_data_format	= reader.ReadInt16();
	}

	void Cvt::Parse(Scriptorium::Reader& reader)
	{

	}

	void Prep::Parse(Scriptorium::Reader& reader)
	{

	}

	void Kern::Parse(Scriptorium::Reader& reader)
	{

	}

	void Hhea::Parse(Scriptorium::Reader& reader)
	{
		version.Parse(reader);

		ascender				= reader.ReadInt16();
		descender				= reader.ReadInt16();
		line_gap				= reader.ReadInt16();
		advance_width_max		= reader.ReadUInt16();
		min_left_side_bearing	= reader.ReadInt16();
		min_right_side_bearing	= reader.ReadInt16();
		x_max_extend			= reader.ReadInt16();
		caret_slope_rise		= reader.ReadInt16();
		caret_slope_run			= reader.ReadInt16();
		reader.Skip(10);
		metric_data_format		= reader.ReadInt16();
		number_of_hmetrics		= reader.ReadUInt16();
	}

	void Hmtx::Parse(Scriptorium::Reader& reader, u2be num_glyphs, u2be number_of_hmetrics)
	{
		hor_metrics.reserve(number_of_hmetrics);

		for (size_t i = 0; i < number_of_hmetrics; i++)
		{
			LongHorMetric metric;
			metric.advanceWidth		= reader.ReadUInt16();
			metric.leftSideBearing	= reader.ReadInt16();

			hor_metrics.push_back(metric);
		}

		for (size_t i = 0; i < num_glyphs - number_of_hmetrics; i++)
			leftSideBearing.push_back(reader.ReadInt16());
	}

	void Post::Parse(Scriptorium::Reader& reader)
	{
		format.Parse(reader);
		italic_angle.Parse(reader);

		underline_position	= reader.ReadInt16();
		underline_thichness = reader.ReadInt16();
		is_fixed_pitch		= reader.ReadUInt32();
		min_mem_type42		= reader.ReadUInt32();
		max_mem_type42		= reader.ReadUInt32();
		min_mem_type1		= reader.ReadUInt32();
		max_mem_type1		= reader.ReadUInt32();

		format20.Parse(reader);
	}

	void Os2::Parse(Scriptorium::Reader& reader)
	{

	}

	void Name::Parse(Scriptorium::Reader& reader)
	{

	}

	void Maxp::Parse(Scriptorium::Reader& reader)
	{
		table_version_number.Parse(reader);
		num_glyphs								= reader.ReadUInt16();

		version10_body.max_points				= reader.ReadUInt16();
		version10_body.max_contours				= reader.ReadUInt16();
		version10_body.max_composite_points		= reader.ReadUInt16();
		version10_body.max_composite_contours	= reader.ReadUInt16();
		version10_body.max_zones				= reader.ReadUInt16();
		version10_body.max_twilight_points		= reader.ReadUInt16();
		version10_body.max_storage				= reader.ReadUInt16();
		version10_body.max_function_defs		= reader.ReadUInt16();
		version10_body.max_instruction_defs		= reader.ReadUInt16();
		version10_body.max_stack_elements		= reader.ReadUInt16();
		version10_body.max_size_of_instructions = reader.ReadUInt16();
		version10_body.max_component_elements	= reader.ReadUInt16();
		version10_body.max_component_depth		= reader.ReadUInt16();
	}

	void Glyf::Parse(Scriptorium::Reader& reader)
	{
		number_of_contours	= reader.ReadInt16();
		x_min				= reader.ReadInt16();
		y_min				= reader.ReadInt16();
		x_max				= reader.ReadInt16();
		y_max				= reader.ReadInt16();

		if (number_of_contours == -1) // Compound glyf
			ParseCompoundGlyf(reader);
		else
			ParseSimpleGlyf(reader);
	}

	void Glyf::ParseSimpleGlyf(Scriptorium::Reader& reader)
	{
		value.end_pts_of_contours.resize(number_of_contours);
		for (size_t i = 0; i < number_of_contours; i++)
			value.end_pts_of_contours[i] = reader.ReadUInt16();

		size_t num_points = value.end_pts_of_contours.size() ? value.end_pts_of_contours.back() + 1 : 0;
		value.flags.resize(num_points);
		value.coordinates.resize(num_points);

		value.instruction_length = reader.ReadUInt16();
		reader.Skip(value.instruction_length);

		for (size_t i = 0; i < num_points; i++)
		{
			uint8_t flag = reader.ReadUInt8();
			SimpleGlyphFlags sgflag;
			sgflag.value = flag;
			value.flags[i] = sgflag;
			value.coordinates[i].on_curve = sgflag.on_curve;

			if (sgflag.repeat)
			{
				size_t reps = reader.ReadUInt8();
				for (size_t r = 0; r < reps; r++)
				{
					i++;

					value.flags[i] = sgflag;
					value.coordinates[i].on_curve = sgflag.on_curve;
				}
			}
		}

		for (size_t i = 0; i < num_points; i++)
		{
			value.coordinates[i].x = i == 0 ? 0 : value.coordinates[i - 1].x;

			if (value.flags[i].x_short_vector)
			{
				uint8_t offset = reader.ReadUInt8();
				int sign = value.flags[i].x_is_same ? 1 : -1;
				value.coordinates[i].x += offset * sign;
			}
			else if (!value.flags[i].x_is_same)
				value.coordinates[i].x += reader.ReadInt16();
		}

		for (size_t i = 0; i < num_points; i++)
		{
			value.coordinates[i].y = i == 0 ? 0 : value.coordinates[i - 1].y;

			if (value.flags[i].y_short_vector)
			{
				uint8_t offset = reader.ReadUInt8();
				int sign = value.flags[i].y_is_same ? 1 : -1;
				value.coordinates[i].y += offset * sign;
			}
			else if (!value.flags[i].y_is_same)
				value.coordinates[i].y += reader.ReadInt16();
		}
	}

	void Glyf::ParseCompoundGlyf(Scriptorium::Reader& reader)
	{
		do
		{
			CompoundGlyph compound;

			compound.flags.value = reader.ReadUInt16();
			compound.glyph_index = reader.ReadUInt16();

			if (!compound.flags.args_are_xy_values)
				assert(false && "args are points, not offsets");

			compound.x_offset = compound.flags.arg_1_and_2_are_words ? reader.ReadInt16() : reader.ReadInt8();
			compound.y_offset = compound.flags.arg_1_and_2_are_words ? reader.ReadInt16() : reader.ReadInt8();

			compound.x_scale = 1.0;
			compound.y_scale = 1.0;

			if (compound.flags.we_have_a_scale)
			{
				Fixed_2_14 scale;
				scale.Parse(reader);
				compound.x_scale = scale.GetFloat();
				compound.y_scale = scale.GetFloat();
			}
			else if (compound.flags.we_have_an_x_and_y_scale)
			{
				Fixed_2_14 scale_x; 
				Fixed_2_14 scale_y;
				scale_x.Parse(reader);
				scale_y.Parse(reader);

				compound.x_scale = scale_x.GetFloat();
				compound.y_scale = scale_y.GetFloat();
			}
			else if (compound.flags.we_have_a_two_by_two)
			{
				assert(false && "we_have_a_two_by_two");
			}

			compounds.push_back(compound);
		} while (compounds.back().flags.more_components);
	}

	void Fpgm::Parse(Scriptorium::Reader& reader)
	{
		
	}

	uint32_t Cmap::operator[](uint32_t char_code) const
	{
		if (map.find(char_code) == map.end())
			return 0;

		if (map.find(char_code) == map.end())
			return 2;

		return map.at(char_code);
	}

	void Cmap::Parse(Scriptorium::Reader& reader)
	{
		size_t cmap_offset = reader.GetCursor();

		version		= reader.ReadUInt16();
		num_tables	= reader.ReadUInt16();

		uint32_t cmap_subtable_offset = (uint32_t)-1;

		for (size_t i = 0; i < num_tables; i++)
		{
			EncodingRecord encoding_record;
			encoding_record.platform_id		= (EncodingRecordPlatformID)reader.ReadUInt16();
			encoding_record.encoding_id		= reader.ReadUInt16();
			encoding_record.subtable_offset = reader.ReadUInt32();

			if (encoding_record.platform_id == EncodingRecordPlatformID::Unicode)
			{
				const auto& unicode_version_info = encoding_record.encoding_id;

				if (unicode_version_info == 3) // 	Unicode 2.0 and onwards semantics, Unicode BMP only
					cmap_subtable_offset = encoding_record.subtable_offset;
				else if(unicode_version_info == 4) // 	Unicode 2.0 and onwards semantics, Unicode full repertoire
					cmap_subtable_offset = encoding_record.subtable_offset;
			}
		}

		assert(cmap_subtable_offset != (uint32_t)-1 && "Font does not contain supported character map type");

		reader.PushCursor(cmap_offset + cmap_subtable_offset);

		uint16_t format = reader.ReadUInt16();
		assert((format == 4 || format == 12) && "Font cmap format not supported");

		if (format == 12)
		{
			int reserved				= reader.ReadUInt16();
			uint32_t subtable_length	= reader.ReadUInt32();
			uint32_t language_code		= reader.ReadUInt32();
			uint32_t num_groups			= reader.ReadUInt32();

			for (uint32_t i = 0; i < num_groups; i++)
			{
				uint32_t start_char_code	= reader.ReadUInt32();
				uint32_t end_char_code		= reader.ReadUInt32();
				uint32_t start_glyph_index	= reader.ReadUInt32();

				uint32_t num_chars = end_char_code - start_char_code + 1;
				for (uint32_t charCodeOffset = 0; charCodeOffset < num_chars; charCodeOffset++)
				{
					uint32_t char_code		= start_char_code + charCodeOffset;
					uint32_t glyph_index	= start_glyph_index + charCodeOffset;

					map.try_emplace(char_code, glyph_index);
				}
			}
		}
		else if (format == 4)
		{
			uint16_t subtable_length	= reader.ReadUInt16();
			uint16_t language_code		= reader.ReadUInt16();

			uint16_t seg_count_2x = reader.ReadUInt16();
			uint16_t seg_count = seg_count_2x / 2;

			uint16_t search_range	= reader.ReadUInt16();
			uint16_t entry_selector = reader.ReadUInt16();
			uint16_t range_shift	= reader.ReadUInt16();

			std::vector<uint16_t> end_codes(seg_count);
			for (uint16_t i = 0; i < seg_count; i++)
				end_codes[i] = reader.ReadUInt16();

			reader.Skip(2); // Reserved

			std::vector<uint16_t> start_codes(seg_count);
			for (uint16_t i = 0; i < seg_count; i++)
				start_codes[i] = reader.ReadUInt16();

			std::vector<uint16_t> id_deltas(seg_count);
			for (uint16_t i = 0; i < seg_count; i++)
				id_deltas[i] = reader.ReadUInt16();

			std::vector<std::pair<uint16_t, size_t>> id_range_offsets(seg_count);
			for (uint16_t i = 0; i < seg_count; i++)
			{
				size_t location = reader.GetCursor();
				uint16_t offset = reader.ReadUInt16();
				id_range_offsets[i] = std::make_pair(offset, location);
			}

			for (size_t i = 0; i < start_codes.size(); i++)
			{
				uint32_t end_code = end_codes[i];
				uint32_t current_code = start_codes[i];
				
				while (current_code <= end_code)
				{
					int glyph_index = 0;

					if (id_range_offsets[i].first == 0)
					{
						glyph_index = (current_code + id_deltas[i]) % 65536;
					}
					else 
					{
						size_t range_offset_location = id_range_offsets[i].first + id_range_offsets[i].second;
						size_t glypg_index_array_location = 2 * (current_code - start_codes[i]) + range_offset_location;

						reader.PushCursor(glypg_index_array_location);
						uint16_t glyph_index_offset = reader.ReadUInt16();
						reader.PopCursor();

						if (glyph_index_offset != 0)
							glyph_index = (glyph_index_offset + id_deltas[i]) % 65536;
					}

					map.try_emplace(current_code, glyph_index);
					current_code++;
				}
			}
		}

		reader.PopCursor();
	}

}