#pragma once

#include <cstdint>
#include <string>
#include "BitFlag.h"
#include "enums.h"
#include "Operation.h"
#include "PeripheralLine.h"

class CPU_6502 {
private:
	// CPU Registers
	union {
		const uint64_t RegisterState;
		struct {
			uint16_t PC;
			uint8_t SP, A, X, Y;
			union {
				const uint8_t P;

				BitFlag<7> N; // Negative Flag
				BitFlag<6> V; // Overflow Flag
							  // BitFlag<5> is unused
				//BitFlag<4> B; // Break Command Flag
				BitFlag<3> D; // Decimal Mode Flag - NOT USED IN 2A03
				BitFlag<2> I; // Interrupt Disable Flag
				BitFlag<1> Z; // Zero Flag
				BitFlag<0> C; // Carry Flag
			};
		};
	};

	// Instructions
	void ADC(uint16_t offset, AddrMode addrMode);
	void AND(uint16_t offset, AddrMode addrMode);
	void ASL(uint16_t offset, AddrMode addrMode);
	void BCC(uint16_t offset, AddrMode addrMode);
	void BCS(uint16_t offset, AddrMode addrMode);
	void BEQ(uint16_t offset, AddrMode addrMode);
	void BIT(uint16_t offset, AddrMode addrMode);
	void BMI(uint16_t offset, AddrMode addrMode);
	void BNE(uint16_t offset, AddrMode addrMode);
	void BPL(uint16_t offset, AddrMode addrMode);
	void BRK(uint16_t offset, AddrMode addrMode);
	void BVC(uint16_t offset, AddrMode addrMode);
	void BVS(uint16_t offset, AddrMode addrMode);
	void CLC(uint16_t offset, AddrMode addrMode);
	void CLD(uint16_t offset, AddrMode addrMode);
	void CLI(uint16_t offset, AddrMode addrMode);
	void CLV(uint16_t offset, AddrMode addrMode);
	void CMP(uint16_t offset, AddrMode addrMode);
	void CPX(uint16_t offset, AddrMode addrMode);
	void CPY(uint16_t offset, AddrMode addrMode);
	void DEC(uint16_t offset, AddrMode addrMode);
	void DEX(uint16_t offset, AddrMode addrMode);
	void DEY(uint16_t offset, AddrMode addrMode);
	void EOR(uint16_t offset, AddrMode addrMode);
	void INC(uint16_t offset, AddrMode addrMode);
	void INX(uint16_t offset, AddrMode addrMode);
	void INY(uint16_t offset, AddrMode addrMode);
	void JMP(uint16_t offset, AddrMode addrMode);
	void JSR(uint16_t offset, AddrMode addrMode);
	void LDA(uint16_t offset, AddrMode addrMode);
	void LDX(uint16_t offset, AddrMode addrMode);
	void LDY(uint16_t offset, AddrMode addrMode);
	void LSR(uint16_t offset, AddrMode addrMode);
	void NOP(uint16_t offset, AddrMode addrMode);
	void ORA(uint16_t offset, AddrMode addrMode);
	void PHA(uint16_t offset, AddrMode addrMode);
	void PHP(uint16_t offset, AddrMode addrMode);
	void PLA(uint16_t offset, AddrMode addrMode);
	void PLP(uint16_t offset, AddrMode addrMode);
	void ROL(uint16_t offset, AddrMode addrMode);
	void ROR(uint16_t offset, AddrMode addrMode);
	void RTI(uint16_t offset, AddrMode addrMode);
	void RTS(uint16_t offset, AddrMode addrMode);
	void SBC(uint16_t offset, AddrMode addrMode);
	void SEC(uint16_t offset, AddrMode addrMode);
	void SED(uint16_t offset, AddrMode addrMode);
	void SEI(uint16_t offset, AddrMode addrMode);
	void STA(uint16_t offset, AddrMode addrMode);
	void STX(uint16_t offset, AddrMode addrMode);
	void STY(uint16_t offset, AddrMode addrMode);
	void TAX(uint16_t offset, AddrMode addrMode);
	void TAY(uint16_t offset, AddrMode addrMode);
	void TSX(uint16_t offset, AddrMode addrMode);
	void TXA(uint16_t offset, AddrMode addrMode);
	void TXS(uint16_t offset, AddrMode addrMode);
	void TYA(uint16_t offset, AddrMode addrMode);
	
	// Stack Management
	void StackPush(uint8_t data);
	uint8_t StackPop();

	// Processor Status Management
	void PopToP();
	void PushP(bool shouldSetBit4);

	// Page Crossing
	bool hasPageCrossed, isPageCrossPossible;
	void BranchAndCheckForPageCrossing(int8_t relVal);
	uint16_t AddAndCheckForPageCrossing(uint8_t lowVal, uint16_t regVal);

	// Interrupts // pg143 (154) for turning off
	PeripheralLine irqLine, nmiLine;
	bool nmiFlipFlop = false;
	DigitalState prevNMIState = LOW; //TODO: Check if should be HIGH;
	void CheckForInterrupt();
	void RespondToInterrupt(bool isIRQ); // Priority = Start > _NMI > _IRQ
	bool IRQ, NMI;

protected:
	// CPU Memory
	uint8_t* RAM;
	virtual uint8_t Read(uint16_t offset, bool shouldTick = true);
	virtual void Write(uint16_t offset, uint8_t data, bool shouldTick = true);
	uint8_t ReadNoTick(uint16_t offset);
	void WriteNoTick(uint16_t offset, uint8_t data);

	// Timing
	int currentOpNumCycles = 0;
	int totalCycles = 0;
	int totalNumInstrs = 0;
	virtual void Tick();

	// Logging
	uint16_t prevPC;
	Operation prevOp;
	uint16_t prevArgOffset;
	std::string DisassembleInstr();

	// Operation Table
	Operation operationTable[0xFF];
	void SetupOperationTable();

public:
	// Peripherals
	uint8_t PeripheralRead(uint16_t addr);
	void PeripheralWrite(uint16_t addr, uint8_t data);
	void AddIRQConnection(PeripheralConnection* irqConnection);
	void RemoveIRQConnection(PeripheralConnection* irqConnection);
	void AddNMIConnection(PeripheralConnection* nmiConnection);
	void RemoveNMIConnection(PeripheralConnection* nmiConnection);

	// Initialization
	void EASY6502STARTUP();
	void LOADTEST(uint8_t* arr, unsigned int len);
	void LoadFromFile(std::string filename, uint16_t toOffset);

	// Running
	void RunNextOpcode();
	uint16_t GetPC();
	int GetTotalNumInstrs();

	// Debugging
	void SetP(uint8_t val);
	void SetPC(uint16_t _PC);
	void SetI();
	uint8_t GetI();
	void SetSP(uint8_t val);
	uint8_t GetSP();
	void SetA(uint8_t val);
	void SetX(uint8_t val);
	void SetY(uint8_t val);
	int GetTotalCycles();

	// Logging
	void PrintDebugInfoMesenBasic();
	void PrintDebugInfo();

	// Constructor
	CPU_6502(int sizeOfRam = 0x10000);
	~CPU_6502();
};