#pragma once

#include <cstdint>

#include "CPU_NES.h"

// I'm going to be lazy and just use blargg's library
#include "nes_apu/Nes_Apu.h"

class APU {
private:
	Blip_Buffer buf;
	Nes_Apu apu;
	void output_samples(const blip_sample_t* smpl, size_t count);
	static const size_t out_size = 4096;
	blip_sample_t out_buf[out_size];

	// Maybe for full apu support
	cpu_time_t cpu_end_time;

	int total_cycles;
	int cycles_remain;

	int elapsed();

	CPU_NES* cpuPtr;
	//cpu_time_t earliest_irq_before(cpu_time_t end_time);
	//void irq_changed(void*);

	void end_time_frame(int length);


public:
	uint8_t Read(uint16_t offset);
	void Write(uint16_t offset, uint8_t data);

	void RunFrame(int numCycles);

	APU(CPU_NES* _cpuPtr);
};