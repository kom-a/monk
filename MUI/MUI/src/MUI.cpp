#include "MUI.h"

#include <cassert>
#include <array>
#include <unordered_map>

#include <MOGL/MOGL.h>
#include <MFL/MFL.h>

#include "Render/Renderer.h"

namespace mui
{
	IMPL_LOGGER("MUI");

	struct Style
	{
		Vec4f WindowBackgroudColor			= Vec4f(0.20f, 0.20f, 0.20f, 0.8f);
		Vec4f WindowBackgroudHotColor		= Vec4f(0.20f, 0.20f, 0.20f, 0.8f);
		Vec4f WindowTitlebarColor			= Vec4f(0.10f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowTitlebarHotColor		= Vec4f(0.10f, 0.10f, 0.20f, 1.0f);
		Vec4f WindowBorderColor				= Vec4f(0.10f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowResizeColor				= Vec4f(0.80f, 0.80f, 0.80f, 1.0f);
		Vec4f WindowCloseButtonColor		= Vec4f(0.60f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowCloseButtonHotColor		= Vec4f(0.70f, 0.20f, 0.20f, 1.0f);
		Vec4f WindowCollapseButtonColor		= Vec4f(0.50f, 0.50f, 0.50f, 1.0f);
		Vec4f WindowCollapseButtonHotColor	= Vec4f(0.60f, 0.60f, 0.60f, 1.0f);

		Vec4f ButtonColor					= Vec4f(0.10f, 0.10f, 0.30f, 1.0f);

		Vec2f ButtonSize					= Vec2f(100.0f, 25.0f);

		float WindowTitlebarHeight			= 24.0f;
		float WindowTitlebarItem			= 0.6f;
		float WindowTitlebarItemPaddings	= 4.0f;
		float WindowBorderThickness			= 2.0f;
		float WindowResizeThickness			= 16.0f;
		float Padding						= 8.0f;
	};

	struct Window
	{
		std::string	Name;
		bool Active;
		bool Collapsed;
		bool* Open;

		uint32_t Order;

		Vec2f Position	= Vec2f(100, 100);
		Vec2f Size		= Vec2f(400, 400);

		bool TitlebarHot()	const;
		bool BodyHot()		const;
		bool ResizeHot()	const;
		bool CollapseHot()	const;
		bool CloseHot()		const;
		bool Hot()			const;
	};

	struct Context
	{
		HWND Handle = nullptr;

		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t ShaderID = 0;
		std::array<float, 16> OrthoMatrix;
		uint32_t DisplayWidth;
		uint32_t DisplayHeight;

		Vec2f NextFirstWindowCreationPosition = Vec2f(100.0f, 100.0f);

		std::unordered_map<std::string, Window> Windows;
		std::list<Window*> WindowsDrawOrder;
		Window* CurrentWindow	= nullptr;
		Window* DraggingWindow	= nullptr;
		Window* ResizingWindow	= nullptr;
		Window* HotWindow		= nullptr;

		std::vector<Vertex> DrawBuffer;
	};

	struct RenderSettings
	{
		uint32_t MaxVertices = 4096;
	};

	static Context				g_Context;
	static OpenGLRestoreState	g_OpenGlRestoreState;
	static const RenderSettings g_RenderSettings;
	static Style				g_Style;
	static Input				g_Input;
	static Renderer*			g_Renderer;

	static const std::string g_VertexShaderSrc = R"(
		#version 330 core

		layout (location = 0) in vec2 a_Position;
		layout (location = 1) in vec4 a_Color;
			
		uniform mat4 u_ProjectionMatrix;

		out vec4 v_Color;

		void main()
		{
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0f, 1.0f);
			v_Color		= a_Color;
		}
	)";

	static const std::string g_FragmentShaderSrc = R"(
		#version 330 core

		in vec4 v_Color;
		
		out vec4 FragColor;

		void main()
		{
			FragColor = v_Color;
		}
	)";

	bool Window::TitlebarHot() const
	{
		const auto& x = g_Input.MouseX;
		const auto& y = g_Input.MouseY;

		const auto mx = Position.X + Size.X;
		const auto my = Position.Y + g_Style.WindowTitlebarHeight;

		return x >= Position.X && x <= mx && y >= Position.Y && y <= my;
	}

