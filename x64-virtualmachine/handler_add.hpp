#pragma once

// X86
#include "instruction.hpp"

// VM
#include "virtual_machine.hpp"

namespace vm::handler::add
{
	namespace impl
	{
		template <class T>
		void handle_flags(virtual_machine* vm, T value, T previous_value, T addition)
		{
			// ZERO FLAG
			vm->context().flags().zero = value == 0x000;

			// OVERFLOW
			vm->context().flags().overflow =
				numerical_helper::addition_overflows(value, addition);

			// CARRY
			vm->context().flags().carry =
				numerical_helper::is_signed(value) != numerical_helper::is_signed(previous_value);

			// SIGN
			vm->context().flags().sign = numerical_helper::is_signed(value) ? 1 : 0;

			// PARITY
			const auto set_bit_count = numerical_helper::least_significant_bits(value);
			vm->context().flags().parity = set_bit_count % 2 == 0; // EVEN NUMBER OF SET BITS
		}
	}

	void handle(virtual_machine* vm, x86::instruction& instr);
}