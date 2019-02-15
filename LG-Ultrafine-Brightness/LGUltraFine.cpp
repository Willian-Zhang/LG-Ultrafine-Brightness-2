#include "LGUltraFine.h"
#include <algorithm>
using std::runtime_error;

const uint16_t vendor_id = 0x43e;
const uint16_t product_id = 0x9a40;

const uint16_t max_brightness = 0xd2f0;
const uint16_t min_brightness = 0x0190;
const uint16_t min_max_brightness_distance = max_brightness - min_brightness;

LGUltraFine::LGUltraFine()
{
	// Willian: I don't know what this does
	//struct hid_device_info *devs, *cur_dev;
	if (hid_init())
		throw runtime_error("Cannot init hid device list");

	this->handle = hid_open(vendor_id, product_id, NULL);

	if (!this->handle)
		throw runtime_error("Unable to open device");

}
LGUltraFine::~LGUltraFine()
{
	hid_close(this->handle);
	hid_exit();
}

inline const uint16_t brightness_amount_from_step(const float step) {
	return min_max_brightness_distance * step;
}
inline const uint16_t brightness_from_level(const float level) {
	return (uint16_t)(min_max_brightness_distance * level + min_brightness);
}
inline const float level_from_brightness(const uint16_t brightness) {
	return 1.0f * (brightness - min_brightness) / min_max_brightness_distance;
}
void LGUltraFine::set_level(const float level) {
	const auto now = brightness_from_level(level);
	this->set_brightness(now);
}
void LGUltraFine::set_level_up(const float step)
{
	uint16_t now = (uint16_t)(this->get_brightness() + brightness_amount_from_step(step));
	if (now > max_brightness)
		now = max_brightness;
	this->set_brightness(now);
}

void LGUltraFine::set_level_down(const float step)
{
	uint16_t now = std::max(this->get_brightness() - brightness_amount_from_step(step), 0);
	if (now < min_brightness)
		now = min_brightness;
	this->set_brightness(now);
}

float LGUltraFine::get_level()
{
	return level_from_brightness(this->get_brightness());
}

#pragma low level
uint16_t inline LGUltraFine::get_brightness() {
	uint8_t data[7] = { 0 };
	int res = hid_get_feature_report(this->handle, data, sizeof(data));
	if (res < 0)
		throw runtime_error((char *)hid_error(this->handle));
	//{
	//	printf("Unable to get a feature report.\n");
	//	printf("%ls", hid_error(this->handle));
	//}
	return data[1] + (data[2] << 8);
}

void inline LGUltraFine::set_brightness(uint16_t val) {
	uint8_t data[7] = {
		0x00,
		uint8_t(val & 0x00ff),
		uint8_t((val >> 8) & 0x00ff),
		0x00, 0x00, 0x00, 0x00 };
	int res = hid_send_feature_report(this->handle, data, sizeof(data));
	if (res < 0)
		throw runtime_error((char *)hid_error(this->handle));
	//{
	//	printf("Unable to set a feature report.\n");
	//	printf("%ls", hid_error(this->handle));
	//}
}

