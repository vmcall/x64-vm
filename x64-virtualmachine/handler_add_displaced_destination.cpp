// VM
#include "handler_add_displaced_destination.hpp"
#include "handler_add.hpp"

// HELPER
#include "compiler_helper.hpp"
#include "numerical_helper.hpp"

#include "global.hpp"

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
		// ADD [DEST], SRC
		vm::handler::add::impl::displaced_destination_zero(vm, modifier, instr, addition, deref);
		break;

	case 1:
		// ADD [DEST+8-bit], SRC
		vm::handler::add::impl::displaced_destination_one(vm, modifier, instr, addition, offset, deref);
		break;

	case 2:
		// ADD [DEST+32-bit], SRC
		vm::handler::add::impl::displaced_destination_two(vm, modifier, instr, addition, offset, deref);
		break;

	case 3:
		// ADD DEST, SRC
		vm::handler::add::impl::displaced_destination_three(vm, modifier, instr, addition, deref);
		break;
	}

	// GET DESTINATION SIZE
	auto size = x86::registr::size::qword;

	// 32-bit?
	if (!instr.get_modifier(0).wide)
	{
		// 16-bit OVERRIDE?
		size = instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE) ?
			x86::registr::size::word :
			x86::registr::size::dword;
	}

	// DEREF?
	auto& source_reg = vm->context().get(modifier.source_register);
	if (deref) // DEREFERENCE RESULT?
	{
		// UPDATE DEREF'D VALUE
		auto address = source_reg.ptr + offset;

		switch (size)
		{
		case x86::registr::size::word:
		{
			const auto previous_value = vm->memory().read<std::uint16_t>(address);
			const auto casted_addition = static_cast<std::uint16_t>(addition);
			std::uint16_t value = previous_value + casted_addition;
			vm->memory().write(value, address);


			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm, value, previous_value, casted_addition);

			break;
		}
			

		case x86::registr::size::dword:
		{
			// SAVE OLD VALUE AND CALCULATE NEW
			const auto previous_value = vm->memory().read<std::uint32_t>(address);
			const auto casted_addition = static_cast<std::uint32_t>(addition);
			std::uint32_t value = previous_value + casted_addition;

			// WRITE
			vm->memory().write(value, address);

			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm, value, previous_value, casted_addition);

			break;
		}


		case x86::registr::size::qword:
		{
			// SAVE OLD VALUE AND CALCULATE NEW
			const auto previous_value = vm->memory().read<std::uint64_t>(address);
			auto value = previous_value + addition;

			// WRITE
			vm->memory().write(value, address);

			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm, value, previous_value, addition);
			break;
		}
		}

	}
	else
	{
		// UPDATE REGISTER VALUE
		switch (size)
		{
		case x86::registr::size::word:
		{
			const auto casted_addition = static_cast<std::uint16_t>(addition);
			const auto previous_value = source_reg.word;
			source_reg.word += casted_addition;

			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm,
				source_reg.word,
				previous_value,
				casted_addition);

			break;
		}

		case x86::registr::size::dword:
		{
			const auto casted_addition = static_cast<std::uint32_t>(addition);
			const auto previous_value = source_reg.dword;
			source_reg.dword += static_cast<std::uint32_t>(addition);

			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm,
				source_reg.dword,
				previous_value,
				casted_addition);
			break;
		}

		case x86::registr::size::qword:
		{
			const auto previous_value = source_reg.qword;
			source_reg.qword += addition;

			// SET FLAGS
			vm::handler::add::impl::handle_flags(vm,
				source_reg.qword,
				previous_value,
				addition);
			break;
		}
		}
	}
}

void vm::handler::add::impl::displaced_destination_zero(
	virtual_machine* vm [[maybe_unused]], 
	x86::instruction::modifier_data_t& modifier [[maybe_unused]],
	x86::instruction& instr, 
	uint64_t& addition, 
	bool& derefence [[maybe_unused]])
{
	const auto operand = instr.operand().get<std::uint32_t>(1);

	// GET DESTINATION SIZE
	auto size = x86::registr::size::qword;

	// 32-bit?
	if (!instr.get_modifier(0).wide)
	{
		// 16-bit OVERRIDE?
		size = instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE) ?
			x86::registr::size::word :
			x86::registr::size::dword;
	}

	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[size].c_str();

	global::console.log_raw("[O]   ADD [{}], {}\n", dest_reg_name, operand);

	addition = operand;
}

void vm::handler::add::impl::displaced_destination_one(
	virtual_machine* vm [[maybe_unused]],
	x86::instruction::modifier_data_t& modifier [[maybe_unused]],
	x86::instruction& instr, 
	uint64_t& addition,
	uint64_t& offset,
	bool& derefence [[maybe_unused]])
{
	// REVERSE ORDER UNPACK FOR SOME REASON
	const auto[operand, displacement] = instr.operand().get_multiple<std::uint8_t, std::uint32_t>(1);


	offset = displacement;
	addition = operand;
}

void vm::handler::add::impl::displaced_destination_two(
	virtual_machine* vm[[maybe_unused]],
	x86::instruction::modifier_data_t& modifier, 
	x86::instruction& instr, 
	uint64_t& addition,
	uint64_t& offset,
	bool& derefence[[maybe_unused]])
{
	// REVERSE ORDER UNPACK FOR SOME REASON
	const auto[operand, displacement] = instr.operand().get_multiple<std::uint32_t, std::uint32_t>(1);

	// GET DESTINATION SIZE
	auto size = x86::registr::size::qword;

	// 32-bit?
	if (!instr.get_modifier(0).wide)
	{
		// 16-bit OVERRIDE?
		size = instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE) ?
			x86::registr::size::word :
			x86::registr::size::dword;
	}


	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[size].c_str();

	global::console.log_raw("[O]   ADD [{}+{:X}], {:X}\n", dest_reg_name, displacement, operand);

	offset = displacement;
	addition = operand;
}

void vm::handler::add::impl::displaced_destination_three(
	virtual_machine* vm[[maybe_unused]],
	x86::instruction::modifier_data_t& modifier, 
	x86::instruction& instr, 
	uint64_t& addition, 
	bool& derefence)
{
	const auto operand = instr.operand().get<std::uint32_t>(1);

	// GET DESTINATION SIZE
	auto size = x86::registr::size::qword;

	// 32-bit?
	if (!instr.get_modifier(0).wide)
	{
		// 16-bit OVERRIDE?
		size = instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE) ?
			x86::registr::size::word :
			x86::registr::size::dword;
	}

	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[size].c_str();

	global::console.log_raw("[O]   ADD {}, {:X}\n", dest_reg_name, operand);

	derefence = false;
	addition = operand;
}
