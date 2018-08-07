// VM
#include "handler_add_displaced_destination.hpp"

// HELPER
#include "compiler_helper.hpp"

void vm::handler::add::displaced_destination(virtual_machine* vm, x86::instruction& instr)
{
	// PARSE INSTRUCTION MODIFIER
	const auto modifier = instr.get_modifier(0);

	const auto source_name = x86::registr::names[modifier.source_register][x86::registr::size::qword].c_str();

	bool deref = true;
	auto addition = 0ull;
	auto address_offset = 0ull;

	// TODO, HANDLE REX PREFIX AND 66/67 OVERRIDE

	switch (modifier.mode)
	{
		// ADD [REG], OPERAND
	case 0:
	{
		const auto operand = instr.operand().get<std::uint32_t>(1);

		printf("[Operation] ADD [%s], %lx\n", source_name, operand);

		addition = operand;

		break;

	}
	// ADD [REG+8-bit], OPERAND
	case 1:
	{
		// REVERSE ORDER UNPACK FOR SOME REASON
		const auto[operand, displacement] = instr.operand().get_multiple<std::uint8_t, std::uint32_t>(1);

		printf("[Operation] ADD [%s+%x], %lx\n", source_name, displacement, operand);

		address_offset = displacement;
		addition = operand;

		break;
	}
	// ADD [REG+32-bit], OPERAND
	case 2:
	{
		// REVERSE ORDER UNPACK FOR SOME REASON
		const auto[operand, displacement] = instr.operand().get_multiple<std::uint32_t, std::uint32_t>(1);
		printf("[Operation] ADD [%s+%lx], %lx\n", source_name, displacement, operand);

		address_offset = displacement;
		addition = operand;
		break;
	}
	// ADD REG, OPERAND
	case 3:
	{
		const auto operand = instr.operand().get<std::uint32_t>(1);
		printf("[Operation] ADD %s, %lx\n", source_name, operand);

		deref = false;
		addition = operand;

		break;
	}
	}

	// DEREF?
	if (deref)
	{
		// UPDATE DEREF VALUE
		auto address = vm->context().get(modifier.source_register).ptr + address_offset;
		auto value = vm->memory().read<std::uint64_t>(address) + addition;
		vm->memory().write(value, address);
	}
	else
	{
		// UPDATE REGISTER VALUE
		vm->context().get(modifier.source_register).qword += addition;
	}
}
