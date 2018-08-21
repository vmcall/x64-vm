#pragma once

// X86
#include "instruction.hpp"

// VM
#include "virtual_machine.hpp"

namespace vm::handler::add
{
	namespace impl
	{
		void displaced_destination_zero(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition,
			bool& derefence);

		void displaced_destination_one(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition,
			uint64_t& offset,
			bool& derefence);

		void displaced_destination_two(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition,
			uint64_t& offset,
			bool& derefence);

		void displaced_destination_three(
			virtual_machine* vm,
			x86::instruction::modifier_data_t& modifier,
			x86::instruction& instr,
			uint64_t& addition,
			bool& derefence);
	}

	void displaced_destination(virtual_machine* vm, x86::instruction& instr);
}