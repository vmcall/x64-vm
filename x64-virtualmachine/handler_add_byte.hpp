#pragma once

// X86
#include "instruction.hpp"

// VM
#include "virtual_machine.hpp"

namespace vm::handler::add
{
	void byte(virtual_machine* vm, x86::instruction& instr);
}