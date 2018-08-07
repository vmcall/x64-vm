#pragma once

// STD
#include <array>

namespace x86
{
	class registr
	{
	public:
		// REGISTER ID's
		enum id : std::uint8_t
		{
			rax,	// 0000
			rcx,	// 0001
			rdx,	// 0010
			rbx,	// 0011
			rsp,	// 0100
			rbp,	// 0101
			rsi,	// 0110
			rdi,	// 0111
			r8,		// 1000
			r9,     // 1001
			r10,	// 1010
			r11,	// 1011
			r12,	// 1100
			r13,	// 1101
			r14,	// 1110
			r15,	// 1111
			END_OF_REGISTER
		};

		enum size : std::uint8_t
		{
			qword,
			dword,
			word,
			byte,
			END_OF_SIZE
		};

		// REGISTER DATA
		union register_data
		{
			std::uint8_t*	ptr;
			uint64_t		qword;
			uint32_t		dword;
			uint16_t		word;
			uint8_t			byte;


			std::uint8_t* dword_ptr()
			{
				std::uint64_t tmp = this->dword;
				return reinterpret_cast<std::uint8_t*>(tmp);
			}
			std::uint8_t* word_ptr()
			{
				std::uint64_t tmp = this->word;
				return reinterpret_cast<std::uint8_t*>(tmp);
			}
		};

		// R/EFLAGS STRUCTURE, DOCUMENTED :)
		struct flags_t
		{
			// 0, Carry flag
			// Set if an arithmetic operation generates a carry or a borrow out of 
			// the mostsignificant bit of the result; cleared otherwise. This flag 
			// indicates an overflow condition for unsigned integer arithmetic. 
			// It is also used in multiple precision arithmetic.
			std::uint8_t carry : 1;

		private:
			// 1, Reserved
			std::uint8_t reserved_1 : 1;
		public:

			// 2, Parity flag
			// Set if the least-significant byte of the result
			// contains an even number of 1 bits.
			std::uint8_t parity : 1;

		private:
			// 3, Reserved
			std::uint8_t reserved_3 : 1; 
		public:

			// 4, Auxiliary carry flag
			// Set if an arithmetic operation generates a carry or a 
			// borrow out of bit 3 of the result.
			std::uint8_t auxiliary_carry : 1; 

		private:
			// 5, Reserved
			std::uint8_t reserved_5 : 1; 
		public:

			// 6, Zero flag
			// Set if the result is zero
			std::uint8_t zero : 1;

			// 7, Sign flag
			// Set equal to the most-significant bit of the result.
			std::uint8_t sign : 1; 

			// 8, Trap 
			// Set to enable single-step mode for debugging
			std::uint8_t trap : 1; 

			// 9, Interrupt
			// Set to respond to maskable interrupts
			std::uint8_t interrupt : 1;

			// 10, Direction flags
			// Controls string instructions. Setting the flag causes string
			// instructions to auto-decrement (to process strings from high addresses to low addresses)
			// Clearing the flag causes instructions to auto-increment
			std::uint8_t direction : 1; 

			// 11, Overflow flag
			// If integer result is too large a positive number or too small a negative number.
			std::uint8_t overflow : 1;

			// 12-13, I/O privilege level
			// Indicates current privilege level
			// The current privilege level (CPL) of the currently running program or task must be less
			// than or equal to the I / O privilege level to access the I / O address space.
			std::uint8_t privilege_level : 2;

			// 14, Nested task flag
			// Controls the chaining of interrupted and called tasks. Set when the
			// current task is linked to the previously executed task; cleared when the current task is not
			// linked to another task.
			std::uint8_t nested_task : 1;

		private:
			// 15, Reserved
			std::uint8_t reserved_15 : 1;
		public:

			// 16, Resume flag
			// Controls the processor's response to debug exceptions.
			std::uint8_t resume : 1;

			// 17, VM flag
			// Set to enable virtual-8086 mode
			std::uint8_t vm : 1; 

			// 18, Alignment check flag
			// If the AM bit is set in the CR0 register, alignment checking 
			// of user-mode data accesses is enabled.
			// If the SMAP bit is set in the CR4 register, 
			// explicit supervisor-mode data accesses to user-mode pages are allowed.
			std::uint8_t alignment_check : 1;

			// 19, Virtual interrupt flag
			// Virtual image of the IF flag. Used in conjunction with the VIP flag.
			// (To use this flag and the VIP flag the virtual mode extensions are enabled by setting the VME
			//  flag in control register CR4.)
			std::uint8_t virtual_interrupt : 1; 

			// 20, Virtual interrupt pending flag
			// Set to indicate that an interrupt is pending; clear when no
			// interrupt is pending. (Software sets and clears this flag; the processor only reads it.) Used in
			// conjunction with the VIF flag.
			std::uint8_t virtual_interrupt_pending : 1;

			// 21, Identification flag
			// Support for CPUID instruction
			std::uint8_t identification : 1; 

		private:
			std::uint8_t reserved_22 : 1; 
			std::uint8_t reserved_23 : 1; 
			std::uint8_t reserved_24 : 1; 
			std::uint8_t reserved_25 : 1; 
			std::uint8_t reserved_26 : 1; 
			std::uint8_t reserved_27 : 1; 
			std::uint8_t reserved_28 : 1; 
			std::uint8_t reserved_29 : 1; 
			std::uint8_t reserved_30 : 1; 
			std::uint8_t reserved_31 : 1; 
		};

		// GET REGISTER
		x86::registr::register_data& get(std::uint8_t id);

		// GET INSTRUCTION POINTER
		std::uint8_t*& instruction_pointer();

		// GET FLAGS
		flags_t& flags();

		// DATA BUFFER
		using data_t = std::array<x86::registr::register_data, x86::registr::id::END_OF_REGISTER>;
		data_t& general_buffer();

		// NAME ARRAY FOR DEBUGGING
		using register_names_t = std::array<std::string, x86::registr::size::END_OF_SIZE>;
		using register_name_map_t = std::array<register_names_t, x86::registr::id::END_OF_REGISTER>;
		static const register_name_map_t names;

	private:
		data_t m_general_buffer;
		std::uint8_t* m_instruction_pointer;
		flags_t m_flags;

	};
}