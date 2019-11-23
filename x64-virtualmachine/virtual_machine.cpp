// VM
#include "virtual_machine.hpp"
#include "handlers.hpp"

// x86
#include "instruction_modrm.hpp"
#include "register.hpp"

std::vector<std::uint8_t>& virtual_machine::buffer()
{
	return this->m_buffer;
}
x86::registr& virtual_machine::context()
{
	return this->m_context;
}

vm::virtual_memory& virtual_machine::memory()
{
	return this->m_memory;
}

void virtual_machine::handle_instruction(x86::instruction& instr)
{
	auto vm_instance = const_cast<virtual_machine*>(this);

	// ADD OPCODE CATEGORY?
	switch (instr.opcode().as<uint32_t>())
	{
	
	// ADD
	case x86::opcode::add_byte:			
	case x86::opcode::add_displaced_byte:
	case x86::opcode::add_displaced_source:
	case x86::opcode::add_al:
	case x86::opcode::add_rax:
	case x86::opcode::add_wide:
	case x86::opcode::add_displaced_destination:
	case x86::opcode::add_byte_extended:
		vm::handler::add::handle(vm_instance, instr);
		return;
	
	default:
		vm::handler::unknown(instr);
	}
}