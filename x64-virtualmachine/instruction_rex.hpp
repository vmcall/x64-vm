#pragma once

// STD
#include <cstdint>
#include <string>

namespace x86
{
	// REX PREFIX STRUCTURE
	struct rex
	{
		std::uint8_t b : 1;		// EXTENSION OF THE ModR/M 'r/m' FIELD, SIB 'base' FIELD, OR OPCODE 'reg' FIELD
		std::uint8_t x : 1;		// EXTENSION OF SIB 'index' FIELD
		std::uint8_t r : 1;		// EXTENSION OF ModR/M 'reg' FIELD
		std::uint8_t w	: 1;	// OPERAND SIZE OVERRIDE ( 1 = 64-bit OPERAND, 0 = CS.D OPERAND )
		std::uint8_t id : 4;	// 0100 IDENTIFIER

		std::string to_string()
		{
			std::string str{};
			str.reserve(40);
			sprintf_s(str.data(), 40, "[B->%X, X->%X, R->%X, W->%X]", this->b, this->x, this->r, this->w);

			return str;
		}
	};
};