#include "virtual_section.hpp"


std::uint8_t* vm::virtual_section::get(size_t index)
{
	return this->pages().at(index).get();
}

size_t vm::virtual_section::count()
{
	return this->pages().size();
}

std::uint8_t*& vm::virtual_section::base()
{
	return this->m_base;
}

vm::virtual_section::page_list_t& vm::virtual_section::pages()
{
	return this->m_pages;
}