	bool Window::BodyHot() const
	{
		if (Collapsed)
			return false;

		const auto& x = g_Input.MouseX;
		const auto& y = g_Input.MouseY;

		const auto mx = Position.X + Size.X;
		const auto my = Position.Y + Size.Y;

		return x >= Position.X && x <= mx && 
			y >= Position.Y + g_Style.WindowTitlebarHeight && y <= my + g_Style.WindowTitlebarHeight;
	}

	bool Window::ResizeHot() const
	{
		const auto& x = g_Input.MouseX;
		const auto& y = g_Input.MouseY;

		const auto rx = Position.X + Size.X - g_Style.WindowResizeThickness;
		const auto ry = Position.Y + Size.Y + g_Style.WindowTitlebarHeight - g_Style.WindowResizeThickness;

		return x >= rx && x <= rx + g_Style.WindowResizeThickness && y >= ry && y <= ry + g_Style.WindowResizeThickness;
	}

	bool Window::CollapseHot() const
	{
		const Vec2f itemSize = Vec2f(
			g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight, 
			g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight);

		Vec2f position = Vec2f(
			Position.X + Size.X - g_Style.WindowTitlebarItemPaddings - itemSize.X,
			Position.Y + (g_Style.WindowTitlebarHeight - itemSize.Y) * 0.5f);

		if (Open)
			position.X -= itemSize.X + g_Style.WindowTitlebarItemPaddings;

		const auto& x = g_Input.MouseX;
		const auto& y = g_Input.MouseY;

		const auto rx = position.X + itemSize.X;
		const auto ry = position.Y + itemSize.Y;

		return x >= position.X && x <= rx && y >= position.Y && y <= ry;
	}

	bool Window::CloseHot() const
	{
		if (!Open)
			return false;

		const Vec2f itemSize = Vec2f(
			g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight,
			g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight);

		Vec2f position = Vec2f(
			Position.X + Size.X - g_Style.WindowTitlebarItemPaddings - itemSize.X,
			Position.Y + (g_Style.WindowTitlebarHeight - itemSize.Y) * 0.5f);

		const auto& x = g_Input.MouseX;
		const auto& y = g_Input.MouseY;

		const auto rx = position.X + itemSize.X;
		const auto ry = position.Y + itemSize.Y;

		return x >= position.X && x <= rx && y >= position.Y && y <= ry;
	}

	bool Window::Hot() const
	{
		return TitlebarHot() || BodyHot();
	}

