
#define reg_lo          0x00
#define reg_hi          0x01
//#define reg_ft			0x02 //Non real opcode

//Braching signals
#define branch_ab		0x00
#define branch_eq		0x01
#define branch_ba		0x02
#define branch_zero		0x03
#define branch_ovr		0x04
#define branch_neg		0x05
#define branch_noop		0x06

#define IR_HI_LO(hi,lo) ((lo & 0xff) | ((hi << 8) & 0xff00))
#define BYTE_MERGE(hi,lo) IR_HI_LO(hi,lo)

//Addresing modes encoding
#define ALUF_ADDR_MODE(ins, op, rt, s1, s2) ((ins) | (op << 3) | (rt << 7) | (s1 << 10) | (s2 << 13))
#define LI_ADDR_MODE(ins, hl, gs, rt, immv) ((ins) | (hl << 3) | (gs << 4) | (rt << 5) | (immv << 8))
#define JMPD_ADDR_MODE(ins, immv) ((ins) | (immv << 3))
#define MOVF_ADDR_MODE(ins, hl_t, hl_s, ft_b, op, rt, s1) ((ins) | (hl_t << 3) | (hl_s << 4) | (ft_b << 5) | (op << 9) | (rt << 10) | (s1 << 13))
#define JMPFR_ADDR_MODE(ins, ivr, op, ap1, t1, t2) ((ins) | (ivr << 3) | (op << 4) | (reg))
#define LSINS_ADDR_MODE(ins, ls, gs, hl, gs_p, hl_p , t1, p1, immv) ((ins) | (ls << 3) | (gs << 4) | (hl << 5) | (gs_p << 6) | (hl_p << 7) | (t1 << 10) | (p1 << 13) | (immv << 12))
//Target becomes Source when LSINS STORE
//Data addresing modes (Actual Instruction Execution Arrangament)
//Final encoded instruction (temporal holder)
//unsigned short int INS_COMPLETE; //(16bits)

//Addressing modes decoding
#define INS_ADDR_MODE_DEC(ir) (ir & 0x7)
#define ALUF_ADDR_MODE_OP_DEC(ir) ((ir & 0x78) >> 0x3)
#define ALUF_ADDR_MODE_RT_DEC(ir) ((ir & 0x380) >> 0x7)
#define ALUF_ADDR_MODE_S1_DEC(ir) ((ir & 0x1c00) >> 0xa)
#define ALUF_ADDR_MODE_S2_DEC(ir) ((ir & 0xe000) >> 0xd)

#define LI_ADDR_MODE_LH_DEC(ir) ((ir & 0x8) >> 0x3)
#define LI_ADDR_MODE_GS_DEC(ir) ((ir & 0x10) >> 0x4)
#define LI_ADDR_MODE_RT_DEC(ir) ((ir & 0xe0) >> 0x5)
#define LI_ADDR_MODE_IM_DEC(ir) ((ir & 0xff00) >> 0x8)

#define JMPD_ADDR_MODE_IM_DEC(ir) ((ir & 0xfff8) >> 0x3)

#define MOVF_ADDR_MODE_HL_T_DEC(ir) ((ir & 0x8) >> 0x3)
#define MOVF_ADDR_MODE_HL_S_DEC(ir) ((ir & 0x10) >> 0x4)
#define MOVF_ADDR_MODE_FT_B_DEC(ir) ((ir & 0x20) >> 0x5)
#define MOVF_ADDR_MODE_OP_DEC(ir) ((ir & 0x3c0) >> 0x6)
#define MOVF_ADDR_MODE_RT_DEC(ir) ((ir & 0x1c00) >> 0xa)
#define MOVF_ADDR_MODE_S1_DEC(ir) ((ir & 0xe000) >> 0xd)

#define JMPFR_ADDR_MODE_IVR(ir) ((ir & 0x8) >> 0x3)
#define JMPFR_ADDR_MODE_OP(ir) ((ir & 0x70) >> 0x4)
#define JMPFR_ADDR_MODE_AP1(ir) ((ir & 0x380) >> 0x7)
#define JMPFR_ADDR_MODE_T1(ir) ((ir & 0x1c00) >> 0xa)
#define JMPFR_ADDR_MODE_T2(ir) ((ir & 0xe000) >> 0xd)

#define LSINS_ADDR_MODE_LS(ir) ((ir & 0x8) >> 0x3)
#define LSINS_ADDR_MODE_GS(ir) ((ir & 0x10) >> 0x4)
#define LSINS_ADDR_MODE_HL(ir) ((ir & 0x20) >> 0x5)
#define LSINS_ADDR_MODE_T1(ir) ((ir & 0x1c0) >> 0x6)
#define LSINS_ADDR_MODE_P1(ir) ((ir & 0xe00) >> 0x9)
#define LSINS_ADDR_MODE_IM(ir) ((ir & 0xf000) >> 0x12)

#define INMI_ADDR_MODE_OP(ir) ((ir & 0xf8) >> 0x3)
#define INMI_ADDR_MODE_GS(ir) ((ir & 0x100) >> 0x8)
#define INMI_ADDR_MODE_HL(ir) ((ir & 0x200) >> 0x9)
#define INMI_ADDR_MODE_S1(ir) ((ir & 0x1c00) >> 0xa)
#define INMI_ADDR_MODE_T1(ir) ((ir & 0xe000) >> 0xd)
#define INMI_ADDR_MODE_IM(ir) ((ir & 0xff00) >> 0x8)
//
union ALUF_ENCODING //(ADDRESSING MODE)
{
	struct ALUF_DATA_STRUCT
	{
		unsigned char ins : 3;
		unsigned char op : 4;
		unsigned char target_register : 3;
		unsigned char source_register_0 : 3;
		unsigned char source_register_1 : 3;
	} ALUF_DATA_TYPE;
	long int FULL_DATA_TYPE;
};
union LI_ENCODING //(ADDRESSING MODE)
{
	struct LI_DATA_STRUCT
	{
		unsigned char ins : 3;
		unsigned char hl_bit : 1;
		unsigned char gs_bit : 1;
		unsigned char target_register : 3;
		unsigned char inmmediate_value : 8; 
		
	} LI_DATA_TYPE;
	unsigned short int FULL_DATA_TYPE;
};
union MOVF_ENCODING //(ADDRESSING MODE)
{
	struct MOVF_DATA_STRUCT
	{
		unsigned char ins : 3;
		unsigned char hl_t : 1;
		unsigned char hl_s : 1;
		unsigned char ft_bit : 1;
		unsigned char op : 4;
		unsigned char target_register : 3;
		unsigned char source_register : 3;	
	} MOVF_DATA_TYPE;
	unsigned short int FULL_DATA_TYPE;
};
union JMPI_ENCODING
{
	struct JMPI_DATA_STRUCT
	{
		unsigned char ins : 3;
		unsigned char invert_ops : 1;
		unsigned char op : 3;
		unsigned short int immediate_value : 9;
	} JMPI_DATA_TYPE;
	unsigned short int FULL_DATA_TYPE;
};
union JMPFR_ENCODING
{
	struct JMPFR_DATA_STRUCT
	{
		unsigned char ins : 3;
		unsigned char invert_ops : 1;
		unsigned char op : 3;
		unsigned char target_register : 3;
		unsigned char immediate_value : 6;
	} JMPFR_DATA_TYPE;
	unsigned short int FULL_DATA_TYPE;
};

