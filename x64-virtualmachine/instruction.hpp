#pragma once

// STD
#include <cstdint>
#include <optional>
#include <vector>

// x86
#include "instruction_opcode.hpp"
#include "instruction_rex.hpp"
#include "instruction_operand.hpp"
#include "instruction_prefix.hpp"

namespace x86
{
	// BASE INSTRUCTION CLASS
	class instruction
	{
	public:
		// INIT CHECKS
		bool& opcode_initialised();
		bool& operand_initialised();
		bool& prefix_initialised();
		bool& rex_initialised();

		// INFORMATION
		std::uint8_t& size();				// INSTRUCTION SIZE
		std::string name();					// OPCODE NAME
		x86::opcode& opcode();				// OPCODE VALUE
		x86::operand& operand();			// OPERAND VALUE
		x86::prefix& prefix();				// PREFIX VALUE
		std::optional<x86::rex>& rex();		// REX VALUE

		// RETURNS MODRM DATA EXTENDED WITH POSSIBLE REX PREFIX, AND REX DATA
		struct modifier_data_t
		{
			std::uint8_t mode;
			std::uint8_t wide;
			std::uint8_t source_register;
			std::uint8_t destination_register;
		};
		modifier_data_t get_modifier(size_t index);

	private:
		bool m_opcode_init;
		bool m_operand_init;
		bool m_prefix_init;
		bool m_rex_init;

		std::uint8_t m_size;

		x86::opcode m_opcode;					// 1-3 BYTES
		x86::operand m_operand;					// 0-8 BYTES
		x86::prefix m_prefix;					// 0-4 BYTES
		std::optional<x86::rex> m_rex;			// 0-1 BYTES
	};
}