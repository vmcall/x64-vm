#pragma once
#include "loggr.hpp"
#include <chrono>

class spinner
{
public:
	spinner(loggr* new_logger) : m_logger(new_logger) { }

	static constexpr std::uint8_t CHARACTER_SEQUENCE[] =
	{
		'|', '/', '-', '\\'
	};

	inline void start()
	{
		if (this->m_active)
		{
			this->m_logger->log_error("Cannot initialise spinner object more than once!");
			return;
		}

		this->m_active = true;
	}

	inline void update()
	{
		if (!this->m_active)
		{
			this->m_logger->log_error("Cannot initialise spinner object without initialization!");
			return;
		}


		// GET TIME SINCE LAST UPDATE
		using clock_t = std::chrono::high_resolution_clock;
		const auto time = clock_t::now();
		const auto time_delta = time - m_last_update_time;
		const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_delta);


		// ONLY UPDATE SPINNER EVERY x MS
		constexpr auto update_time = 150;
		if (duration.count() <= update_time)
		{
			return;
		}

		// UPDATE THE SPINNER CHARACTER
		this->m_logger->clear_line();
		this->m_logger->log_raw("[{}]", static_cast<char>(spinner::CHARACTER_SEQUENCE[this->m_seq_index]));

		// INCREMENT AND WRAP
		this->m_seq_index++;
		this->m_seq_index %= sizeof(spinner::CHARACTER_SEQUENCE);

		this->m_last_update_time = time;
	
	}
	inline void stop()
	{
		if (!this->m_active)
		{
			this->m_logger->log_error("Spinner is not active!");
			return;
		}

		this->m_logger->clear_line();
		this->m_active = false;
	}

private:
	const loggr* m_logger;
	bool m_active = false;
	std::size_t m_seq_index = 0;
	std::chrono::steady_clock::time_point m_last_update_time;
};