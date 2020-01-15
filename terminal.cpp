#include <iostream>
#include <mutex>
#include "terminal.h"

terminal::terminal(device_port *some, bool& kill, std::mutex &terminal_mutex, unsigned int &update_rate)
{
    this->some = some;
    this->kill = &kill;
	this->terminal_mutex = &terminal_mutex;
	this->update_rate = &update_rate;
}

void terminal::read()
{
	const auto wait_duration = std::chrono::milliseconds(*update_rate);
    while(true)
    {
		terminal_mutex->lock();
		if(!some->FIFO_I.empty())
		{
			//for(size_t i = 0;i <= some->FIFO_I.size();i++)
			while(!some->FIFO_I.empty())
			{
				putchar(some->FIFO_I.front());
				some->FIFO_I.pop();
				std::cout.flush();
				//std::this_thread::sleep_for(wait_duration);
			}
		}
		std::this_thread::sleep_for(wait_duration); 
		terminal_mutex->unlock();
    }
}
void terminal::write()
{
	const auto wait_duration = std::chrono::milliseconds(*update_rate);
    while(true)
    {
		if(some->FIFO_O.size() != buffer_lenght)
		{
			terminal_mutex->lock();
			some->FIFO_O.push(getchar_unlocked());
			terminal_mutex->unlock();
		}
		std::this_thread::sleep_for(wait_duration); 
    }
}
