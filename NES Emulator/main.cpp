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
	{
		const std::string logFile = "logfile";
		Console console(logFile);
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Nestest\\nestest.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Donkey Kong Classics (U).nes");
		console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\color_test.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\blargg_ppu_tests_2005.09.15b\\vbl_clear_time.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\blargg_ppu_tests_2005.09.15b\\palette_ram.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\blargg_ppu_tests_2005.09.15b\\sprite_ram.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\blargg_ppu_tests_2005.09.15b\\vram_access.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\blargg_ppu_tests_2005.09.15b\\power_up_palette.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Ice Climber (USA, Europe).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Nestest\\nestest - jump to test.nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Mario Bros (JU).nes");
		//console.LoadINES("C:\\Users\\Jack\\Desktop\\Projects\\NESEM\\Games\\Spelunker (USA).nes");
		
		int SystemReturnValue = console.Run();
	}

	char x;
	std::cin >> x;
}