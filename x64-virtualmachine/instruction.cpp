// X86
#include "instruction.hpp"
#include "instruction_rex.hpp"
#include "instruction_prefix.hpp"
#include "instruction_operand.hpp"
#include "instruction_modrm.hpp"

bool& x86::instruction::opcode_initialised()
{
	return this->m_opcode_init;
}

bool& x86::instruction::operand_initialised()
{
	return this->m_operand_init;
}

bool& x86::instruction::prefix_initialised()
{
	return this->m_prefix_init;
}

bool& x86::instruction::rex_initialised()
{
	return this->m_rex_init;
}

std::uint8_t& x86::instruction::size()
{
	return this->m_size;
}

std::string x86::instruction::name()
{
	return x86::opcode::names.at(this->opcode().as<uint32_t>());
}

x86::opcode& x86::instruction::opcode()
{
	return this->m_opcode;
}

x86::operand& x86::instruction::operand()
{
	return this->m_operand;
}

x86::prefix& x86::instruction::prefix()
{
	return this->m_prefix;
}

std::optional<x86::rex>& x86::instruction::rex()
{
	return this->m_rex;
}

x86::instruction::modifier_data_t x86::instruction::get_modifier(size_t index)
{
	auto modrm = this->operand().get<x86::modrm>(index);

	auto mode = modrm.mode;
	auto destination_reg = modrm.reg;
	auto source_reg = modrm.rm;
	std::uint8_t wide = 0;

	// HANDLE REX PREFIX IF REX IS SET AND OPERAND OVERRIDE IS SET
	if (this->rex().has_value() && this->rex().value().w == 1)
	{
		// EXTENSION OF MODR/M
		auto extended_modrm = x86::modrm_e(modrm, this->rex().value());

		// SAVE EXTENDED VALUES
		destination_reg = extended_modrm.reg;
		source_reg = extended_modrm.rm;
		wide = this->rex().value().w;
	}

	return x86::instruction::modifier_data_t{ mode, wide, source_reg, destination_reg };
}
