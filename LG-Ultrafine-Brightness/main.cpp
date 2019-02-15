#include <iostream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <iterator>
#include <string> 

#include "LGUltrafine.h"
#include "wincppextensions.h"

#include <windows.h>

const auto is_console = GetConsoleWindow() != nullptr;

#include <stdio.h>
void PressEnterToContinue(){
	if (is_console) {
		int c;
		printf("Press ENTER to continue... ");
		fflush(stdout);
		do c = getchar(); while ((c != '\n') && (c != EOF));
	}	
}

const auto help_string = "\
Run application as `brightness.exe <option>` or `brightness.<option>.exe`\n\
	Valid <option> arguments: - + [ ] 0 9 p\n\
		'-' or '+' to adjust brightness.\n\
		'[' or: ']' to fine tune.\n\
		'p' to print infomation\n\
		'0' or '9' to use the minimium or maximum brightness\n\
";
std::string brightness_to_string(uint16_t brightness) {
	return std::to_string( int((float(brightness) / 54000) * 100.0) );
}


void win_dialog(const char* text, const char* title="LG Ultrafine Brightness") {
	if (is_console) {
		std::cout << text;
	}else{
		MessageBox(NULL, text, title, MB_OK);
	}
}
void win_dialog(const std::string text, const char* title = "LG Ultrafine Brightness") {
	win_dialog(text.c_str(), title);
}
char get_option_from_name(char* const name) {
	std::reverse_iterator<const char*> last(name);
	for (std::reverse_iterator<const char*> c(name + strlen(name)); c != last; ++c) {
		// find first dot
		if (*c == '.') {
			++c;
			if (c != last) {
				const auto option = *c;
				// dot verification
				++c;
				if (c != last && *c == '.') {
					return option;
				}
			}
			break;
		}
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	auto ultraFine = std::make_unique<LGUltraFine>();


	auto brightness = ultraFine->get_brightness();

	char option = 'h';

	if (argc < 2) {
		const auto name = argv[0];
		const auto candidate = get_option_from_name(name);
		if (!candidate)
			win_dialog("ERROR: You need at least one argument.");
		else
			option = candidate;
	}
	else {
		option = *argv[1];
	}

	std::stringstream ss;
	auto need_confirm_on_exit = false;
	switch (option) {
	case '+':
		brightness = ultraFine->next_step(brightness, LGUltraFine::big_steps);
		ultraFine->set_brightness(brightness);
		break;
	case '-':
		brightness = ultraFine->prev_step(brightness, LGUltraFine::big_steps);
		ultraFine->set_brightness(brightness);
		break;
	case '[':
		brightness = ultraFine->prev_step(brightness, LGUltraFine::small_steps);
		ultraFine->set_brightness(brightness);
		break;
	case ']':
		brightness = ultraFine->next_step(brightness, LGUltraFine::small_steps);
		ultraFine->set_brightness(brightness);
		break;
	// TODO: 
	case 'p':
		ss << "Current brightness = " << brightness_to_string(brightness) << "    \r" << std::endl;
		win_dialog(ss.str());
		need_confirm_on_exit = true;
		break;
	case 'h':
		ss << "Current brightness = " << brightness_to_string(brightness) << "    \r" << std::endl;
		ss << help_string;
		win_dialog(ss.str());
		need_confirm_on_exit = true;
		break;
	default:
		ss << "Error: invalid argument!" << std::endl;
		ss << help_string;
		win_dialog(ss.str());
		need_confirm_on_exit = true;
	}

	if (need_confirm_on_exit) {
		PressEnterToContinue();
	}
	return 0;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	int argCount;
	
	auto szArgList = CommandLineToArgvA(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		MessageBox(NULL, "Unable to parse command line", "Error", MB_OK);
		return 10;
	}

	auto code = main(argCount, szArgList);

	LocalFree(szArgList);

	return code;
}
