#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include "BitFlag.h"
#include "Operation.h"
#include "CPUMem.h"
#include "enums.h"
#include "Log.h"

class CPU {
private:
	// CPU Registers
	//union {
	//	const uint64_t RegisterState;
	//	struct {
			uint16_t PC;
			uint8_t SP, A, X, Y;
			union {
				const uint8_t P = 0x34;

				BitFlag<7> N; // Negative Flag
				BitFlag<6> V; // Overflow Flag
							  // BitFlag<5> is unused
				BitFlag<4> B; // Break Command Flag
				BitFlag<3> D; // Decimal Mode Flag - NOT USED IN 2A03
				BitFlag<2> I; // Interrupt Disable Flag
				BitFlag<1> Z; // Zero Flag
				BitFlag<0> C; // Carry Flag
			};
	//	};
	//};

	// CPU Memory
	CPUMem cpuMem;
	void Write(uint16_t offset, uint8_t data);
	uint8_t Read(uint16_t offset);
	uint8_t ReadNoTick(uint16_t offset);

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
	
	void StackPush(uint8_t data);
	uint8_t StackPop();
	void PopToP();
	void PushP(bool shouldSetBit4);
	void SetP(uint8_t val, bool shouldSetBit4);

	Operation operationTable[0xFF];
	void SetupOperationTable();

	Log log;

	bool _IRQ, _NMI; // pg143 (154) for turning off

	void StartCycle();
	void RespondToInterrupt(bool isIRQ);
	void CheckForInterrupt();
	void Tick();

public:
	void PowerUp();
	void EASY6502STARTUP();
	void TESTSTARTUP();
	void LOADTEST(uint8_t* arr, unsigned int len);
	void LOADTESTFROMFILE(std::string filename);
	void RunNextOpcode();
	uint16_t GetPC();
	//void WritePPURegister(uint8_t data, uint8_t offset);
	//uint8_t ReadPPURegister(uint8_t offset);

	#ifdef _DEBUG_MODE
		void PrintDebugInfo();
	#endif

	CPU::CPU();
	CPU::CPU(std::string logFile);
	CPU(uint8_t __P, uint8_t __A, uint8_t __X, uint8_t __Y, uint16_t __PC, uint8_t __SP);
};

#endif