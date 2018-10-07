#include <iostream>
#include "FunctionalTest.h"
#include "NesTest.h"
#include "Console.h"

int main() {
	if (true) {
		// 6502
		FunctionalTest("Tests/6502_functional_test.bin", 0x0, 0x400, 0x336d, false, false);
		FunctionalTest("Tests/6502_interrupt_test.bin", 0x0, 0x400, 0x06f5, true, false);
		FunctionalTest("Tests/timingtest-1.bin", 0x1000, 0x1000, 0x1000, false, false);

		// CPU
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_reads.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Nestest\\nestest - jump to test.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\nestest.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\1.Branch_Basics.nes", 200000, "e809688d");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\2.Backward_Branch.nes", 200000, "95e4bb25");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\3.Forward_Branch.nes", 200000, "7182e822");

		// PPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\ppu_vbl_nmi.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\01-vbl_basics.nes", 1700000, "16ae22ab");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\02-vbl_set_time.nes", 2000000, "e342cfc9");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\03-vbl_clear_time.nes", 1800000, "146791c5");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\04-nmi_control.nes", 300000, "42f443a6");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\05-nmi_timing.nes", 2300000, "a109edc7");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\06-suppression.nes", 2200000, "23c7bfc8");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\07-nmi_on_timing.nes", 1900000, "8b8c8f5b");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\08-nmi_off_timing.nes", 2300000, "f64eda96");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\09-even_odd_frames.nes", 900000, "0244a41d");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\10-even_odd_timing.nes");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\1.frame_basics.nes", 2100000, "df5b0009");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\2.vbl_timing.nes", 1800000, "ef3b53b4");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\3.even_odd_frames.nes", 1100000, "483d0de4");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\4.vbl_clear_timing.nes", 1300000, "d58f9708");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\5.nmi_suppression.nes", 2200000, "9390f23f");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\6.nmi_disable.nes", 1400000, "5b359c39");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\7.nmi_timing.nes", 1600000, "5fcd7101");
		
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\palette_ram.nes", 200000, "13dc4776");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\power_up_palette.nes", 200000, "13dc4776");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\sprite_ram.nes", 200000, "13dc4776");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vbl_clear_time.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vram_access.nes");

		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\nmi_sync\\demo_ntsc.nes");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\01-basics.nes", 319189, "c7b5608e");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\02-alignment.nes", 289939, "3b3ed10a");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\03-corners.nes", 186100, "4c9b3486");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\04-flip.nes", 165301, "ceb0a080");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\05-left_clip.nes", 294721, "0619e27c");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\06-right_edge.nes", 206910, "2ad906c9");
		PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\07-screen_bottom.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\08-double_height.nes", 186120, "b2b60be3");
		PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\09_timing.nes");
		PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\10_timing_order.nes");

		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_open_bus\\ppu_open_bus.nes");

	}
	{
		const std::string logFileName = "logfile.log";
		Console console(logFileName);
		// GAMES
		console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Donkey Kong Classics (U).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mega Man (U).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Super Mario Bros. (JU) (PRG0) [!].nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Ice Climber (USA, Europe).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mario Bros. (JU) [!].nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Spelunker (USA).nes");

		// TESTS
		//	CPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_oam.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_ppumem.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_apu.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_ppuio.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\1-cli_latency.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\2-nmi_and_brk.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\3-nmi_and_irq.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\4-irq_and_dma.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\5-branch_delays_irq.nes");

		//	PPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\color_test.nes");

		// Controller
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\allpads.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\count_errors.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\count_errors_fast.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\test_buttons.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\thorough_test.nes");

		console.Run();
	}
}