#include <cstdint>
#include "OperationTable.h"
#include "Operation.h"

void OperationTable::SetupOperationTable() {
	operationTable[0x69] = Operation(INSTR_ADC, AM_IMM, "ADC");
	operationTable[0x65] = Operation(INSTR_ADC, AM_ZP, "ADC");
	operationTable[0x75] = Operation(INSTR_ADC, AM_ZPX, "ADC");
	operationTable[0x6D] = Operation(INSTR_ADC, AM_ABS, "ADC");
	operationTable[0x7D] = Operation(INSTR_ADC, AM_ABSX, "ADC");
	operationTable[0x79] = Operation(INSTR_ADC, AM_ABSY, "ADC");
	operationTable[0x61] = Operation(INSTR_ADC, AM_IDXIND, "ADC");
	operationTable[0x71] = Operation(INSTR_ADC, AM_INDIDX, "ADC");
	operationTable[0x29] = Operation(INSTR_AND, AM_IMM, "AND");
	operationTable[0x25] = Operation(INSTR_AND, AM_ZP, "AND");
	operationTable[0x35] = Operation(INSTR_AND, AM_ZPX, "AND");
	operationTable[0x2D] = Operation(INSTR_AND, AM_ABS, "AND");
	operationTable[0x3D] = Operation(INSTR_AND, AM_ABSX, "AND");
	operationTable[0x39] = Operation(INSTR_AND, AM_ABSY, "AND");
	operationTable[0x21] = Operation(INSTR_AND, AM_IDXIND, "AND");
	operationTable[0x31] = Operation(INSTR_AND, AM_INDIDX, "AND");
	operationTable[0x0A] = Operation(INSTR_ASL, AM_ACC, "ASL");
	operationTable[0x06] = Operation(INSTR_ASL, AM_ZP, "ASL");
	operationTable[0x16] = Operation(INSTR_ASL, AM_ZPX, "ASL");
	operationTable[0x0E] = Operation(INSTR_ASL, AM_ABS, "ASL");
	operationTable[0x1E] = Operation(INSTR_ASL, AM_ABSX, "ASL");
	operationTable[0x90] = Operation(INSTR_BCC, AM_REL, "BCC");
	operationTable[0xB0] = Operation(INSTR_BCS, AM_REL, "BCS");
	operationTable[0xF0] = Operation(INSTR_BEQ, AM_REL, "BEQ");
	operationTable[0x24] = Operation(INSTR_BIT, AM_ZP, "BIT");
	operationTable[0x2C] = Operation(INSTR_BIT, AM_ABS, "BIT");
	operationTable[0x30] = Operation(INSTR_BMI, AM_REL, "BMI");
	operationTable[0xD0] = Operation(INSTR_BNE, AM_REL, "BNE");
	operationTable[0x10] = Operation(INSTR_BPL, AM_REL, "BPL");
	operationTable[0x00] = Operation(INSTR_BRK, AM_IMP, "BRK");
	operationTable[0x50] = Operation(INSTR_BVC, AM_REL, "BVC");
	operationTable[0x70] = Operation(INSTR_BVS, AM_REL, "BVS");
	operationTable[0x18] = Operation(INSTR_CLC, AM_IMP, "CLC");
	operationTable[0xD8] = Operation(INSTR_CLD, AM_IMP, "CLD");
	operationTable[0x58] = Operation(INSTR_CLI, AM_IMP, "CLI");
	operationTable[0xB8] = Operation(INSTR_CLV, AM_IMP, "CLV");
	operationTable[0xC9] = Operation(INSTR_CMP, AM_IMM, "CMP");
	operationTable[0xC5] = Operation(INSTR_CMP, AM_ZP, "CMP");
	operationTable[0xD5] = Operation(INSTR_CMP, AM_ZPX, "CMP");
	operationTable[0xCD] = Operation(INSTR_CMP, AM_ABS, "CMP");
	operationTable[0xDD] = Operation(INSTR_CMP, AM_ABSX, "CMP");
	operationTable[0xD9] = Operation(INSTR_CMP, AM_ABSY, "CMP");
	operationTable[0xD1] = Operation(INSTR_CMP, AM_IDXIND, "CMP");
	operationTable[0xE0] = Operation(INSTR_CPX, AM_IMM, "CPX");
	operationTable[0xE4] = Operation(INSTR_CPX, AM_ZP, "CPX");
	operationTable[0xEC] = Operation(INSTR_CPX, AM_ABS, "CPX");
	operationTable[0xC0] = Operation(INSTR_CPY, AM_IMM, "CPY");
	operationTable[0xC4] = Operation(INSTR_CPY, AM_ZP, "CPY");
	operationTable[0xCC] = Operation(INSTR_CPY, AM_ABS, "CPY");
	operationTable[0xC6] = Operation(INSTR_DEC, AM_ZP, "DEC");
	operationTable[0xD6] = Operation(INSTR_DEC, AM_ZPX, "DEC");
	operationTable[0xCE] = Operation(INSTR_DEC, AM_ABS, "DEC");
	operationTable[0xDE] = Operation(INSTR_DEC, AM_ABSX, "DEC");
	operationTable[0xCA] = Operation(INSTR_DEX, AM_IMP, "DEX");
	operationTable[0x88] = Operation(INSTR_DEY, AM_IMP, "DEY");
	operationTable[0x49] = Operation(INSTR_EOR, AM_IMM, "EOR");
	operationTable[0x45] = Operation(INSTR_EOR, AM_ZP, "EOR");
	operationTable[0x55] = Operation(INSTR_EOR, AM_ZPX, "EOR");
	operationTable[0x4D] = Operation(INSTR_EOR, AM_ABS, "EOR");
	operationTable[0x5D] = Operation(INSTR_EOR, AM_ABSX, "EOR");
	operationTable[0x59] = Operation(INSTR_EOR, AM_ABSY, "EOR");
	operationTable[0x41] = Operation(INSTR_EOR, AM_IDXIND, "EOR");
	operationTable[0x51] = Operation(INSTR_EOR, AM_INDIDX, "EOR");
	operationTable[0xE6] = Operation(INSTR_INC, AM_ZP, "INC");
	operationTable[0xF6] = Operation(INSTR_INC, AM_ZPX, "INC");
	operationTable[0xEE] = Operation(INSTR_INC, AM_ABS, "INC");
	operationTable[0xFE] = Operation(INSTR_INC, AM_ABSX, "INC");
	operationTable[0xE8] = Operation(INSTR_INX, AM_IMP, "INX");
	operationTable[0xC8] = Operation(INSTR_INY, AM_IMP, "INY");
	operationTable[0x4C] = Operation(INSTR_JMP, AM_ABS, "JMP");
	operationTable[0x4C] = Operation(INSTR_JMP, AM_IND, "JMP");
	operationTable[0x20] = Operation(INSTR_JSR, AM_ABS, "JSR");
	operationTable[0xA9] = Operation(INSTR_LDA, AM_IMM, "LDA");
	operationTable[0xA5] = Operation(INSTR_LDA, AM_ZP, "LDA");
	operationTable[0xB5] = Operation(INSTR_LDA, AM_ZPX, "LDA");
	operationTable[0xAD] = Operation(INSTR_LDA, AM_ABS, "LDA");
	operationTable[0xBD] = Operation(INSTR_LDA, AM_ABSX, "LDA");
	operationTable[0xB9] = Operation(INSTR_LDA, AM_ABSY, "LDA");
	operationTable[0xA1] = Operation(INSTR_LDA, AM_IDXIND, "LDA");
	operationTable[0xB1] = Operation(INSTR_LDA, AM_INDIDX, "LDA");
	operationTable[0xA2] = Operation(INSTR_LDX, AM_IMM, "LDX");
	operationTable[0xA6] = Operation(INSTR_LDX, AM_ZP, "LDX");
	operationTable[0xB6] = Operation(INSTR_LDX, AM_ZPY, "LDX");
	operationTable[0xAE] = Operation(INSTR_LDX, AM_ABS, "LDX");
	operationTable[0xBE] = Operation(INSTR_LDX, AM_ABSY, "LDX");
	operationTable[0xA0] = Operation(INSTR_LDY, AM_IMM, "LDY");
	operationTable[0xA4] = Operation(INSTR_LDY, AM_ZP, "LDY");
	operationTable[0xB4] = Operation(INSTR_LDY, AM_ZPX, "LDY");
	operationTable[0xAC] = Operation(INSTR_LDY, AM_ABS, "LDY");
	operationTable[0xBC] = Operation(INSTR_LDY, AM_ABSX, "LDY");
	operationTable[0x4A] = Operation(INSTR_LSR, AM_ACC, "LSR");
	operationTable[0x46] = Operation(INSTR_LSR, AM_ZP, "LSR");
	operationTable[0x56] = Operation(INSTR_LSR, AM_ZPX, "LSR");
	operationTable[0x4E] = Operation(INSTR_LSR, AM_ABS, "LSR");
	operationTable[0x5E] = Operation(INSTR_LSR, AM_ABSX, "LSR");
	operationTable[0xEA] = Operation(INSTR_NOP, AM_IMP, "NOP");
	operationTable[0x09] = Operation(INSTR_ORA, AM_IMM, "ORA");
	operationTable[0x05] = Operation(INSTR_ORA, AM_ZP, "ORA");
	operationTable[0x15] = Operation(INSTR_ORA, AM_ZPX, "ORA");
	operationTable[0x0D] = Operation(INSTR_ORA, AM_ABS, "ORA");
	operationTable[0x1D] = Operation(INSTR_ORA, AM_ABSX, "ORA");
	operationTable[0x19] = Operation(INSTR_ORA, AM_ABSY, "ORA");
	operationTable[0x01] = Operation(INSTR_ORA, AM_IDXIND, "ORA");
	operationTable[0x11] = Operation(INSTR_ORA, AM_INDIDX, "ORA");
	operationTable[0x48] = Operation(INSTR_PHA, AM_IMP, "PHA");
	operationTable[0x08] = Operation(INSTR_PHP, AM_IMP, "PHP");
	operationTable[0x68] = Operation(INSTR_PLA, AM_IMP, "PLA");
	operationTable[0x28] = Operation(INSTR_PLP, AM_IMP, "PLP");
	operationTable[0x2A] = Operation(INSTR_ROL, AM_ACC, "ROL");
	operationTable[0x26] = Operation(INSTR_ROL, AM_ZP, "ROL");
	operationTable[0x36] = Operation(INSTR_ROL, AM_ZPX, "ROL");
	operationTable[0x2E] = Operation(INSTR_ROL, AM_ABS, "ROL");
	operationTable[0x3E] = Operation(INSTR_ROL, AM_ABSX, "ROL");
	operationTable[0x6A] = Operation(INSTR_ROR, AM_ACC, "ROR");
	operationTable[0x66] = Operation(INSTR_ROR, AM_ZP, "ROR");
	operationTable[0x76] = Operation(INSTR_ROR, AM_ZPX, "ROR");
	operationTable[0x6E] = Operation(INSTR_ROR, AM_ABS, "ROR");
	operationTable[0x7E] = Operation(INSTR_ROR, AM_ABSX, "ROR");
	operationTable[0x40] = Operation(INSTR_RTI, AM_IMP, "RTI");
	operationTable[0x60] = Operation(INSTR_RTS, AM_IMP, "RTS");
	operationTable[0xE9] = Operation(INSTR_SBC, AM_IMM, "SBC");
	operationTable[0xE5] = Operation(INSTR_SBC, AM_ZP, "SBC");
	operationTable[0xF5] = Operation(INSTR_SBC, AM_ZPX, "SBC");
	operationTable[0xED] = Operation(INSTR_SBC, AM_ABS, "SBC");
	operationTable[0xFD] = Operation(INSTR_SBC, AM_ABSX, "SBC");
	operationTable[0xF9] = Operation(INSTR_SBC, AM_ABSY, "SBC");
	operationTable[0xE1] = Operation(INSTR_SBC, AM_IDXIND, "SBC");
	operationTable[0xF1] = Operation(INSTR_SBC, AM_INDIDX, "SBC");
	operationTable[0x28] = Operation(INSTR_SEC, AM_IMP, "SEC");
	operationTable[0xF8] = Operation(INSTR_SED, AM_IMP, "SED");
	operationTable[0x78] = Operation(INSTR_SEI, AM_IMP, "SEI");
	operationTable[0x85] = Operation(INSTR_STA, AM_ZP, "STA");
	operationTable[0x95] = Operation(INSTR_STA, AM_ZPX, "STA");
	operationTable[0x8D] = Operation(INSTR_STA, AM_ABS, "STA");
	operationTable[0x9D] = Operation(INSTR_STA, AM_ABSX, "STA");
	operationTable[0x99] = Operation(INSTR_STA, AM_ABSY, "STA");
	operationTable[0x81] = Operation(INSTR_STA, AM_IDXIND, "STA");
	operationTable[0x91] = Operation(INSTR_STA, AM_INDIDX, "STA");
	operationTable[0x86] = Operation(INSTR_STX, AM_ZP, "STX");
	operationTable[0x96] = Operation(INSTR_STX, AM_ZPY, "STX");
	operationTable[0x8E] = Operation(INSTR_STX, AM_ABS, "STX");
	operationTable[0x84] = Operation(INSTR_STY, AM_ZP, "STY");
	operationTable[0x94] = Operation(INSTR_STY, AM_ZPX, "STY");
	operationTable[0x8C] = Operation(INSTR_STY, AM_ABS, "STY");
	operationTable[0xAA] = Operation(INSTR_TAX, AM_IMP, "TAX");
	operationTable[0xA8] = Operation(INSTR_TAY, AM_IMP, "TAY");
	operationTable[0xBA] = Operation(INSTR_TSX, AM_IMP, "TSX");
	operationTable[0x8A] = Operation(INSTR_TXA, AM_IMP, "TXA");
	operationTable[0x9A] = Operation(INSTR_TXS, AM_IMP, "TXS");
	operationTable[0x98] = Operation(INSTR_TYA, AM_IMP, "TYA");
}

Operation OperationTable::GetOperation(uint8_t opcode) {
	return operationTable[opcode];
}
void OperationTable::LinkFunction(Instr instr, void(CPU::*_Run)(uint16_t offset, AddrMode addrMode)) {
	for (int i = 0; i < 0xFF; i++) {
		if (operationTable[i].instr == instr) {
			operationTable[i].Run = _Run;
		}
	}
}

OperationTable::OperationTable() {
	SetupOperationTable();
}