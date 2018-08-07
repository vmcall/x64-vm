#include "register.hpp"

// REGISTER NAME MAP
const x86::registr::register_name_map_t x86::registr::names =
{
	x86::registr::register_names_t { "RAX", "EAX", "AH", "AL" },
	x86::registr::register_names_t { "RCX", "ECX", "CH", "CL" },
	x86::registr::register_names_t { "RDX", "EDX", "DX", "DL" },
	x86::registr::register_names_t { "RBX", "EBX", "BH", "BL" },
	x86::registr::register_names_t { "RSP", "ESP", "SP", "" },
	x86::registr::register_names_t { "RBP", "EBP", "BP", "" },
	x86::registr::register_names_t { "RSI", "ESI", "SI", "" },
	x86::registr::register_names_t { "RDI", "EDI", "DI", "" },
	x86::registr::register_names_t { "R8", "R8D", "R8W", "R8B" },
	x86::registr::register_names_t { "R9", "R9D", "R9W", "R9B" },
	x86::registr::register_names_t { "R10", "R10D", "R10W", "R10B" },
	x86::registr::register_names_t { "R11", "R11D", "R11W", "R11B" },
	x86::registr::register_names_t { "R12", "R12D", "R12W", "R12B" },
	x86::registr::register_names_t { "R13", "R13D", "R13W", "R13B" },
	x86::registr::register_names_t { "R14", "R14D", "R14W", "R14B" },
	x86::registr::register_names_t { "R15", "R15D", "R15W", "R15B" },
};

x86::registr::data_t& x86::registr::general_buffer()
{
	return this->m_general_buffer;
}

x86::registr::register_data& x86::registr::get(std::uint8_t id)
{
	return this->general_buffer().at(id);
}

std::uint8_t*& x86::registr::instruction_pointer()
{
	return this->m_instruction_pointer;
}

x86::registr::flags_t& x86::registr::flags()
{
	return this->m_flags;
}
