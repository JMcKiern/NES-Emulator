#include <iostream>
#include "FunctionalTest.h"
#include "NesTest.h"
#include "Console.h"

int main() {
	if (false) {
		// 6502
		FunctionalTest("Tests/6502_functional_test.bin", 0x0, 0x400, 0x336d, false, false);
		FunctionalTest("Tests/6502_interrupt_test.bin", 0x0, 0x400, 0x06f5, true, false);
		FunctionalTest("Tests/timingtest-1.bin", 0x1000, 0x1000, 0x1000, false, false);

		// CPU
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_reads.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Nestest\\nestest - jump to test.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\nestest.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\1.Branch_Basics.nes", 127975, "7d84fe2d");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\2.Backward_Branch.nes", 156241, "f3ddede5");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\branch_timing_tests\\3.Forward_Branch.nes", 156033, "ce51c7e6");

		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\1-cli_latency.nes");

		// PPU
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\ppu_vbl_nmi.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\01-vbl_basics.nes", 1666560, "a069cf23");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\02-vbl_set_time.nes", 1798437, "a05dbed9");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\03-vbl_clear_time.nes", 1707366, "c909c505");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\04-nmi_control.nes", 302984, "f75bbc3a");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\05-nmi_timing.nes", 2272532, "0566ccef");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\06-suppression.nes", 2173184, "6461c604");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\07-nmi_on_timing.nes", 1855652, "383b9dd3");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\08-nmi_off_timing.nes", 2265642, "2fb5e6ea");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\09-even_odd_frames.nes", 821446, "e91565bd");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_vbl_nmi\\rom_singles\\10-even_odd_timing.nes");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\1.frame_basics.nes", 2075574, "567b5999");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\2.vbl_timing.nes", 1742482, "3dfd8260");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\3.even_odd_frames.nes", 1098303, "2f972610");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\4.vbl_clear_timing.nes", 1304968, "6dfbc004");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\5.nmi_suppression.nes", 2123375, "d16a86c7");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\6.nmi_disable.nes", 1354726, "457d8549");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\vbl_nmi_timing\\7.nmi_timing.nes", 1559500, "a0b6b2b1");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\palette_ram.nes", 178166, "a028c0ca");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\power_up_palette.nes", 178299, "a028c0ca");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\sprite_ram.nes", 169720, "6f4afa73"); // Fails because test is bad
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vbl_clear_time.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\blargg_ppu_tests_2005.09.15b\\vram_access.nes", 186572, "a028c0ca");

		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\nmi_sync\\demo_ntsc.nes");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\01-basics.nes", 319189, "9841a902");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\02-alignment.nes", 289939, "675733ee");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\03-corners.nes", 186100, "197580da");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\04-flip.nes", 165301, "6303c2dc");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\05-left_clip.nes", 294721, "2ab6d048");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\06-right_edge.nes", 206910, "fda8b859");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\07-screen_bottom.nes");
		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\08-double_height.nes", 186120, "c6b0913b");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\09-timing.nes");
		//PrintHash("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_sprite_hit\\rom_singles\\10-timing_order.nes");

		Test("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\ppu_open_bus\\ppu_open_bus.nes", 3046392, "6bcc0f25");
	}
	{
		const std::string logFileName = "logfile.log";
		Console console(logFileName);
		// GAMES
		console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Donkey Kong Classics (U).nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mega Man (U).nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Super Mario Bros. 3 (U) (PRG1) [!].nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Super Mario Bros. (JU) (PRG0) [!].nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Ice Climber (USA, Europe).nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mario Bros. (JU) [!].nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Spelunker (USA).nes");

		// TESTS
		//	CPU
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_oam.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_dummy_writes\\cpu_dummy_writes_ppumem.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_apu.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_exec_space\\test_cpu_exec_space_ppuio.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\2-nmi_and_brk.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\3-nmi_and_irq.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\4-irq_and_dma.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\CPU\\cpu_interrupts_v2\\rom_singles\\5-branch_delays_irq.nes");

		//	PPU
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\PPU\\color_test.nes");

		// Controller
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\allpads.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\count_errors.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\count_errors_fast.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\test_buttons.nes");
		//console.Run("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Tests\\Controller\\read_joy3\\thorough_test.nes");
	}
}