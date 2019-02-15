#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <conio.h>
#include <iterator>
#include <string> 

#include <windows.h>

#include "LGUltrafine.h"
#include "wincppextensions.h"


const auto is_console = GetConsoleWindow() != nullptr;


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
	Valid <option> arguments: - + [ ] 0 9 p h\n\
		'-' or '+' to adjust brightness.\n\
		'[' or: ']' to fine tune.\n\
		'p' to print infomation\n\
		'0' ~ '9' to use the 0% thru 100% level brightness\n\
		'h' to show this info\
";

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
inline const std::string string_from_level(const float level) {
	return std::to_string(int(level * 100));
}
int main(int argc, char *argv[]) {
	auto ultraFine = std::make_unique<LGUltraFine>();
	auto should_confirm_on_exit = false;

	std::stringstream ss;
	char option = 'h';
	if (argc < 2) {
		const auto name = argv[0];
		const auto candidate = get_option_from_name(name);
		if (!candidate) {
			ss << "Current brightness: " << string_from_level(ultraFine->get_level()) << "%    \r" << std::endl;
			win_dialog(ss.str());
			should_confirm_on_exit = true;
		}
		else
			option = candidate;
	}
	else {
		option = *argv[1];
	}

	switch (option) {
	case '+':
		ultraFine->set_level_up();
		break;
	case '-':
		ultraFine->set_level_down();
		break;
	case '[':
		ultraFine->set_level_up(0.02);
		break;
	case ']':
		ultraFine->set_level_down(0.02);
		break;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		ultraFine->set_level( 0.1f * (option - '0') / 0.9f );
		break;
	case 'p':
		ss << "Current brightness: " << string_from_level(ultraFine->get_level()) << "%    \r" << std::endl;
		win_dialog(ss.str());
		should_confirm_on_exit = true;
		break;
	case 'h':
		win_dialog(help_string);
		should_confirm_on_exit = true;
		break;
	default:
		ss << "Error: invalid argument!" << std::endl;
		ss << help_string;
		win_dialog(ss.str());
		should_confirm_on_exit = true;
	}

	if (should_confirm_on_exit) {
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
