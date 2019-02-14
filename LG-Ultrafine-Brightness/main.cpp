#include <iostream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <iterator>
#include <string> 
#include "hidapi.h"

using std::vector;
const uint16_t vendor_id = 0x43e;
const uint16_t product_id = 0x9a40;
const uint16_t max_brightness = 0xd2f0;
const uint16_t min_brightness = 0x0190;

const std::vector<uint16_t> small_steps = {
	0x0190, 0x01af, 0x01d2, 0x01f7,
	0x021f, 0x024a, 0x0279, 0x02ac,
	0x02e2, 0x031d, 0x035c, 0x03a1,
	0x03eb, 0x043b, 0x0491, 0x04ee,
	0x0553, 0x05c0, 0x0635, 0x06b3,
	0x073c, 0x07d0, 0x086f, 0x091b,
	0x09d5, 0x0a9d, 0x0b76, 0x0c60,
	0x0d5c, 0x0e6c, 0x0f93, 0x10d0,
	0x1227, 0x1399, 0x1529, 0x16d9,
	0x18aa, 0x1aa2, 0x1cc1, 0x1f0b,
	0x2184, 0x2430, 0x2712, 0x2a2e,
	0x2d8b, 0x312b, 0x3516, 0x3951,
	0x3de2, 0x42cf, 0x4822, 0x4de1,
	0x5415, 0x5ac8, 0x6203, 0x69d2,
	0x7240, 0x7b5a, 0x852d, 0x8fc9,
	0x9b3d, 0xa79b, 0xb4f5, 0xc35f,
	0xd2f0,
};
const std::vector<uint16_t> big_steps = {
	0x0190, 0x021f, 0x02e2, 0x03eb,
	0x0553, 0x073c, 0x09d5, 0x0d5c,
	0x1227, 0x18aa, 0x2184, 0x2d8b,
	0x3de2, 0x5415, 0x7240, 0x9b3d,
	0xd2f0,
};

uint16_t next_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end) {
		auto mid = start + (end - start) / 2;
		if (steps[mid] > val) {
			end = mid;
		}
		else {
			start = mid;
		}
	}
	return steps[end];
}

uint16_t prev_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end) {
		auto mid = start + (end - start) / 2;
		if (steps[mid] >= val) {
			end = mid;
		}
		else {
			start = mid;
		}
	}
	return steps[start];
}

uint16_t get_brightness(hid_device *handle) {
	uint8_t data[7] = { 0 };
	int res = hid_get_feature_report(handle, data, sizeof(data));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls", hid_error(handle));
	}
	return data[1] + (data[2] << 8);
}

void set_brightness(hid_device *handle, uint16_t val) {
	uint8_t data[7] = {
		0x00,
		uint8_t(val & 0x00ff),
		uint8_t((val >> 8) & 0x00ff),
		0x00, 0x00, 0x00, 0x00 };
	int res = hid_send_feature_report(handle, data, sizeof(data));
	if (res < 0) {
		printf("Unable to set a feature report.\n");
		printf("%ls", hid_error(handle));
	}
}

void PressEnterToContinue(){
	std::cout << "Press ENTER to continue... " << std::flush;
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}

const auto help_string = """Run application as `brightness.exe <option>` or `brightness.<option>.exe`\n\
	valid <option> argument: - + [ ] 0 9 p\n\
	'-' or '+' to adjust brightness.\n\
	'[' or: ']' to fine tune.\n\
	'p' to print infomation\n\
	'0' or '9' to use the minimium or maximum brightness\n\
""";
std::string brightness_to_string(uint16_t brightness) {
	return std::to_string( int((float(brightness) / 54000) * 100.0) );
}

#include <windows.h>
void win_dialog(const char* text,const char* title="LG Ultrafine Brightness") {
	if (GetConsoleWindow()) {
		std::cout << text;
	}else{
		MessageBox(NULL, text, title, MB_OK);
	}
}
void win_dialog(const std::string text, const char* title = "LG Ultrafine Brightness") {
	win_dialog(text.c_str(), title);
}

int main(int argc, char *argv[]) {
	hid_device *handle;
	struct hid_device_info *devs, *cur_dev;

	if (hid_init())
		return -1;

	handle = hid_open(vendor_id, product_id, NULL);
	if (!handle) {
		win_dialog("unable to open device\n");
		return 1;
	}

	auto brightness = get_brightness(handle);

	char option = 'h';

	if (argc < 2) {
		const auto name = argv[0];

		bool has_arg = false;
		std::reverse_iterator<const char*> last(name);
		for (std::reverse_iterator<const char*> c(name + strlen(name)); c != last; ++c) {
			// find first dot
			if (*c == '.') {
				++c;
				if (c != last) {
					const auto option_candidate = *c;
					// dot verification
					++c;
					if (c != last && *c == '.') {
						option = option_candidate;
						has_arg = true;
					}
				}
				break;
			}
		}
		if (!has_arg) {
			win_dialog("ERROR: You need at least one argument.");
		}
	}
	else {
		option = *argv[1];
	}

	std::stringstream ss;
	auto need_confirm_on_exit = false;
	switch (option) {
	case '+':
		brightness = next_step(brightness, big_steps);
		set_brightness(handle, brightness);
		break;
	case '-':
		brightness = prev_step(brightness, big_steps);
		set_brightness(handle, brightness);
		break;
	case '[':
		brightness = prev_step(brightness, small_steps);
		set_brightness(handle, brightness);
		break;
	case ']':
		brightness = next_step(brightness, small_steps);
		set_brightness(handle, brightness);
		break;
	case '0':
		brightness = min_brightness;
		set_brightness(handle, brightness);
		break;
	case '9':
		brightness = max_brightness;
		set_brightness(handle, brightness);
		break;
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

	hid_close(handle);
	hid_exit();

	if (need_confirm_on_exit) {
		PressEnterToContinue();
	}
	return 0;
}



PCHAR*
CommandLineToArgvA(
	PCHAR CmdLine,
	int* _argc
)
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(CmdLine);
	i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len + 2) * sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv) + i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while (a = CmdLine[i]) {
		if (in_QM) {
			if (a == '\"') {
				in_QM = FALSE;
			}
			else {
				_argv[j] = a;
				j++;
			}
		}
		else {
			switch (a) {
			case '\"':
				in_QM = TRUE;
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				in_SPACE = FALSE;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (in_TEXT) {
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = FALSE;
				in_SPACE = TRUE;
				break;
			default:
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = FALSE;
				break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
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
