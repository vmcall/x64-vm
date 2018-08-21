
// VM
#include "handler_add.hpp"
#include "handler_add_displaced_source.hpp"
#include "handler_add_displaced_destination.hpp"
#include "handler_add_byte.hpp"

// HELPER
#include "compiler_helper.hpp"

void vm::handler::add::handle(virtual_machine* vm, x86::instruction& instr)
{
	// HANDLE ADD VARIANTS
	switch (instr.opcode().as<uint32_t>())
	{

	// DISPLACED SOURCE
	case x86::opcode::add_displaced_source:
		vm::handler::add::displaced_source(vm, instr);
		return;

	// NORMAL
	case x86::opcode::add_displaced_destination:
		vm::handler::add::displaced_destination(vm, instr);
		return;


	// BYTE
	case x86::opcode::add_byte:
		vm::handler::add::byte(vm, instr);
		return;

	default:
		throw std::exception("Unsupported ADD");
	}
}
