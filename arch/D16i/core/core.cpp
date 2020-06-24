#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <string.h>
#include "../OPCODES/inset.h"
#include "../OPCODES/alu_ops.h"
#include "../OPCODES/addressing_modes.h"
#include <bitset>
#include "core.h"
#include <bitset>
#include <mutex>

void CORE::debug_registers()
{
	std::cout << std::endl;
	for (size_t i = 0; i <= 0x7; i++)
	{
		std::cout << "address: 0x" << std::hex << i << " GPR_HI: " << std::bitset <8> (GPR_HI [i]);
		std::cout << "\taddress: 0x" << std::hex << i << " GPR_LO: " << std::bitset <8> (GPR_LO [i]) << "\n";
		std::cout.flush();
	}
	
	for (size_t i = 0; i <= 0x7; i++)
	{
		std::cout << "address: 0x" << std::hex << i << " SPR_HI: " << std::bitset <8> (SPR_HI [i]);
		std::cout << "\taddress: 0x" << std::hex << i << " SPR_LO: " << std::bitset <8> (SPR_LO [i]) << "\n";
		std::cout.flush();
	}
	std::cout << "address: 0x0" << " MMA: " << std::bitset <16> (MMA) << std::endl;
	std::cout << "address: 0x1" << " MMC: " << std::bitset <16> (MMC) << std::endl;
	std::cout << "address: 0x0" << " PC : " << std::bitset <16> (PC) << std::endl;
	std::cout.flush();
}
bool first_addr = true;
void CORE::execute(struct MEMORY_ARRAY *mem_space, bool &HALT)
{
	unsigned short int post_inc_addr;
	//std::cout << "The fucking address at 0x00, is equal to: " << std::bitset <16> ((char)mem_space->ROM_mem [0x00]) << std::endl;
	//This fucking shit isn't fucking working
	if(first_addr)
	{
		first_addr = false;
	}
	else
	{
		if(STMR.JMP == true) //Not increment, other wise it would execute the wrong data
		{
			//Because the PC is already set to the address of execution
			//Do nothing :(
			STMR.JMP = false;
		}
		else PC = post_inc_addr + 1;
	}
	post_inc_addr = PC + 1;
	IR = IR_HI_LO(mem_space->ROM_mem [PC] , mem_space->ROM_mem [post_inc_addr]);
	//IR = (unsigned short int)tmp;
	//IR = IR_HI_LO(memory_access_read(mem_space, PC++), memory_access_read(mem_space, PC));//Fetch
	//PC++;

	if(debug)
	{
		std::cout << "PC ->" << std::hex << PC << std::endl;
		std::cout << "Post PC ->" << std::hex << post_inc_addr << std::endl;
		std::cout << "IR ->" << std::bitset <16> (IR) << std::endl;
		//Decode
		std::cout << "IR.INS: " << std::bitset <3> (INS_ADDR_MODE_DEC(IR)) << std::endl;
	}
	switch (INS_ADDR_MODE_DEC(IR))
	{
	case iNOOP: if(debug) std::cout<< "Do Nothing!\n"; break;
	case iLI:
		if(debug)
		{
			std::cout << "LI Instruction: " << std::endl;
			std::cout << "\tLH: " << std::bitset <1> (LI_ADDR_MODE_LH_DEC(IR)) << std::endl;
			std::cout << "\tGS: " << std::bitset <1> (LI_ADDR_MODE_GS_DEC(IR)) << std::endl; 
			std::cout << "\tRT: " << std::bitset <3> (LI_ADDR_MODE_RT_DEC(IR)) << std::endl;
			std::cout << "\tIM: " << std::bitset <8> (LI_ADDR_MODE_IM_DEC(IR)) << std::endl;
		}
		if (LI_ADDR_MODE_LH_DEC(IR) == 0) //LOAD LOW
		{
			if(LI_ADDR_MODE_GS_DEC(IR) == 0) //LOAD GPR
			{
				switch (LI_ADDR_MODE_RT_DEC(IR))
				{
				case iAX: GPR_LO [iAX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iBX: GPR_LO [iBX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iCX: GPR_LO [iCX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iDX: GPR_LO [iDX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iEX: GPR_LO [iEX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iFX: GPR_LO [iFX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iGX: GPR_LO [iGX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iHX: GPR_LO [iHX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				default:
					break;
				}
			}
			else if(LI_ADDR_MODE_GS_DEC(IR) == 1) //LOAD SPR
			{
				switch (LI_ADDR_MODE_RT_DEC(IR))
				{
				case iSP: SPR_LO [iSP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iBP: SPR_LO [iBP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iDMAP: SPR_LO [iDMAP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iKRNLP: SPR_LO [iKRNLP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iTMRIP: SPR_LO [iTMRIP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iSYSCR: SPR_LO [iSYSC] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iLPR: SPR_LO [iLPR] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iSTMR:	SPR_LO [iSTMR] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				default:
					break;
				}
			}
		}
		else if (LI_ADDR_MODE_LH_DEC(IR) == 1) //LOAD HI
		{

			if(LI_ADDR_MODE_GS_DEC(IR) == 0) //LOAD GPR
			{
				switch (LI_ADDR_MODE_RT_DEC(IR))
				{
				case iAX: GPR_HI [iAX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iBX: GPR_HI [iBX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iCX: GPR_HI [iCX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iDX: GPR_HI [iDX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iEX: GPR_HI [iEX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iFX: GPR_HI [iFX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iGX: GPR_HI [iGX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iHX: GPR_HI [iHX] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				default:
					break;
				}
			}
			else if(LI_ADDR_MODE_GS_DEC(IR) == 1) //LOAD SPR
			{
				switch (LI_ADDR_MODE_RT_DEC(IR))
				{
				case iSP: SPR_HI [iSP] = (LI_ADDR_MODE_IM_DEC(IR)); break; 
				case iBP: SPR_HI [iBP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iDMAP: SPR_HI [iDMAP] = (LI_ADDR_MODE_IM_DEC(IR));
				case iKRNLP: SPR_HI [iKRNLP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iTMRIP: SPR_HI [iTMRIP] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iSYSCR: SPR_HI [iSYSC] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iLPR: SPR_HI [iLPR] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				case iSTMR:	SPR_HI [iSTMR] = (LI_ADDR_MODE_IM_DEC(IR)); break;
				default:
					break;
				}
			}
		}
		
	break;
	case iLSINS:
		if(debug)
		{
			std::cout << "LSINS Instruction:" << std::endl;
			std::cout << "\tLS: " << std::bitset <1> (LSINS_ADDR_MODE_LS(IR)) << std::endl;
			std::cout << "\tGS: " << std::bitset <1> (LSINS_ADDR_MODE_GS(IR)) << std::endl;
			std::cout << "\tHL: " << std::bitset <1> (LSINS_ADDR_MODE_HL(IR)) << std::endl;
			std::cout << "\tT1: " << std::bitset <3> (LSINS_ADDR_MODE_T1(IR)) << std::endl;
			std::cout << "\tP1: " << std::bitset <3> (LSINS_ADDR_MODE_P1(IR)) << std::endl;
			std::cout << "\tIM: " << std::bitset <4> (LSINS_ADDR_MODE_IM(IR)) << std::endl;
		}
		if(LSINS_ADDR_MODE_LS(IR) == 0) //LOAD
		{
			if(LSINS_ADDR_MODE_GS(IR) == 0) //GPR
			{
				//Address = Register + Immediate
				if(LSINS_ADDR_MODE_HL(IR) == 0)
				{
					GPR_LO [LSINS_ADDR_MODE_T1(IR)] = (unsigned char)(memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 8 ) + LSINS_ADDR_MODE_IM(IR))));
				}
				else if(LSINS_ADDR_MODE_HL(IR) == 1)
				{
					GPR_HI [LSINS_ADDR_MODE_T1(IR)] = (unsigned char)(memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 8 ) + LSINS_ADDR_MODE_IM(IR))));
				}
			}
			else if(LSINS_ADDR_MODE_GS(IR) == 1) //SPR
			{
				if(LSINS_ADDR_MODE_HL(IR) == 0)
				{
					SPR_LO [LSINS_ADDR_MODE_T1(IR)] = (unsigned char)(memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 8 ) + LSINS_ADDR_MODE_IM(IR))));
				}
				else if(LSINS_ADDR_MODE_HL(IR) == 1)
				{
					SPR_HI [LSINS_ADDR_MODE_T1(IR)] = (unsigned char)(memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 8 ) + LSINS_ADDR_MODE_IM(IR))));
				}
			}
		}
		else if(LSINS_ADDR_MODE_LS(IR) == 1) //STORE
		{
			if(LSINS_ADDR_MODE_HL(IR) == 1)
			{
				//(GPR_LO [LSINS_ADDR_MODE_T1(IR)] | (GPR_HI [LSINS_ADDR_MODE_T1(IR)] << 0x8))
				memory_access_write(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8)), GPR_HI [LSINS_ADDR_MODE_T1(IR)]);
				if(debug)
				{
					std::cout << "\tWROTE: " << std::endl;
					std::cout << "\t\tADDR: " << std::bitset <16> ((GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8))) << std::endl;
					std::cout << "\t\tDATA: " << std::bitset <16> ((GPR_LO [LSINS_ADDR_MODE_T1(IR)] | (GPR_HI [LSINS_ADDR_MODE_T1(IR)] << 0x8))) << std::endl;
					std::cout << "\t\tDATA RAM: " << std::bitset <8> (memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8)))) << std::endl;
				}
			}
			else if(LSINS_ADDR_MODE_HL(IR) == 0)
			{
				memory_access_write(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8)), GPR_LO [LSINS_ADDR_MODE_T1(IR)]);
				if(debug)
				{
					std::cout << "\tWROTE: " << std::endl;
					std::cout << "\t\tADDR: " << std::bitset <16> ((GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8))) << std::endl;
					std::cout << "\t\tDATA: " << std::bitset <16> ((GPR_LO [LSINS_ADDR_MODE_T1(IR)] | (GPR_HI [LSINS_ADDR_MODE_T1(IR)] << 0x8))) << std::endl;
					std::cout << "\t\tDATA RAM: " << std::bitset <8> (memory_access_read(mem_space, (GPR_LO [LSINS_ADDR_MODE_P1(IR)] | (GPR_HI [LSINS_ADDR_MODE_P1(IR)] << 0x8)))) << std::endl;
				}
			}
			
		}
	break;
	case iALUF:
		if(debug)
		{
			std::cout << "ALUF Instruction:" << std::endl;
			std::cout << "\tRT: " << std::bitset <3> (ALUF_ADDR_MODE_RT_DEC(IR)) << std::endl; 
			std::cout << "\tOP: " << std::bitset <4> (ALUF_ADDR_MODE_OP_DEC(IR)) << std::endl; 
			std::cout << "\tS2: " << std::bitset <3> (ALUF_ADDR_MODE_S2_DEC(IR)) << std::endl; 
			std::cout << "\tS1: " << std::bitset <3> (ALUF_ADDR_MODE_S1_DEC(IR)) << std::endl; 
		}
		switch (ALUF_ADDR_MODE_OP_DEC(IR))
		{
		case iADD:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] + GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] + GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)]; 
		break;
		case iSUB:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] - GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] - GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)];
		break;
		case iADDCO: break;
		case iSUBCO: break;
		case iADDCI: break;
		case iSUBCI: break;
		case iADDCOCI: break;
		case iSUBCOCI: break;
		case iAND:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] & GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] & GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)];
		break;
		case iXOR:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] ^ GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] ^ GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)];
		break;
		case iOR:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] | GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] | GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)];		
		break;
		case iNOT:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = !GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = !GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)]; 
		break;
		case iSHFTL:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] << GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] << GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)]; 
		break;
		case iSHFTR:
			GPR_HI [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [ALUF_ADDR_MODE_S1_DEC(IR)] >> GPR_HI [ALUF_ADDR_MODE_S2_DEC(IR)]; 
			GPR_LO [ALUF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [ALUF_ADDR_MODE_S1_DEC(IR)] >> GPR_LO [ALUF_ADDR_MODE_S2_DEC(IR)];
		break;
		default:
			break;
		}
	break;
	case iINMI:
		if(false)
		{
			std::cout << "INMI Instruction:" << std::endl;
			std::cout << "\tOP: " << std::bitset <5> (INMI_ADDR_MODE_OP(IR)) << std::endl;
			std::cout << "\tGS: " << std::bitset <1> (INMI_ADDR_MODE_GS(IR)) << std::endl;
			std::cout << "\tHL: " << std::bitset <1> (INMI_ADDR_MODE_HL(IR)) << std::endl;
			std::cout << "\tS1: " << std::bitset <3> (INMI_ADDR_MODE_S1(IR)) << std::endl;
			std::cout << "\tT1: " << std::bitset <3> (INMI_ADDR_MODE_T1(IR)) << std::endl;
			std::cout << "\tIM: " << std::bitset <8> (INMI_ADDR_MODE_IM(IR)) << std::endl;
		}
		switch (INMI_ADDR_MODE_OP(IR))
		{
		case iSPpp:
			temporal = (SPR_LO  [iSP] | (SPR_HI [iSP] << 0x8));
			temporal += INMI_ADDR_MODE_IM(IR);
			SPR_HI [iSP] = ((temporal >> 0x8) & 0xff);
			SPR_LO [iSP] = (temporal & 0xff);
		break; 
		case iSPss:
			temporal = (SPR_LO  [iSP] | (SPR_HI [iSP] << 0x8));
			temporal -= INMI_ADDR_MODE_IM(IR);
			SPR_HI [iSP] = ((temporal >> 0x8) & 0xff);
			SPR_LO [iSP] = (temporal & 0xff);
		break;
		case iSTSMR: 

		break;
		case iDMABDM:
			//std::cout << "dev read" << std::endl;
			dma_mem_write(mem_space);
		break;
		case iDMABMD:
			//std::cout << "memory write" << std::endl;
			dma_dev_write(mem_space);
		break;
		case iHALT:
			//The Halt Instruction will only terminate core execution, while the architecture matures
			//and until a proper kernel is developed
			HALT = true;
			//std::lock_guard<std::mutex> lck (*core_mutex);
			core_mutex->lock();
			debug_registers();
			core_mutex->unlock();
			std::cout << "CORE: HALT" << std::endl;
		break;
		case iSPUSH:
			//std::cout << "iSPUSH\n";
			if(INMI_ADDR_MODE_HL(IR) == 0x00)
			{
				if(INMI_ADDR_MODE_GS(IR) == iGPR)
				{
					memory_access_write(mem_space, (SPR_LO [iSP] | (SPR_HI [iSP] << 0x8)), GPR_LO [INMI_ADDR_MODE_S1(IR)]);
				}
				else if(INMI_ADDR_MODE_GS(IR) == iSPR)
				{
					memory_access_write(mem_space, (SPR_LO [iSP] | (SPR_HI [iSP] << 0x8)), SPR_LO [INMI_ADDR_MODE_S1(IR)]);
				}
			}
			else if(INMI_ADDR_MODE_HL(IR) == 0x01)
			{
				if(INMI_ADDR_MODE_GS(IR) == iGPR)
				{
					memory_access_write(mem_space, (SPR_LO [iSP] | (SPR_HI [iSP] << 0x8)), GPR_HI [INMI_ADDR_MODE_S1(IR)]);
				}
				else if(INMI_ADDR_MODE_GS(IR) == iSPR)
				{
					memory_access_write(mem_space, (SPR_LO [iSP] | (SPR_HI [iSP] << 0x8)), SPR_HI [INMI_ADDR_MODE_S1(IR)]);
				}
			}
			//memory_access_write(mem_space, address, data);
		break;
		case iRETURN:
			//std::cout << "iRETURN\n";
			PC = (SPR_LO [iLPR] | (SPR_HI [iLPR] << 0x8));
			STMR.JMP = true;
		break;
		case iCALL:
			temporal = PC + 2;
			//std::cout << "iCALL Ins\n";
			SPR_HI [iLPR] = ((temporal >> 0x8) & 0xff);
			SPR_LO [iLPR] = (temporal & 0xff);
			STMR.JMP = true;
			PC = (GPR_LO [INMI_ADDR_MODE_S1(IR)] | (GPR_HI [INMI_ADDR_MODE_S1(IR)] << 0x8));
			//std::cout << "Jumping to address: " << std::bitset <16> (PC) << std::endl;
		break;
		default:
			break;
		}
	break;
	case iJMPR:
		if(debug)
		{
			std::cout << "JMPFR Instruction:" << std::endl;
			std::cout << "\tIVR: " << std::bitset <1> (JMPFR_ADDR_MODE_IVR(IR)) << std::endl;
			std::cout << "\tOP: " << std::bitset <3> (JMPFR_ADDR_MODE_OP(IR)) << std::endl;
			std::cout << "\tAP1: " << std::bitset <3> (JMPFR_ADDR_MODE_AP1(IR)) << std::endl;
			std::cout << "\tT1: " << std::bitset <3> (JMPFR_ADDR_MODE_T1(IR)) << std::endl;
			std::cout << "\tT2: " << std::bitset <3> (JMPFR_ADDR_MODE_T2(IR)) << std::endl;
		}
		if(JMPFR_ADDR_MODE_IVR(IR) == 0)
		{
			switch (JMPFR_ADDR_MODE_OP(IR))
			{
			case iJMPAB: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) > (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPEQ: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) == (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPBA: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) < (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPNEG: if(((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) >> 0xf) == 0x1)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPOVR: if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) == 0xffff)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPZERO: if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) == 0x0000)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPNONE: PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true; break;
			default:
				break;
			}
		}
		else
		{
			switch (JMPFR_ADDR_MODE_OP(IR))
			{
			case iJMPAB: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) < (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPEQ: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) != (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPBA: 
				if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) > (GPR_LO [JMPFR_ADDR_MODE_T2(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T2(IR)] << 0x8)))
					{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPNEG: if(((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) >> 0xf) != 0x1)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPOVR: if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) != 0xffff)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPZERO: if((GPR_LO [JMPFR_ADDR_MODE_T1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_T1(IR)] << 0x8)) != 0x0000)
				{PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true;} break;
			case iJMPNONE: PC = (GPR_LO [JMPFR_ADDR_MODE_AP1(IR)] | (GPR_HI [JMPFR_ADDR_MODE_AP1(IR)] << 0x8)); STMR.JMP = true; break;
			default:
				break;
			}
		}
	break;
	case iMOVF: //Not Fully implemented Instruction (Almost xd)
		if(debug)
		{
			std::cout << "MOVF Instruction:" << std::endl;
			std::cout << "\tHL_T: " << std::bitset <1> (MOVF_ADDR_MODE_HL_T_DEC(IR)) << std::endl;
			std::cout << "\tHL_S: " << std::bitset <1> (MOVF_ADDR_MODE_HL_S_DEC(IR)) << std::endl;
			std::cout << "\tFT_B: " << std::bitset <1> (MOVF_ADDR_MODE_FT_B_DEC(IR)) << std::endl;
			std::cout << "\tOP: " << std::bitset <3> (MOVF_ADDR_MODE_OP_DEC(IR)) << std::endl;
			std::cout << "\tRT: " << std::bitset <3> (MOVF_ADDR_MODE_RT_DEC(IR)) << std::endl;
			std::cout << "\tS1: " << std::bitset <3> (MOVF_ADDR_MODE_S1_DEC(IR)) << std::endl;
		}
		if(MOVF_ADDR_MODE_FT_B_DEC(IR) == 0x1) //Full Transfer
		{
			switch (MOVF_ADDR_MODE_OP_DEC(IR))
			{
			case iMOVGPRGPR:	
				GPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				GPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
			break;
			case iMOVGPRSPR:
				GPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = SPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				GPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = SPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
			break;
			case iMOVSPRGPR:
				SPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				SPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
			break;
			case iMOVGPRDMAR:
				switch (MOVF_ADDR_MODE_RT_DEC(IR))
				{
				case iMMC:
					MMC = ((GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)] & 0xff) | ((GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)] & 0xff) << 0x8));
					//std::cout << "MMC wrote: " << std::bitset <16> (MMC) << std::endl;
				break;
				case iMMA:
					MMA = ((GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)] & 0xff) | ((GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)] & 0xff) << 0x8)); 
					//std::cout << "MMA wrote: " << std::bitset <16> (MMA) << std::endl;
				break;
				}
			default:
				break;
			}
		}	
		else
		{
			switch (MOVF_ADDR_MODE_OP_DEC(IR))
			{
			case iMOVGPRGPR:
				if(MOVF_ADDR_MODE_HL_S_DEC(IR) == 1) source = GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				else source = GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
				if(MOVF_ADDR_MODE_HL_T_DEC(IR) == 1) GPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = source;
				else GPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = source; 
			break;
			case iMOVGPRSPR:
				if(MOVF_ADDR_MODE_HL_S_DEC(IR) == 1) source = SPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				else source = SPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
				if(MOVF_ADDR_MODE_HL_T_DEC(IR) == 1) GPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = source;
				else GPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = source;	
			break;
			case iMOVSPRGPR:
				if(MOVF_ADDR_MODE_HL_S_DEC(IR) == 1) source = GPR_HI [MOVF_ADDR_MODE_S1_DEC(IR)];
				else source = GPR_LO [MOVF_ADDR_MODE_S1_DEC(IR)];
				if(MOVF_ADDR_MODE_HL_T_DEC(IR) == 1) SPR_HI [MOVF_ADDR_MODE_RT_DEC(IR)] = source;
				else SPR_LO [MOVF_ADDR_MODE_RT_DEC(IR)] = source;
			break;
			default:
				break;
			}
			
		}
		
	break;
	case iJMPD:
		if(debug)
		{ 
		std::cout << "JMPD Instruction:" << std::endl;
		std::cout << "IM: " << std::bitset <13> (JMPD_ADDR_MODE_IM_DEC(IR)) << std::endl;
		}
		STMR.JMP = true;
		PC = JMPD_ADDR_MODE_IM_DEC(IR);
	break;
	default:
		break;
	}
}
void CORE::reset_vectors()
{
	for(size_t i = 0;i <= 0xf;i++)
	{
		SPR_LO [i] = 0x00;
		SPR_HI [i] = 0x00;
		GPR_LO [i] = 0x00;
		GPR_HI [i] = 0x00;
	}
	IR	= 0x00;
	PC	= 0x00;
	//Set stack Address
	SPR_HI [iSP] = STACK_ADDR;
	SPR_LO [iSP] = STACK_ADDR;
	SPR_HI [iBP] = STACK_ADDR;
	SPR_LO [iBP] = STACK_ADDR;
}

