#pragma once

namespace numerical_helper
{
	template <class T>
	__forceinline constexpr T round_to_multiple(T number, T alignment)
	{
		const auto remainder = number % alignment;

		if (alignment == 0x00 || remainder == 0x00)
			return number;

		return number + alignment - remainder;
	}

	template <class T>
	__forceinline constexpr std::uint8_t least_significant_bits(T a)
	{
		auto byte = static_cast<std::uint8_t>(a);
		
		std::uint8_t count = 0;
		for (size_t i = 0; i < 8; i++)
		{
			const auto bit = (byte >> i) & 0x01;
			if (bit == 0x01)
				++count;
		}

		return count;
	}

	template <class T>
	__forceinline constexpr bool addition_overflows(T a, T b)
	{
		return (b != 0 && (a > std::numeric_limits<T>::max() - b));
	}

	template <class T>
	__forceinline constexpr T sign_bit_mask()
	{
		// GET MOST SIGNIFICANT BIT
		const T bit_count = sizeof(T) * T{8};
		return T{1} << (bit_count - T{1});
	}

	template <class T>
	__forceinline constexpr bool is_signed(T value)
	{
		return (value & sign_bit_mask<T>()) == sign_bit_mask<T>();
	}
}