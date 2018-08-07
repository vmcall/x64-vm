#pragma once

// STD
#include <map>
#include <memory>
#include <algorithm>

// VM
#include "virtual_section.hpp"

namespace vm
{
	class virtual_memory
	{
	public:
		static constexpr const size_t page_size = 0x1000;

		bool allocate(std::uint8_t* address, size_t size);
		bool free(std::uint8_t* address);

	// PRIVATE TEMPLATE HELPERS
	private:
		using section_pointer_t = std::_Ptr_base<vm::virtual_section>::element_type*;

		template<typename T>
		T handle_iterative_read(uint8_t* address, section_pointer_t section, const size_t& page_index)
		{
			T result{};

			auto page_address = address;
			auto current_page_index = page_index;

			size_t buffer_offset = 0;
			size_t bytes_left = sizeof(T);

			// ITERATIVELY READ ACROSS PAGES
			do
			{
				// GET PAGE OFFSET, ONLY FOR FIRST ITERATION
				const auto page_offset = vm::virtual_memory::get_page_offset(page_address);
			
				// CALCULATE ACTUAL BYTES ALLOCATED FROM THE RESPECTIVE SIZE SPECIFIED
				const auto count_to_end = vm::virtual_memory::page_size - page_offset;
				const auto bytes_read = std::min(page_offset == 0 ? vm::virtual_memory::page_size : count_to_end, bytes_left);

				// READ BUFFER
				auto page_buffer = reinterpret_cast<std::uint8_t*>(section->get(current_page_index));
				for (auto i = 0; i < bytes_read; i++)
				{
					auto buffer = reinterpret_cast<std::uint8_t*>(&result);
					buffer[buffer_offset + i] = page_buffer[page_offset + i];
				}

				// RESET PAGE ADDRESS
				page_address = nullptr;

				// INCREMENT BUFFER OFFSET
				buffer_offset += bytes_read;

				// NEGATE READ BYTES FROM COUNTER
				bytes_left -= bytes_read;

				// INCREMENT PAGE INDEX
				++current_page_index;

			} while (bytes_left > 0);

			return result;
		}
	
		template<typename T>
		void handle_iterative_write(T& value, uint8_t* address, section_pointer_t section)
		{
			const auto buffer = reinterpret_cast<std::uint8_t*>(&value);

			// CALCULATE SIZE FOR FIRST PAGE AND THEN REST OF BUFFER
			size_t buffer_offset = 0;
			size_t bytes_left = sizeof(T);
			auto current_page_index = vm::virtual_memory::get_page_index(address, section->base());

			// ITERATIVELY WRITE
			auto page_address = address;
			do
			{
				// GET PAGE OFFSET, ONLY FOR FIRST ITERATION
				const auto page_offset = vm::virtual_memory::get_page_offset(page_address);

				// CALCULATE ACTUAL BYTES ALLOCATED FROM THE RESPECTIVE SIZE SPECIFIED
				const auto count_to_end = vm::virtual_memory::page_size - page_offset;
				const auto bytes_written = std::min(page_offset == 0 ? vm::virtual_memory::page_size : count_to_end, bytes_left);

				auto page_buffer = reinterpret_cast<std::uint8_t*>(section->get(current_page_index));

				for (auto i = 0; i < bytes_written; i++)
				{
					page_buffer[page_offset + i] = buffer[buffer_offset + i];
				}

				// RESET PAGE ADDRESS
				page_address = nullptr;

				// INCREMENT BUFFER OFFSET
				buffer_offset += bytes_written;

				// NEGATE WRITTEN BYTES FROM COUNTER
				bytes_left -= bytes_written;

				// INCREMENT PAGE INDEX
				++current_page_index;
			} while (bytes_left > 0);
		}

	public:
		template <typename T>
		T read(std::uint8_t* address)
		{
			// ROUND PAGE DOWN TO NEAREST MULTIPLE
			auto page_start = vm::virtual_memory::round_to_page(address);

			// SEE IF IN MEMORY PAGE
			const auto search = this->address_space().find(page_start);
			if (search != this->address_space().end())
			{
				auto section = search->second.get();

				// GET PAGE INDEX
				const auto page_index = vm::virtual_memory::get_page_index(page_start, section->base());

				const size_t buffer_size = sizeof(T);

				// SEE IF DATA IS ON TWO DIFFERENT PAGES
				const auto next_page_start = page_start + vm::virtual_memory::page_size;
				const bool exceeds_page = (address + sizeof(T) ) >= next_page_start;

				if (!exceeds_page)
				{
					const auto page_offset = vm::virtual_memory::get_page_offset(address);
					return *reinterpret_cast<T*>(section->get(page_index) + page_offset);
				}
				else
				{
					return handle_iterative_read<T>(address, section, page_index);
				}
			}

			throw std::exception("Pagefault");
		}

		template <typename T>
		void write(T& value, std::uint8_t* address)
		{
			// ROUND PAGE DOWN TO NEAREST MULTIPLE
			const auto page_start = vm::virtual_memory::round_to_page(address);

			// SEE IF IN MEMORY PAGE
			const auto search = this->address_space().find(page_start);
			if (search != this->address_space().end())
			{
				auto section = search->second.get();

				// IF IT DOESN'T EXCEED A PAGE, JUST WRITE NORMALLY
				const auto next_page_start = page_start + vm::virtual_memory::page_size;
				const bool exceeds_page = (address + sizeof(T)) >= next_page_start;

				if (!exceeds_page)
				{
					const auto page_index = vm::virtual_memory::get_page_index(address, section->base());
					const auto page_offset = vm::virtual_memory::get_page_offset(address);
					*reinterpret_cast<T*>(section->get(page_index) + page_offset) = value;
				}
				else
				{
					handle_iterative_write(value, address, section);
				}

				return;
			}

			throw std::exception("Pagefault");
			return;
		}

		using memory_map_t = std::map<std::uint8_t*, std::shared_ptr<vm::virtual_section>>;
		memory_map_t& address_space();

	private:
		template <typename T>
		static constexpr T round_to_page(T address)
		{
			// CLEAR LOWER 12 BITS
			return reinterpret_cast<T>(reinterpret_cast<std::uint64_t>(address) & ~(0xFFF));
		}

		template <typename T>
		static constexpr std::uintptr_t get_page_offset(T address)
		{
			return reinterpret_cast<std::uintptr_t>(address) & (0xFFF);
		}
		template <typename T>
		static constexpr size_t get_page_index(T address, T base_address)
		{
			const auto page_delta = vm::virtual_memory::round_to_page(address) - vm::virtual_memory::round_to_page(base_address);
			return page_delta / vm::virtual_memory::page_size;
		}

		memory_map_t m_address_space;
	};
}