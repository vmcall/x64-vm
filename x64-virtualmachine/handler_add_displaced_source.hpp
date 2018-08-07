#pragma once

// X86
#include "instruction.hpp"

// VM
#include "virtual_machine.hpp"

namespace vm::handler::add
{
	namespace impl
	{
		void displaced_source_zero(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition);

		void displaced_source_one(
			virtual_machine* vm, 
			x86::instruction::modifier_data_t& modifier, 
			x86::instruction& instr, 
			uint64_t& addition);

		void displaced_source_two(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition);

		void displaced_source_three(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition);
	}
	void displaced_source(virtual_machine* vm, x86::instruction& instr);
}