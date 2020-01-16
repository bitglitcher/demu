
#ifndef CORE_H // include guard
#define CORE_H
#include <queue>
#include <mutex>
//std::vector <unsigned short int> RAM_array;
#define debug false

#define ROM_SIZE 0xff
#define RAM_SIZE 0xfe00
#define IO_BUFFERS_SPACE_SIZE 0xff
//Memory Mapping

//#define RAM_ADDR_SPACE //every thing from 0xff
#define ROM_ADDR_SPACE 0xff

//IO Mapping
#define Console_Write_Buffer_Addr 0xfff0
#define Console_Read_Buffer_Addr 0xfff0

struct device_port
{
    std::queue <unsigned char> FIFO_I;
    std::queue <unsigned char> FIFO_O;
};

struct MEMORY_ARRAY
{
	std::vector <char> ROM_mem;	
	std::vector <char> RAM_mem;	
};

struct STMR_ORG
{
	bool AB;
	bool BA;
	bool EQ;
	bool OVR;
	bool NEG;
	bool ZERO;
	bool JMP;
};
class CORE
{
	public:
	char memory_access_read(struct MEMORY_ARRAY *, unsigned short int);
	void memory_access_write(struct MEMORY_ARRAY *, unsigned short int, char);
	void execute(struct MEMORY_ARRAY *, bool &);
	void run(struct MEMORY_ARRAY *, bool &);
	void reset_vectors();
	CORE(std::vector <device_port*>*, std::mutex&);
	void debug_registers();
	void dma_mem_write(struct MEMORY_ARRAY *);
	void dma_dev_write(struct MEMORY_ARRAY *);
	private:
	unsigned char GPR_LO [0xf];
	unsigned char GPR_HI [0xf];
	unsigned char SPR_LO [0xf];
	unsigned char SPR_HI [0xf];
	unsigned short int PC;
	unsigned short int IR;
	struct STMR_ORG STMR;
	//Source BUS for the MOV instruction
	unsigned short int source;
	unsigned short int temporal;
	//wait slot stack logic
	bool stack_wait_slot = true; //Default true to address 0xffff
	//Stack pre increment
	unsigned short int stack_pre_dec = 0xffff;
	//CORE ID
	unsigned int CORE_ID;
	//DMA stuff
	unsigned short int MMA; //Mapped Memory Address
    unsigned short int MMC; ////Mapped Memory Count
    //unsigned short int MME; ////Mapped Memory End
	unsigned short int PID; //Port ID
	std::vector <device_port*> *ports;
	//Thread stuff
	bool *stop;
	std::mutex *core_mutex;
};
#endif