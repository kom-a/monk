#include "graphics/Renderer2D.h"

#include <MOGL/MOGL.h>
#include "core/Assert.h"

#include <string>

namespace monk
{
	const std::string g_QuadVertexSource = R"(
		#version 330 core

		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec4 a_Color;
		layout (location = 2) in float a_TextureID;
		layout (location = 3) in vec2 a_TextureCoords;

		uniform mat4 u_ProjectionView;

		out vec4 v_Color;
		out float v_TextureID;
		out vec2 v_TextureCoords;

		void main()
		{
			gl_Position = u_ProjectionView * vec4(a_Position, 1.0f);

			v_Color = a_Color;
			v_TextureID = a_TextureID;
			v_TextureCoords = a_TextureCoords;
		}
	)";

	const std::string g_QuadFragmentSource = R"(
		#version 330 core 

		out vec4 FragColor;

		in vec4 v_Color;
		in float v_TextureID;
		in vec2 v_TextureCoords;

		uniform sampler2D u_Texture0; // White texture
		uniform sampler2D u_Texture1;
		uniform sampler2D u_Texture2;
		uniform sampler2D u_Texture3;
		uniform sampler2D u_Texture4;
		uniform sampler2D u_Texture5;
		uniform sampler2D u_Texture6;
		uniform sampler2D u_Texture7;
		uniform sampler2D u_Texture8;
		uniform sampler2D u_Texture9;
		uniform sampler2D u_Texture10;
		uniform sampler2D u_Texture11;
		uniform sampler2D u_Texture12;
		uniform sampler2D u_Texture13;
		uniform sampler2D u_Texture14;
		uniform sampler2D u_Texture15;

		vec4 foo(float v)
		{
			return vec4(v, v, v, 1.0f);
		}

		void main()
		{
			int textureIndex = int(v_TextureID);
			switch(textureIndex)
			{
				case 0: FragColor = foo(texture(u_Texture0, v_TextureCoords).r) * v_Color; break;
				case 1: FragColor = foo(texture(u_Texture1, v_TextureCoords).r) * v_Color; break;
				case 2: FragColor = foo(texture(u_Texture2, v_TextureCoords).r) * v_Color; break;
				case 3: FragColor = foo(texture(u_Texture3, v_TextureCoords).r) * v_Color; break;
				case 4: FragColor = foo(texture(u_Texture4, v_TextureCoords).r) * v_Color; break;
				case 5: FragColor = foo(texture(u_Texture5, v_TextureCoords).r) * v_Color; break;
				case 6: FragColor = foo(texture(u_Texture6, v_TextureCoords).r) * v_Color; break;
				case 7: FragColor = foo(texture(u_Texture7, v_TextureCoords).r) * v_Color; break;
				case 8: FragColor = foo(texture(u_Texture8, v_TextureCoords).r) * v_Color; break;
				case 9: FragColor = foo(texture(u_Texture9, v_TextureCoords).r) * v_Color; break;
				case 10: FragColor = foo(texture(u_Texture10, v_TextureCoords).r) * v_Color; break;
				case 11: FragColor = foo(texture(u_Texture11, v_TextureCoords).r) * v_Color; break;
				case 12: FragColor = foo(texture(u_Texture12, v_TextureCoords).r) * v_Color; break;
				case 13: FragColor = foo(texture(u_Texture13, v_TextureCoords).r) * v_Color; break;
				case 14: FragColor = foo(texture(u_Texture14, v_TextureCoords).r) * v_Color; break;
				case 15: FragColor = foo(texture(u_Texture15, v_TextureCoords).r) * v_Color; break;
				default: FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
			}
		}
	)";

	const std::string g_CircleVertexSource = R"(
		#version 330 core

		layout (location = 0) in vec3  a_Position;
		layout (location = 1) in vec3  a_Center;
		layout (location = 2) in vec4  a_Color;
		layout (location = 3) in float a_Radius;

		uniform mat4 u_ProjectionView;

		out vec3 v_Position;
		out vec3 v_Center;
		out vec4 v_Color;
		out float v_Radius;

		void main()
		{
			gl_Position = u_ProjectionView * vec4(a_Position, 1.0f);

			v_Position	= a_Position;
			v_Center	= a_Center;
			v_Color		= a_Color;
			v_Radius	= a_Radius;
		}
	)";

	const std::string g_CircleFragmentSource = R"(
		#version 330 core 

		out vec4 FragColor;

		in vec3 v_Position;
		in vec3 v_Center;
		in vec4 v_Color;
		in float v_Radius;

		void main()
		{
			float dist = length(v_Position - v_Center);

			const float eps = 0.5f;
			FragColor = vec4(v_Color.xyz, 1.0f - smoothstep(v_Radius - eps, v_Radius + eps, dist));
		}
	)";

	const std::string g_TextVertexSource = R"(
		#version 330 core

		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec4 a_Color;
		layout (location = 2) in float a_AtlasIndex;
		layout (location = 3) in vec2 a_TextureCoords;

		uniform mat4 u_ProjectionView;

		out vec4 v_Color;
		out vec2 v_TextureCoords;
		out float v_AtlasIndex;

		void main()
		{
			gl_Position = u_ProjectionView * vec4(a_Position, 1.0f);

			v_Color = a_Color;
			v_AtlasIndex = a_AtlasIndex;
			v_TextureCoords = a_TextureCoords;
		}
	)";

	const std::string g_TextFragmentSource = R"(
		#version 330 core 

		out vec4 FragColor;

		in vec4 v_Color;
		in float v_AtlasIndex;
		in vec2 v_TextureCoords;

		uniform sampler2D u_Atlas0;
		uniform sampler2D u_Atlas1;
		uniform sampler2D u_Atlas2;
		uniform sampler2D u_Atlas3;
		uniform sampler2D u_Atlas4;
		uniform sampler2D u_Atlas5;
		uniform sampler2D u_Atlas6;
		uniform sampler2D u_Atlas7;
		uniform sampler2D u_Atlas8;
		uniform sampler2D u_Atlas9;
		uniform sampler2D u_Atlas10;
		uniform sampler2D u_Atlas11;
		uniform sampler2D u_Atlas12;
		uniform sampler2D u_Atlas13;
		uniform sampler2D u_Atlas14;
		uniform sampler2D u_Atlas15;

		vec4 GetRedChannel(vec4 color)
		{
			return vec4(color.r, color.r, color.r, color.r);
		}

		void main()
		{
			int textureIndex = int(v_AtlasIndex);
			switch(textureIndex)
			{
				case 0: FragColor = GetRedChannel(texture(u_Atlas0, v_TextureCoords)) * v_Color; break;
				case 1: FragColor = GetRedChannel(texture(u_Atlas1, v_TextureCoords)) * v_Color; break;
				case 2: FragColor = GetRedChannel(texture(u_Atlas2, v_TextureCoords)) * v_Color; break;
				case 3: FragColor = GetRedChannel(texture(u_Atlas3, v_TextureCoords)) * v_Color; break;
				case 4: FragColor = GetRedChannel(texture(u_Atlas4, v_TextureCoords)) * v_Color; break;
				case 5: FragColor = GetRedChannel(texture(u_Atlas5, v_TextureCoords)) * v_Color; break;
				case 6: FragColor = GetRedChannel(texture(u_Atlas6, v_TextureCoords)) * v_Color; break;
				case 7: FragColor = GetRedChannel(texture(u_Atlas7, v_TextureCoords)) * v_Color; break;
				case 8: FragColor = GetRedChannel(texture(u_Atlas8, v_TextureCoords)) * v_Color; break;
				case 9: FragColor = GetRedChannel(texture(u_Atlas9, v_TextureCoords)) * v_Color; break;
				case 10: FragColor = GetRedChannel(texture(u_Atlas10, v_TextureCoords)) * v_Color; break;
				case 11: FragColor = GetRedChannel(texture(u_Atlas11, v_TextureCoords)) * v_Color; break;
				case 12: FragColor = GetRedChannel(texture(u_Atlas12, v_TextureCoords)) * v_Color; break;
				case 13: FragColor = GetRedChannel(texture(u_Atlas13, v_TextureCoords)) * v_Color; break;
				case 14: FragColor = GetRedChannel(texture(u_Atlas14, v_TextureCoords)) * v_Color; break;
				case 15: FragColor = GetRedChannel(texture(u_Atlas15, v_TextureCoords)) * v_Color; break;
				default: FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
			}
		}
	)";

	Renderer2D::Renderer2D()
	{
		InitQuads();
		InitCircles();
		InitText();

		uint32_t whiteTexture = 0xffffffff;
		m_WhiteTexture = CreateUnique<Texture2D>(1, 1, TextureFormat::RGBA, (uint8_t*)&whiteTexture);

		m_DefaultFont = CreateRef<MFL::Font>("C:/Windows/Fonts/arial.ttf");
		m_AtlasTexture = Texture2D::Create(m_DefaultFont->GetAtlas().GetWidth(), m_DefaultFont->GetAtlas().GetHeight(), TextureFormat::RED, m_DefaultFont->GetAtlas().GetTexture());
	}

	void Renderer2D::Begin(Ref<const OrthographicCamera> camera)
	{
		m_Camera = camera;
		BeginQuadsBatch();
		BeginCirclesBatch();
		BeginTextBatch();
		ZDepth = -1.0f + FLT_EPSILON;
	}

	void Renderer2D::End()
	{
		EndQuadsBatch();
		EndCirclesBatch();
		EndTextBatch();
		Flush();
	}

	void Renderer2D::DrawQuad(const mml::vec2& position, const mml::vec2& size, const mml::vec4& color)
	{
		MONK_ASSERT(m_QuadVertexDataPtr);

		m_QuadVertexArray->Bind();

		if (m_QuadBatchStats.Quads >= QuadBatchSettings.MaxQuads)
			NextQuadBatch();

		QuadVertex* topLeftVertex = m_QuadVertexDataPtr;
		topLeftVertex->Position = mml::vec3(position.x, position.y, ZDepth);
		topLeftVertex->Color = color;
		topLeftVertex->TextureID = 0;
		topLeftVertex->TextureCoords = mml::vec2(0.0f);
		m_QuadVertexDataPtr++;

		QuadVertex* topRightVertex = m_QuadVertexDataPtr;
		*topRightVertex = *topLeftVertex;
		topRightVertex->Position.x += size.x;
		m_QuadVertexDataPtr++;

		QuadVertex* bottomRightVertex = m_QuadVertexDataPtr;
		*bottomRightVertex = *topRightVertex;
		bottomRightVertex->Position.y += size.y;
		m_QuadVertexDataPtr++;

		QuadVertex* bottomLeftVertex = m_QuadVertexDataPtr;
		*bottomLeftVertex = *topLeftVertex;
		bottomLeftVertex->Position.y += size.y;
		m_QuadVertexDataPtr++;

		m_QuadBatchStats.Quads += 1;
		m_QuadBatchStats.Verticies += 4;
		m_QuadBatchStats.Indices += 6;

		ZDepth += FLT_EPSILON;
	}

	void Renderer2D::DrawTexture(const mml::vec2& position, const mml::vec2& size, Ref<Texture2D> texture)
	{
		MONK_ASSERT(m_QuadVertexDataPtr);

		m_QuadVertexArray->Bind();

		if (m_QuadBatchStats.Quads >= QuadBatchSettings.MaxQuads || m_QuadBatchStats.Textures.size() >= QuadBatchSettings.MaxTextures)
			NextQuadBatch();

		float textureID = GetBatchTextureID(texture);

		QuadVertex* topLeftVertex = m_QuadVertexDataPtr;
		topLeftVertex->Position = mml::vec3(position.x, position.y, ZDepth);
		topLeftVertex->Color = mml::vec4(1.0f);
		topLeftVertex->TextureID = textureID;
		topLeftVertex->TextureCoords = mml::vec2(0.0f, 0.0f);
		m_QuadVertexDataPtr++;

		QuadVertex* topRightVertex = m_QuadVertexDataPtr;
		*topRightVertex = *topLeftVertex;
		topRightVertex->Position.x += size.x;
		topRightVertex->TextureCoords = mml::vec2(1.0f, 0.0f);
		m_QuadVertexDataPtr++;

		QuadVertex* bottomRightVertex = m_QuadVertexDataPtr;
		*bottomRightVertex = *topRightVertex;
		bottomRightVertex->Position.y += size.y;
		bottomRightVertex->TextureCoords = mml::vec2(1.0f, 1.0f);
		m_QuadVertexDataPtr++;

		QuadVertex* bottomLeftVertex = m_QuadVertexDataPtr;
		*bottomLeftVertex = *topLeftVertex;
		bottomLeftVertex->Position.y += size.y;
		bottomLeftVertex->TextureCoords = mml::vec2(0.0f, 1.0f);
		m_QuadVertexDataPtr++;

		m_QuadBatchStats.Quads += 1;
		m_QuadBatchStats.Verticies += 4;
		m_QuadBatchStats.Indices += 6;

		ZDepth += FLT_EPSILON;
	}

	void Renderer2D::DrawCircle(const mml::vec2& center, float radius, const mml::vec4& color)
	{
		MONK_ASSERT(m_CircleVertexDataPtr);

		m_CircleVertexArray->Bind();

		if (m_CircleBatchStats.Circles >= CircleBatchSettings.MaxCircles)
			NextCircleBatch();

		CircleVertex* topLeftVertex = m_CircleVertexDataPtr;
		topLeftVertex->Position = mml::vec3(center.x - radius, center.y - radius, ZDepth);
		topLeftVertex->Center = mml::vec3(center.x, center.y, 0.0f);
		topLeftVertex->Color = color;
		topLeftVertex->Radius = radius;
		m_CircleVertexDataPtr++;

		CircleVertex* topRightVertex = m_CircleVertexDataPtr;
		topRightVertex->Position = mml::vec3(center.x + radius, center.y - radius, ZDepth);
		topRightVertex->Center = mml::vec3(center.x, center.y, 0.0f);
		topRightVertex->Color = color;
		topRightVertex->Radius = radius;
		m_CircleVertexDataPtr++;

		CircleVertex* bottomRightVertex = m_CircleVertexDataPtr;
		bottomRightVertex->Position = mml::vec3(center.x + radius, center.y + radius, ZDepth);
		bottomRightVertex->Center = mml::vec3(center.x, center.y, 0.0f);
		bottomRightVertex->Color = color;
		bottomRightVertex->Radius = radius;
		m_CircleVertexDataPtr++;

		CircleVertex* bottomLeftVertex = m_CircleVertexDataPtr;
		bottomLeftVertex->Position = mml::vec3(center.x - radius, center.y + radius, ZDepth);
		bottomLeftVertex->Center = mml::vec3(center.x, center.y, 0.0f);
		bottomLeftVertex->Color = color;
		bottomLeftVertex->Radius = radius;
		m_CircleVertexDataPtr++;

		m_CircleBatchStats.Circles += 1;
		m_CircleBatchStats.Verticies += 4;
		m_CircleBatchStats.Indices += 6;

		ZDepth += FLT_EPSILON;
	}

	void Renderer2D::DrawString(const std::string& text, const mml::vec2& position, float fontSize, const mml::vec4& color /*= mml::vec4(1.0f)*/)
	{
		MONK_ASSERT(m_TextVertexDataPtr);

		m_TextVertexArray->Bind();

		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float maxRowHeight = 0.0f;

		for (uint32_t c : text)
		{
			MFL::GlyphData glyphData = m_DefaultFont->GetAtlas().GetGlyphData(c);
			const MFL::Glyf& glyf = m_DefaultFont->GetTTF().GetGlyfByUnicode(c);

			float ttfScale = (m_DefaultFont->GetAtlas().GetFontSize() / (m_DefaultFont->GetTTF().hhea.ascender - m_DefaultFont->GetTTF().hhea.descender));
			float scale = m_DefaultFont->GetAtlas().GetScaleForFontSize(fontSize);

			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyf.y_max * ttfScale * scale;
			float descender = glyf.y_min * ttfScale * scale;

			if (c == ' ')
			{
				float ttfAdvance = m_DefaultFont->GetTTF().GetGlyfMetricsByUnicode(c).Advance * ttfScale;
				float advance = ttfAdvance != 0 ? ttfAdvance : glyphData.Width;

				offsetX += advance * scale;
				continue;
			}
			else if (c == '\n' || position.x + width + offsetX > 1600)
			{
				offsetX = 0;
				offsetY += maxRowHeight + 8;
				maxRowHeight = 0;
				continue;
			}

			maxRowHeight = mml::Max(height, maxRowHeight);


			TextVertex* topLeftVertex = m_TextVertexDataPtr;
			topLeftVertex->Position = mml::vec3(position.x + offsetX, position.y + offsetY - ascender, ZDepth);
			topLeftVertex->Color = color;
			topLeftVertex->TextureCoords = mml::vec2((float)glyphData.UV_TopLeft.U / m_DefaultFont->GetAtlas().GetWidth(), (float)glyphData.UV_TopLeft.V / m_DefaultFont->GetAtlas().GetHeight());
			topLeftVertex->AtlasIndex = 0;
			m_TextVertexDataPtr++;

			TextVertex* topRightVertex = m_TextVertexDataPtr;
			topRightVertex->Position = mml::vec3(position.x + width + offsetX, position.y + offsetY - ascender, ZDepth);
			topRightVertex->Color = color;
			topRightVertex->TextureCoords = mml::vec2((float)glyphData.UV_TopRight.U / m_DefaultFont->GetAtlas().GetWidth(), (float)glyphData.UV_TopRight.V / m_DefaultFont->GetAtlas().GetHeight());
			topRightVertex->AtlasIndex = 0;
			m_TextVertexDataPtr++;

			TextVertex* bottomRightVertex = m_TextVertexDataPtr;
			bottomRightVertex->Position = mml::vec3(position.x + width + offsetX, position.y + height + offsetY - ascender, ZDepth);
			bottomRightVertex->Color = color;
			bottomRightVertex->TextureCoords = mml::vec2((float)glyphData.UV_BottomRight.U / m_DefaultFont->GetAtlas().GetWidth(), (float)glyphData.UV_BottomRight.V / m_DefaultFont->GetAtlas().GetHeight());
			bottomRightVertex->AtlasIndex = 0;
			m_TextVertexDataPtr++;

			TextVertex* bottomLeftVertex = m_TextVertexDataPtr;
			bottomLeftVertex->Position = mml::vec3(position.x + offsetX, position.y + height + offsetY - ascender, ZDepth);
			bottomLeftVertex->Color = color;
			bottomLeftVertex->TextureCoords = mml::vec2((float)glyphData.UV_BottomLeft.U / m_DefaultFont->GetAtlas().GetWidth(), (float)glyphData.UV_BottomLeft.V / m_DefaultFont->GetAtlas().GetHeight());
			bottomLeftVertex->AtlasIndex = 0;
			m_TextVertexDataPtr++;

			float ttfAdvance = m_DefaultFont->GetTTF().GetGlyfMetricsByUnicode(c).Advance * ttfScale;
			float advance = ttfAdvance != 0 ? ttfAdvance : glyphData.Width;

			offsetX += advance * scale;

			m_TextBatchStats.Characters += 1;
			m_TextBatchStats.Verticies += 4;
			m_TextBatchStats.Indices += 6;
		}

		ZDepth += FLT_EPSILON;
	}

	void Renderer2D::DrawLine(const mml::vec2& p0, const mml::vec2& p1, const mml::vec4& color, float thickness /*= 1.0f*/)
	{
		// TODO
	}

	void Renderer2D::DrawBezierCurve(const mml::vec2& p0, const mml::vec2& p1, const mml::vec2& controlPoint, const mml::vec4& color, float thickness /*= 2.0f*/, uint32_t segments /*= 64*/)
	{
#if 0
		MONK_ASSERT(m_VertexBufferData);

		glBindVertexArray(m_VAO);

		mml::vec2 prev = p0;

		mml::vec2 dir = mml::Normalize(p0 - controlPoint);
		mml::vec2 side = mml::vec2(-dir.y, dir.x) * thickness * 0.5f;

		mml::vec2 prevL = prev - side;
		mml::vec2 prevR = prev + side;

		for (uint32_t i = 0; i < segments; i++)
		{
			if (m_BatchStats.Quads >= m_BatchSettings.MaxQuads)
			{
				NextQuadBatch();
			}

			float t = (float)(i + 1) / segments;

			mml::vec2 tp0 = mml::Lerp(p0, controlPoint, t);
			mml::vec2 tp1 = mml::Lerp(controlPoint, p1, t);
			mml::vec2 point = mml::Lerp(tp0, tp1, t);

			mml::vec2 dir = mml::Normalize(point - prev);
			mml::vec2 side = mml::vec2(-dir.y, dir.x) * thickness * 0.5f;

			mml::vec2 pointL = point - side;
			mml::vec2 pointR = point + side;

			Vertex& _prevL = m_VertexBufferData[m_BatchStats.Verticies + 0];
			Vertex& _prevR = m_VertexBufferData[m_BatchStats.Verticies + 1];
			Vertex& _pointR = m_VertexBufferData[m_BatchStats.Verticies + 2];
			Vertex& _pointL = m_VertexBufferData[m_BatchStats.Verticies + 3];

			_prevL.Position = mml::vec3(prevL.x, prevL.y, 0.0f);
			_prevL.Color = color;

			_prevR.Position = mml::vec3(prevR.x, prevR.y, 0.0f);
			_prevR.Color = color;

			_pointR.Position = mml::vec3(pointR.x, pointR.y, 0.0f);
			_pointR.Color = color;

			_pointL.Position = mml::vec3(pointL.x, pointL.y, 0.0f);
			_pointL.Color = color;

			prev = point;
			prevL = pointL;
			prevR = pointR;

			m_BatchStats.Verticies += 4;
			m_BatchStats.Indices += 6;
			m_BatchStats.Quads += 1;
		}
#endif // 0
	}

	void Renderer2D::PushFont(Ref<MFL::Font> font)
	{
		m_FontStack.push_back(font);
	}

	void Renderer2D::PopFont()
	{
		if (m_FontStack.size() < 1)
		{
			LOG_WARNING("Attempt to pop default font");
			return;
		}

		m_FontStack.pop_back();
	}

	void Renderer2D::InitQuads()
	{
		m_QuadVertexArray = VertexArray::Create();
		m_QuadVertexArray->Bind();

		const size_t maxVertecies = QuadBatchSettings.MaxQuads * 4;
		const size_t maxIndices = QuadBatchSettings.MaxQuads * 6;

		m_QuadVertexBuffer = VertexBuffer::Create(sizeof(QuadVertex) * maxVertecies, QuadVertex::GetLayout());

		auto* indices = new uint32_t[maxIndices];

		size_t quad_index = 0;
		const size_t pattern[6] = { 0, 1, 2, 2, 3, 0 };
		for (int i = 0; i < maxIndices; i++)
		{
			if (i != 0 && i % 6 == 0)
				quad_index += 4;

			indices[i] = quad_index + pattern[i % 6];
		}

		m_QuadIndexBuffer = IndexBuffer::Create(indices, maxIndices);
		delete[] indices;

		glBindVertexArray(0);

		m_QuadShader = Shader::Create(g_QuadVertexSource, g_QuadFragmentSource);
	}

	void Renderer2D::InitCircles()
	{
		m_CircleVertexArray = VertexArray::Create();
		m_CircleVertexArray->Bind();

		const size_t maxVertecies = CircleBatchSettings.MaxCircles * 4;
		const size_t maxIndices = CircleBatchSettings.MaxCircles * 6;

		m_CircleVertexBuffer = VertexBuffer::Create(sizeof(CircleVertex) * maxVertecies, CircleVertex::GetLayout());

		auto* indices = new uint32_t[maxIndices];

		size_t quad_index = 0;
		const size_t pattern[6] = { 0, 1, 2, 2, 3, 0 };
		for (int i = 0; i < maxIndices; i++)
		{
			if (i != 0 && i % 6 == 0)
				quad_index += 4;

			indices[i] = quad_index + pattern[i % 6];
		}

		m_CircleIndexBuffer = IndexBuffer::Create(indices, maxIndices);
		delete[] indices;

		glBindVertexArray(0);

		m_CircleShader = Shader::Create(g_CircleVertexSource, g_CircleFragmentSource);
	}

	void Renderer2D::InitText()
	{
		m_TextVertexArray = VertexArray::Create();
		m_TextVertexArray->Bind();

		const size_t maxVertecies = TextBatchSettings.MaxChars * 6;
		const size_t maxIndices = TextBatchSettings.MaxChars * 4;

		m_TextVertexBuffer = VertexBuffer::Create(sizeof(TextVertex) * maxVertecies, TextVertex::GetLayout());

		auto* indices = new uint32_t[maxIndices];

		size_t quad_index = 0;
		const size_t pattern[6] = { 0, 1, 2, 2, 3, 0 };
		for (int i = 0; i < maxIndices; i++)
		{
			if (i != 0 && i % 6 == 0)
				quad_index += 4;

			indices[i] = quad_index + pattern[i % 6];
		}

		m_TextIndexBuffer = IndexBuffer::Create(indices, maxIndices);
		delete[] indices;

		glBindVertexArray(0);

		m_TextShader = Shader::Create(g_TextVertexSource, g_TextFragmentSource);
	}

	void Renderer2D::BeginQuadsBatch()
	{
		MONK_ASSERT(m_QuadVertexDataBase == nullptr);

		m_QuadVertexDataBase = (QuadVertex*)m_QuadVertexBuffer->Map();
		m_QuadVertexDataPtr = m_QuadVertexDataBase;

		m_QuadBatchStats.Textures.clear();
		m_QuadBatchStats.Textures.push_back(m_WhiteTexture);
	}

	void Renderer2D::EndQuadsBatch()
	{
		MONK_ASSERT(m_QuadVertexDataBase);

		m_QuadVertexBuffer->Unmap();

		m_QuadVertexDataBase = nullptr;
		m_QuadVertexDataPtr = nullptr;
	}

	void Renderer2D::BeginCirclesBatch()
	{
		MONK_ASSERT(m_CircleVertexDataBase == nullptr);

		m_CircleVertexDataBase = (CircleVertex*)m_CircleVertexBuffer->Map();
		m_CircleVertexDataPtr = m_CircleVertexDataBase;
	}

	void Renderer2D::EndCirclesBatch()
	{
		MONK_ASSERT(m_CircleVertexDataBase);

		m_CircleVertexBuffer->Unmap();

		m_CircleVertexDataBase = nullptr;
		m_CircleVertexDataPtr = nullptr;
	}

	void Renderer2D::BeginTextBatch()
	{
		MONK_ASSERT(m_TextVertexDataBase == nullptr);

		m_TextVertexDataBase = (TextVertex*)m_TextVertexBuffer->Map();
		m_TextVertexDataPtr = m_TextVertexDataBase;

		m_TextBatchStats.FontAtlases.clear();
		m_TextBatchStats.FontAtlases.push_back(m_AtlasTexture);
	}

	void Renderer2D::EndTextBatch()
	{
		MONK_ASSERT(m_TextVertexDataBase);

		m_TextVertexBuffer->Unmap();

		m_TextVertexDataBase = nullptr;
		m_TextVertexDataPtr = nullptr;
	}

	void Renderer2D::Flush()
	{
		if (m_QuadBatchStats.Quads)
			FlushQuads();
		if (m_CircleBatchStats.Circles)
			FlushCircles();
		if (m_TextBatchStats.Characters)
			FlushText();
	}

	void Renderer2D::FlushQuads()
	{
		MONK_ASSERT(m_QuadVertexDataPtr == nullptr && "Unmap quad buffer first");

		m_QuadVertexArray->Bind();
		m_QuadVertexBuffer->Bind();
		m_QuadIndexBuffer->Bind();
		m_QuadShader->Bind();

		m_QuadShader->SetMatrix4("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		for (int i = 0; i < m_QuadBatchStats.Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_QuadBatchStats.Textures[i]->GetID());
			m_QuadShader->SetInt(std::string("u_Texture") + std::to_string(i), i);
		}

		glDrawElements(GL_TRIANGLES, m_QuadBatchStats.Indices, GL_UNSIGNED_INT, nullptr);
		m_QuadBatchStats.Reset();
	}

	void Renderer2D::FlushCircles()
	{
		MONK_ASSERT(m_CircleVertexDataPtr == nullptr && "Unmap circles buffer first");

		m_CircleVertexArray->Bind();
		m_CircleVertexBuffer->Bind();
		m_CircleIndexBuffer->Bind();
		m_CircleShader->Bind();

		m_CircleShader->SetMatrix4("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		glDrawElements(GL_TRIANGLES, m_CircleBatchStats.Indices, GL_UNSIGNED_INT, nullptr);
		m_CircleBatchStats.Reset();
	}

	void Renderer2D::FlushText()
	{
		MONK_ASSERT(m_TextVertexDataPtr == nullptr && "Unmap text buffer first");

		m_TextVertexArray->Bind();
		m_TextVertexBuffer->Bind();
		m_TextIndexBuffer->Bind();
		m_TextShader->Bind();

		m_TextShader->SetMatrix4("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		for (int i = 0; i < m_TextBatchStats.FontAtlases.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_TextBatchStats.FontAtlases[i]->GetID());
			m_TextShader->SetInt(std::string("u_Atlas") + std::to_string(i), i);
		}

		glDrawElements(GL_TRIANGLES, m_TextBatchStats.Indices, GL_UNSIGNED_INT, nullptr);
		m_TextBatchStats.Reset();
	}

	void Renderer2D::NextQuadBatch()
	{
		EndQuadsBatch();
		FlushQuads();
		BeginQuadsBatch();
	}

	void Renderer2D::NextCircleBatch()
	{
		EndCirclesBatch();
		FlushCircles();
		BeginCirclesBatch();
	}

	float Renderer2D::GetBatchTextureID(Ref<Texture2D> texture)
	{
		uint32_t textureID = 0;

		for (int i = 0; i < m_QuadBatchStats.Textures.size(); i++)
		{
			if (texture->GetID() == m_QuadBatchStats.Textures[i]->GetID())
			{
				textureID = i;
				break;
			}
		}

		if (!textureID)
		{
			m_QuadBatchStats.Textures.push_back(texture);
			textureID = m_QuadBatchStats.Textures.size() - 1;
		}

		return (float)textureID;
	}

	}
