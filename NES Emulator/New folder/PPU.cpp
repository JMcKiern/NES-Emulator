#include "PPU.h"

uint8_t PPU::AccessPPUControlRegister() {
	//CPUMemPtr->Read();
}
void PPU::SetPPUStatusRegister(uint8_t data) {

}
PPU::PPU(CPUMem* _CPUMemPtr) {
	CPUMemPtr = _CPUMemPtr;
}