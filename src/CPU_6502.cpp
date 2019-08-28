#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "CPU_6502.h"
#include "enums.h"
#include "Exceptions.h"
#include "Logger.h"
#include "LoggerUtils.h"

extern Logger logger;

// Instructions
void CPU_6502::ADC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	unsigned int temp = A + M + C;
	C = ((temp >> 8) & 0x1);
	Z = (temp & 0xFF) == 0;
	V = C ^ ((A >> 7) & 0x1) ^ ((M >> 7) & 0x1) ^ ((temp >> 7) & 0x1);
	N = (temp >> 7) & 0x1;
	A = temp;
}
void CPU_6502::AND(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A & M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::ASL(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	if (addrMode == AM_ACC) {
		temp = A << 1;
		A = (temp & 0xFF);
	}
	else {
		if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
		uint8_t M = Read(offset);
		Tick();
		temp = M << 1;
		Write(offset, (temp & 0xFF));
	}
	C = (temp >> 8) & 0x1;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU_6502::BCC(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (C == 0) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BCS(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (C == 1) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BEQ(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (Z == 1) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BIT(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	Z = !(A & M);
	V = (M >> 6) & 0x1;
	N = (M >> 7) & 0x1;
}
void CPU_6502::BMI(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (N == 1) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BNE(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (Z == 0) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BPL(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (N == 0) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BRK(uint16_t offset, AddrMode addrMode) {
	PC += 1;
	StackPush((PC >> 8) & 0xFF);
	StackPush(PC & 0xFF);
	PushP(true);
	PC = Read(0xFFFE) + (Read(0xFFFF) << 8);
	I = 1; // http://6502.org/tutorials/interrupts.html#2.2
}
void CPU_6502::BVC(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (V == 0) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::BVS(uint16_t offset, AddrMode addrMode) {
	int8_t value = static_cast<int8_t>(Read(offset));
	if (V == 1) {
		BranchAndCheckForPageCrossing(value);
	}
}
void CPU_6502::CLC(uint16_t offset, AddrMode addrMode) {
	C = 0;
}
void CPU_6502::CLD(uint16_t offset, AddrMode addrMode) {
	D = 0;
}
void CPU_6502::CLI(uint16_t offset, AddrMode addrMode) {
	I = 0;
}
void CPU_6502::CLV(uint16_t offset, AddrMode addrMode) {
	V = 0;
}
void CPU_6502::CMP(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = A >= M;
	Z = A == M;
	N = ((A - M) >> 7) & 0x1;
}
void CPU_6502::CPX(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = X >= M;
	Z = X == M;
	N = ((X - M) >> 7) & 0x1;
}
void CPU_6502::CPY(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	C = Y >= M;
	Z = Y == M;
	N = ((Y - M) >> 7) & 0x1;
}
void CPU_6502::DEC(uint16_t offset, AddrMode addrMode) {
	if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
	uint8_t M = Read(offset) - 1;
	Tick();
	Write(offset, M);
	Z = M == 0;
	N = (M >> 7) & 0x1;
}
void CPU_6502::DEX(uint16_t offset, AddrMode addrMode) {
	X = X - 1;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU_6502::DEY(uint16_t offset, AddrMode addrMode) {
	Y = Y - 1;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU_6502::EOR(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A ^ M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::INC(uint16_t offset, AddrMode addrMode) {
	if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
	uint8_t M = Read(offset) + 1;
	Tick();
	Write(offset, M);
	Z = M == 0;
	N = (M >> 7) & 0x1;
}
void CPU_6502::INX(uint16_t offset, AddrMode addrMode) {
	X = X + 1;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU_6502::INY(uint16_t offset, AddrMode addrMode) {
	Y = Y + 1;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU_6502::JMP(uint16_t offset, AddrMode addrMode) {
	PC = offset;
}
void CPU_6502::JSR(uint16_t offset, AddrMode addrMode) {
	// TODO: Potential Error: ADH should only be fetched on cycle 6
	PC -= 1;
	StackPush((PC >> 8) & 0xFF);
	StackPush(PC & 0xFF);
	Tick();
	PC = offset;
}
void CPU_6502::LDA(uint16_t offset, AddrMode addrMode) {
	A = Read(offset);
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::LDX(uint16_t offset, AddrMode addrMode) {
	X = Read(offset);
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU_6502::LDY(uint16_t offset, AddrMode addrMode) {
	Y = Read(offset);
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU_6502::LSR(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	uint8_t oldBit;
	if (addrMode == AM_ACC) {
		temp = A >> 1;
		oldBit = A & 0x1;
		A = (temp & 0xFF);
	}
	else {
		if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
		uint8_t M = Read(offset);
		Tick();
		temp = M >> 1;
		oldBit = M & 0x1;
		Write(offset, (temp & 0xFF));
	}
	C = oldBit;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU_6502::NOP(uint16_t offset, AddrMode addrMode) {
}
void CPU_6502::ORA(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	A = A | M;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::PHA(uint16_t offset, AddrMode addrMode) {
	StackPush(A);
}
void CPU_6502::PHP(uint16_t offset, AddrMode addrMode) {
	PushP(true);
}
void CPU_6502::PLA(uint16_t offset, AddrMode addrMode) {
	Tick();
	A = StackPop();
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::PLP(uint16_t offset, AddrMode addrMode) {
	Tick();
	PopToP();
}
void CPU_6502::ROL(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	if (addrMode == AM_ACC) {
		temp = A << 1;
		temp = temp | C;
		A = (temp & 0xFF);
	}
	else {
		if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
		uint8_t M = Read(offset);
		Tick();
		temp = M << 1;
		temp = temp | C;
		Write(offset, (temp & 0xFF));
	}
	C = (temp >> 8) & 0x1;
	Z = (temp & 0xFF) == 0;
	N = (temp >> 7) & 0x1;
}
void CPU_6502::ROR(uint16_t offset, AddrMode addrMode) {
	unsigned int temp;
	uint8_t oldBit;
	if (addrMode == AM_ACC) {
		temp = A >> 1;
		temp = temp | (C << 7);
		oldBit = A & 0x1;
		A = (temp & 0xFF);
	}
	else {
		if (!hasPageCrossed && addrMode == AM_ABSX) Tick();
		uint8_t M = Read(offset);
		Tick();
		temp = M >> 1;
		temp = temp | (C << 7);
		oldBit = M & 0x1;
		Write(offset, (temp & 0xFF));
	}
	C = oldBit & 0x1;
	Z = temp == 0;
	N = (temp >> 7) & 0x1;
}
void CPU_6502::RTI(uint16_t offset, AddrMode addrMode) {
	Tick();
	PopToP();						// 4
	uint8_t PC_l = StackPop();		// 5
	uint8_t PC_h = StackPop();		// 6
	PC = (PC_h << 8) + PC_l;
}
void CPU_6502::RTS(uint16_t offset, AddrMode addrMode) {
	Tick();
	uint8_t PC_l = StackPop();
	uint8_t PC_h = StackPop();
	PC = (PC_h << 8) + PC_l;
	PC += 1; Tick();
}
void CPU_6502::SBC(uint16_t offset, AddrMode addrMode) {
	uint8_t M = Read(offset);
	unsigned int temp = A + ~M + C;

	V = ((A ^ (temp & 0xFF)) & ((~M) ^ (temp & 0xFF)) & 0x80) != 0;
	A = (temp & 0xFF);
	C = !((temp >> 8) & 0x1);
	Z = A == 0;
	N = (A >> 7);
}
void CPU_6502::SEC(uint16_t offset, AddrMode addrMode) {
	C = 1;
}
void CPU_6502::SED(uint16_t offset, AddrMode addrMode) {
	D = 1;
}
void CPU_6502::SEI(uint16_t offset, AddrMode addrMode) {
	I = 1;
}
void CPU_6502::STA(uint16_t offset, AddrMode addrMode) {
	if (!hasPageCrossed
	    && (addrMode == AM_ABSX || addrMode == AM_ABSY
	        || addrMode == AM_INDIDX))
		Tick();
	Write(offset, A);
}
void CPU_6502::STX(uint16_t offset, AddrMode addrMode) {
	if ((addrMode == AM_ABSX || addrMode == AM_ABSY) && !hasPageCrossed) Tick();
	Write(offset, X);
}
void CPU_6502::STY(uint16_t offset, AddrMode addrMode) {
	if (!hasPageCrossed && (addrMode == AM_ABSX || addrMode == AM_ABSY)) Tick();
	Write(offset, Y);
}
void CPU_6502::TAX(uint16_t offset, AddrMode addrMode) {
	X = A;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU_6502::TAY(uint16_t offset, AddrMode addrMode) {
	Y = A;
	Z = Y == 0;
	N = (Y >> 7) & 0x1;
}
void CPU_6502::TSX(uint16_t offset, AddrMode addrMode) {
	X = SP;
	Z = X == 0;
	N = (X >> 7) & 0x1;
}
void CPU_6502::TXA(uint16_t offset, AddrMode addrMode) {
	A = X;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}
void CPU_6502::TXS(uint16_t offset, AddrMode addrMode) {
	SP = X;
}
void CPU_6502::TYA(uint16_t offset, AddrMode addrMode) {
	A = Y;
	Z = A == 0;
	N = (A >> 7) & 0x1;
}

// Stack Management
void CPU_6502::StackPush(uint8_t data) {
	Write(0x100 + SP, data);
	SP = (SP - 1) & 0xFF;
}
uint8_t CPU_6502::StackPop() {
	// Below is SP+1 since SP points to next free location
	uint8_t data = Read(0x100 + ((SP + 1) & 0xFF));
	SP = (SP + 1) & 0xFF;
	return data;
}

// Processor Status Management
void CPU_6502::PopToP() {
	uint8_t temp = StackPop();
	N = ((temp >> N.GetBitNum()) & 0x1);
	V = ((temp >> V.GetBitNum()) & 0x1);
	// Bit 4 and 5 are ignored for PLP and RTI
	D = ((temp >> D.GetBitNum()) & 0x1);
	I = ((temp >> I.GetBitNum()) & 0x1);
	Z = ((temp >> Z.GetBitNum()) & 0x1);
	C = ((temp >> C.GetBitNum()) & 0x1);
}
void CPU_6502::PushP(bool shouldSetBit4) {
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

// Page Crossing
void CPU_6502::BranchAndCheckForPageCrossing(int8_t relVal) {
	uint16_t newPC = PC + relVal;
	Tick();
	if ((newPC & 0xFF00) != (PC & 0xFF00)) {
		Tick();
	}
	PC = newPC;
}
uint16_t CPU_6502::AddAndCheckForPageCrossing(uint8_t lowVal, uint16_t regVal) {
	isPageCrossPossible = true;
	uint16_t addition = lowVal + regVal;
	if (addition & 0x100) {
		Tick(); // Page Boundary crossed so must waste cycle
		hasPageCrossed = true;
	}
	return addition;
}

// Interrupts
void CPU_6502::CheckForInterrupt() {
	while (IRQ || nmiFlipFlop) {
		if (nmiFlipFlop) {
			RespondToInterrupt(false);
			break;
		}
		else if (IRQ) {
			RespondToInterrupt(true); // BRK ??
		}
	}
}
void CPU_6502::RespondToInterrupt(bool isIRQ) {
	//PC += 1;
	StackPush((PC >> 8) & 0xFF);                 // 3
	StackPush(PC & 0xFF);                        // 4
	PushP(false); // ??                          // 5
	if (isIRQ) {
		PC = Read(0xFFFE) + (Read(0xFFFF) << 8); // 6, 7
	}
	else { // is _NMI
		PC = Read(0xFFFA) + (Read(0xFFFB) << 8); // 6, 7
		nmiFlipFlop = false;
	}
	I = 1;
}

// CPU Memory
uint8_t CPU_6502::Read(uint16_t offset, bool shouldTick/*= true*/) {
	if (shouldTick) Tick();
	return *(RAM + offset);
}
void CPU_6502::Write(uint16_t offset, uint8_t data, bool shouldTick/*= true*/) {
	if (shouldTick) Tick();
	*(RAM + offset) = data;
}
uint8_t CPU_6502::ReadNoTick(uint16_t offset) {
	return Read(offset, false);
}
void CPU_6502::WriteNoTick(uint16_t offset, uint8_t data) {
	Write(offset, data, false);
}

// Timing
void CPU_6502::Tick() {
	currentOpNumCycles++;
	totalCycles++;

	IRQ = irqLine.GetState() == LOW && I == 0;
	NMI = nmiLine.GetState() == LOW && prevNMIState == HIGH;
	prevNMIState = nmiLine.GetState();
	if (NMI)
		nmiFlipFlop = true;
}

// Operation Table
void CPU_6502::SetupOperationTable() {
	operationTable[0x69] = Operation(INSTR_ADC, "ADC", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::ADC, 1 << 2);
	operationTable[0x65] = Operation(INSTR_ADC, "ADC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::ADC, 1 << 3);
	operationTable[0x75] = Operation(INSTR_ADC, "ADC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::ADC, 1 << 4);
	operationTable[0x6D] = Operation(INSTR_ADC, "ADC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::ADC, 1 << 4);
	operationTable[0x7D] = Operation(INSTR_ADC, "ADC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::ADC, (1 << 4) + (1 << 5));
	operationTable[0x79] = Operation(INSTR_ADC, "ADC", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::ADC, (1 << 4) + (1 << 5));
	operationTable[0x61] = Operation(INSTR_ADC, "ADC", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::ADC, 1 << 6);
	operationTable[0x71] = Operation(INSTR_ADC, "ADC", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::ADC, (1 << 5) + (1 << 6));
	operationTable[0x29] = Operation(INSTR_AND, "AND", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::AND, 1 << 2);
	operationTable[0x25] = Operation(INSTR_AND, "AND", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::AND, 1 << 3);
	operationTable[0x35] = Operation(INSTR_AND, "AND", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::AND, 1 << 4);
	operationTable[0x2D] = Operation(INSTR_AND, "AND", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::AND, 1 << 4);
	operationTable[0x3D] = Operation(INSTR_AND, "AND", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::AND, (1 << 4) + (1 << 5));
	operationTable[0x39] = Operation(INSTR_AND, "AND", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::AND, (1 << 4) + (1 << 5));
	operationTable[0x21] = Operation(INSTR_AND, "AND", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::AND, 1 << 6);
	operationTable[0x31] = Operation(INSTR_AND, "AND", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::AND, (1 << 5) + (1 << 6));
	operationTable[0x0A] = Operation(INSTR_ASL, "ASL", AM_ACC, "ACCUMULATOR ( INS A )", &CPU_6502::ASL, 1 << 2);
	operationTable[0x06] = Operation(INSTR_ASL, "ASL", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::ASL, 1 << 5);
	operationTable[0x16] = Operation(INSTR_ASL, "ASL", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::ASL, 1 << 6);
	operationTable[0x0E] = Operation(INSTR_ASL, "ASL", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::ASL, 1 << 6);
	operationTable[0x1E] = Operation(INSTR_ASL, "ASL", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::ASL, 1 << 7);
	operationTable[0x90] = Operation(INSTR_BCC, "BCC", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BCC, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xB0] = Operation(INSTR_BCS, "BCS", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BCS, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xF0] = Operation(INSTR_BEQ, "BEQ", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BEQ, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x24] = Operation(INSTR_BIT, "BIT", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::BIT, 1 << 3);
	operationTable[0x2C] = Operation(INSTR_BIT, "BIT", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::BIT, 1 << 4);
	operationTable[0x30] = Operation(INSTR_BMI, "BMI", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BMI, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0xD0] = Operation(INSTR_BNE, "BNE", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BNE, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x10] = Operation(INSTR_BPL, "BPL", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BPL, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x00] = Operation(INSTR_BRK, "BRK", AM_IMP, "IMPLIED ( INS )", &CPU_6502::BRK, 1 << 7);
	operationTable[0x50] = Operation(INSTR_BVC, "BVC", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BVC, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x70] = Operation(INSTR_BVS, "BVS", AM_REL, "RELATIVE ( INS $???? )", &CPU_6502::BVS, (1 << 2) + (1 << 3) + (1 << 4));
	operationTable[0x18] = Operation(INSTR_CLC, "CLC", AM_IMP, "IMPLIED ( INS )", &CPU_6502::CLC, 1 << 2);
	operationTable[0xD8] = Operation(INSTR_CLD, "CLD", AM_IMP, "IMPLIED ( INS )", &CPU_6502::CLD, 1 << 2);
	operationTable[0x58] = Operation(INSTR_CLI, "CLI", AM_IMP, "IMPLIED ( INS )", &CPU_6502::CLI, 1 << 2);
	operationTable[0xB8] = Operation(INSTR_CLV, "CLV", AM_IMP, "IMPLIED ( INS )", &CPU_6502::CLV, 1 << 2);
	operationTable[0xC9] = Operation(INSTR_CMP, "CMP", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::CMP, 1 << 2);
	operationTable[0xC5] = Operation(INSTR_CMP, "CMP", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::CMP, 1 << 3);
	operationTable[0xD5] = Operation(INSTR_CMP, "CMP", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::CMP, 1 << 4);
	operationTable[0xCD] = Operation(INSTR_CMP, "CMP", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::CMP, 1 << 4);
	operationTable[0xDD] = Operation(INSTR_CMP, "CMP", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::CMP, (1 << 4) + (1 << 5));
	operationTable[0xD9] = Operation(INSTR_CMP, "CMP", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::CMP, (1 << 4) + (1 << 5));
	operationTable[0xC1] = Operation(INSTR_CMP, "CMP", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::CMP, 1 << 6);
	operationTable[0xD1] = Operation(INSTR_CMP, "CMP", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::CMP, (1 << 5) + (1 << 6));
	operationTable[0xE0] = Operation(INSTR_CPX, "CPX", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::CPX, 1 << 2);
	operationTable[0xE4] = Operation(INSTR_CPX, "CPX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::CPX, 1 << 3);
	operationTable[0xEC] = Operation(INSTR_CPX, "CPX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::CPX, 1 << 4);
	operationTable[0xC0] = Operation(INSTR_CPY, "CPY", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::CPY, 1 << 2);
	operationTable[0xC4] = Operation(INSTR_CPY, "CPY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::CPY, 1 << 3);
	operationTable[0xCC] = Operation(INSTR_CPY, "CPY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::CPY, 1 << 4);
	operationTable[0xC6] = Operation(INSTR_DEC, "DEC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::DEC, 1 << 5);
	operationTable[0xD6] = Operation(INSTR_DEC, "DEC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::DEC, 1 << 6);
	operationTable[0xCE] = Operation(INSTR_DEC, "DEC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::DEC, 1 << 6);
	operationTable[0xDE] = Operation(INSTR_DEC, "DEC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::DEC, 1 << 7);
	operationTable[0xCA] = Operation(INSTR_DEX, "DEX", AM_IMP, "IMPLIED ( INS )", &CPU_6502::DEX, 1 << 2);
	operationTable[0x88] = Operation(INSTR_DEY, "DEY", AM_IMP, "IMPLIED ( INS )", &CPU_6502::DEY, 1 << 2);
	operationTable[0x49] = Operation(INSTR_EOR, "EOR", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::EOR, 1 << 2);
	operationTable[0x45] = Operation(INSTR_EOR, "EOR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::EOR, 1 << 3);
	operationTable[0x55] = Operation(INSTR_EOR, "EOR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::EOR, 1 << 4);
	operationTable[0x4D] = Operation(INSTR_EOR, "EOR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::EOR, 1 << 4);
	operationTable[0x5D] = Operation(INSTR_EOR, "EOR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::EOR, (1 << 4) + (1 << 5));
	operationTable[0x59] = Operation(INSTR_EOR, "EOR", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::EOR, (1 << 4) + (1 << 5));
	operationTable[0x41] = Operation(INSTR_EOR, "EOR", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::EOR, 1 << 6);
	operationTable[0x51] = Operation(INSTR_EOR, "EOR", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::EOR, (1 << 5) + (1 << 6));
	operationTable[0xE6] = Operation(INSTR_INC, "INC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::INC, 1 << 5);
	operationTable[0xF6] = Operation(INSTR_INC, "INC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::INC, 1 << 6);
	operationTable[0xEE] = Operation(INSTR_INC, "INC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::INC, 1 << 6);
	operationTable[0xFE] = Operation(INSTR_INC, "INC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::INC, 1 << 7);
	operationTable[0xE8] = Operation(INSTR_INX, "INX", AM_IMP, "IMPLIED ( INS )", &CPU_6502::INX, 1 << 2);
	operationTable[0xC8] = Operation(INSTR_INY, "INY", AM_IMP, "IMPLIED ( INS )", &CPU_6502::INY, 1 << 2);
	operationTable[0x4C] = Operation(INSTR_JMP, "JMP", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::JMP, 1 << 3);
	operationTable[0x6C] = Operation(INSTR_JMP, "JMP", AM_IND, "INDIRECT", &CPU_6502::JMP, 1 << 5);
	operationTable[0x20] = Operation(INSTR_JSR, "JSR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::JSR, 1 << 6);
	operationTable[0xA9] = Operation(INSTR_LDA, "LDA", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::LDA, 1 << 2);
	operationTable[0xA5] = Operation(INSTR_LDA, "LDA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::LDA, 1 << 3);
	operationTable[0xB5] = Operation(INSTR_LDA, "LDA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::LDA, 1 << 4);
	operationTable[0xAD] = Operation(INSTR_LDA, "LDA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::LDA, 1 << 4);
	operationTable[0xBD] = Operation(INSTR_LDA, "LDA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::LDA, (1 << 4) + (1 << 5));
	operationTable[0xB9] = Operation(INSTR_LDA, "LDA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::LDA, (1 << 4) + (1 << 5));
	operationTable[0xA1] = Operation(INSTR_LDA, "LDA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::LDA, 1 << 6);
	operationTable[0xB1] = Operation(INSTR_LDA, "LDA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::LDA, (1 << 5) + (1 << 6));
	operationTable[0xA2] = Operation(INSTR_LDX, "LDX", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::LDX, 1 << 2);
	operationTable[0xA6] = Operation(INSTR_LDX, "LDX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::LDX, 1 << 3);
	operationTable[0xB6] = Operation(INSTR_LDX, "LDX", AM_ZPY, "ZERO PAGE, Y ( INS $??,Y )", &CPU_6502::LDX, 1 << 4);
	operationTable[0xAE] = Operation(INSTR_LDX, "LDX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::LDX, 1 << 4);
	operationTable[0xBE] = Operation(INSTR_LDX, "LDX", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::LDX, (1 << 4) + (1 << 5));
	operationTable[0xA0] = Operation(INSTR_LDY, "LDY", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::LDY, 1 << 2);
	operationTable[0xA4] = Operation(INSTR_LDY, "LDY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::LDY, 1 << 3);
	operationTable[0xB4] = Operation(INSTR_LDY, "LDY", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::LDY, 1 << 4);
	operationTable[0xAC] = Operation(INSTR_LDY, "LDY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::LDY, 1 << 4);
	operationTable[0xBC] = Operation(INSTR_LDY, "LDY", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::LDY, (1 << 4) + (1 << 5));
	operationTable[0x4A] = Operation(INSTR_LSR, "LSR", AM_ACC, "ACCUMULATOR ( INS A )", &CPU_6502::LSR, 1 << 2);
	operationTable[0x46] = Operation(INSTR_LSR, "LSR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::LSR, 1 << 5);
	operationTable[0x56] = Operation(INSTR_LSR, "LSR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::LSR, 1 << 6);
	operationTable[0x4E] = Operation(INSTR_LSR, "LSR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::LSR, 1 << 6);
	operationTable[0x5E] = Operation(INSTR_LSR, "LSR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::LSR, 1 << 7);
	operationTable[0xEA] = Operation(INSTR_NOP, "NOP", AM_IMP, "IMPLIED ( INS )", &CPU_6502::NOP, 1 << 2);
	operationTable[0x09] = Operation(INSTR_ORA, "ORA", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::ORA, 1 << 2);
	operationTable[0x05] = Operation(INSTR_ORA, "ORA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::ORA, 1 << 3);
	operationTable[0x15] = Operation(INSTR_ORA, "ORA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::ORA, 1 << 4);
	operationTable[0x0D] = Operation(INSTR_ORA, "ORA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::ORA, 1 << 4);
	operationTable[0x1D] = Operation(INSTR_ORA, "ORA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::ORA, (1 << 4) + (1 << 5));
	operationTable[0x19] = Operation(INSTR_ORA, "ORA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::ORA, (1 << 4) + (1 << 5));
	operationTable[0x01] = Operation(INSTR_ORA, "ORA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::ORA, 1 << 6);
	operationTable[0x11] = Operation(INSTR_ORA, "ORA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::ORA, (1 << 5) + (1 << 6));
	operationTable[0x48] = Operation(INSTR_PHA, "PHA", AM_IMP, "IMPLIED ( INS )", &CPU_6502::PHA, 1 << 3);
	operationTable[0x08] = Operation(INSTR_PHP, "PHP", AM_IMP, "IMPLIED ( INS )", &CPU_6502::PHP, 1 << 3);
	operationTable[0x68] = Operation(INSTR_PLA, "PLA", AM_IMP, "IMPLIED ( INS )", &CPU_6502::PLA, 1 << 4);
	operationTable[0x28] = Operation(INSTR_PLP, "PLP", AM_IMP, "IMPLIED ( INS )", &CPU_6502::PLP, 1 << 4);
	operationTable[0x2A] = Operation(INSTR_ROL, "ROL", AM_ACC, "ACCUMULATOR ( INS A )", &CPU_6502::ROL, 1 << 2);
	operationTable[0x26] = Operation(INSTR_ROL, "ROL", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::ROL, 1 << 5);
	operationTable[0x36] = Operation(INSTR_ROL, "ROL", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::ROL, 1 << 6);
	operationTable[0x2E] = Operation(INSTR_ROL, "ROL", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::ROL, 1 << 6);
	operationTable[0x3E] = Operation(INSTR_ROL, "ROL", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::ROL, 1 << 7);
	operationTable[0x6A] = Operation(INSTR_ROR, "ROR", AM_ACC, "ACCUMULATOR ( INS A )", &CPU_6502::ROR, 1 << 2);
	operationTable[0x66] = Operation(INSTR_ROR, "ROR", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::ROR, 1 << 5);
	operationTable[0x76] = Operation(INSTR_ROR, "ROR", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::ROR, 1 << 6);
	operationTable[0x6E] = Operation(INSTR_ROR, "ROR", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::ROR, 1 << 6);
	operationTable[0x7E] = Operation(INSTR_ROR, "ROR", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::ROR, 1 << 7);
	operationTable[0x40] = Operation(INSTR_RTI, "RTI", AM_IMP, "IMPLIED ( INS )", &CPU_6502::RTI, 1 << 6);
	operationTable[0x60] = Operation(INSTR_RTS, "RTS", AM_IMP, "IMPLIED ( INS )", &CPU_6502::RTS, 1 << 6);
	operationTable[0xE9] = Operation(INSTR_SBC, "SBC", AM_IMM, "IMMEDIATE ( INS #?? )", &CPU_6502::SBC, 1 << 2);
	operationTable[0xE5] = Operation(INSTR_SBC, "SBC", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::SBC, 1 << 3);
	operationTable[0xF5] = Operation(INSTR_SBC, "SBC", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::SBC, 1 << 4);
	operationTable[0xED] = Operation(INSTR_SBC, "SBC", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::SBC, 1 << 4);
	operationTable[0xFD] = Operation(INSTR_SBC, "SBC", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::SBC, (1 << 4) + (1 << 5));
	operationTable[0xF9] = Operation(INSTR_SBC, "SBC", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::SBC, (1 << 4) + (1 << 5));
	operationTable[0xE1] = Operation(INSTR_SBC, "SBC", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::SBC, 1 << 6);
	operationTable[0xF1] = Operation(INSTR_SBC, "SBC", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::SBC, (1 << 5) + (1 << 6));
	operationTable[0x38] = Operation(INSTR_SEC, "SEC", AM_IMP, "IMPLIED ( INS )", &CPU_6502::SEC, 1 << 2);
	operationTable[0xF8] = Operation(INSTR_SED, "SED", AM_IMP, "IMPLIED ( INS )", &CPU_6502::SED, 1 << 2);
	operationTable[0x78] = Operation(INSTR_SEI, "SEI", AM_IMP, "IMPLIED ( INS )", &CPU_6502::SEI, 1 << 2);
	operationTable[0x85] = Operation(INSTR_STA, "STA", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::STA, 1 << 3);
	operationTable[0x95] = Operation(INSTR_STA, "STA", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::STA, 1 << 4);
	operationTable[0x8D] = Operation(INSTR_STA, "STA", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::STA, 1 << 4);
	operationTable[0x9D] = Operation(INSTR_STA, "STA", AM_ABSX, "ABSOLUTE, X ( INS $????,X )", &CPU_6502::STA, 1 << 5);
	operationTable[0x99] = Operation(INSTR_STA, "STA", AM_ABSY, "ABSOLUTE, Y ( INS $????,Y )", &CPU_6502::STA, 1 << 5);
	operationTable[0x81] = Operation(INSTR_STA, "STA", AM_IDXIND, "INDEXED INDIRECT ( INS ($??,X) )", &CPU_6502::STA, 1 << 6);
	operationTable[0x91] = Operation(INSTR_STA, "STA", AM_INDIDX, "INDIRECT INDEXED ( INS ($??),Y )", &CPU_6502::STA, 1 << 6);
	operationTable[0x86] = Operation(INSTR_STX, "STX", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::STX, 1 << 3);
	operationTable[0x96] = Operation(INSTR_STX, "STX", AM_ZPY, "ZERO PAGE, Y ( INS $??,Y )", &CPU_6502::STX, 1 << 4);
	operationTable[0x8E] = Operation(INSTR_STX, "STX", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::STX, 1 << 4);
	operationTable[0x84] = Operation(INSTR_STY, "STY", AM_ZP, "ZERO PAGE ( INS $?? )", &CPU_6502::STY, 1 << 3);
	operationTable[0x94] = Operation(INSTR_STY, "STY", AM_ZPX, "ZERO PAGE, X ( INS $??,X )", &CPU_6502::STY, 1 << 4);
	operationTable[0x8C] = Operation(INSTR_STY, "STY", AM_ABS, "ABSOLUTE ( INS $???? )", &CPU_6502::STY, 1 << 4);
	operationTable[0xAA] = Operation(INSTR_TAX, "TAX", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TAX, 1 << 2);
	operationTable[0xA8] = Operation(INSTR_TAY, "TAY", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TAY, 1 << 2);
	operationTable[0xBA] = Operation(INSTR_TSX, "TSX", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TSX, 1 << 2);
	operationTable[0x8A] = Operation(INSTR_TXA, "TXA", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TXA, 1 << 2);
	operationTable[0x9A] = Operation(INSTR_TXS, "TXS", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TXS, 1 << 2);
	operationTable[0x98] = Operation(INSTR_TYA, "TYA", AM_IMP, "IMPLIED ( INS )", &CPU_6502::TYA, 1 << 2);
}

// Peripheral
uint8_t CPU_6502::PeripheralRead(uint16_t addr) {
	return ReadNoTick(addr);
}
void CPU_6502::PeripheralWrite(uint16_t addr, uint8_t data) {
	WriteNoTick(addr, data);
}
void CPU_6502::AddIRQConnection(PeripheralConnection* irqConnection) {
	irqLine.AddConnection(irqConnection);
}
void CPU_6502::RemoveIRQConnection(PeripheralConnection* irqConnection) {
	irqLine.RemoveConnection(irqConnection);
}
void CPU_6502::AddNMIConnection(PeripheralConnection* nmiConnection) {
	nmiLine.AddConnection(nmiConnection);
}
void CPU_6502::RemoveNMIConnection(PeripheralConnection* nmiConnection) {
	nmiLine.RemoveConnection(nmiConnection);
}

// Initialization
void CPU_6502::EASY6502STARTUP() {
	SetP(0x30);
	A = 0;
	X = 0;
	Y = 0;
	PC = 0x600;
	SP = 0xFF;
}
void CPU_6502::LOADTEST(uint8_t* arr, unsigned int len) {
	for (unsigned int i=0;i<len;++i) {
		Write(0x600 + i, *(arr + i));
	}
	EASY6502STARTUP();
}
void CPU_6502::LoadFromFile(std::string filename, uint16_t toOffset) {
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
		WriteNoTick(toOffset + i, *(memblock + i));
	}
	delete[] memblock;
}

// Running
void CPU_6502::RunNextOpcode() {
	CheckForInterrupt();
	//PrintDebugInfoMesenBasic();

	currentOpNumCycles = 0;
	hasPageCrossed = false;
	isPageCrossPossible = false;
	uint8_t opcode = Read(PC);

	Operation& op = operationTable[opcode];

	uint16_t argOffset = 0;
	uint8_t argLen = 0;

	switch (op.addrMode) {
		case AM_IMP: {
			argLen = 0;
			argOffset = 0;
			Tick();
			break;
		}
		case AM_ACC: {
			argLen = 0;
			argOffset = 0;
			Tick();
			break;
		}
		case AM_IMM: {
			argLen = 1;
			argOffset = PC + 1;
			break;
		}
		case AM_ZP: {
			argLen = 1;
			argOffset = Read(PC + 1);
			break;
		}
		case AM_ZPX: {
			argLen = 1;
			argOffset = ((Read(PC + 1) + X) & 0xFF);
			Tick();
			break;
		}
		case AM_ZPY: {
			argLen = 1;
			argOffset = ((Read(PC + 1) + Y) & 0xFF);
			Tick();
			break;
		}
		case AM_ABS: {
			argLen = 2;
			argOffset = Read(PC + 1);
			argOffset += Read(PC + 2) << 8;
			break;
		}
		case AM_ABSX: {
			argLen = 2;
			argOffset = AddAndCheckForPageCrossing(Read(PC + 1), X);
			argOffset += Read(PC + 2) << 8;
			break;
		}
		case AM_ABSY: {
			argLen = 2;
			argOffset = AddAndCheckForPageCrossing(Read(PC + 1), Y);
			argOffset += Read(PC + 2) << 8;
			break;
		}
		case AM_IDXIND: {
			argLen = 1;
			uint8_t BAL = Read(PC + 1);
			Tick();
			argOffset = Read(((BAL + X) & 0xFF));
			argOffset += Read(((BAL + X + 1) & 0xFF)) << 8;
			break;
		}
		case AM_INDIDX: {
			argLen = 1;
			uint8_t IAL = Read(PC + 1);
			argOffset = AddAndCheckForPageCrossing(Read(IAL), Y);
			argOffset += Read((IAL + 1) & 0xFF) << 8;
 			break;
		}
		case AM_REL: {
			argLen = 1;
			argOffset = PC + 1;
			break;
		}
		// http://forums.nesdev.com/viewtopic.php?t=5388
		case AM_IND: {
			argLen = 2;
			uint8_t IAL = Read(PC + 1);
			uint8_t IAH = Read(PC + 2);
			argOffset = Read(((IAH << 8) + IAL));
			argOffset += Read((IAH << 8) + ((IAL + 1) & 0xFF)) << 8;
			break;
		 }
		default: {
			throw std::invalid_argument("Invalid AddrMode: "
			                            + std::to_string(op.addrMode)
			                            + " (" + op.addrModeStr + ")");
		 }
	}

	// Update values for logging
	prevPC = PC;
	prevOp = op;
	prevArgOffset = argOffset;

	totalNumInstrs++;
	PC += argLen + 1;
	(this->*op.Run)(argOffset, op.addrMode);
	if (!((op.numCycles >> currentOpNumCycles) & 0x1)) {
		throw BadInstrTimingException();
	}
}
uint16_t CPU_6502::GetPC() {
	return PC;
}
int CPU_6502::GetTotalNumInstrs() {
	return totalNumInstrs;
}

// Debugging
void CPU_6502::SetP(uint8_t val) {
	N = ((val >> N.GetBitNum()) & 0x1);
	V = ((val >> V.GetBitNum()) & 0x1);
	// Bit 4 and 5 are ignored
	D = ((val >> D.GetBitNum()) & 0x1);
	I = ((val >> I.GetBitNum()) & 0x1);
	Z = ((val >> Z.GetBitNum()) & 0x1);
	C = ((val >> C.GetBitNum()) & 0x1);
}

void CPU_6502::SetPC(uint16_t _PC) {
	PC = _PC;
}
void CPU_6502::SetI() {
	I = 1;
}
uint8_t CPU_6502::GetI() {
	return I;
}
void CPU_6502::SetSP(uint8_t val) {
	SP = val;
}
uint8_t CPU_6502::GetSP() {
	return SP;
}
void CPU_6502::SetA(uint8_t val) {
	A = val;
}
void CPU_6502::SetX(uint8_t val) {
	X = val;
}
void CPU_6502::SetY(uint8_t val) {
	Y = val;
}
int CPU_6502::GetTotalCycles() {
	return totalCycles;
}

// Logging
std::string CPU_6502::DisassembleInstr() {
	uint8_t opcode = ReadNoTick(PC);
	Operation& op = operationTable[opcode];
	std::string dis = op.addrModeStr;

	size_t locPar = dis.find("(");
	bool isRelative = dis.find("RELATIVE") != std::string::npos;
	if (locPar != std::string::npos) {
		// Only keep what's in brackets
		dis.erase(0, locPar + 2); // "xxxx ( "
		dis.erase(dis.size() - 2, 2); // " )"

		// Replace INS with op.instrStr
		size_t locINS = dis.find("INS");
		dis.replace(locINS, 3, op.instrStr);
		if (!isRelative) {
			size_t locHash = dis.find('#');
			if (locHash != std::string::npos) dis.insert(locHash + 1, "$");
			int numBytes = std::count(dis.begin(), dis.end(), '?') / 2;
			for (int i = numBytes; i > 0; i--) {
				uint8_t byte = ReadNoTick(PC + i);
				std::string byteStr = LoggerUtils::ToHexStr(byte);
				size_t locByte = dis.find("??");
				dis.replace(locByte, 2, byteStr);
			}
		}
		else {
			uint16_t finalDest = PC + (int8_t)(ReadNoTick(PC + 1)) + 2;
			std::string byteStr = LoggerUtils::ToHexStr(finalDest);
			size_t locByte = dis.find("????");
			dis.replace(locByte, 4, byteStr);
		}
		return dis;
	}
	else {
		return op.instrStr;
	}
}
void CPU_6502::PrintDebugInfoMesenBasic() {
	// [PC, h] [Disassembly][Align, 20] A:[A, h] X : [X, h] Y : [Y, h] P : [P, h] SP : [SP, h]
	logger << std::hex << std::uppercase << std::setfill('0');

	logger << std::setw(4) << (unsigned int)PC;
	logger << " ";
	logger << std::setfill(' ') << std::left
	       << std::setw(15) << DisassembleInstr();
	logger << std::setfill('0') << std::right;

	//logger << std::setw(4) << prevOp.instrStr << " ";
	//logger << std::setw(4) << prevArgOffset;

	logger << " A:" << std::setw(2) << (unsigned int)A;
	logger << " X:" << std::setw(2) << (unsigned int)X;
	logger << " Y:" << std::setw(2) << (unsigned int)Y;
	logger << " P:" << std::setw(2) << (unsigned int)P;
	logger << " SP:" << std::setw(2) << (unsigned int)SP;
	logger << '\n';
	logger.flush();
}

void CPU_6502::PrintDebugInfo() {
	logger << std::hex << std::uppercase << std::setfill('0');
	logger << std::setw(4) << (unsigned int)PC;
	logger << " A:" << std::setw(2) << (unsigned int)A;
	logger << " X:" << std::setw(2) << (unsigned int)X;
	logger << " Y:" << std::setw(2) << (unsigned int)Y;
	logger << " P:" << std::setw(2) << (unsigned int)P;
	logger << " SP:" << std::setw(2) << (unsigned int)SP;
	logger << '\n';
	/*
	if (SP != 0xFF) logger << "\n ";
	for (uint8_t i = 0xFF; i>SP; i--) {
		logger << (unsigned int)ReadNoTick(0x100 + i);
		logger << " | ";
	}
	logger << std::hex << std::setfill('0');
	logger << "\nA = $" << std::setw(2) << (unsigned int)A;
	logger << " X = $" << std::setw(2) << (unsigned int)X;
	logger << " Y = $" << std::setw(2) << (unsigned int)Y << '\n';
	logger << " SP = $" << std::setw(2) << (unsigned int)SP;
	logger << " PC = $" << std::setw(4) << (unsigned int)PC << '\n';
	logger << "      NV-BDIZC\n      ";
	for (int i=7;i>=0;--i) {
		logger << ((P >> i) & 0x1);
	}
	logger << "\n\n";
	*/
}

// Constructor
CPU_6502::CPU_6502(int sizeOfRam/*= 0x10000*/) {
	SetupOperationTable();
	RAM = new uint8_t[sizeOfRam]();
}
CPU_6502::~CPU_6502() {
	delete[] RAM;
}
