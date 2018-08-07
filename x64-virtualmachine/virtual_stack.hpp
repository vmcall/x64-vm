#pragma once

// STD
#include <vector>

namespace vm
{
	class virtual_stack
	{
	public:
		// CREATE CLASS INSTANCE WITH SPECIFIED ALIGNMENT AND SIZE
		virtual_stack(std::uint8_t alignment, std::uint32_t size) : m_alignment(alignment), m_size(size), m_buffer(m_size), m_offset(static_cast<std::int32_t>(m_size) - 1) { }

		// GET STACK POINTER, CALCULATED FROM STACK END PLUS STACK OFFSET
		std::uint8_t* stack_pointer();

		// GET STACK OFFSET, MODIFIED BY ALLOCATING/RELEASING MEMORY, DEFAULT IS SIZE MINUS ONE
		std::int32_t& stack_offset();

		// ALIGNMENT OF STACK, DEFAULT 0x08 ON MODERN MACHINES
		std::uint8_t get_alignment();

		// SIZE OF STACK, DEFAULT PAGE_SIZE(0x1000) ON MODERN MACHINES
		std::uint32_t get_size();

		// ALLOCATE n='size' BYTES ON STACK, ALIGNED AS SPECIFIED
		void allocate(std::uint32_t size);

		// RELEASE n='size' BYTES ON STACK, ALIGNED AS SPECIFIED
		void release(std::uint32_t size);

		// PUSH 'value' OF TYPE 'T' ON TOP OF STACK
		template <class T>
		void push(T value)
		{
			// ALLOCATE STACK CHUNK, ALIGNED AS SPECIFIED
			this->allocate(sizeof(T));

			// WRITE VALUE
			*reinterpret_cast<T*>(this->stack_pointer()) = value;
		}

		// POP TOP OF STACK OF TYPE 'T', RETURN SAID DATA AS TYPE 'T'
		template <class T>
		T pop()
		{
			// READ VALUE
			const T value = *reinterpret_cast<T*>(this->stack_pointer());

			// RELEASE STACK CHUNK, ALIGNED AS SPECIFIED
			this->release(sizeof(T));

			// RETURN VALUE
			return value;
		}

	private:

		// PRIVATE MEMBER FIELDS, FOR DOCUMENTATION SEE PUBLIC ACCESSORS.
		const std::uint8_t			m_alignment;
		const std::uint32_t			m_size;
		std::vector<std::uint8_t>	m_buffer;
		std::int32_t				m_offset;
	};
}
