#ifndef CPUMEM_H
#define CPUMEM_H

#include <cstdint>
#include "BitFlag.h"
#include "PPUMem.h"

class CPUMem {
private:
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
	};

	PPUMem* PPUMemPtr;

protected:
	void Write(uint8_t data, uint8_t* absLocation);
	uint8_t Read(uint8_t* absLocation);
	
	void SetPC(uint16_t val);
	uint16_t GetPC();
	void SetSP(uint8_t val);
	uint8_t GetSP();
	void SetA(uint8_t val);
	uint8_t GetA();
	void SetX(uint8_t val);
	uint8_t GetX();
	void SetY(uint8_t val);
	uint8_t GetY();
	void SetP(uint8_t val);
	uint8_t GetP();
	void SetN(int val);
	int GetN();
	void SetV(int val);
	int GetV();
	void SetB(int val);
	int GetB();
	void SetD(int val);
	int GetD();
	void SetI(int val);
	int GetI();
	void SetZ(int val);
	int GetZ();
	void SetC(int val);
	int GetC();

public:
	uint8_t* GetMemAddress();
	uint8_t* GetAAddr();
};

#endif