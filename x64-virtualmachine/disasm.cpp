// STD
#include <algorithm>

// x86
#include "disasm.hpp"
#include "instruction_prefix.hpp"
#include "instruction_rex.hpp"
#include "instruction_opcode.hpp"
#include "instruction_operand.hpp"
#include "instruction_modrm.hpp"

std::uint8_t* x86::disassembler::buffer()
{
	return this->m_buffer.data();
}

size_t x86::disassembler::buffer_size()
{
	return this->m_buffer.size();
}

void x86::disassembler::handle_opcode(x86::instruction& instr, std::uint8_t*& buffer)
{
	// HANDLE OPCODE INSTRUCTION

	// SAVE FIRST BYTE, TODO: MULTIPLE BYTE OPCODES
	instr.opcode().buffer().emplace_back(*buffer);
	++buffer;

	// SET OPCODE INIT
	instr.opcode_initialised() = true;
}

void x86::disassembler::handle_operand(x86::instruction& instr, std::uint8_t*& buffer)
{
	// HANDLE OPERAND DATA
	const auto search = x86::operand::info.find(instr.opcode().as<std::uint32_t>());

	// FOUND OPERAND SIZE
	if (search != x86::operand::info.end())
	{
		// HARDCODED OPERAND INFORMATION

		const auto info = search->second;
		auto size = info.size;

		// IF MODRM EXISTS, PARSE MODE AND ADD TO SIZE
		if (info.modrm)
		{
			size += reinterpret_cast<x86::modrm*>(buffer)->data_size();
		}

		// SAVE ALL OPERAND BYTES
		for (size_t i = 0; i < size; i++, buffer++)
			instr.operand().buffer().emplace_back(*buffer);
	}

	// SET OPERAND INIT
	instr.operand_initialised() = true;
}

void x86::disassembler::handle_prefix(x86::instruction& instr, std::uint8_t*& buffer)
{
	// HANDLE LEGACY PREFIX
	for (size_t i = 0; i < 4; i++)
	{
		// IF LEGACY PREFIX IS FOUND, SAVE IT AND CONTINUE ITERATION
		// SKIP IF WE ALREADY HAVE SAVED FOUR DIFFERENT PREFIXES (ONE FROM EACH GROUP)
		if (x86::prefix::is(*buffer))
		{
			// SAVE LEGACY PREFIX BYTE
			instr.prefix().append(*buffer);
			++buffer;
			continue;
		}
		else
		{
			// STOP WHEN NO PREFIX WAS FOUND
			break;
		}
	}

	// SET INIT STATUS
	instr.prefix_initialised() = true;
}

void x86::disassembler::handle_rex(x86::instruction& instr, std::uint8_t*& buffer)
{
	// HANDLE REX BYTE
	auto rex = reinterpret_cast<x86::rex*>(buffer);

	// 0100, REX IDENTIFIER
	if (rex->id == 0x04)
	{
		// SAVE REX PREFIX
		instr.rex() = *rex;

		// GO TO NEXT BYTE
		++buffer;
	}

	// SET INIT STATUS
	instr.rex_initialised() = true;
}
