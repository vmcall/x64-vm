#pragma once

// STD
#include <vector>
#include <memory>

namespace vm
{
	class virtual_section
	{
	public:
		virtual_section(std::uint8_t* base) : m_base(base) {}

		using page_list_t = std::vector<std::unique_ptr<std::uint8_t[]>>;

		// ALLOCATE PAGE AND ADD TO SECTION
		template <size_t N>
		void add()
		{
			this->pages().emplace_back(std::make_unique<std::uint8_t[]>(N));
		}

		// GET BASE OF nth PAGE
		std::uint8_t* get(size_t index);
		
		// GET PAGE COUNT
		size_t count();

		// GET SECTION BASE
		std::uint8_t*& base();

	private:
		// INTERNAL PAGE LIST
		page_list_t& pages();
		page_list_t m_pages;

		std::uint8_t* m_base;
	};
}