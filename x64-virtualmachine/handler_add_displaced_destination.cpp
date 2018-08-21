// VM
#include "handler_add_displaced_destination.hpp"

// HELPER
#include "compiler_helper.hpp"

void vm::handler::add::displaced_destination(virtual_machine* vm, x86::instruction& instr)
{
	std::uint64_t addition = 0;
	std::uint64_t offset = 0;
	bool deref = true;

	// PARSE INSTRUCTION MODIFIER
	auto modifier = instr.get_modifier(0);
	switch (modifier.mode)
	{
	case 0:
		// ADD DEST, [SRC]
		vm::handler::add::impl::displaced_destination_zero(vm, modifier, instr, addition, deref);
		break;

	case 1:
		// ADD DEST, [SRC+8-bit]
		vm::handler::add::impl::displaced_destination_one(vm, modifier, instr, addition, offset, deref);
		break;

	case 2:
		// ADD DEST, [SRC+32-bit]
		vm::handler::add::impl::displaced_destination_two(vm, modifier, instr, addition, offset, deref);
		break;

	case 3:
		// ADD DEST, SRC
		vm::handler::add::impl::displaced_destination_three(vm, modifier, instr, addition, deref);
		break;
	}

	/*
		PREFIX PSEUDO:

		IF REX.W
			IGNORE PREFIX-66

			DESTINATION_REG = 64-bit

			IF PREFIX67
				SOURCE_REG = 32-bit
			ELSE 
				SOURCE-REG = 64-bit
			

		ELSE	
			IGNORE PREFIX-67

			IF PREFIX-66
				DESTINATION_REG = 16-bit
				SOURCE_REG = 16-bit
			ELSE
				DESTINATION_REG = 32-bit
				SOURCE_REG = 32-bit
	*/

	if (modifier.wide)
	{
		
	}
	else
	{
		// PREFIX 66 OVERRIDES PREFIX 67
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			// IF 66 IS SET, ADDITION WILL BE READ (BY THE HANDLERS) AS 16-bit

		}
		else // NO PREFIX, WRITE DWORD
		{

		}
	}

	// DEREF?
	if (deref)
	{
		// UPDATE DEREF VALUE
		auto address = vm->context().get(modifier.source_register).ptr + offset;
		auto value = vm->memory().read<std::uint64_t>(address) + addition;
		vm->memory().write(value, address);
	}
	else
	{
		// UPDATE REGISTER VALUE
		vm->context().get(modifier.source_register).qword += addition;
	}
}

void vm::handler::add::impl::displaced_destination_zero(
	virtual_machine* vm, 
	x86::instruction::modifier_data_t& modifier, 
	x86::instruction& instr, 
	uint64_t& addition, bool& derefence)
{
	const auto operand = instr.operand().get<std::uint32_t>(1);

	//printf("[Operation] ADD [%s], %lx\n", 
	//	x86::registr::names[modifier.source_register][source_size].c_str(), 
	//	operand);

	compiler::unreferenced_variable(vm);
	compiler::unreferenced_variable(modifier);
	compiler::unreferenced_variable(derefence);


	addition = operand;
}

void vm::handler::add::impl::displaced_destination_one(
	virtual_machine* vm, 
	x86::instruction::modifier_data_t& modifier,
	x86::instruction& instr, 
	uint64_t& addition,
	uint64_t& offset,
	bool& derefence)
{
	// REVERSE ORDER UNPACK FOR SOME REASON
	const auto[operand, displacement] = instr.operand().get_multiple<std::uint8_t, std::uint32_t>(1);

	//printf("[Operation] ADD [%s+%x], %lx\n", 
	//	x86::registr::names[modifier.source_register][source_size].c_str(), 
	//	displacement, 
	//	operand);

	compiler::unreferenced_variable(vm);
	compiler::unreferenced_variable(modifier);
	compiler::unreferenced_variable(derefence);

	offset = displacement;
	addition = operand;
}

void vm::handler::add::impl::displaced_destination_two(
	virtual_machine* vm, 
	x86::instruction::modifier_data_t& modifier, 
	x86::instruction& instr, 
	uint64_t& addition,
	uint64_t& offset,
	bool& derefence)
{
	// REVERSE ORDER UNPACK FOR SOME REASON
	const auto[operand, displacement] = instr.operand().get_multiple<std::uint32_t, std::uint32_t>(1);

	//printf("[Operation] ADD [%s+%lx], %lx\n", 
	//	x86::registr::names[modifier.source_register][source_size].c_str(), 
	//	displacement, 
	//	operand);

	compiler::unreferenced_variable(vm);
	compiler::unreferenced_variable(modifier);
	compiler::unreferenced_variable(derefence);

	offset = displacement;
	addition = operand;
}

void vm::handler::add::impl::displaced_destination_three(
	virtual_machine* 
	vm, x86::instruction::modifier_data_t& modifier, 
	x86::instruction& instr, 
	uint64_t& addition, 
	bool& derefence)
{
	const auto operand = instr.operand().get<std::uint32_t>(1);

	//printf("[Operation] ADD %s, %lx\n", 
	//	x86::registr::names[modifier.source_register][source_size].c_str(), 
	//	operand);

	compiler::unreferenced_variable(vm);
	compiler::unreferenced_variable(modifier);
	compiler::unreferenced_variable(derefence);

	derefence = false;
	addition = operand;
}
