#pragma once

#include <string>
#include <functional>
#include <inttypes.h>
#include <filesystem>
#include <optional>

#include "Events.h"

namespace mwl
{
	struct WindowProps
	{
		std::wstring Title	= L"MWL Window";
		uint32_t Width		= 1600;
		uint32_t Height		= 900;
		bool VSync			= true;
		bool Closed			= false;
	};

	enum class OpenGLVersion
	{
		OPENGL_1_0,
		OPENGL_1_1,
		OPENGL_1_2,
		OPENGL_1_3,
		OPENGL_1_4,
		OPENGL_1_5,
		OPENGL_2_0,
		OPENGL_2_1,
		OPENGL_3_0,
		OPENGL_3_1,
		OPENGL_3_2,
		OPENGL_3_3,
		OPENGL_4_0,
		OPENGL_4_1,
		OPENGL_4_2,
		OPENGL_4_3,
		OPENGL_4_4,
		OPENGL_4_5,
		OPENGL_4_6
	};

	void SetOpenGLVersion(OpenGLVersion openglVersion);

	struct Cursor
	{
		explicit Cursor(const std::filesystem::path& filename);

		std::optional<std::wstring> Pointer;
		std::optional<std::wstring> Help;
		std::optional<std::wstring> Work;
		std::optional<std::wstring> Busy;
		std::optional<std::wstring> Cross;
		std::optional<std::wstring> Text;
		std::optional<std::wstring> Hand;
		std::optional<std::wstring> Unavailiable;
		std::optional<std::wstring> Vert;
		std::optional<std::wstring> Horz;
		std::optional<std::wstring> Dgn1;
		std::optional<std::wstring> Dgn2;
		std::optional<std::wstring> Move;
		std::optional<std::wstring> Alternate;
		std::optional<std::wstring> Link;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

	public:
		virtual void Update()					= 0;
		virtual void SwapBuffers()				= 0;
		virtual void Close()					= 0;
		virtual void EnableVSync(bool enable)	= 0;
		virtual void MakeContextCurrent()		= 0;

		virtual bool Closed()			const = 0;
		virtual bool IsFullscreen()		const = 0;
		virtual uint32_t GetWidth()		const = 0;
		virtual uint32_t GetHeight()	const = 0;

		virtual void* GetNative() = 0;
		
		virtual void SetCursor			(const Cursor& cursor);
		virtual void SetFullscreen		(bool fullscreen) = 0;

		void SetWindowResizeCallback	(const WindowResizeCallbackFn& callback);
		void SetMouseMovedCallback		(const MouseMovedCallbackFn& callback);
		void SetMouseButtonDownCallback	(const MouseButtonDownCallbackFn& callback);
		void SetMouseButtonUpCallback	(const MouseButtonUpCallbackFn& callback);
		void SetMouseClickedCallback	(const MouseClickedCallbackFn& callback);
		void SetMouseScrollCalback		(const MouseScrollCallbackFn& callback);
		void SetKeyDownCallback			(const KeyDownCallbackFn& callback);
		void SetKeyUpCallback			(const KeyUpCallbackFn& callback);

	protected:
		struct
		{
			WindowResizeCallbackFn WindowResizeCallback			= nullptr;

			MouseMovedCallbackFn MouseMovedCallback				= nullptr;
			MouseButtonDownCallbackFn MouseButtonDownCallback	= nullptr;
			MouseButtonUpCallbackFn MouseButtonUpCallback		= nullptr;
			MouseClickedCallbackFn MouseClickedCallback			= nullptr;
			MouseScrollCallbackFn MouseScrollCallback			= nullptr;

			KeyDownCallbackFn KeyDownCallback					= nullptr;
			KeyUpCallbackFn KeyUpCallback						= nullptr;
		} Callbacks;

		Cursor m_Cursor = Cursor(L"");
	};

	Window* Create(const WindowProps& windowProps = WindowProps());
}