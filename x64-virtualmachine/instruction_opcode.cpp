#include "instruction_opcode.hpp"

// NAMES
const x86::opcode::name_map_t x86::opcode::names =
{
	// ADD
	{ x86::opcode::add_byte,					"ADD" },
	{ x86::opcode::add_reg,						"ADD" },
	{ x86::opcode::add_displaced_byte,			"ADD" },
	{ x86::opcode::add_displaced_source,		"ADD" },
	{ x86::opcode::add_al,						"ADD" },
	{ x86::opcode::add_rax,						"ADD" },
	{ x86::opcode::add_wide,					"ADD" },
	{ x86::opcode::add_displaced_destination,	"ADD" },
	{ x86::opcode::add_byte_extended,			"ADD" }

};

std::vector<std::uint8_t>& x86::opcode::buffer()
{
	return this->m_opcode;
}
