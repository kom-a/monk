#include "Window.h"

#include <fstream>
#include <codecvt>
#include <sstream>

namespace mwl
{
	static std::wstring ReadFile(const std::filesystem::path& filepath)
	{
		std::wifstream wif(filepath.c_str());
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
		wss << wif.rdbuf();
		return wss.str();
	}

	static std::vector<std::wstring> SplitString(std::wstring str, std::wstring_view delimiter)
	{
		std::vector<std::wstring> split;

		size_t pos = 0;
		std::wstring token;
		while ((pos = str.find(delimiter)) != std::wstring::npos) {
			token = str.substr(0, pos);
			split.push_back(token);
			str.erase(0, pos + delimiter.length());
		}

		return split;
	}

	static std::wstring ParseCursorFilename(const std::wstring& line)
	{
		size_t left_quote = line.find(L'\"');
		size_t right_quote = line.find(L'\"', left_quote + 1);

		if (left_quote == std::wstring::npos || right_quote == std::wstring::npos)
			return L"";

		return line.substr(left_quote + 1, right_quote - left_quote - 1);
	}

	CursorData::CursorData(const std::filesystem::path& filepath)
	{
		std::wstring inf_data = ReadFile(filepath);
		if (inf_data.empty())
			return;

		size_t i = inf_data.find(L"[Strings]");

		if (i == std::wstring::npos)
			return;

		std::vector<std::wstring> strings = SplitString(inf_data.substr(i), L"\n");

		for (const auto& str : strings)
		{
			size_t equals_index = str.find(L'=');
			if (equals_index == std::wstring::npos)
				continue;

			const std::wstring name = str.substr(0, equals_index);
			const std::wstring cursor_path = filepath.parent_path() / ParseCursorFilename(str);

			if (name.find(L"pointer") != std::wstring::npos)
				Pointer = cursor_path;
			else if (name.find(L"help") != std::wstring::npos)
				Help = cursor_path;
			else if (name.find(L"work") != std::wstring::npos)
				Work = cursor_path;
			else if (name.find(L"busy") != std::wstring::npos)
				Busy = cursor_path;
			else if (name.find(L"cross") != std::wstring::npos)
				Cross = cursor_path;
			else if (name.find(L"text") != std::wstring::npos)
				Text = cursor_path;
			else if (name.find(L"hand") != std::wstring::npos)
				Hand = cursor_path;
			else if (name.find(L"unavailiable") != std::wstring::npos)
				Unavailiable = cursor_path;
			else if (name.find(L"vert") != std::wstring::npos)
				Vert = cursor_path;
			else if (name.find(L"horz") != std::wstring::npos)
				Horz = cursor_path;
			else if (name.find(L"dgn1") != std::wstring::npos)
				Dgn1 = cursor_path;
			else if (name.find(L"dgn2") != std::wstring::npos)
				Dgn2 = cursor_path;
			else if (name.find(L"move") != std::wstring::npos)
				Move = cursor_path;
			else if (name.find(L"alternate") != std::wstring::npos)
				Alternate = cursor_path;
			else if (name.find(L"link") != std::wstring::npos)
				Link = cursor_path;
		}
	}

	void Window::LoadCursorData(const CursorData& cursor)
	{
		m_Cursor = cursor;
	}

	void Window::SetWindowResizeCallback(const WindowResizeCallbackFn& callback)
	{
		Callbacks.WindowResizeCallback = callback;
	}

	void Window::SetMouseMovedCallback(const MouseMovedCallbackFn& callback)
	{
		Callbacks.MouseMovedCallback = callback;
	}

	void Window::SetMouseButtonDownCallback(const MouseButtonDownCallbackFn& callback)
	{
		Callbacks.MouseButtonDownCallback = callback;
	}

	void Window::SetMouseButtonUpCallback(const MouseButtonUpCallbackFn& callback)
	{
		Callbacks.MouseButtonUpCallback = callback;
	}

	void Window::SetMouseClickedCallback(const MouseClickedCallbackFn& callback)
	{
		Callbacks.MouseClickedCallback = callback;
	}

	void Window::SetMouseScrollCalback(const MouseScrollCallbackFn& callback)
	{
		Callbacks.MouseScrollCallback = callback;
	}

	void Window::SetKeyDownCallback(const KeyDownCallbackFn& callback)
	{
		Callbacks.KeyDownCallback = callback;
	}

	void Window::SetKeyUpCallback(const KeyUpCallbackFn& callback)
	{
		Callbacks.KeyUpCallback = callback;
	}
}