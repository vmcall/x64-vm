#pragma once

// STD
#include <cstdint>
#include <map>
#include <vector>


namespace x86
{
	class opcode
	{
	public:
		// FROM VECTOR
		opcode(std::vector<std::uint8_t> opcode) : m_opcode(opcode) {}

		// FROM COMPILER CONSTANT ARRAY
		template <class T, size_t N>
		opcode(T(&opcode)[N]) : m_opcode(opcode) {}

		// DEFAULT CTOR
		opcode() : m_opcode() {}

		// BUFFER
		std::vector<std::uint8_t>& buffer();

		// READ AS 'T', SANITY CHECKED
		template <class T>
		T as()
		{
			const auto size = this->buffer().size();
		
			// HANDLE WHEN CALLING WITH BIGGER INTEGER SIZE THAN BUFFER CAPACITY
			if (sizeof(T) > size)
			{
				// ASSUME INTEGER, BAD PRACTICE
				// NEED TO USE STD::ENABLE_IF FOR THIS
				T value{};
		
				for (size_t i = 0; i < size; i++)
					value += this->buffer().at(i) << (8 * i);
		
				return value;
			}
		
			return *reinterpret_cast<T*>(this->buffer().data());
		}

#pragma region Opcodes
#pragma region ADD
		/*
		NAME: ADD
		DESCRIPTION: {
		The ADD instruction performs integer addition.
		It evaluates the result for both signed and unsigned integer operands
		and sets the CF and OF flags to indicate a carry (overflow)
		in the signed or unsigned result, respectively. The SF flag
		indicates the sign of the signed result.
		}

		*/
		constexpr static std::uint32_t add_byte = 0x00;
		constexpr static std::uint32_t add_reg = 0x01;
		constexpr static std::uint32_t add_displaced_byte = 0x02;
		constexpr static std::uint32_t add_displaced_source = 0x03;			// DONE? MISSING PREFIX SUPPORT (66 AND LOCK) AND FLAGS
		constexpr static std::uint32_t add_al = 0x04;
		constexpr static std::uint32_t add_rax = 0x05;
		constexpr static std::uint32_t add_wide = 0x80;
		constexpr static std::uint32_t add_displaced_destination = 0x81;	// DONE? MISSING PREFIX SUPPORT (66 NEEDS TO BE TESTED) AND FLAGS
		constexpr static std::uint32_t add_byte_extended = 0x83;
#pragma endregion
		// INC
		// DEC
		// SUB
		// DIV
		// MUL
		// POP
		// PUSH
		// JMP
		// CALL
		// RET
		// MOV
#pragma endregion

		// STATIC OPCODE NAME LIST
		using name_map_t = std::map<std::uint32_t, std::string>;
		const static name_map_t names;

	private:
		// RAW OPCODE DATA
		std::vector<std::uint8_t> m_opcode;

	};
}