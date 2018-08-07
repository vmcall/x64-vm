#pragma once

// STD
#include <map>
#include <vector>
#include <tuple>

// x86
#include "instruction_opcode.hpp"

namespace x86
{
	class operand
	{
public:
		// FROM VECTOR
		operand(std::vector<std::uint8_t> operand) : m_operand(operand) {}

		// FROM COMPILER CONSTANT ARRAY
		template <class T, size_t N>
		operand(T(&operand)[N]) : m_operand(operand) {}

		// DEFAULT CTOR
		operand() : m_operand() {}

		// OPERAND BUFFER
		std::vector<std::uint8_t>& buffer();

		// READ AS 'T' AT INDEX 'index', SANITY CHECKED
		template <typename T>
		T get(size_t index)
		{
			if (index + sizeof(T) > this->buffer().size())
			{
				throw std::exception("Operand size mismatch");
			}

			return *reinterpret_cast<T*>(this->buffer().data() + index);
		}

	private:
		// READ AS 'T' AT INDEX 'index', SANITY CHECKED AND INDEX GET'S UPDATED DYNAMICALLY
		template <typename T>
		T get_dynamic(size_t& index)
		{
			const T value = get<T>(index);
			index += sizeof(T);
			return value;
		}
	public:
		// READ MULTIPLE OPERANDS IN MEMORY ORDER, UNPACK IN REVERSE
		template <typename ... T>
		std::tuple<T...> get_multiple(size_t index)
		{
			return std::make_tuple(get_dynamic<T>(index)...);
		}

		// OPERAND INFORMATION
		// CONTAINS SIZE AND IF MODRM EXISTS
		struct operand_info
		{
			//operand_info(bool new_modrm, std::uint8_t new_size) : modrm(new_modrm), size(new_size) {}
			bool modrm;
			std::uint8_t size;
		};

		// STATIC OPERAND INFO TABLE
		using operand_map_t = std::map<std::uint32_t, x86::operand::operand_info>;
		const static operand_map_t info;

	private:
		std::vector<std::uint8_t> m_operand;
	};
}