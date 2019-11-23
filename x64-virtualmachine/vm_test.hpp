#pragma once
#include "virtual_machine.hpp"
#include "virtual_memory.hpp"
#include "global.hpp"
#include "loggr.hpp"

template <class T, size_t N>
class vm_tester
{
public:

	// CONSTRUCTORS

	explicit vm_tester(T(&buffer)[N]) : m_vm(buffer) {}

	~vm_tester() = default;

	// TEST METHODS

	inline void prepare_test()
	{
		// SETUP REGISTERS FOR EMULATOR TEST
		global::console.log("Preparing virtual machine");

		// RBX
		this->m_vm.context().get(x86::registr::id::rbx).qword = 0x1337;

		global::console.log_indented<1>("Prepared RBX");


		// RDI
		const auto rdi_address = reinterpret_cast<std::uint8_t*>(0xFFFFF);
		auto rdi_value = 0xABCDEF;
		this->m_vm.memory().allocate(rdi_address, sizeof(rdi_value));
		this->m_vm.memory().write(rdi_value, rdi_address + 0x0A);
		this->m_vm.context().get(x86::registr::id::rdi).ptr = rdi_address;
		global::console.log_indented<1>("Prepared RDI");

		// R9
		const auto r9_address = reinterpret_cast<std::uint8_t*>(0xFEEEEFEEEE);
		auto r9_value = 0xEEEE;
		this->m_vm.memory().allocate(r9_address + 0x420, sizeof(r9_value));
		this->m_vm.memory().write(r9_value, r9_address + 0x420);
		this->m_vm.context().get(x86::registr::id::r9).ptr = r9_address;
		global::console.log_indented<1>("Prepared R9");
	}

	inline void run()
	{
		this->print_registers();
		this->print_memory_test();

		// RUN VM WITH SPECIFIED RELATIVE INSTRUCTION POINTER (BEGINNING)
		global::console.log("Running virtual machine");
		this->m_vm.run(0x00);
		global::console.log_indented<1>("Finished");

		this->print_registers();
		this->print_memory_test();
	}

private:

	// INTERNAL HELPERS
	inline void print_registers()
	{
		global::console.log("Printing registers");
		for (size_t i = 0; i < x86::registr::id::END_OF_REGISTER; i++)
		{
			const auto id = static_cast<x86::registr::id>(i);
			const auto register_name_container = x86::registr::names[id];
			const auto register_name = register_name_container[x86::registr::size::qword];
			const auto register_value = this->m_vm.context().get(id).qword;

			global::console.log_indented<1, true>(register_name, register_value);
		}
	}

	inline void print_memory_test()
	{
		global::console.log("Printing memory buffers");

		const auto r9_address = reinterpret_cast<std::uint8_t*>(0xFEEEEFEEEE);
		const auto memory_value_before = this->m_vm.memory().read<std::uint64_t>(r9_address + 0x420);

		global::console.log_indented<1, true>("R9+0x420", memory_value_before);
	}

	virtual_machine m_vm;
};