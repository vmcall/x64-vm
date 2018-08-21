// VM
#include "handler_add_byte.hpp"

// COMPILER
#include "compiler_helper.hpp"

void vm::handler::add::byte(virtual_machine* vm, x86::instruction& instr)
{
	compiler::unreferenced_variable(vm);
	compiler::unreferenced_variable(instr);
}
