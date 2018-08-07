#pragma once

namespace compiler
{
	namespace detail
	{
		template <typename T>
		void unreferenced_variable(T val)
		{
			static_cast<void>(val);
		}

		template <typename T, typename ...Rest>
		void unreferenced_variable(T val, Rest... rest)
		{
			compiler::detail::unreferenced_variable(val);
			compiler::detail::unreferenced_variable(rest...);
		}
	}

	template <typename ...Rest>
	void unreferenced_variable(Rest... rest)
	{
		compiler::detail::unreferenced_variable(rest...);
	}
}