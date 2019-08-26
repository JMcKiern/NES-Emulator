#include <iostream>
#include <vector>
#include "FunctionalTest.h"
#include "NesTest.h"
#include "../../src/Logger.h"

Logger logger("log.txt");

int main() {
	// 6502 Tests
	std::vector<FunctionalTestRun> ftrs = {
		//{"../roms/6502_functional_test.bin", 0x0, 0x400, 0x336d, false, false},
		//{"../roms/6502_interrupt_test.bin", 0x0, 0x400, 0x06f5, true, false},
		//{"../roms/timingtest-1.bin", 0x1000, 0x1000, 0x1000, false, false}
	};

	// NES Tests
	std::vector<NesTestRun> ntrs = {
		// CPU
		//PrintHash("../tests/CPU/cpu_dummy_reads.nes"},
		//PrintHash("../tests/Nestest/nestest - jump to test.nes"},
		//PrintHash("../tests/CPU/nestest.nes"},
		{"../tests/CPU/branch_timing_tests/1.Branch_Basics.nes", 127975, "7d84fe2d"},
		{"../tests/CPU/branch_timing_tests/2.Backward_Branch.nes", 156241, "f3ddede5"},
		{"../tests/CPU/branch_timing_tests/3.Forward_Branch.nes", 156033, "ce51c7e6"},


		//console.Run("../tests/CPU/cpu_dummy_writes/cpu_dummy_writes_oam.nes");
		//console.Run("../tests/CPU/cpu_dummy_writes/cpu_dummy_writes_ppumem.nes");
		//console.Run("../tests/CPU/cpu_exec_space/test_cpu_exec_space_apu.nes");
		//console.Run("../tests/CPU/cpu_exec_space/test_cpu_exec_space_ppuio.nes");

		//PrintHash("../tests/CPU/cpu_interrupts_v2/rom_singles/1-cli_latency.nes"},
		//console.Run("../tests/CPU/cpu_interrupts_v2/rom_singles/2-nmi_and_brk.nes");
		//console.Run("../tests/CPU/cpu_interrupts_v2/rom_singles/3-nmi_and_irq.nes");
		//console.Run("../tests/CPU/cpu_interrupts_v2/rom_singles/4-irq_and_dma.nes");
		//console.Run("../tests/CPU/cpu_interrupts_v2/rom_singles/5-branch_delays_irq.nes");

		// PPU
		//PrintHash("../tests/PPU/ppu_vbl_nmi/ppu_vbl_nmi.nes"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/01-vbl_basics.nes", 1666560, "a069cf23"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/02-vbl_set_time.nes", 1798437, "a05dbed9"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/03-vbl_clear_time.nes", 1707366, "c909c505"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/04-nmi_control.nes", 302984, "f75bbc3a"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/05-nmi_timing.nes", 2272532, "0566ccef"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/06-suppression.nes", 2173184, "6461c604"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/07-nmi_on_timing.nes", 1855652, "383b9dd3"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/08-nmi_off_timing.nes", 2265642, "2fb5e6ea"},
		{"../tests/PPU/ppu_vbl_nmi/rom_singles/09-even_odd_frames.nes", 821446, "e91565bd"},
		//PrintHash("../tests/PPU/ppu_vbl_nmi/rom_singles/10-even_odd_timing.nes"},

		{"../tests/PPU/vbl_nmi_timing/1.frame_basics.nes", 2075574, "567b5999"},
		{"../tests/PPU/vbl_nmi_timing/2.vbl_timing.nes", 1742482, "3dfd8260"},
		{"../tests/PPU/vbl_nmi_timing/3.even_odd_frames.nes", 1098303, "2f972610"},
		{"../tests/PPU/vbl_nmi_timing/4.vbl_clear_timing.nes", 1304968, "6dfbc004"},
		{"../tests/PPU/vbl_nmi_timing/5.nmi_suppression.nes", 2123375, "d16a86c7"},
		{"../tests/PPU/vbl_nmi_timing/6.nmi_disable.nes", 1354726, "457d8549"},
		{"../tests/PPU/vbl_nmi_timing/7.nmi_timing.nes", 1559500, "a0b6b2b1"},

		{"../tests/PPU/blargg_ppu_tests_2005.09.15b/palette_ram.nes", 178166, "a028c0ca"},
		{"../tests/PPU/blargg_ppu_tests_2005.09.15b/power_up_palette.nes", 178299, "a028c0ca"},
		//{"../tests/PPU/blargg_ppu_tests_2005.09.15b/sprite_ram.nes", 169720, "a028c0ca"},
		//PrintHash("../tests/PPU/blargg_ppu_tests_2005.09.15b/sprite_ram.nes"},
		//PrintHash("../tests/PPU/blargg_ppu_tests_2005.09.15b/vbl_clear_time.nes"},
		{"../tests/PPU/blargg_ppu_tests_2005.09.15b/vram_access.nes", 186572, "a028c0ca"},

		//PrintHash("../tests/PPU/nmi_sync/demo_ntsc.nes"},

		{"../tests/PPU/ppu_sprite_hit/rom_singles/01-basics.nes", 319189, "9841a902"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/02-alignment.nes", 289939, "675733ee"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/03-corners.nes", 186100, "197580da"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/04-flip.nes", 165301, "6303c2dc"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/05-left_clip.nes", 294721, "2ab6d048"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/06-right_edge.nes", 206910, "fda8b859"},
		//PrintHash("../tests/PPU/ppu_sprite_hit/rom_singles/07-screen_bottom.nes"},
		{"../tests/PPU/ppu_sprite_hit/rom_singles/08-double_height.nes", 186120, "c6b0913b"},
		//PrintHash("../tests/PPU/ppu_sprite_hit/rom_singles/09-timing.nes"},
		//PrintHash("../tests/PPU/ppu_sprite_hit/rom_singles/10-timing_order.nes"},

		//console.Run("../tests/PPU/color_test.nes");

		{"../tests/PPU/ppu_open_bus/ppu_open_bus.nes", 3046392, "6bcc0f25"},

		// Mappers
		{"../tests/holydiverbatman-bin-0.01/testroms/M0_P32K_C8K_V.nes", 859728, "56dfded7"},
		{"../tests/holydiverbatman-bin-0.01/testroms/M2_P128K_V.nes", 859756, "38f478c3"},
		{"../tests/holydiverbatman-bin-0.01/testroms/M3_P32K_C32K_H.nes", 3220777, "7859454d"}

		// Controller
		//console.Run("../tests/Controller/allpads.nes");
		//console.Run("../tests/Controller/read_joy3/count_errors.nes");
		//console.Run("../tests/Controller/read_joy3/count_errors_fast.nes");
		//console.Run("../tests/Controller/read_joy3/test_buttons.nes");
		//console.Run("../tests/Controller/read_joy3/thorough_test.nes");
	};
	//PrintHash("../tests/holydiverbatman-bin-0.01/testroms/M1_P128K.nes");
	//PrintHash("../tests/holydiverbatman-bin-0.01/testroms/M1_P128K_C32K.nes");
	//PrintHash("../tests/holydiverbatman-bin-0.01/testroms/M1_P128K_C32K_S8K.nes");
	//PrintHash("../tests/holydiverbatman-bin-0.01/testroms/M1_P128K_C32K_W8K.nes");

	std::vector<std::string> failedFilenames;
	for (auto it = ftrs.begin(); it != ftrs.end(); ++it) {
		if (!FunctionalTest(it->filename, it->memOffset,
			                it->startPC, it->successPC,
			                it->shouldRegIntr, it->shouldPrint)) {
			failedFilenames.push_back(it->filename);
		}
	}
	for (auto it = ntrs.begin(); it != ntrs.end(); ++it) {
		if (!Test(it->filename, it->numInstrs, it->desiredHash)) {
			failedFilenames.push_back(it->filename);
		}
	}
	std::cout << "All tests completed\n";
	if (failedFilenames.size() > 0) {
		std::cerr << "Failed: \n";
		for (auto it = failedFilenames.begin();
		     it != failedFilenames.end();
		     ++it) {
			std::cerr << (*it) << '\n';
		}
	}
	else {
		std::cout << "Passed all tests!\n";
	}
	return 0;
}
