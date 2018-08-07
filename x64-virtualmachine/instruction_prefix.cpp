// X86
#include "instruction_prefix.hpp"

const x86::prefix::prefix_list_t x86::prefix::list = {
	x86::prefix::id::LOCK,
	x86::prefix::id::REPNE,
	x86::prefix::id::REP,	
	x86::prefix::id::BOUND,
	x86::prefix::id::CS_OVERRIDE,
	x86::prefix::id::SS_OVERRIDE,
	x86::prefix::id::DS_OVERRIDE,
	x86::prefix::id::ES_OVERRIDE,
	x86::prefix::id::FS_OVERRIDE,
	x86::prefix::id::GS_OVERRIDE,
	x86::prefix::id::BR_NOT_TAKEN,
	x86::prefix::id::BR_TAKEN,
	x86::prefix::id::OPERAND_SIZE_OVERRIDE, 
	x86::prefix::id::ADDRESS_SIZE_OVERRIDE,
};

bool x86::prefix::has(std::uint8_t prefix)
{
	return std::any_of(
		this->buffer().begin(), this->buffer().end(),
		[prefix](std::uint8_t iter_prefix) { 
			return iter_prefix == prefix; 
		});
}

bool x86::prefix::is(std::uint8_t prefix)
{
	return std::any_of(
		x86::prefix::list.begin(), x86::prefix::list.end(),
		[prefix](std::uint8_t iter_prefix) {
		return iter_prefix == prefix;
	});
}

void x86::prefix::append(std::uint8_t prefix)
{
	this->buffer().emplace_back(prefix);
}
;
x86::prefix::vector_iter_t x86::prefix::begin()
{
	return this->buffer().begin();
}

x86::prefix::vector_iter_t x86::prefix::end()
{
	return this->buffer().end();
}

std::vector<std::uint8_t>& x86::prefix::buffer()
{
	return this->m_buffer;
}
