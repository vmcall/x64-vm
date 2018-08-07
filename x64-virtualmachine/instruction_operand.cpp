#include "instruction_operand.hpp"

const x86::operand::operand_map_t x86::operand::info =
{
	// ADD
	{ x86::opcode::add_displaced_source,		{ true, 1/*ModRM*/ } },
	{ x86::opcode::add_displaced_destination,	{ true, 1/*ModRM*/ + 4/*DATA*/ } }

};

std::vector<std::uint8_t>& x86::operand::buffer()
{
	return this->m_operand;
}
