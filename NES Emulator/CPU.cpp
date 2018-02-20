#include <iostream>
#ifdef _DEBUG_MODE
	#include <iostream>
	#include <iomanip>
#endif
#include <fstream>
#include <stdexcept>
#include "CPU.h"
#include "enums.h"
#include "PPU.h"

// CPU Memory
void CPU::Write(uint16_t offset, uint8_t data) {
	if (offset == 0x4014) PPUPtr->DMA();
	cpuMem.Write(offset, data);
	Tick();
}
uint8_t CPU::Read(uint16_t offset) { 
	uint8_t data = cpuMem.Read(offset); 
	Tick(); 
	return data;
}
uint8_t CPU::ReadNoTick(uint16_t offset) {
	uint8_t data = cpuMem.Read(offset);
	return data;
}

// Instructions
void CPU::ADC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	unsigned int temp = A + M + C;
	C = ((temp >> 8) & 0x1);
	Z = (temp & 0xFF) == 0;
	V = C ^ ((A >> 7) & 0x1) ^ ((M >> 7) & 0x1) ^ ((temp >> 7) & 0x1);
	N = (temp >> 7) & 0x1;
	A = temp;
}
void CPU::AND(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A & M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::ASL(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	if (addrMode == AM_ACC) {
		temp = A << 1;
		A = (temp & 0xFF);
	}
	else {
		uint8_t M = Read(offset);
		temp = M << 1;
		Write(offset, (temp & 0xFF));
	}
	C = (temp >> 8) & 0x1;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU::BCC(uint16_t offset, AddrMode addrMode) {
	if (C == 0) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BCS(uint16_t offset, AddrMode addrMode) {
	if (C == 1) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BEQ(uint16_t offset, AddrMode addrMode) {
	if (Z == 1) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BIT(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	Z = !(A & M);
	V = (M >> 6) & 0x1;
	N = (M >> 7) & 0x1;
}
void CPU::BMI(uint16_t offset, AddrMode addrMode) {
	if (N == 1) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BNE(uint16_t offset, AddrMode addrMode) {
	if (Z == 0) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BPL(uint16_t offset, AddrMode addrMode) {
	if (N == 0) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BRK(uint16_t offset, AddrMode addrMode) { // TODO: Check if correct
	PC += 1;
	StackPush((PC >> 8) & 0xFF);
	StackPush(PC & 0xFF);
	PushP(true);
	PC = Read(0xFFFE) + (Read(0xFFFF) << 8);
	I = 1; // http://6502.org/tutorials/interrupts.html#2.2
}
void CPU::BVC(uint16_t offset, AddrMode addrMode) {
	if (V == 0) PC += static_cast<int8_t>(Read(offset));
}
void CPU::BVS(uint16_t offset, AddrMode addrMode) {
	if (V == 1) PC += static_cast<int8_t>(Read(offset));
}
void CPU::CLC(uint16_t offset, AddrMode addrMode) {
	C = 0;
}
void CPU::CLD(uint16_t offset, AddrMode addrMode) {
	D = 0;
}
void CPU::CLI(uint16_t offset, AddrMode addrMode) {
	I = 0;
}
void CPU::CLV(uint16_t offset, AddrMode addrMode) {
	V = 0;
}
void CPU::CMP(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = A >= M;
	Z = A == M;
	N = ((A - M) >> 7) & 0x1;
}
void CPU::CPX(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = X >= M;
	Z = X == M;
	N = ((X - M) >> 7) & 0x1;
}
void CPU::CPY(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = Y >= M;
	Z = Y == M;
	N = ((Y - M) >> 7) & 0x1;
}
void CPU::DEC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset) - 1;
	Write(offset, M);
	Z = M == 0;
	N = (M >> 7) & 0x1;
}
void CPU::DEX(uint16_t offset, AddrMode addrMode) {
	X = X - 1;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU::DEY(uint16_t offset, AddrMode addrMode) {
	Y = Y - 1;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU::EOR(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A ^ M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::INC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset) + 1;
	Write(offset, M);
	Z = M == 0;
	N = (M >> 7) & 0x1;
}
void CPU::INX(uint16_t offset, AddrMode addrMode) {
	X = X + 1;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU::INY(uint16_t offset, AddrMode addrMode) {
	Y = Y + 1;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU::JMP(uint16_t offset, AddrMode addrMode) {
	PC = offset;
}
void CPU::JSR(uint16_t offset, AddrMode addrMode) {
	PC -= 1;
	StackPush((PC >> 8) & 0xFF);	// 4
	StackPush(PC & 0xFF);			// 5
	Tick();							// 6
	PC = offset;
}
void CPU::LDA(uint16_t offset, AddrMode addrMode) {
	A = Read(offset);
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::LDX(uint16_t offset, AddrMode addrMode) {
	X = Read(offset);
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU::LDY(uint16_t offset, AddrMode addrMode) {
	Y = Read(offset);
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU::LSR(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	uint8_t oldBit;
	if (addrMode == AM_ACC) {
		temp = A >> 1;
		oldBit = A & 0x1;
		A = (temp & 0xFF);
	}
	else {
		uint8_t M = Read(offset);
		temp = M >> 1;
		oldBit = M & 0x1;
		Write(offset, (temp & 0xFF));
	}
	C = oldBit;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU::NOP(uint16_t offset, AddrMode addrMode) {
}
void CPU::ORA(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A | M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::PHA(uint16_t offset, AddrMode addrMode) {
	StackPush(A);
}
void CPU::PHP(uint16_t offset, AddrMode addrMode) {
	PushP(true);
}
void CPU::PLA(uint16_t offset, AddrMode addrMode) {
	A = StackPop();
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::PLP(uint16_t offset, AddrMode addrMode) {
	PopToP();
}
void CPU::ROL(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	if (addrMode == AM_ACC) {
		temp = A << 1;
		temp = temp | C;
		A = (temp & 0xFF);
	}
	else {
		uint8_t M = Read(offset);
		temp = M << 1;
		temp = temp | C;
		Write(offset, (temp & 0xFF));
	}
	C = (temp >> 8) & 0x1;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU::ROR(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	uint8_t oldBit;
	if (addrMode == AM_ACC) {
		temp = A >> 1;
		temp = temp | (C << 7);
		oldBit = A & 0x1;
		A = (temp & 0xFF);
	}
	else {
		uint8_t M = Read(offset);
		temp = M >> 1;
		temp = temp | (C << 7);
		oldBit = M & 0x1;
		Write(offset, (temp & 0xFF));
	}
	C = oldBit & 0x1;
	Z = temp == 0;
	N = (temp >> 7) & 0x1;
}
void CPU::RTI(uint16_t offset, AddrMode addrMode) {
	Tick(); Tick();					// 2,3 Check pg133 of book. A
	PopToP();						// 4
	uint8_t PC_l = StackPop();		// 5
	uint8_t PC_h = StackPop();		// 6
	PC = (PC_h << 8) + PC_l;
}
void CPU::RTS(uint16_t offset, AddrMode addrMode) {
	Tick(); Tick();					// 2,3
	uint8_t PC_l = StackPop();		// 4
	uint8_t PC_h = StackPop();		// 5
	PC = (PC_h << 8) + PC_l;
	Tick();							// 6
	PC += 1;
}
void CPU::SBC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	unsigned int temp = A + ~M + C;

	V = ((A ^ (temp & 0xFF)) & ((~M) ^ (temp & 0xFF)) & 0x80) != 0;
	A = (temp & 0xFF);
	C = !((temp >> 8) & 0x1);
	Z = A == 0;
	N = (A >> 7);
}
void CPU::SEC(uint16_t offset, AddrMode addrMode) {
	C = 1;
}
void CPU::SED(uint16_t offset, AddrMode addrMode) {
	D = 1;
}
void CPU::SEI(uint16_t offset, AddrMode addrMode) {
	I = 1;
}
void CPU::STA(uint16_t offset, AddrMode addrMode) {
	Write(offset, A);
}
void CPU::STX(uint16_t offset, AddrMode addrMode) {
	Write(offset, X);
}
void CPU::STY(uint16_t offset, AddrMode addrMode) {
	Write(offset, Y);
}
void CPU::TAX(uint16_t offset, AddrMode addrMode) {
	X = A;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU::TAY(uint16_t offset, AddrMode addrMode) {
	Y = A;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU::TSX(uint16_t offset, AddrMode addrMode) {
	X = SP;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU::TXA(uint16_t offset, AddrMode addrMode) {
	A = X;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU::TXS(uint16_t offset, AddrMode addrMode) {
	SP = X;
}
void CPU::TYA(uint16_t offset, AddrMode addrMode) {
	A = Y;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}

// Stack Management
void CPU::StackPush(uint8_t data) {
#ifdef _DEBUG_MODE 
	log << "Pushing: " << (int)data << '\n';
#endif
	Write(0x100 + SP, data);
	SP = (SP - 1) & 0xFF;
}
uint8_t CPU::StackPop() {
	uint8_t data = Read(0x100 + ((SP + 1) & 0xFF)); // +1 since SP points to next free location
#ifdef _DEBUG_MODE
	log << "Popping: " << (int)data << '\n';
#endif
	SP = (SP + 1) & 0xFF;
	return data;
}

// Processor Status Management
void CPU::PopToP() {
	uint8_t temp = StackPop();
	N = ((temp >> N.GetBitNum()) & 0x1);
	V = ((temp >> V.GetBitNum()) & 0x1);
	// Bit 4 and 5 are ignored 
	// for PLP and RTI
	D = ((temp >> D.GetBitNum()) & 0x1);
	I = ((temp >> I.GetBitNum()) & 0x1);
	Z = ((temp >> Z.GetBitNum()) & 0x1);
	C = ((temp >> C.GetBitNum()) & 0x1);
}
void CPU::PushP(bool shouldSetBit4) {
	uint8_t temp = 0x00;
	uint8_t bit4 = shouldSetBit4 ? 1 : 0;
	temp = temp | ((N & 0x1) << N.GetBitNum());
	temp = temp | ((V & 0x1) << V.GetBitNum());
	temp = temp | ((1 & 0x1) << 5);
	temp = temp | ((bit4 & 0x1) << 4);
	temp = temp | ((D & 0x1) << D.GetBitNum());
	temp = temp | ((I & 0x1) << I.GetBitNum());
	temp = temp | ((Z & 0x1) << Z.GetBitNum());
	temp = temp | ((C & 0x1) << C.GetBitNum());
	StackPush(temp);
}
void CPU::SetP(uint8_t val, bool shouldSetBit4) {
	N = ((val >> N.GetBitNum()) & 0x1);
	V = ((val >> V.GetBitNum()) & 0x1);
	// Bit 5 is ignored 
	if (shouldSetBit4) B = ((val >> B.GetBitNum()) & 0x1);
	D = ((val >> D.GetBitNum()) & 0x1);
	I = ((val >> I.GetBitNum()) & 0x1);
	Z = ((val >> Z.GetBitNum()) & 0x1);
	C = ((val >> C.GetBitNum()) & 0x1);
}

// Operation Table
void CPU::SetupOperationTable() {
	operationTable[0x69] = Operation(INSTR_ADC, "ADC", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::ADC, 1 << 2);
	operationTable[0x65] = Operation(INSTR_ADC, "ADC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::ADC, 1 << 3);
	operationTable[0x75] = Operation(INSTR_ADC, "ADC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::ADC, 1 << 4);
	operationTable[0x6D] = Operation(INSTR_ADC, "ADC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::ADC, 1 << 4);
	operationTable[0x7D] = Operation(INSTR_ADC, "ADC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::ADC, (1 << 4) + (1 << 5));
	operationTable[0x79] = Operation(INSTR_ADC, "ADC", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::ADC, (1 << 4) + (1 << 5));
	operationTable[0x61] = Operation(INSTR_ADC, "ADC", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::ADC, 1 << 6);
	operationTable[0x71] = Operation(INSTR_ADC, "ADC", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::ADC, (1 << 5) + (1 << 6));
	operationTable[0x29] = Operation(INSTR_AND, "AND", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::AND, 1 << 1);
	operationTable[0x25] = Operation(INSTR_AND, "AND", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::AND, 1 << 2);
	operationTable[0x35] = Operation(INSTR_AND, "AND", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::AND, 1 << 4);
	operationTable[0x2D] = Operation(INSTR_AND, "AND", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::AND, 1 << 4);
	operationTable[0x3D] = Operation(INSTR_AND, "AND", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::AND, (1 << 4) + (1 << 5));
	operationTable[0x39] = Operation(INSTR_AND, "AND", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::AND, (1 << 4) + (1 << 5));
	operationTable[0x21] = Operation(INSTR_AND, "AND", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::AND, 1 << 6);
	operationTable[0x31] = Operation(INSTR_AND, "AND", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::AND, 1 << 5);
	operationTable[0x0A] = Operation(INSTR_ASL, "ASL", AM_ACC, "ACCUMULATOR ( INS A )", &CPU::ASL, 1 << 2);
	operationTable[0x06] = Operation(INSTR_ASL, "ASL", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::ASL, 1 << 5);
	operationTable[0x16] = Operation(INSTR_ASL, "ASL", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::ASL, 1 << 6);
	operationTable[0x0E] = Operation(INSTR_ASL, "ASL", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::ASL, 1 << 6);
	operationTable[0x1E] = Operation(INSTR_ASL, "ASL", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::ASL, 1 << 7);
	operationTable[0x90] = Operation(INSTR_BCC, "BCC", AM_REL, "RELATIVE", &CPU::BCC, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xB0] = Operation(INSTR_BCS, "BCS", AM_REL, "RELATIVE", &CPU::BCS, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xF0] = Operation(INSTR_BEQ, "BEQ", AM_REL, "RELATIVE", &CPU::BEQ, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x24] = Operation(INSTR_BIT, "BIT", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::BIT, 1 << 3);
	operationTable[0x2C] = Operation(INSTR_BIT, "BIT", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::BIT, 1 << 4);
	operationTable[0x30] = Operation(INSTR_BMI, "BMI", AM_REL, "RELATIVE", &CPU::BMI, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xD0] = Operation(INSTR_BNE, "BNE", AM_REL, "RELATIVE", &CPU::BNE, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x10] = Operation(INSTR_BPL, "BPL", AM_REL, "RELATIVE", &CPU::BPL, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x00] = Operation(INSTR_BRK, "BRK", AM_IMP, "IMPLIED ( INS )", &CPU::BRK, 1 << 7);
	operationTable[0x50] = Operation(INSTR_BVC, "BVC", AM_REL, "RELATIVE", &CPU::BVC, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x70] = Operation(INSTR_BVS, "BVS", AM_REL, "RELATIVE", &CPU::BVS, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x18] = Operation(INSTR_CLC, "CLC", AM_IMP, "IMPLIED ( INS )", &CPU::CLC, 1 << 2);
	operationTable[0xD8] = Operation(INSTR_CLD, "CLD", AM_IMP, "IMPLIED ( INS )", &CPU::CLD, 1 << 2);
	operationTable[0x58] = Operation(INSTR_CLI, "CLI", AM_IMP, "IMPLIED ( INS )", &CPU::CLI, 1 << 2);
	operationTable[0xB8] = Operation(INSTR_CLV, "CLV", AM_IMP, "IMPLIED ( INS )", &CPU::CLV, 1 << 2);
	operationTable[0xC9] = Operation(INSTR_CMP, "CMP", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::CMP, 1 << 2);
	operationTable[0xC5] = Operation(INSTR_CMP, "CMP", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::CMP, 1 << 3);
	operationTable[0xD5] = Operation(INSTR_CMP, "CMP", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::CMP, 1 << 4);
	operationTable[0xCD] = Operation(INSTR_CMP, "CMP", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::CMP, 1 << 4);
	operationTable[0xDD] = Operation(INSTR_CMP, "CMP", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::CMP, (1 << 4) + (1 << 5));
	operationTable[0xD9] = Operation(INSTR_CMP, "CMP", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::CMP, (1 << 4) + (1 << 5));
	operationTable[0xC1] = Operation(INSTR_CMP, "CMP", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::CMP, 1 << 6);
	operationTable[0xD1] = Operation(INSTR_CMP, "CMP", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::CMP, (1 << 5) + (1 << 6));
	operationTable[0xE0] = Operation(INSTR_CPX, "CPX", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::CPX, 1 << 2);
	operationTable[0xE4] = Operation(INSTR_CPX, "CPX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::CPX, 1 << 3);
	operationTable[0xEC] = Operation(INSTR_CPX, "CPX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::CPX, 1 << 4);
	operationTable[0xC0] = Operation(INSTR_CPY, "CPY", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::CPY, 1 << 2);
	operationTable[0xC4] = Operation(INSTR_CPY, "CPY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::CPY, 1 << 3);
	operationTable[0xCC] = Operation(INSTR_CPY, "CPY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::CPY, 1 << 4);
	operationTable[0xC6] = Operation(INSTR_DEC, "DEC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::DEC, 1 << 5);
	operationTable[0xD6] = Operation(INSTR_DEC, "DEC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::DEC, 1 << 6);
	operationTable[0xCE] = Operation(INSTR_DEC, "DEC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::DEC, 1 << 6);
	operationTable[0xDE] = Operation(INSTR_DEC, "DEC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::DEC, 1 << 7);
	operationTable[0xCA] = Operation(INSTR_DEX, "DEX", AM_IMP, "IMPLIED ( INS )", &CPU::DEX, 1 << 2);
	operationTable[0x88] = Operation(INSTR_DEY, "DEY", AM_IMP, "IMPLIED ( INS )", &CPU::DEY, 1 << 2);
	operationTable[0x49] = Operation(INSTR_EOR, "EOR", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::EOR, 1 << 2);
	operationTable[0x45] = Operation(INSTR_EOR, "EOR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::EOR, 1 << 3);
	operationTable[0x55] = Operation(INSTR_EOR, "EOR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::EOR, 1 << 4);
	operationTable[0x4D] = Operation(INSTR_EOR, "EOR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::EOR, 1 << 4);
	operationTable[0x5D] = Operation(INSTR_EOR, "EOR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::EOR, (1 << 4) + (1 << 5));
	operationTable[0x59] = Operation(INSTR_EOR, "EOR", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::EOR, (1 << 4) + (1 << 5));
	operationTable[0x41] = Operation(INSTR_EOR, "EOR", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::EOR, 1 << 6);
	operationTable[0x51] = Operation(INSTR_EOR, "EOR", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::EOR, (1 << 5) + (1 << 6));
	operationTable[0xE6] = Operation(INSTR_INC, "INC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::INC, 1 << 5);
	operationTable[0xF6] = Operation(INSTR_INC, "INC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::INC, 1 << 6);
	operationTable[0xEE] = Operation(INSTR_INC, "INC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::INC, 1 << 6);
	operationTable[0xFE] = Operation(INSTR_INC, "INC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::INC, 1 << 7);
	operationTable[0xE8] = Operation(INSTR_INX, "INX", AM_IMP, "IMPLIED ( INS )", &CPU::INX, 1 << 2);
	operationTable[0xC8] = Operation(INSTR_INY, "INY", AM_IMP, "IMPLIED ( INS )", &CPU::INY, 1 << 2);
	operationTable[0x4C] = Operation(INSTR_JMP, "JMP", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::JMP, 1 << 3);
	operationTable[0x6C] = Operation(INSTR_JMP, "JMP", AM_IND, "INDIRECT", &CPU::JMP, 1 << 5);
	operationTable[0x20] = Operation(INSTR_JSR, "JSR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::JSR, 1 << 6);
	operationTable[0xA9] = Operation(INSTR_LDA, "LDA", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::LDA, 1 << 2);
	operationTable[0xA5] = Operation(INSTR_LDA, "LDA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::LDA, 1 << 3);
	operationTable[0xB5] = Operation(INSTR_LDA, "LDA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::LDA, 1 << 4);
	operationTable[0xAD] = Operation(INSTR_LDA, "LDA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::LDA, 1 << 4);
	operationTable[0xBD] = Operation(INSTR_LDA, "LDA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::LDA, (1 << 4) + (1 << 5));
	operationTable[0xB9] = Operation(INSTR_LDA, "LDA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::LDA, (1 << 4) + (1 << 5));
	operationTable[0xA1] = Operation(INSTR_LDA, "LDA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::LDA, 1 << 6);
	operationTable[0xB1] = Operation(INSTR_LDA, "LDA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::LDA, (1 << 5) + (1 << 6));
	operationTable[0xA2] = Operation(INSTR_LDX, "LDX", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::LDX, 1 << 2);
	operationTable[0xA6] = Operation(INSTR_LDX, "LDX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::LDX, 1 << 3);
	operationTable[0xB6] = Operation(INSTR_LDX, "LDX", AM_ZPY, "ZERO PAGE, Y ( INS $??,Y )", &CPU::LDX, 1 << 4);
	operationTable[0xAE] = Operation(INSTR_LDX, "LDX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::LDX, 1 << 4);
	operationTable[0xBE] = Operation(INSTR_LDX, "LDX", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::LDX, (1 << 4) + (1 << 5));
	operationTable[0xA0] = Operation(INSTR_LDY, "LDY", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::LDY, 1 << 2);
	operationTable[0xA4] = Operation(INSTR_LDY, "LDY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::LDY, 1 << 3);
	operationTable[0xB4] = Operation(INSTR_LDY, "LDY", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::LDY, 1 << 4);
	operationTable[0xAC] = Operation(INSTR_LDY, "LDY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::LDY, 1 << 4);
	operationTable[0xBC] = Operation(INSTR_LDY, "LDY", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::LDY, (1 << 4) + (1 << 5));
	operationTable[0x4A] = Operation(INSTR_LSR, "LSR", AM_ACC, "ACCUMULATOR ( INS A )", &CPU::LSR, 1 << 2);
	operationTable[0x46] = Operation(INSTR_LSR, "LSR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::LSR, 1 << 5);
	operationTable[0x56] = Operation(INSTR_LSR, "LSR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::LSR, 1 << 6);
	operationTable[0x4E] = Operation(INSTR_LSR, "LSR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::LSR, 1 << 6);
	operationTable[0x5E] = Operation(INSTR_LSR, "LSR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::LSR, 1 << 7);
	operationTable[0xEA] = Operation(INSTR_NOP, "NOP", AM_IMP, "IMPLIED ( INS )", &CPU::NOP, 1 << 2);
	operationTable[0x09] = Operation(INSTR_ORA, "ORA", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::ORA, 1 << 2);
	operationTable[0x05] = Operation(INSTR_ORA, "ORA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::ORA, 1 << 3);
	operationTable[0x15] = Operation(INSTR_ORA, "ORA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::ORA, 1 << 4);
	operationTable[0x0D] = Operation(INSTR_ORA, "ORA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::ORA, 1 << 4);
	operationTable[0x1D] = Operation(INSTR_ORA, "ORA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::ORA, (1 << 4) + (1 << 5));
	operationTable[0x19] = Operation(INSTR_ORA, "ORA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::ORA, (1 << 4) + (1 << 5));
	operationTable[0x01] = Operation(INSTR_ORA, "ORA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::ORA, 1 << 6);
	operationTable[0x11] = Operation(INSTR_ORA, "ORA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::ORA, 1 << 5);
	operationTable[0x48] = Operation(INSTR_PHA, "PHA", AM_IMP, "IMPLIED ( INS )", &CPU::PHA, 1 << 3);
	operationTable[0x08] = Operation(INSTR_PHP, "PHP", AM_IMP, "IMPLIED ( INS )", &CPU::PHP, 1 << 3);
	operationTable[0x68] = Operation(INSTR_PLA, "PLA", AM_IMP, "IMPLIED ( INS )", &CPU::PLA, 1 << 4);
	operationTable[0x28] = Operation(INSTR_PLP, "PLP", AM_IMP, "IMPLIED ( INS )", &CPU::PLP, 1 << 4);
	operationTable[0x2A] = Operation(INSTR_ROL, "ROL", AM_ACC, "ACCUMULATOR ( INS A )", &CPU::ROL, 1 << 2);
	operationTable[0x26] = Operation(INSTR_ROL, "ROL", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::ROL, 1 << 5);
	operationTable[0x36] = Operation(INSTR_ROL, "ROL", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::ROL, 1 << 6);
	operationTable[0x2E] = Operation(INSTR_ROL, "ROL", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::ROL, 1 << 6);
	operationTable[0x3E] = Operation(INSTR_ROL, "ROL", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::ROL, 1 << 7);
	operationTable[0x6A] = Operation(INSTR_ROR, "ROR", AM_ACC, "ACCUMULATOR ( INS A )", &CPU::ROR, 1 << 2);
	operationTable[0x66] = Operation(INSTR_ROR, "ROR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::ROR, 1 << 5);
	operationTable[0x76] = Operation(INSTR_ROR, "ROR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::ROR, 1 << 6);
	operationTable[0x6E] = Operation(INSTR_ROR, "ROR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::ROR, 1 << 6);
	operationTable[0x7E] = Operation(INSTR_ROR, "ROR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::ROR, 1 << 7);
	operationTable[0x40] = Operation(INSTR_RTI, "RTI", AM_IMP, "IMPLIED ( INS )", &CPU::RTI, 1 << 6);
	operationTable[0x60] = Operation(INSTR_RTS, "RTS", AM_IMP, "IMPLIED ( INS )", &CPU::RTS, 1 << 6);
	operationTable[0xE9] = Operation(INSTR_SBC, "SBC", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU::SBC, 1 << 2);
	operationTable[0xE5] = Operation(INSTR_SBC, "SBC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::SBC, 1 << 3);
	operationTable[0xF5] = Operation(INSTR_SBC, "SBC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::SBC, 1 << 4);
	operationTable[0xED] = Operation(INSTR_SBC, "SBC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::SBC, 1 << 4);
	operationTable[0xFD] = Operation(INSTR_SBC, "SBC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::SBC, (1 << 4) + (1 << 5));
	operationTable[0xF9] = Operation(INSTR_SBC, "SBC", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::SBC, (1 << 4) + (1 << 5));
	operationTable[0xE1] = Operation(INSTR_SBC, "SBC", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::SBC, 1 << 6);
	operationTable[0xF1] = Operation(INSTR_SBC, "SBC", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::SBC, (1 << 5) + (1 << 6));
	operationTable[0x38] = Operation(INSTR_SEC, "SEC", AM_IMP, "IMPLIED ( INS )", &CPU::SEC, 1 << 2);
	operationTable[0xF8] = Operation(INSTR_SED, "SED", AM_IMP, "IMPLIED ( INS )", &CPU::SED, 1 << 2);
	operationTable[0x78] = Operation(INSTR_SEI, "SEI", AM_IMP, "IMPLIED ( INS )", &CPU::SEI, 1 << 2);
	operationTable[0x85] = Operation(INSTR_STA, "STA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::STA, 1 << 3);
	operationTable[0x95] = Operation(INSTR_STA, "STA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::STA, 1 << 4);
	operationTable[0x8D] = Operation(INSTR_STA, "STA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::STA, 1 << 4);
	operationTable[0x9D] = Operation(INSTR_STA, "STA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU::STA, 1 << 5);
	operationTable[0x99] = Operation(INSTR_STA, "STA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU::STA, 1 << 5);
	operationTable[0x81] = Operation(INSTR_STA, "STA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU::STA, 1 << 6);
	operationTable[0x91] = Operation(INSTR_STA, "STA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU::STA, 1 << 6);
	operationTable[0x86] = Operation(INSTR_STX, "STX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::STX, 1 << 3);
	operationTable[0x96] = Operation(INSTR_STX, "STX", AM_ZPY, "ZERO PAGE, Y ( INS $??,Y )", &CPU::STX, 1 << 4);
	operationTable[0x8E] = Operation(INSTR_STX, "STX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::STX, 1 << 4);
	operationTable[0x84] = Operation(INSTR_STY, "STY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU::STY, 1 << 3);
	operationTable[0x94] = Operation(INSTR_STY, "STY", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU::STY, 1 << 4);
	operationTable[0x8C] = Operation(INSTR_STY, "STY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU::STY, 1 << 4);
	operationTable[0xAA] = Operation(INSTR_TAX, "TAX", AM_IMP, "IMPLIED ( INS )", &CPU::TAX, 1 << 2);
	operationTable[0xA8] = Operation(INSTR_TAY, "TAY", AM_IMP, "IMPLIED ( INS )", &CPU::TAY, 1 << 2);
	operationTable[0xBA] = Operation(INSTR_TSX, "TSX", AM_IMP, "IMPLIED ( INS )", &CPU::TSX, 1 << 2);
	operationTable[0x8A] = Operation(INSTR_TXA, "TXA", AM_IMP, "IMPLIED ( INS )", &CPU::TXA, 1 << 2);
	operationTable[0x9A] = Operation(INSTR_TXS, "TXS", AM_IMP, "IMPLIED ( INS )", &CPU::TXS, 1 << 2);
	operationTable[0x98] = Operation(INSTR_TYA, "TYA", AM_IMP, "IMPLIED ( INS )", &CPU::TYA, 1 << 2);
}

// Interrupts
void CPU::RespondToInterrupt(bool isIRQ) {
	PC += 1;
	StackPush((PC >> 8) & 0xFF);					// 3
	StackPush(PC & 0xFF);							// 4
	PushP(false); // ??								// 5
	if (isIRQ) {
		PC = Read(0xFFFE) + (Read(0xFFFF) << 8);	// 6, 7
	}
	else { // is _NMI
		PC = Read(0xFFFA) + (Read(0xFFFB) << 8);	// 6, 7
	}
	I = 1;
}
void CPU::CheckForInterrupt() { // Check interal timings wrt _NMI taking over before _IRQ vectors are selected
	while ((_IRQ && I == 0) || _NMI) {
		if (_NMI) {
			RespondToInterrupt(false);
			break;
		}
		else if (_IRQ && I == 0) {
			RespondToInterrupt(true); // BRK ??
		}
	}
}

// Peripherals
void CPU::SetIRQ(bool stateOn) {
	if (stateOn) {
		_IRQ++;
	}
	else if (!stateOn && _IRQ > 0) {
		_IRQ--;
	}
	else {
		_IRQ = 0;
	}
}
void CPU::SetNMI(bool stateOn) {
	_NMI = stateOn;
}

// Timing
void CPU::Tick() {
	cycle++;
}

void CPU::PowerUp() {
	SetP(0x34, true);
	A = 0;
	X = 0;
	Y = 0;
	PC = Read(0xFFFC) + (Read(0xFFFD) << 8); // RESET VECTOR
	SP = 0xFD;
}
void CPU::EASY6502STARTUP() {
	SetP(0x30, true);
	A = 0;
	X = 0;
	Y = 0;
	PC = 0x600;
	SP = 0xFF;
}
void CPU::FunctionalTestStartup(std::string filename) {
	LoadFromFile(filename, 0);
	SetP(0x34, true);
	A = 0;
	X = 0;
	Y = 0;
	PC = 0x400;
	SP = 0xFF;
}
void CPU::StartCycle() {
	Tick();
	Tick();
	Tick();
	Tick();
	Tick();
	PC = Read(0xFFFC) + (Read(0xFFFD) << 8); // RESET VECTOR
	I = 1;
}
void CPU::LOADTEST(uint8_t* arr, unsigned int len) {
	for (unsigned int i=0;i<len;++i) {
		Write(0x600 + i, *(arr + i));
	}
	EASY6502STARTUP();
}
void CPU::LoadFromFile(std::string filename, uint16_t toOffset) {
	std::ifstream f;
	std::streampos size;
	char* memblock;

	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
	size = f.tellg();
	f.seekg(0, std::ios::beg);
	memblock = new char[static_cast<int>(size)];
	f.read(memblock, size);
	f.close();
	for (unsigned int i = 0; i < size; ++i) {
		Write(toOffset + i, *(memblock + i));
	}
	delete[] memblock;
}

// Running
void CPU::RunNextOpcode() {
	cycle = 0;

	uint8_t opcode = Read(PC);

	Operation op = operationTable[opcode];

	uint16_t argOffset = NULL;
	uint8_t argLen = 0;

	switch (op.addrMode) {
		case AM_IMP:	argLen = 0;	argOffset = NULL; Tick();	 																							break;
		case AM_ACC:	argLen = 0;	argOffset = NULL;																								break;
		case AM_IMM:	argLen = 1;	argOffset = PC + 1;																								break;
		case AM_ZP:		argLen = 1;	argOffset = Read(PC + 1);																						break;
		case AM_ZPX:	argLen = 1;	argOffset = ((Read(PC + 1) + X) & 0xFF);																		break;
		case AM_ZPY:	argLen = 1;	argOffset = ((Read(PC + 1) + Y) & 0xFF);																		break;
		case AM_ABS:	argLen = 2;	argOffset = (Read(PC + 1)) + ((Read(PC + 2)) << 8);																break;
		case AM_ABSX:	argLen = 2;	argOffset = (Read(PC + 1) + (Read(PC + 2) << 8)) + X; 															break;
		case AM_ABSY:	argLen = 2;	argOffset = (Read(PC + 1) + (Read(PC + 2) << 8)) + Y; 															break;
		case AM_IDXIND:	argLen = 1;	argOffset = Read(((Read(PC + 1) + X) & 0xFF)) + (Read(((Read(PC + 1) + X + 1) & 0xFF)) << 8); 					break;
		case AM_INDIDX:	argLen = 1;	argOffset = Read((Read(PC + 1))) + (Read((Read(PC + 1) + 1)) << 8) + Y; 										break; // TODO: Fix so doesn't mess with cycles
		case AM_REL:	argLen = 1;	argOffset = PC + 1;																								break;
		case AM_IND:	argLen = 2;	argOffset = Read((Read(PC + 1) + (Read(PC + 2) << 8))) + (Read((Read(PC + 1) + (Read(PC + 2) << 8)) + 1) << 8);	break;
		default:		throw std::invalid_argument("Invalid AddrMode: " + std::to_string(op.addrMode) + "(" + op.addrModeStr + ")");				break;
	}

	#ifdef _DEBUG_MODE
		log << "Opcode: 0x" << std::hex << (unsigned int)opcode;
		log << " | Instruction: " << op.instrStr;
		log << " | AddrMode: " << op.addrModeStr;
		if (op.addrMode != AM_IMP) {
			log << "\nargOffset: " << std::hex << (uintptr_t)argOffset;
			log << " | Value at argOffset: " << std::hex << (unsigned int)ReadNoTick(argOffset);
		}
		log << "\n";
	#endif

	PC += argLen + 1;
	(this->*op.Run)(argOffset, op.addrMode);
	//if (!((op.numCycles >> cycle) & 0x1)) {
	//	std::cout << cycle;
	//	throw - 1;
	//}
	CheckForInterrupt();
}
uint16_t CPU::GetPC() {
	return PC;
}

// Logging
#ifdef _DEBUG_MODE
void CPU::PrintDebugInfo() {
	if (SP != 0xFF) log << "\n ";
	for (uint8_t i = 0xFF; i>SP; i--) {
		log << (unsigned int)ReadNoTick(0x100 + i);
		log << " | ";
	}
	log << std::hex << std::setfill('0');
	log << "\nA = $" << std::setw(2) << (unsigned int)A;
	log << " X = $" << std::setw(2) << (unsigned int)X;
	log << " Y = $" << std::setw(2) << (unsigned int)Y << '\n';
	log << " SP = $" << std::setw(2) << (unsigned int)SP;
	log << " PC = $" << std::setw(4) << (unsigned int)PC << '\n';
	log << "      NV-BDIZC\n      ";
	for (int i=7;i>=0;--i) {
		log << ((P >> i) & 0x1);
	}
	log << "\n\n";
}
#endif

// Constructors
CPU::CPU(bool shouldSetupMirrors, std::string logFilename) : 
	cpuMem(shouldSetupMirrors),
	log(logFilename)
{
	SetupOperationTable();
}