	static void CheckError(uint32_t object, uint32_t status)
	{
		switch (status)
		{
		case GL_COMPILE_STATUS:
		{
			int success;
			glGetShaderiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetShaderInfoLog(object, length, &length, infoLog);
				LOG_ERROR(infoLog);
			}
		} break;
		case GL_LINK_STATUS:
		{
			int success;
			glGetProgramiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetProgramInfoLog(object, length, &length, infoLog);
				LOG_ERROR(infoLog);
			}
		} break;
		default:
		{
			LOG_ERROR("Unexpected CheckError status");
		}
		}
	}

	static uint32_t CompileShader(std::string_view shaderSrc, uint32_t type)
	{
		uint32_t shader = glCreateShader(type);

		const char* src = shaderSrc.data();
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);
		CheckError(shader, GL_COMPILE_STATUS);

		return shader;
	}

	static uint32_t CreateShader(std::string_view vertexSrc, std::string_view fragmentSrc)
	{
		uint32_t shader = glCreateProgram();
		uint32_t vertexShader = CompileShader(vertexSrc, GL_VERTEX_SHADER);
		uint32_t fragmentShader = CompileShader(fragmentSrc, GL_FRAGMENT_SHADER);
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		CheckError(shader, GL_LINK_STATUS);

		return shader;
	}

	static uint32_t CreateVertexArrayObject()
	{
		uint32_t vertexArrayObject = 0;
		glGenVertexArrays(1, &vertexArrayObject);

		return vertexArrayObject;
	}

	static uint32_t CreateVertexBufferObject()
	{
		glBindVertexArray(g_Context.VAO);

		uint32_t vertexBufferObject = 0;
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		const uint32_t bufferSize = g_RenderSettings.MaxVertices * sizeof(Vertex);

		glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		return vertexBufferObject;
	}

	static void SetupRenderState()
	{
		RECT displayRect = { 0 };
		::GetClientRect(g_Context.Handle, &displayRect);

		g_Context.DisplayWidth	= displayRect.right - displayRect.left;
		g_Context.DisplayHeight = displayRect.bottom - displayRect.top;

		const auto L = (float)displayRect.left;
		const auto R = (float)displayRect.right;
		const auto T = (float)displayRect.top;
		const auto B = (float)displayRect.bottom;

		g_Context.OrthoMatrix = {
			2.0f / (R - L)		, 0.0f				, 0.0f	, 0.0f,
			0.0f				, 2.0f / (T - B)	, 0.0f	, 0.0f,
			0.0f				, 0.0f				, -1.0f	, 0.0f,
			(R + L) / (L - R)	, (T + B) / (B - T)	, 0.0f	, 1.0f
		};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	static void InitRenderData()
	{
		g_Context.ShaderID	= CreateShader(g_VertexShaderSrc, g_FragmentShaderSrc);
		g_Context.VAO		= CreateVertexArrayObject();
		g_Context.VBO		= CreateVertexBufferObject();
	}

	static void ClampWindowInDisplayView(Window* window)
	{
		if (g_Context.DisplayHeight == 0)
			return;

		const float padding = 25;

		if (window->Position.X > g_Context.DisplayWidth)
			window->Position.X = g_Context.DisplayWidth - padding;

		if (window->Position.Y > g_Context.DisplayHeight)
			window->Position.Y = g_Context.DisplayHeight - padding;

	}

	void InitForWin32(HWND handle)
	{
		g_Context.Handle = handle;

		InitRenderData();
		SetupRenderState();

		g_Renderer = new Renderer();
	}

	void Shutdown()
	{
		delete g_Renderer;
	}

	Input& GetInput()
	{
		return g_Input;
	}

	void NewFrame()
	{
		g_Input.MouseLeftClicked = g_Input.MouseLeftDown && !g_Input.LastMouseLeftDown;

		for (auto& [name, window] : g_Context.Windows)
		{
			window.Active = false;
		}

		g_Context.WindowsDrawOrder.clear();

		g_Renderer->BeginDraw();
		g_Renderer->DrawRect(Vec2f(100, 100), Vec2f(500, 500), Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
	}

	void EndFrame()
	{
		Window* clickedWindow	= nullptr;
		Window* hotWindow		= nullptr;
		Window* draggingWindow	= nullptr;
		Window* resizeingWidnow = nullptr;
		Window* collapsingWindow = nullptr;
		Window* closingWindow	= nullptr;

		for (auto& [name, window] : g_Context.Windows)
		{
			if(!window.Active)
				continue;

			if (window.Hot() && g_Input.MouseLeftClicked)
			{
				if (clickedWindow)
				{
					if (window.Order > clickedWindow->Order)
						clickedWindow = &window;
				}
				else 
					clickedWindow = &window;
			}

			g_Context.WindowsDrawOrder.push_back(&window);
		}

		if (clickedWindow)
			clickedWindow->Order = (uint32_t)-1;

		g_Context.WindowsDrawOrder.sort([](const Window* left, const Window* right) {
				return left->Order < right->Order;
			});

		uint32_t order = 0;
		for (auto window : g_Context.WindowsDrawOrder)
		{
			ClampWindowInDisplayView(window);

			if (window->TitlebarHot() && g_Input.MouseLeftClicked)
				draggingWindow = window;

			if (window->ResizeHot() && g_Input.MouseLeftClicked)
				resizeingWidnow = window;

			if (window->CollapseHot() && g_Input.MouseLeftClicked)
				collapsingWindow = window;

			if (window->CloseHot() && g_Input.MouseLeftClicked)
				closingWindow = window;

			if (window->Hot())
				hotWindow = window;

			window->Order = order++;
		}

		g_Context.HotWindow = hotWindow;

		if (!g_Input.MouseLeftDown)
		{
			g_Context.DraggingWindow = nullptr;
			g_Context.ResizingWindow = nullptr;
		}

		if (clickedWindow != draggingWindow)
			draggingWindow = nullptr;
		if (clickedWindow != resizeingWidnow)
			resizeingWidnow = nullptr;
		if (clickedWindow != collapsingWindow)
			collapsingWindow = nullptr;

		if (g_Context.DraggingWindow && g_Context.ResizingWindow)
		{
			if (g_Context.DraggingWindow->Order > g_Context.ResizingWindow->Order)
				g_Context.ResizingWindow = nullptr;
			else
				g_Context.DraggingWindow = nullptr;
		}
		
		if (g_Context.DraggingWindow)
		{
			int dx = g_Input.MouseX - g_Input.LastMouseX;
			int dy = g_Input.MouseY - g_Input.LastMouseY;

			g_Context.DraggingWindow->Position = Vec2f(g_Context.DraggingWindow->Position.X + dx, 
				g_Context.DraggingWindow->Position.Y + dy);
		}

		if (g_Context.ResizingWindow && !g_Context.ResizingWindow->Collapsed)
		{
			int dx = g_Input.MouseX - g_Input.LastMouseX;
			int dy = g_Input.MouseY - g_Input.LastMouseY;

			g_Context.ResizingWindow->Size = Vec2f(g_Context.ResizingWindow->Size.X + dx,
				g_Context.ResizingWindow->Size.Y + dy);

			if (g_Context.ResizingWindow->Size.X < g_Style.WindowResizeThickness * 4)
				g_Context.ResizingWindow->Size.X = g_Style.WindowResizeThickness * 4;
				
			if (g_Context.ResizingWindow->Size.Y < g_Style.WindowResizeThickness * 2)
				g_Context.ResizingWindow->Size.Y = g_Style.WindowResizeThickness * 2;
		}

		if (collapsingWindow)
			collapsingWindow->Collapsed = !collapsingWindow->Collapsed;
		else if (closingWindow)
			*closingWindow->Open = false;
		else
		{
			if (draggingWindow && !g_Context.DraggingWindow)
				g_Context.DraggingWindow = draggingWindow;

			if (resizeingWidnow && !g_Context.ResizingWindow)
				g_Context.ResizingWindow = resizeingWidnow;
		}

		g_Input.LastMouseLeftDown = g_Input.MouseLeftDown;
		g_Input.LastMouseX = g_Input.MouseX;
		g_Input.LastMouseY = g_Input.MouseY;

		g_Renderer->EndDraw();
	}

	void Begin(const std::string& name, bool* open)
	{
		auto itWnd = g_Context.Windows.find(name);
		if (itWnd != g_Context.Windows.end())
		{
			// Restore window
			g_Context.CurrentWindow = &itWnd->second;
		}
		else
		{
			// Create new window
			Window newWindow;
			newWindow.Name = name;
			newWindow.Position = g_Context.NextFirstWindowCreationPosition;
			newWindow.Size = Vec2f(300, 300);
			newWindow.Order = g_Context.Windows.size() + 1;
			newWindow.Collapsed = false;
			g_Context.Windows.try_emplace(name, newWindow);

			g_Context.CurrentWindow = &g_Context.Windows.find(name)->second;

			g_Context.NextFirstWindowCreationPosition = Vec2f(
				g_Context.NextFirstWindowCreationPosition.X + 50,
				g_Context.NextFirstWindowCreationPosition.Y + 50);
		}

		g_Context.CurrentWindow->Active = open ? *open : true;
		g_Context.CurrentWindow->Open	= open;
	}

	void End()
	{
		//
	}

	bool Button(const std::string& text)
	{
		


		return false;
	}

	void Showdown()
	{
		// 
	}

	void DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color)
	{
		/*Vertex v0;
		v0.Position = position;
		v0.Color = color;

		Vertex v1;
		v1.Position = Vec2f(position.X + size.X, position.Y);
		v1.Color = color;

		Vertex v2;
		v2.Position = Vec2f(position.X + size.X, position.Y + size.Y);
		v2.Color = color;

		Vertex v3;
		v3.Position = Vec2f(position.X, position.Y + size.Y);
		v3.Color = color;

		g_Context.DrawBuffer.push_back(v0);
		g_Context.DrawBuffer.push_back(v1);
		g_Context.DrawBuffer.push_back(v2);

		g_Context.DrawBuffer.push_back(v0);
		g_Context.DrawBuffer.push_back(v2);
		g_Context.DrawBuffer.push_back(v3);*/
	}

	void DrawWindowTitlebar(const Window& window)
	{
		const Vec2f titlebarSize = Vec2f(window.Size.X, g_Style.WindowTitlebarHeight);
		const Vec4f titlebarColor = (g_Context.HotWindow && g_Context.HotWindow == &window) ? g_Style.WindowTitlebarHotColor : g_Style.WindowTitlebarColor;
		
		// Draw titlebar background
		DrawQuad(window.Position, titlebarSize, titlebarColor);

		const float itemSize = titlebarSize.Y * g_Style.WindowTitlebarItem;
		const Vec2f titlebarItemSize = Vec2f(itemSize, itemSize);

		// Draw close button
		const Vec2f closeButtonPosition = Vec2f(
			window.Position.X + window.Size.X - g_Style.WindowTitlebarItemPaddings - itemSize,
			window.Position.Y + (titlebarSize.Y - itemSize) * 0.5f);
		
		if (window.Open != nullptr)
		{
			const Vec4f closeButtonColor = window.CloseHot() ? g_Style.WindowCloseButtonHotColor : g_Style.WindowCloseButtonColor;
			DrawQuad(closeButtonPosition, titlebarItemSize, closeButtonColor);
		}

		// Draw collapse button
		Vec4f collapseButtonColor = window.CollapseHot() ? g_Style.WindowCollapseButtonHotColor : g_Style.WindowCollapseButtonColor;
		Vec2f collapseButtonPosition = closeButtonPosition;
		if(window.Open != nullptr)
			collapseButtonPosition.X -= titlebarItemSize.X + g_Style.WindowTitlebarItemPaddings;
		DrawQuad(collapseButtonPosition, titlebarItemSize, collapseButtonColor);
	}

	void Render()
	{
		SetupRenderState();

		for (const auto& windowPtr : g_Context.WindowsDrawOrder)
		{
			const Window& window = *windowPtr;

			if(!window.Active)
 				continue;

			DrawWindowTitlebar(window);

			if(window.Collapsed)
				continue;

			const Vec2f titlebarSize = Vec2f(window.Size.X, g_Style.WindowTitlebarHeight);

			// Draw window backgroud
			const Vec2f windowClientPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y);
			const Vec4f backgroundColor = (g_Context.HotWindow && g_Context.HotWindow == &window) ? g_Style.WindowBackgroudHotColor : g_Style.WindowBackgroudColor;
			DrawQuad(windowClientPosition, window.Size, backgroundColor);

			// Draw window resize
			Vec2f resizePosition = Vec2f(
				window.Position.X + window.Size.X - g_Style.WindowResizeThickness,
				window.Position.Y + window.Size.Y + g_Style.WindowTitlebarHeight - g_Style.WindowResizeThickness);
			Vec2f resizeSize = Vec2f(g_Style.WindowResizeThickness, g_Style.WindowResizeThickness);
			DrawQuad(resizePosition, resizeSize, g_Style.WindowResizeColor);

			// Draw window borders
			const Vec2f leftBorderPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y);
			const Vec2f leftBorderSize = Vec2f(g_Style.WindowBorderThickness, window.Size.Y);

			const Vec2f bottomBorderPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y + window.Size.Y - g_Style.WindowBorderThickness);
			const Vec2f bottomBorderSize = Vec2f(window.Size.X, g_Style.WindowBorderThickness);

			const Vec2f rightBorderPosition = Vec2f(leftBorderPosition.X + window.Size.X - g_Style.WindowBorderThickness, leftBorderPosition.Y);
			const Vec2f rightBorderSize = Vec2f(g_Style.WindowBorderThickness, window.Size.Y);

			DrawQuad(leftBorderPosition, leftBorderSize, g_Style.WindowBorderColor);
			DrawQuad(bottomBorderPosition, bottomBorderSize, g_Style.WindowBorderColor);
			DrawQuad(rightBorderPosition, rightBorderSize, g_Style.WindowBorderColor);
		}

		glViewport(0, 0, g_Context.DisplayWidth, g_Context.DisplayHeight);

		glBindVertexArray(g_Context.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, g_Context.VBO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glUseProgram(g_Context.ShaderID);
		int projectionMatrixLocation = glGetUniformLocation(g_Context.ShaderID, "u_ProjectionMatrix");
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &g_Context.OrthoMatrix[0]);

		glBufferSubData(GL_ARRAY_BUFFER, 0, g_Context.DrawBuffer.size() * sizeof(Vertex), g_Context.DrawBuffer.data());

		glDrawArrays(GL_TRIANGLES, 0, g_Context.DrawBuffer.size());

		g_Context.DrawBuffer.clear();
	}
}