char CORE::memory_access_read(struct MEMORY_ARRAY * MAPPED_MEM, unsigned short int address)
{
	if((address >> 0x8) == 0x00) //ROM mapped address
	{
		return MAPPED_MEM->ROM_mem [address];
	}
	if((address >> 0x8) != 0x00) //RAM mapped address
	{
		return MAPPED_MEM->RAM_mem [address];
	}
	return 0x00; //If nothing is return, or unknow address/out of bounds address
}

void CORE::memory_access_write(struct MEMORY_ARRAY * MAPPED_MEM, unsigned short int address, char data)
{
	if((address >> 0x8) == 0x00) //ROM mapped address
	{
		MAPPED_MEM->ROM_mem [address] = data;
	}
	if((address >> 0x8) != 0x00) //RAM mapped address
	{
		MAPPED_MEM->RAM_mem [address] = data;
	}
}

CORE::CORE(std::vector <device_port*> *ports, std::mutex &core_mutex)
{
	this->ports = ports;
	this->core_mutex = &core_mutex;
}
//To memory
void CORE::dma_mem_write(struct MEMORY_ARRAY * MAPPED_MEM)
{
    core_mutex->lock();
    for(size_t i = 0;i <= MMC;i++)
    {
		//device_port *tmp = (*ports)[PID];
		memory_access_write(MAPPED_MEM, (MMA + i), (*ports)[PID]->FIFO_O.front());	
        (*ports)[PID]->FIFO_O.pop();
    }
    core_mutex->unlock();
}
//To device

