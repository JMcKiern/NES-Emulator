#include <exception>
#include "APU.h"

int APU::elapsed() {
	return total_cycles - cycles_remain;
}

uint8_t APU::Read(uint16_t addr) {
	return apu.read_status(elapsed());
}

void APU::Write(uint16_t addr, uint8_t data) {
	apu.write_register(elapsed(), addr, data);
}

// Not part of class
int apu_dmc_read(void* user_data, cpu_addr_t addr) {
	CPU_NES* cpuPtr = reinterpret_cast<CPU_NES*>(user_data);
	return cpuPtr->PPURequestingRead(addr);
}

/*cpu_time_t APU::earliest_irq_before(cpu_time_t end_time) {
	if (!(cpuPtr->GetI())) {
		cpu_time_t irq_time = apu.earliest_irq();
		if (irq_time < end_time)
			end_time = irq_time;
	}
	return end_time;
}

void APU::irq_changed(void*) {
	cpu_end_time = earliest_irq_before(cpu_end_time);
}*/

void APU::end_time_frame(int length) {
	apu.end_frame(length);
	buf.end_frame(length);
	total_cycles -= length;

	if (buf.samples_avail() >= out_size) {
		size_t count = buf.read_samples(out_buf, out_size);
		output_samples(out_buf, count);
	}

}

void APU::RunFrame(int numCycles) {
	total_cycles += numCycles;
	cycles_remain += numCycles;

	end_time_frame(elapsed())
}


APU::APU(CPU_NES* _cpuPtr) {
	cpuPtr = _cpuPtr;
	blargg_err_t error = buf.sample_rate(44100);
	if (error)
		throw std::exception(error);
	buf.clock_rate(1789773);
	apu.output(&buf);
	//apu.dmc_reader(static_cast<APU*>(this)->dmc_read);
	apu.dmc_reader(apu_dmc_read, cpuPtr);
}