#include "virtual_stack.hpp"
#include "numerical_helper.hpp"

namespace vm
{
	std::uint8_t* virtual_stack::stack_pointer()
	{
		// STACK POINTER IS END OF STACK PLUS OFFSET
		return m_buffer.data() + this->stack_offset();
	}

	std::int32_t& virtual_stack::stack_offset()
	{
		// OFFSET FROM STACK END, USED TO CHECK FOR STACK OVERFLOW
		return this->m_offset;
	}

	std::uint8_t virtual_stack::get_alignment()
	{
		return this->m_alignment;
	}

	std::uint32_t virtual_stack::get_size()
	{
		return this->m_size;
	}

	void virtual_stack::allocate(std::uint32_t size)
	{
		// CALCULATE ALIGNED SIZE
		const auto delta = numerical_helper::round_to_multiple(static_cast<std::int32_t>(size), static_cast<std::int32_t>(this->get_alignment()));

		// CHECK FOR OVERFLOW
		if (this->stack_offset() - delta < 0)
			throw std::exception("Stack overflow");

		// ALLOCATE MEMORY ON STACK BY SUBTRACTING AMOUNT OF BYTES (ALIGNED) FROM INDEX
		this->stack_offset() -= delta;
	}

	void virtual_stack::release(std::uint32_t size)
	{
		// CALCULATE ALIGNED SIZE
		const auto delta = numerical_helper::round_to_multiple(static_cast<std::int32_t>(size), static_cast<std::int32_t>(this->get_alignment()));

		// CHECK FOR UNDERFLOW
		if (this->stack_offset() + delta >= static_cast<std::int32_t>(this->get_size()))
			throw std::exception("Stack underflow");

		// RELEASE MEMORY ON STACK BY ADDING AMOUNT OF BYTES TO INDEX
		this->stack_offset() += delta;
	}
}

