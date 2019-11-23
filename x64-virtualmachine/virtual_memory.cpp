#include "virtual_memory.hpp"

bool vm::virtual_memory::allocate(std::uint8_t* address, size_t size)
{
	// ROUND PAGE DOWN TO NEAREST MULTIPLE
	auto page_start = vm::virtual_memory::round_to_page(address);

	// SEE IF ALREADY MAPPED
	const auto search = this->address_space().find(page_start);
	if (search != this->address_space().end())
	{
		return false;
	}

	// CREATE REFERENCE-COUNTED SECTION, WHEN ALL REFERENCES ARE REMOVED FROM ADDRESS SPACE
	// THE RESPECTIVE PAGES WILL BE FREED AS WELL
	auto section = std::make_shared<vm::virtual_section>(page_start);

	// DOES ALLOCATION EXCEED CURRENT PAGE?
	const auto next_page = page_start + vm::virtual_memory::page_size;
	const auto exceeds_page = (address + size) >= next_page;

	// ALLOCATE SINGLE PAGE
	if (!exceeds_page)
	{
		// ADD SECTION TO ADDRESS SPACE
		section.get()->add<vm::virtual_memory::page_size>();
		this->address_space().emplace(page_start, section);
	}
	else
	{
		auto bytes_left = size;
		auto page_index = 0;
		auto page_address = address;
		do
		{
			// GET PAGE OFFSET, ONLY FOR FIRST ITERATION
			const auto page_offset = vm::virtual_memory::get_page_offset(page_address);

			// CALCULATE ACTUAL BYTES ALLOCATED FROM THE RESPECTIVE SIZE SPECIFIED
			const auto count_to_end = vm::virtual_memory::page_size - page_offset;
			auto bytes_allocated =  std::min(page_offset == 0 ? vm::virtual_memory::page_size : count_to_end, bytes_left);

			// UPDATE ADDRESS TO REFLECT NEW PAGE START
			page_address = page_start + vm::virtual_memory::page_size * page_index;

			// ALWAYS ALLOCATE A FULL PAGE
			section.get()->add<vm::virtual_memory::page_size>();
			this->address_space().emplace(page_address, section);

			// INCREMENT PAGE INDEX
			++page_index;

			// NEGATE ALLOCATED BYTES FROM SPECIFIED SIZE
			bytes_left -= bytes_allocated;

		} while (bytes_left > 0);
	}

	return true;
}

bool vm::virtual_memory::free(std::uint8_t* address)
{
	// ROUND PAGE DOWN TO NEAREST MULTIPLE
	auto page_start = vm::virtual_memory::round_to_page(address);

	// SEE IF MAPPED
	const auto search = this->address_space().find(page_start);
	if (search == this->address_space().end())
	{
		// FREE ENTIRE SECTION
		auto section = search->second;
		auto page_count = section.get()->count();
		
		// FREE THE SECTION'S PAGES
		for (size_t page_index = 0; page_index < page_count; page_index++)
		{
			this->address_space().erase(page_start + vm::virtual_memory::page_size * page_index);
		}

		// SUCCESS
		return true;
	}

	// NOT MAPPED, FAIL TO RELEASE
	return false;
}

vm::virtual_memory::memory_map_t& vm::virtual_memory::address_space()
{
	return this->m_address_space;
}
