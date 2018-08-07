#pragma once

// STD
#include <cstdint>

namespace x86
{
	// REGISTER OPERAND
	struct modrm
	{
		std::uint8_t rm : 3;	// SOURCE REGISTER ID
		std::uint8_t reg : 3;	// DESTINATION REGISTER ID
		std::uint8_t mode : 2;	// ADDRESSING MODE

		// FOUR DIFFERENT MODES EXIST (WITH EXCEPTIONS):
		// MODE 0: [REG]
		// MODE 1: [REG]+disp8
		// MODE 2: [REG]+disp32
		// MODE 3: REG

		// CALCULATE OPERAND SIZE FROM DATA MODE
		std::uint8_t data_size()
		{
			switch (this->mode)
			{
			// 8-bit DISPLACEMENT
			case 1:
			{
				return 1;
			}
			// 32-bit DISPLACEMENT
			case 2:
			{
				return 4;
			}
			// NO DISPLACEMENT
			default:
				return 0;
			}
		}

		// GET DATA MODE AS STRING
		std::string mode_to_string()
		{
			const char* modes[] = {
				"Dereference",
				"8-bit displacement",
				"32-bit displacement",
				"No displacement"
			};

			if (this->mode > 3)
				throw std::exception("Invalid modrm mode");

			return std::string( modes[this->mode] );
		}
	};

	// EXTENDED REGISTER OPERAND
	struct modrm_e
	{
		modrm_e() {}

		modrm_e(x86::modrm operand, x86::rex rex)
		{
			this->rm = operand.rm | (rex.b << 3);
			this->reg = operand.reg | (rex.r << 3);
			this->mode = operand.mode;
		}

		std::uint8_t rm : 4;
		std::uint8_t reg : 4;
		std::uint8_t mode : 2;
	};
}