void CORE::dma_dev_write(struct MEMORY_ARRAY * MAPPED_MEM)
{
    core_mutex->lock();
	for(size_t i = 0;i <= MMC;i++)
	{
		//std::cout << "To Device Port\n";
		(*ports)[PID]->FIFO_I.push(memory_access_read(MAPPED_MEM, (MMA + i)));
	}
    core_mutex->unlock();
}

void CORE::run(struct MEMORY_ARRAY *MAPPED_MEM, bool &halt)
{
	while(!halt)
	{
		//tmp_lock.lock();
		execute(MAPPED_MEM, halt);
		//std::system("clear");
	}
	//std::cout << std::endl;
	std::cout << "Stack region: address 0xffff\n";
	for(int addr = 0xffff; addr >= 0xfff0;)
	{
		std::cout << "address: 0x" << std::hex << addr << " data:";
		for(size_t x = 0;x <= 4;x++)
		{
			//memory_access_write(MAPPED_MEM, 0xff, 0xff);

			std::cout << " 0x" << std::bitset <8> (memory_access_read(MAPPED_MEM, addr));
			std::cout.flush();
			addr--;
		}
		std::cout << std::endl;
	}
	//core_mutex->lock();
	core_mutex->unlock();
	std::cout.flush();
}

//EXAMPLE EXAMPLE EXAMPLE EXAMPLE EXAMPLE EXAMPLE EXAMPLE EXAMPLE
/*
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
		std::cout << "the entire file content is in memory" << std::endl;
	}
	else 
	{
		cout << "Unable to open file" << std::endl;
		exit(0);
	}
	//Print loaded ROM memory contents
	std::cout << memblock << std::endl;

	//Load loaded file into RAM
	std::cout << "Loading Data into mapped Memory!" << std::endl;
	std::cout << "Data: " << (memblock) << std::endl;
	struct MEMORY_ARRAY MEMORY_SPACE;
	
	MEMORY_SPACE.ROM_mem.resize(ROM_SIZE);
	MEMORY_SPACE.RAM_mem.resize(RAM_SIZE);
	MEMORY_SPACE.IO_mem.resize(IO_BUFFERS_SPACE_SIZE);

	for(size_t i = 0;i <= strlen(memblock);i += 2)
	{
		if((i >> 0x8) == 0x00) //ROM mapped address
		{
			MEMORY_SPACE.ROM_mem [i] = (memblock [i] | (memblock [i + 1] << 8));//((*(memblock + (i + 1)) << 0xf) & *(memblock + i)));
		}
		if((i >> 0x8) == 0xfe) //RAM mapped address
		{
			MEMORY_SPACE.RAM_mem [i] = (memblock [i] | (memblock [i + 1] << 8));
		}
		if((i >> 0x8) == 0xff) //IO mapped address
		{
			MEMORY_SPACE.IO_mem [i] = (memblock [i] | (memblock [i + 1] << 8));
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
	std::cout << "Data Loaded into ROM: " << std::endl;
	for(int i = 0;i <= 32;i++)
	{
		for(int x = 0;x <= 4;x++)
		{
		unsigned short temp = MEMORY_SPACE.ROM_mem [x + i * 4];//.front();
		std::cout << " " << std::bitset <16> (temp); 
		}
		std::cout << std::endl;
		//std::cout << (char)(temp & 0xff);
		//std::cout << (char)((temp >> 0x8) & 0xff) << std::endl;
		//ROM::ROM_array.pop_front();
	}
	//
	//MEMORY_SPACE->ROM_mem = & ROM::ROM_array;
	

	//std::cout << (char) ROM::ROM_array [2];
	//Create One CORE
	CORE CORE_0;
	CORE_0.reset_vectors();

	for(int i = 0;i <= 0xff;i++)
	{
		CORE_0.execute(&MEMORY_SPACE);
		//std::cin
		//std::cin.get();
	}
	CORE_0.debug_registers();

	delete[] memblock;
	return 0;
}*/
