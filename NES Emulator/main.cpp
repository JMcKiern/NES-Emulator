#include <iostream>
#include "FunctionalTest.h"
#include "Console.h"

int main() {
	if (false)
	{
		FunctionalTest("Tests/6502_functional_test.bin", 0x0, 0x400, 0x336d, false);
		FunctionalTest("Tests/6502_interrupt_test.bin", 0x0, 0x400, 0x06f5, true);
		FunctionalTest("Tests/timingtest-1.bin", 0x1000, 0x1000, 0x1000, false);
	}
	else
	{
		const std::string logFile = "logfile";
		Console console(logFile);
		// GAMES
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Donkey Kong Classics (U).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Ice Climber (USA, Europe).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mario Bros (JU).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Spelunker (USA).nes");

		// TESTS
		//	CPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Nestest\\nestest - jump to test.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\nestest.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_reads.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_oam.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_ppumem.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\1.Branch_Basics.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\2.Backward_Branch.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\3.Forward_Branch.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_apu.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_ppuio.nes");
		//
		//	PPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\color_test.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vbl_clear_time.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\palette_ram.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\sprite_ram.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vram_access.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\power_up_palette.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\ppu_vbl_nmi.nes");
		console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\01-vbl_basics.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\02-vbl_set_time.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\nmi_sync\\demo_ntsc.nes");
		
		int SystemReturnValue = console.Run();
	}

	char x;
	std::cin >> x;
}