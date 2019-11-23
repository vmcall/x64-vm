#pragma once

// STD
#include <array>
#include <vector>

// x86
#include "disasm.hpp"
#include "register.hpp"

// VM
#include "virtual_stack.hpp"
#include "virtual_memory.hpp"

// CONSTANTS
constexpr std::uint8_t vm_alignment = 0x8;
constexpr std::uint32_t vm_stack_size = 0x1000;

// VIRTUAL MACHINE CLASS
class virtual_machine
{
public:
	template <class T, size_t N>
	virtual_machine(T(&buffer)[N]) : m_buffer(buffer, buffer + N), m_stack(vm_alignment, vm_stack_size) {}
	virtual_machine(std::vector<std::uint8_t> buffer) : m_buffer(buffer), m_stack(vm_alignment, vm_stack_size) {}

	// ACCESSORS
	std::vector<std::uint8_t>& buffer();	// INSTRUCTION BUFFER
	x86::registr& context();				// REGISTER DATA
	vm::virtual_memory& memory();			// VIRTUAL MEMORY SPACE

	// INSTRUCTION DISPATCHER
	void handle_instruction(x86::instruction& instr);

	// RUN VM ENGINE
	template <class T>
	void run(T offset)
	{
		// SET UP DISASSEMBLER
		auto disassembler = x86::disassembler(this->buffer());

		// ITERATE INSTRUCTIONS FOR VIRTUALIZATION
		disassembler.iterate(offset, [this](x86::instruction instr, std::uint8_t*& instruction_pointer)
		{
			// SAVE INSTRUCTION POINTER
			this->context().instruction_pointer() = instruction_pointer;

			try
			{
				// VIRTUALIZE INSTRUCTION
				this->handle_instruction(instr);
			}
			catch (std::exception exception)
			{
				// HANDLE EXCEPTIONS
				global::console.log_error_indented<1>(exception.what());
			}
		

			// UPDATE INSTRUCTION POINTER
			instruction_pointer = this->context().instruction_pointer();
		});
	}

private:
	// INSTRUCTION CODE BUFFER
	std::vector<std::uint8_t> m_buffer;

	// REGISTERS
	x86::registr m_context;

	// STACK
	vm::virtual_stack m_stack;

	// MEMORY
	vm::virtual_memory m_memory;
};