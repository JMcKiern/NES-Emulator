#include <iostream>
#include <iomanip>
#include "CPU.h"
#include "enums.h"

// Instructions
void CPU::ADC(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	unsigned int temp = GetA() + M + GetC();
	SetC(temp & 0x100);
	SetZ(temp == 0);
	SetV(GetC() ^ ((GetA() >> 7) & 0x1) ^ ((M >> 7) & 0x1) ^ ((temp >> 7) & 0x1));
	SetN((temp >> 7) & 0x1);
	SetA(temp);
}
void CPU::AND(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	SetA(GetA() & M);
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::ASL(uint8_t* ptr) {
	uint8_t A_or_M = Read(ptr);
	unsigned int temp = A_or_M << 1;
	SetC((A_or_M >> 7) & 0x1);
	SetZ(temp == 0);
	SetN(temp & 0x1 << 7);
	Write((temp & 0xFF), ptr);
}
void CPU::BCC(uint8_t* ptr) {
	if (GetC() == 0) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BCS(uint8_t* ptr) {
	if (GetC() == 1) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BEQ(uint8_t* ptr) {
	if (GetZ() == 1) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BIT(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	SetZ(!(GetA() & M));
	SetV((M >> 6) & 0x1);
	SetN((M >> 7) & 0x1);
}
void CPU::BMI(uint8_t* ptr) {
	if (GetN() == 1) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BNE(uint8_t* ptr) {
	if (GetZ() == 0) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BPL(uint8_t* ptr) {
	if (GetN() == 0) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BRK(uint8_t* ptr) { // TODO: Check if correct
	StackPush((GetPC() >> 8) & 0xFF);
	StackPush(GetPC() & 0xFF);
	StackPush(GetP());
	SetPC(*(mem + 0xFFFE) + (*(mem + 0xFFFF) << 8));
	SetB(1);
}
void CPU::BVC(uint8_t* ptr) {
	if (GetV() == 0) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::BVS(uint8_t* ptr) {
	if (GetV() == 1) SetPC(GetPC() + static_cast<int8_t>(Read(ptr)));
}
void CPU::CLC(uint8_t* ptr) {
	SetC(0);
}
void CPU::CLD(uint8_t* ptr) {
	SetD(0);
}
void CPU::CLI(uint8_t* ptr) {
	SetI(0);
}
void CPU::CLV(uint8_t* ptr) {
	SetV(0);
}
void CPU::CMP(uint8_t* ptr) {
	uint8_t M = Read(ptr); // TODO: Check if this will work for subtraction
	SetC(GetA() >= M);
	SetZ(GetA() == M);
	SetN((GetA() - M >> 7) & 0x1);
}
void CPU::CPX(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	SetC(GetX() >= M);
	SetZ(GetX() == M);
	SetN((GetX() - M >> 7) & 0x1);
}
void CPU::CPY(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	SetC(GetY() >= M);
	SetZ(GetY() == M);
	SetN((GetY() - M >> 7) & 0x1);
}
void CPU::DEC(uint8_t* ptr) {
	Write(Read(ptr) - 1, ptr);
	SetZ(Read(ptr) == 0);
	SetN((Read(ptr) >> 7) & 0x1);
}
void CPU::DEX(uint8_t* ptr) {
	SetX(GetX() - 1);
	SetZ(GetX() == 0);
	SetN((GetX() >> 7) & 0x1);
}
void CPU::DEY(uint8_t* ptr) {
	SetY(GetY() - 1);
	SetZ(GetY() == 0);
	SetN((GetY() >> 7) & 0x1);
}
void CPU::EOR(uint8_t* ptr) {
	uint8_t M = Read(ptr);
	SetA(GetA() ^ M);
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::INC(uint8_t* ptr) {
	Write(Read(ptr) + 1, ptr);
	SetZ(Read(ptr) == 0);
	SetN((Read(ptr) >> 7) & 0x1);
}
void CPU::INX(uint8_t* ptr) {
	SetX(GetX() + 1);
	SetZ(GetX() == 0);
	SetN((GetX() >> 7) & 0x1);
}
void CPU::INY(uint8_t* ptr) {
	SetY(GetY() + 1);
	SetZ(GetY() == 0);
	SetN((GetY() >> 7) & 0x1);
}
void CPU::JMP(uint8_t* ptr) {
	SetPC((ptr - mem) & 0xFFFF);
}
void CPU::JSR(uint8_t* ptr) {
	StackPush((GetPC() >> 8) & 0xFF);
	StackPush(GetPC() & 0xFF);
	SetPC((ptr - mem - 1) & 0xFFFF);
}
void CPU::LDA(uint8_t* ptr) {
	SetA(Read(ptr));
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::LDX(uint8_t* ptr) {
	SetX(Read(ptr));
	SetZ(GetX() == 0);
	SetN((GetX() >> 7) & 0x1);
}
void CPU::LDY(uint8_t* ptr) {
	SetY(Read(ptr));
	SetZ(GetY() == 0);
	SetN((GetY() >> 7) & 0x1);
}
void CPU::LSR(uint8_t* ptr) {
	uint8_t A_or_M = Read(ptr);
	unsigned int temp = A_or_M >> 1;
	SetC(A_or_M & 0x1);
	SetZ(temp == 0);
	SetN((temp >> 7) & 0x1);
	Write((temp & 0xFF), ptr);
}
void CPU::NOP(uint8_t* ptr) {
}
void CPU::ORA(uint8_t* ptr) {
	uint8_t M = *ptr;
	SetA(GetA() | M);
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::PHA(uint8_t* ptr) {
	StackPush(GetA());
}
void CPU::PHP(uint8_t* ptr) {
	StackPush(GetP());
}
void CPU::PLA(uint8_t* ptr) {
	SetA(StackPop());
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::PLP(uint8_t* ptr) {
	SetP(StackPop());
}
void CPU::ROL(uint8_t* ptr) {
	uint8_t A_or_M = *ptr;
	unsigned int temp = A_or_M << 1;
	temp = temp | GetC();
	SetC((A_or_M >> 7) & 0x1);
	SetZ(GetA() == 0);
	SetN((temp >> 7) & 0x1);
	 Write((temp & 0xFF), ptr);
}
void CPU::ROR(uint8_t* ptr) {
	uint8_t A_or_M = *ptr;
	unsigned int temp = A_or_M >> 1;
	temp = temp | (GetC() << 7);
	SetC(A_or_M & 0x1);
	SetZ(GetA() == 0);
	SetN((temp >> 7) & 0x1);
	Write((temp & 0xFF), ptr);
}
void CPU::RTI(uint8_t* ptr) {
	SetP(StackPop());
	uint8_t PC_l = StackPop();
	uint8_t PC_h = StackPop();
	SetPC((PC_h << 8) + PC_l);
}
void CPU::RTS(uint8_t* ptr) {
	uint8_t PC_l = StackPop();
	uint8_t PC_h = StackPop();
	SetPC((PC_h << 8) + PC_l);
}
void CPU::SBC(uint8_t* ptr) { // TODO
}
void CPU::SEC(uint8_t* ptr) {
	SetC(1);
}
void CPU::SED(uint8_t* ptr) {
	SetD(1);
}
void CPU::SEI(uint8_t* ptr) {
	SetI(1);
}
void CPU::STA(uint8_t* ptr) {
	Write(GetA(), ptr);
}
void CPU::STX(uint8_t* ptr) {
	Write(GetX(), ptr);
}
void CPU::STY(uint8_t* ptr) {
	Write(GetY(), ptr);
}
void CPU::TAX(uint8_t* ptr) {
	SetX(GetA());
	SetZ(GetX() == 0);
	SetN((GetX() >> 7) & 0x1);
}
void CPU::TAY(uint8_t* ptr) {
	SetY(GetA());
	SetZ(GetY() == 0);
	SetN((GetY() >> 7) & 0x1);
}
void CPU::TSX(uint8_t* ptr) {
	SetX(GetSP());
	SetZ(GetX() == 0);
	SetN((GetX() >> 7) & 0x1);
}
void CPU::TXA(uint8_t* ptr) {
	SetA(GetX());
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}
void CPU::TXS(uint8_t* ptr) {
	SetSP(GetX());
}
void CPU::TYA(uint8_t* ptr) {
	SetA(GetY());
	SetZ(GetA() == 0);
	SetN((GetA() >> 7) & 0x1);
}

void CPU::RunOpcode(uint8_t opcode, uint8_t* absAddrPC) {
	Operation op = operationTable.GetOperation(opcode);

	uint8_t* argPtr = nullptr;
	uint8_t argLen = 0;

	std::cout << "Opcode: " << std::hex << (unsigned int)opcode
			  << " AddrMode: " << std::hex << (unsigned int)op.addrMode 
			  << " Instr: " << std::hex << (unsigned int)op.instr << '\n';

	switch (op.addrMode) {
		case AM_IMP:	argLen = 0;	numCycles = ; argPtr = nullptr;																															break;
		case AM_ACC:	argLen = 0;	numCycles = ; argPtr = GetAAddr();																																break;
		case AM_IMM:	argLen = 1;	numCycles = 2; argPtr = (absAddrPC + 1);																													break;
		case AM_ZP:		argLen = 1;	numCycles = ; argPtr = mem + *(absAddrPC + 1);																											break;
		case AM_ZPX:	argLen = 1;	numCycles = ; argPtr = mem + ((*(absAddrPC + 1) + GetX()) & 0xFF);																								break;
		case AM_ZPY:	argLen = 1;	numCycles = ; argPtr = mem + ((*(absAddrPC + 1) + GetY()) & 0xFF);																								break;
		case AM_ABS:	argLen = 2;	numCycles = ; argPtr = mem + (*(absAddrPC + 1)) + ((*(absAddrPC + 2)) << 8);																				break;
		case AM_ABSX:	argLen = 2;	numCycles = ; argPtr = mem + (*(absAddrPC + 1) + (*(absAddrPC + 2) << 8)) + GetX(); 																			break;
		case AM_ABSY:	argLen = 2;	numCycles = ; argPtr = mem + (*(absAddrPC + 1) + (*(absAddrPC + 2) << 8)) + GetY(); 																			break;
		case AM_IDXIND:	argLen = 1;	numCycles = ; argPtr = mem + *(mem + ((*(absAddrPC + 1) + GetX()) & 0xFF) ) + (*(mem + ((*(absAddrPC + 1) + GetX() + 1) & 0xFF) ) << 8); 							break;
		case AM_INDIDX:	argLen = 1;	numCycles = ; argPtr = mem + *(mem + (*(absAddrPC + 1))) + (*(mem + (*(absAddrPC + 1) + 1)) << 8) + GetY(); 													break;
		case AM_REL:	argLen = 1;	numCycles = ; argPtr = absAddrPC + 1;																														break;
		case AM_IND:	argLen = 2;	numCycles = ; argPtr = mem + *(mem + (*(absAddrPC + 1) + (*(absAddrPC + 2) << 8))) + (*(mem + (*(absAddrPC + 1) + (*(absAddrPC + 2) << 8)) + 1) << 8);	break;
		default: std::cerr << "Addressing Mode " << op.addrMode << " not yet implemented\n"; 																					break;
	}
	SetPC(GetPC() + argLen);
	std::cout << "argPtr: " << std::hex << (uintptr_t)argPtr << '\n'; 
	std::cout << "Performing Instruction: ";
	switch (op.instr) {
		case INSTR_ADC:	std::cout << "ADC" << '\n'; ADC(argPtr);  break;
		case INSTR_AND: std::cout << "AND" << '\n'; AND(argPtr);  break;
		case INSTR_ASL: std::cout << "ASL" << '\n'; ASL(argPtr);  break;
		case INSTR_BCC: std::cout << "BCC" << '\n'; BCC(argPtr);  break;
		case INSTR_BCS: std::cout << "BCS" << '\n'; BCS(argPtr);  break;
		case INSTR_BEQ: std::cout << "BEQ" << '\n'; BEQ(argPtr);  break;
		case INSTR_BIT: std::cout << "BIT" << '\n'; BIT(argPtr);  break;
		case INSTR_BMI: std::cout << "BMI" << '\n'; BMI(argPtr);  break;
		case INSTR_BNE: std::cout << "BNE" << '\n'; BNE(argPtr);  break;
		case INSTR_BPL: std::cout << "BPL" << '\n'; BPL(argPtr);  break;
		case INSTR_BRK: std::cout << "BRK" << '\n'; BRK(argPtr);  break;
		case INSTR_BVC: std::cout << "BVC" << '\n'; BVC(argPtr);  break;
		case INSTR_BVS: std::cout << "BVS" << '\n'; BVS(argPtr);  break;
		case INSTR_CLC: std::cout << "CLC" << '\n'; CLC(argPtr);  break;
		case INSTR_CLD: std::cout << "CLD" << '\n'; CLD(argPtr);  break;
		case INSTR_CLI: std::cout << "CLI" << '\n'; CLI(argPtr);  break;
		case INSTR_CLV: std::cout << "CLV" << '\n'; CLV(argPtr);  break;
		case INSTR_CMP: std::cout << "CMP" << '\n'; CMP(argPtr);  break;
		case INSTR_CPX: std::cout << "CPX" << '\n'; CPX(argPtr);  break;
		case INSTR_CPY: std::cout << "CPY" << '\n'; CPY(argPtr);  break;
		case INSTR_DEC: std::cout << "DEC" << '\n'; DEC(argPtr);  break;
		case INSTR_DEX: std::cout << "DEX" << '\n'; DEX(argPtr);  break;
		case INSTR_DEY: std::cout << "DEY" << '\n'; DEY(argPtr);  break;
		case INSTR_EOR: std::cout << "EOR" << '\n'; EOR(argPtr);  break;
		case INSTR_INC: std::cout << "INC" << '\n'; INC(argPtr);  break;
		case INSTR_INX: std::cout << "INX" << '\n'; INX(argPtr);  break;
		case INSTR_INY: std::cout << "INY" << '\n'; INY(argPtr);  break;
		case INSTR_JMP: std::cout << "JMP" << '\n'; JMP(argPtr);  break;
		case INSTR_JSR: std::cout << "JSR" << '\n'; JSR(argPtr);  break;
		case INSTR_LDA: std::cout << "LDA" << '\n'; LDA(argPtr);  break;
		case INSTR_LDX: std::cout << "LDX" << '\n'; LDX(argPtr);  break;
		case INSTR_LDY: std::cout << "LDY" << '\n'; LDY(argPtr);  break;
		case INSTR_LSR: std::cout << "LSR" << '\n'; LSR(argPtr);  break;
		case INSTR_NOP: std::cout << "NOP" << '\n'; NOP(argPtr);  break;
		case INSTR_ORA: std::cout << "ORA" << '\n'; ORA(argPtr);  break;
		case INSTR_PHA: std::cout << "PHA" << '\n'; PHA(argPtr);  break;
		case INSTR_PHP: std::cout << "PHP" << '\n'; PHP(argPtr);  break;
		case INSTR_PLA: std::cout << "PLA" << '\n'; PLA(argPtr);  break;
		case INSTR_PLP: std::cout << "PLP" << '\n'; PLP(argPtr);  break;
		case INSTR_ROL: std::cout << "ROL" << '\n'; ROL(argPtr);  break;
		case INSTR_ROR: std::cout << "ROR" << '\n'; ROR(argPtr);  break;
		case INSTR_RTI: std::cout << "RTI" << '\n'; RTI(argPtr);  break;
		case INSTR_RTS: std::cout << "RTS" << '\n'; RTS(argPtr);  break;
		case INSTR_SBC: std::cout << "SBC" << '\n'; SBC(argPtr);  break;
		case INSTR_SEC: std::cout << "SEC" << '\n'; SEC(argPtr);  break;
		case INSTR_SED: std::cout << "SED" << '\n'; SED(argPtr);  break;
		case INSTR_SEI: std::cout << "SEI" << '\n'; SEI(argPtr);  break;
		case INSTR_STA: std::cout << "STA" << '\n'; STA(argPtr);  break;
		case INSTR_STX: std::cout << "STX" << '\n'; STX(argPtr);  break;
		case INSTR_STY: std::cout << "STY" << '\n'; STY(argPtr);  break;
		case INSTR_TAX: std::cout << "TAX" << '\n'; TAX(argPtr);  break;
		case INSTR_TAY: std::cout << "TAY" << '\n'; TAY(argPtr);  break;
		case INSTR_TSX: std::cout << "TSX" << '\n'; TSX(argPtr);  break;
		case INSTR_TXA: std::cout << "TXA" << '\n'; TXA(argPtr);  break;
		case INSTR_TXS: std::cout << "TXS" << '\n'; TXS(argPtr);  break;
		case INSTR_TYA: std::cout << "TYA" << '\n'; TYA(argPtr);  break;

		default: std::cerr << op.instr << " (not yet implemented)\n"; break;
	}
}
/*
void CPU::StackPush(uint8_t data) {
	std::cout << "Pushing: " << (int)data << '\n';
	*(mem + 0x100 + SP) = data;
	SP = (SP - 1) & 0xFF;
}
uint8_t CPU::StackPop() {
	uint8_t data = *(mem + 0x100 + SP + 1); // +1 since SP points to next free location
	std::cout << "Popping: " << (int)data << '\n';
	SP = (SP + 1) & 0xFF;
	return data;
}*/

void CPU::StackPush(uint8_t data) {
	std::cout << "Pushing: " << (int)data << '\n';
	Write(data, (mem + 0x100 + GetSP()));
	SetSP((GetSP() - 1) & 0xFF);
}
uint8_t CPU::StackPop() {
	uint8_t data = Read(mem + 0x100 + GetSP() + 1); // +1 since SP points to next free location
	std::cout << "Popping: " << (int)data << '\n';
	SetSP((GetSP() + 1) & 0xFF);
	return data;
}

void CPU::PowerUp() {
	SetP(0x34);
	SetA(0);
	SetX(0);
	SetY(0);
	SetPC(*(mem + 0xFFFC) + (*(mem + 0xFFFD) << 8)); // RESET VECTOR
	SetSP(0xFD);
}

void CPU::EASY6502STARTUP()  {
	SetP(0x30);
	SetA(0);
	SetX(0);
	SetY(0);
	SetPC(0x600);
	SetSP(0xFF);
}
void CPU::LOADTEST(uint8_t* arr, unsigned int len) {
	for (int i=0;i<len;++i) {
		//*(mem + 0x600 + i) = *(arr + i);
		Write(*(arr + i), mem + 0x600 + i);
	}
	EASY6502STARTUP();
}

uint16_t CPU::RUNINSTR() {
	RunOpcode(*(mem + GetPC()), mem + GetPC());
	// Do other stuff


	SetPC(GetPC() + 1);


	PrintDebugInfo();
	return GetPC();
}

void CPU::PrintDebugInfo() {
	std::cout << std::hex << std::setfill('0');
	std::cout << "\nA = $" << std::setw(2) << (unsigned int)GetA();
	std::cout << " X = $" << std::setw(2) << (unsigned int)GetX();
	std::cout << " Y = $" << std::setw(2) << (unsigned int)GetY() << '\n';
	std::cout << " SP = $" << std::setw(2) << (unsigned int)GetSP();
	std::cout << " PC = $" << std::setw(4) << (unsigned int)GetPC() << '\n';
	std::cout << "      NV-BDIZC\n      ";
	for (int i=7;i>=0;--i) {
		std::cout << ((GetP() >> i) & 0x1);
	}
	std::cout << "\n\n";
}

CPU::CPU () {
}