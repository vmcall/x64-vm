#pragma once

// STD
#include <vector>
#include <initializer_list>

// x86
#include "instruction.hpp"


// BASE DISASSEMBLY CLASS
namespace x86
{
	class disassembler
	{
	public:
		template <class T, size_t N>
		disassembler(T(&buffer)[N]) : m_buffer(buffer, buffer + N) {}

		disassembler(std::vector<std::uint8_t> buffer) : m_buffer(buffer) {}

		// ACCESSORS
		std::uint8_t* buffer();
		size_t buffer_size();

		// INSTRUCTION ITERATOR
		template <class T, class Fn>
		void iterate(T offset, Fn callback)
		{
			for (auto instruction_pointer = this->buffer() + offset; 
				instruction_pointer < this->buffer() + this->buffer_size();)
			{
				x86::instruction instr{};

				// SAVE START TO CALCULATE SIZE
				auto start = instruction_pointer;

				// CHECK FOR PREFIX PRESENCE
				if (!instr.prefix_initialised())
				{
					// HANDLE PREFIX
					x86::disassembler::handle_prefix(instr, instruction_pointer);
				}

				// CHECK FOR REX PREFIX
				if (!instr.rex_initialised())
				{
					// HANDLE REX PREFIX
					x86::disassembler::handle_rex(instr, instruction_pointer);
				}

				// READ OPCODE
				if (!instr.opcode_initialised())
				{
					x86::disassembler::handle_opcode(instr, instruction_pointer);
				}

				// HANDLE OPERAND FROM OPCODE
				if (!instr.operand_initialised())
				{
					x86::disassembler::handle_operand(instr, instruction_pointer);
				}

				// CALCULATE SIZE
				instr.size() = static_cast<std::uint8_t>(instruction_pointer - start);

				// RUN CALLBACK WITH DISASSEMBLED INSTRUCTION
				callback(instr, instruction_pointer);
			}
		}

	private:

		// DISASSEMBLY HANDLERS
		static void handle_opcode(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_operand(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_prefix(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_rex(x86::instruction& instr, std::uint8_t*& buffer);

		std::vector<std::uint8_t> m_buffer;
	};
}