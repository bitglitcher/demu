#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <thread>
#include "CORE/core.h"
#include "terminal.h"
#include <queue>
#include <bitset>
#include <fstream>

//Weird ass looking string declaration
std::string help_message = "D16i Arch Emulator\n--help\t\tLitteraly help duh\n--version\tShow Current Version\n--g\t\tCreate Window For Graphics\n--width\t\tWindow width\n--height\tWindow height\n--rom\t\tSpecify Rom Input File\n#debug\n--md\t\tCreate RAM Dump file before exit\n--rd\t\tCreate Registers file dump before exit\n";
void argument_parser(std::string arg, int argn)
{
/* 
#Types of arguments 
--help
--version
#Open Graphical Window
--g
--widtt
--height
#ROM.bin Files
--rom
#debug
--md //Create RAM Dump file before exit
--rd //Create Registers file dump before exit
*/
	for(size_t i = 0;i <= argn;i++)
	{
		//Parse argument
		if(arg == "--help" || arg == "-help")
		{

		}
		if(arg == "--version" || arg == "-version")
		{
			
		}
		if(arg == "--g" || arg == "-g")
		{
			
		}
		if(arg == "--width" || arg == "-height")
		{
			//Increment the argn becuase a secand argument was used
		}
		if(arg == "--rom" || arg == "-rom")
		{
			//Increment the argn becuase a secand argument was used
		}
		if(arg == "--md" || arg == "-md")
		{
			//Check whether there is a path argument

			//If not use default name and . path

			//Else create file with path and name
		}
		if(arg == "--rd" || arg == "-rd")
		{
			//Check whether there is a path argument

			//If not use default name and . path

			//Else create file with path and name
		}
	}
}

using namespace std;

int main(int argc, char** argv)
{
	argc -= 1;
	std::cout << "Have " << argc << " arguments:" << std::endl;
	if(argc == 0)
	{
		std::cout << "Error: No binary specified!" << std::endl;
		exit(0);
	}
    for(int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
	std::string path = argv [1];
	std::cout << path << std::endl;

	streampos size;
	char * memblock;
	ifstream file (path, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		//std::cout << "the entire file content is in memory" << std::endl;
	}
	else 
	{
		cout << "Unable to open file" << std::endl;
		exit(0);
	}
	//Print loaded ROM memory contents
	//std::cout << memblock << std::endl;

	//Load loaded file into RAM
	/*
	std::cout << "Loading Data into mapped Memory!" << std::endl;
	for(int i = 0;i <= size;i++)
	{
		std::cout << std::bitset <8> (*(memblock + i)) << std::endl;
	}
	*/

	//Memory Init
	struct MEMORY_ARRAY MEMORY_SPACE;
	
	MEMORY_SPACE.ROM_mem.resize(ROM_SIZE, 0x00);
	MEMORY_SPACE.RAM_mem.resize(RAM_SIZE, 0x00);

	std::cout << "Memory Init!\n";
	for(size_t i = 0;i <= size;i++)
	{
		if((i >> 0x8) == 0x00) //ROM mapped address
		{
			//MEMORY_SPACE.ROM_mem [i] = (memblock [i] | (memblock [i + 1] << 8));//((*(memblock + (i + 1)) << 0xf) & *(memblock + i)));
			MEMORY_SPACE.ROM_mem [i] = memblock [i];
		}
		if((i >> 0x8) == 0xfe) //RAM mapped address
		{
			//MEMORY_SPACE.RAM_mem [i] = (memblock [i] | (memblock [i + 1] << 8));
			MEMORY_SPACE.RAM_mem [i] = memblock [i];
		}
	}

	//MEMORY_SPACE.ROM_mem.push_back(0b');

	//MEMORY_SPACE.ROM_mem [0x0] = (LI_ADDR_MODE(iLI,0x0,0x0,0x0,0xff));
	//std::cout << std::bitset <16> (MEMORY_SPACE.ROM_mem [0x0]);
	
	//MEMORY_SPACE.ROM_mem [0x0] = (0b0000000100000001);
	//MEMORY_SPACE.ROM_mem [0x1] = (0b0000000100100001);
	//MEMORY_SPACE.ROM_mem [0x2] = (0b0000000000000000);
	//MEMORY_SPACE.ROM_mem [0x3] = (0b0010000100000011);
	//MEMORY_SPACE.ROM_mem [0x4] = (0b0100000000100110);
	//MEMORY_SPACE.ROM_mem [0x5] = (0b0000000000010111);
	
	//MEMORY_SPACE.ROM_mem.push_back(0x0);
	/*
	std::cout << "Data Loaded into ROM: " << std::endl;
	for(int i = 0;i <= size;i++)
	{	std::cout << std::hex << i << " :";
		for(int x = 0;x <= 5;x++)
		{
		unsigned char temp = MEMORY_SPACE.ROM_mem [x + i * 6];//.front();
		std::cout << " " << std::bitset <8> (temp); 
		}
		std::cout << std::endl;
		//std::cout << (char)(temp & 0xff);
		//std::cout << (char)((temp >> 0x8) & 0xff) << std::endl;
		//ROM::ROM_array.pop_front();
	}
	*/
    //Create CPU Interfaces



	bool HALT = false;
	std::atomic<bool> kill = {false};
	/*Implementation Of the terminal interface is a little loose, so by now it might be skipped*/
	std::vector <device_port*> ports;
	ports.resize(1);
	device_port terminal_port;
	ports [0] = &terminal_port;
	//std::thread terminal_read(&terminal::read, terminal_ptr);
	//std::thread terminal_write(&terminal::write, terminal_ptr);
	std::mutex shared_mutex;
    //Terminal declaration
	unsigned int terminal_update_int = 50;
    terminal *terminal_ptr = new terminal(&terminal_port, HALT, shared_mutex, terminal_update_int);
	//Create One CORE
	CORE *CORE_0 = new CORE(&ports, shared_mutex);
	CORE_0->reset_vectors();
	//std::cout << "The fucking address at 0x00, is equal to: " << bitset <8> (MEMORY_SPACE.ROM_mem [0x00]) << std::endl;
	std::vector <std::thread> threads;
	threads.push_back(std::thread(&CORE::run, CORE_0, &MEMORY_SPACE, std::ref(HALT)));
	threads.push_back(std::thread(&terminal::read, terminal_ptr));
	threads.push_back(std::thread(&terminal::write, terminal_ptr));

	//std::thread CORE_THREAD(&CORE::run, CORE_0, &MEMORY_SPACE, std::ref(HALT));
	//CORE_THREAD.join();
	for(auto& thread : threads){
        thread.join();
    }
	//std::async(std::launch::async, &terminal::read, terminal_ptr);
	//std::async(std::launch::async, &terminal::write, terminal_ptr);
	//std::async(std::launch::async, CORE_THREAD.join());
	delete terminal_ptr;
	delete[] memblock;
	//std::cout << help_message << std::endl;
	return 0;
}