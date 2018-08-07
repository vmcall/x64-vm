#pragma once

// STD
#include <array>
#include <vector>

namespace x86
{
	class prefix
	{
	public:
		prefix() : m_buffer() {}

		// INFORMATION
		bool has(std::uint8_t prefix);

		// SETTERS
		void append(std::uint8_t prefix);

		// ITERATOR
		using vector_iter_t = std::vector<std::uint8_t>::const_iterator;
		vector_iter_t begin();
		vector_iter_t end();

		// STATICS

		// IS BYTE A LEGACY PREFIX?
		static bool is(std::uint8_t prefix);

		// LIST OF PREFIXES
		using prefix_list_t = std::array<std::uint8_t, 14>;
		static const prefix_list_t list;

		// ID LIST OF LEGACY PREFIXES
		enum id
		{
			/* GROUP 1 */
			LOCK = 0xF0,	// FORCES PROCESSOR TO LOCK MEMORY 
			REPNE = 0xF2,	// CAUSES INSTRUCTION TO BE REPEATED TO EACH ELEMENT UNTIL NOT ZERO
			REP = 0xF3,		// CAUSES INSTRUCTION TO BE REPEATED FOR EACH ELEMENT IN STRING/ARRAY
			BOUND = 0xF2,	// ONLY WHEN IT'S ENABLED AND PRECEDES NEAR CALL/RET/JCC

			/* GROUP 2 */

			// Use with branch instruction is reserved
			CS_OVERRIDE = 0x2E,
			SS_OVERRIDE = 0x36,
			DS_OVERRIDE = 0x3E,
			ES_OVERRIDE = 0x26,
			FS_OVERRIDE = 0x64,
			GS_OVERRIDE = 0x65,

			// ONLY WITH JCC 
			BR_NOT_TAKEN = 0x2E,
			BR_TAKEN = 0x3E,

			/* GROUP 3 */
			OPERAND_SIZE_OVERRIDE = 0x66, // SWITCH BETWEEN OPERAND SIZES (PREFIX FORCES NON-DEFAULT)

			/* GROUP 4 */
			ADDRESS_SIZE_OVERRIDE = 0x67 // SWITCH BETWEEN ADDRESS SIZE (PREFIX FORCES NON-DEFAULT)
		};

	private:
		// ACCESSORS
		std::vector<std::uint8_t>& buffer();

		// PRIVATE MEMBERS

		// PREFIX BUFFER
		// MAYBE USE BITMAP?
		std::vector<std::uint8_t> m_buffer;
	};

	
};