#include "CPUMem.h"

void CPUMem::Write(uint8_t data, uint8_t* absLocation) {
	uintptr_t relLocation = reinterpret_cast<uintptr_t>(absLocation - *mem);
	if (relLocation >= 0x0 && relLocation <= 0x7ff) {
		*absLocation = data;
		*(absLocation +  0x800) = data;
		*(absLocation + 0x1000) = data;
		*(absLocation + 0x1800) = data;
	}
	else if (relLocation >= 0x2000 && relLocation <= 0x2007) {
		for (int i = 0; i < 0x3fff; i += 0x8) {
			*(absLocation + i) = data;
		}
	}
	else {
		*absLocation = data;
	}
}
uint8_t CPUMem::Read(uint8_t* absLocation) {
	return *absLocation;
}

void CPUMem::SetPC(uint16_t val){
	Write(val & 0xFF, reinterpret_cast<uint8_t*>(&PC));
	Write((val & 0xFF00) >> 8, reinterpret_cast<uint8_t*>(&PC) + 1);
}
uint16_t CPUMem::GetPC() {
	return Read(reinterpret_cast<uint8_t*>(&PC)) + (Read(reinterpret_cast<uint8_t*>(&PC) + 1) << 8);
}
void CPUMem::SetSP(uint8_t val){
	Write(val, &SP);
}
uint8_t CPUMem::GetSP(){
	return Read(&SP);
}
void CPUMem::SetA(uint8_t val){
	Write(val, &A);
}
uint8_t CPUMem::GetA(){
	return Read(&A);
}
void CPUMem::SetX(uint8_t val){
	Write(val, &X);
}
uint8_t CPUMem::GetX(){
	return Read(&X);
}
void CPUMem::SetY(uint8_t val){
	Write(val, &Y);
}
uint8_t CPUMem::GetY(){
	return Read(&Y);
}
void CPUMem::SetP(uint8_t val){
	Write(val, &P);
}
uint8_t CPUMem::GetP(){
	return Read(&P);
}
void CPUMem::SetN(int val){
	N = val;
	Write(P, &P);
}
int CPUMem::GetN() {
	return N;
}
void CPUMem::SetV(int val){
	V = val;
	Write(P, &P);
}
int CPUMem::GetV() {
	return V;
}
void CPUMem::SetB(int val){
	B = val;
	Write(P, &P);
}
int CPUMem::GetB() {
	return B;
}
void CPUMem::SetD(int val){
	D = val;
	Write(P, &P);
}
int CPUMem::GetD() {
	return D;
}
void CPUMem::SetI(int val){
	I = val;
	Write(P, &P);
}
int CPUMem::GetI() {
	return I;
}
void CPUMem::SetZ(int val){
	Z = val;
	Write(P, &P);
}
int CPUMem::GetZ() {
	return Z;
}
void CPUMem::SetC(int val){
	C = val;
	Write(P, &P);
}
int CPUMem::GetC() {
	return C;
}

uint8_t* CPUMem::GetAAddr() {
	return &A;
}
uint8_t*  CPUMem::GetMemAddress() {
	return mem;
}