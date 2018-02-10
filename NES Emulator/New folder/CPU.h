#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "Operation.h"
#include "OperationTable.h"
#include "CPUMem.h"

class CPU : public CPUMem{
private:/*
	union {
		uint8_t mem[0x10000];
		struct
		{
			uint8_t RAM[0x2000];
			union { // IORegisters
				uint8_t IORegisters[0x2200];
				
				struct
				{
					uint16_t PC;
					uint8_t SP, A, X, Y;
					union {
						uint8_t P;

						BitFlag<7> N; // Negative Flag
						BitFlag<6> V; // Overflow Flag
						//BitFlag<5> is unused
						BitFlag<4> B; // Break Command Flag
						BitFlag<3> D; // Decimal Mode Flag - NOT USED IN 2A03
						BitFlag<2> I; // Interrupt Disable Flag
						BitFlag<1> Z; // Zero Flag
						BitFlag<0> C; // Carry Flag
					};
				};
			};
			uint8_t ExpansionRom[0x1E00];
			uint8_t SRAM[0x2000];
			uint8_t PRGROM[0x2000];
		};
	};*/

	// Instructions
	void ADC(uint8_t* ptr);
	void AND(uint8_t* ptr);
	void ASL(uint8_t* ptr);
	void BCC(uint8_t* ptr);
	void BCS(uint8_t* ptr);
	void BEQ(uint8_t* ptr);
	void BIT(uint8_t* ptr);
	void BMI(uint8_t* ptr);
	void BNE(uint8_t* ptr);
	void BPL(uint8_t* ptr);
	void BRK(uint8_t* ptr);
	void BVC(uint8_t* ptr);
	void BVS(uint8_t* ptr);
	void CLC(uint8_t* ptr);
	void CLD(uint8_t* ptr);
	void CLI(uint8_t* ptr);
	void CLV(uint8_t* ptr);
	void CMP(uint8_t* ptr);
	void CPX(uint8_t* ptr);
	void CPY(uint8_t* ptr);
	void DEC(uint8_t* ptr);
	void DEX(uint8_t* ptr);
	void DEY(uint8_t* ptr);
	void EOR(uint8_t* ptr);
	void INC(uint8_t* ptr);
	void INX(uint8_t* ptr);
	void INY(uint8_t* ptr);
	void JMP(uint8_t* ptr);
	void JSR(uint8_t* ptr);
	void LDA(uint8_t* ptr);
	void LDX(uint8_t* ptr);
	void LDY(uint8_t* ptr);
	void LSR(uint8_t* ptr);
	void NOP(uint8_t* ptr);
	void ORA(uint8_t* ptr);
	void PHA(uint8_t* ptr);
	void PHP(uint8_t* ptr);
	void PLA(uint8_t* ptr);
	void PLP(uint8_t* ptr);
	void ROL(uint8_t* ptr);
	void ROR(uint8_t* ptr);
	void RTI(uint8_t* ptr);
	void RTS(uint8_t* ptr);
	void SBC(uint8_t* ptr);
	void SEC(uint8_t* ptr);
	void SED(uint8_t* ptr);
	void SEI(uint8_t* ptr);
	void STA(uint8_t* ptr);
	void STX(uint8_t* ptr);
	void STY(uint8_t* ptr);
	void TAX(uint8_t* ptr);
	void TAY(uint8_t* ptr);
	void TSX(uint8_t* ptr);
	void TXA(uint8_t* ptr);
	void TXS(uint8_t* ptr);
	void TYA(uint8_t* ptr);
	
	void StackPush(uint8_t data);
	uint8_t StackPop();

	OperationTable operationTable;

	uint8_t* mem = GetMemAddress();

public:
	void PowerUp();
	void PrintDebugInfo();
	void EASY6502STARTUP();
	void LOADTEST(uint8_t* arr, unsigned int len);
	void RunOpcode(uint8_t opcode, uint8_t* absAddrPC);
	void WritePPURegister(uint8_t data, uint8_t offset);
	uint8_t ReadPPURegister(uint8_t offset);
	uint16_t RUNINSTR();
	CPU();
};

#endif