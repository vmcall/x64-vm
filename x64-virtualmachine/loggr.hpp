#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include "fmt/core.h"

class loggr
{
public:
	loggr()
	{
		// SETUP CONSOLE IF NOT PRESENT
		auto console_window = GetConsoleWindow();
		if (console_window == 0x00)
		{
			if (!AllocConsole())
			{
				// ???? SHOULD NEVER HAPPEN
				return;
			}

			std::freopen("CONOUT$", "w", stdout);

			m_did_allocate_console = true;
		}
	}
	~loggr()
	{
		// FREE CONSOLE IF DYNAMICALLY ALLOCATED
		if (m_did_allocate_console)
		{
			const auto freed = FreeConsole();

			if (!freed)
			{
				// ??
			}
		}
	}

	// CONSOLE LOGGING FUNCTIONS
	template <class...T>
	inline void log_raw(T... arguments) const
	{
		fmt::print(arguments...);
	}

	inline void log(std::string_view message) const
	{
		fmt::print("[+] {}\n", message);
	}
	inline void log_error(std::string_view message) const
	{
		fmt::print("[!] {}\n", message);
	}

	template <bool hex = false, class T>
	inline void log(std::string_view variable_name, const T& value) const
	{
		constexpr auto format_string = hex ? 
			"[=] {:<15} {:X}\n" :
			"[=] {:<15} {}\n";

		fmt::print(format_string, variable_name, value);
	}

	template <std::size_t indentation>
	inline void log_error_indented(std::string_view message) const
	{
		fmt::print("[!] {:<{}} {}\n", ' ', indentation, message);
	}

	template <std::size_t indentation>
	inline void log_indented(std::string_view message) const
	{
		fmt::print("[+] {:<{}} {}\n", ' ', indentation, message);
	}

	template <std::size_t indentation, bool hex = false, class T>
	inline void log_indented(std::string_view variable_name, const T& value) const
	{
		constexpr auto format_string = hex ?
			"[=] {:<{}} {:.<15} {:X}\n" :
			"[=] {:<{}} {:.<15} {}\n";

		fmt::print(format_string, ' ', indentation, variable_name, value);
	}


	// CONSOLE MODIFICATION FUNCTIONS
	inline COORD get_position() const
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		{
			this->log_error("Failed to get cursor position");
			return { 0, 0 };
		}

		return info.dwCursorPosition;
	}
	inline void set_position(const COORD cursor) const
	{
		if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor))
		{
			this->log_error("Failed to set cursor position");

		}
	}
	inline void clear_line() const
	{
		// GET CURSOR POSITION
		auto position = this->get_position();


		position.X = 0;

		// CLEAR LINE
		DWORD count = 0;
		const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
		FillConsoleOutputCharacter(handle, ' ', 150, position, &count);

		// RESET POSITION
		set_position(position);
	}

private:
	bool m_did_allocate_console = false;

};