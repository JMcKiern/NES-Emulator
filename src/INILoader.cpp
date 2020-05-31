#include <string>
#include <iostream>
#include "../3rdparty/inih/cpp/INIReader.h"
#include "INILoader.h"

int INILoader::StringToGLFWKeyCode(std::string s) {
	if (stringToGLFWKeyCode.count(s) == 0) {
		return -1;
	}
	return stringToGLFWKeyCode[s];
}


void INILoader::ParseINIFile(std::string filename, IO* io) {
	INIReader reader(filename);

	if (reader.ParseError() < 0) return;

	if (reader.HasSection("Controls")) {
		for (int playerNum=1; playerNum<=2; ++playerNum) {
			std::string playerStr = "player" + std::to_string(playerNum);
			std::map<int, int> keymap;
			bool successful = true;
			for (int btnNum=0; btnNum<numBtns; ++btnNum) {
				std::string setting = playerStr + '.' + btnNames[btnNum];
				std::string key = reader.Get("Controls", setting, "");
				if (key == "") {
					std::cout << "INI parse error: Missing " << setting << '\n';
					std::cout << "Skipping player " << std::to_string(playerNum)
						<< " controller settings\n";
					successful = false;
					break;
				}
				int glfwKeyCode = StringToGLFWKeyCode(key);
				if (glfwKeyCode == -1) {
					std::cout << "INI parse error: Unknown key " << key << '\n';
					std::cout << "Skipping player " << std::to_string(playerNum)
						<< " controller settings\n";
					successful = false;
					break;
				}
				keymap.insert(std::pair<int, int>(glfwKeyCode, btnNum));
			}
			if (successful) {
				io->SetContollerKeyMap(playerNum-1, keymap);
			}
		}
	}
}
