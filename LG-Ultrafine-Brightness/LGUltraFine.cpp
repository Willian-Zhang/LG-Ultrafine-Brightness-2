#include "LGUltraFine.h"

const uint16_t vendor_id = 0x43e;
const uint16_t product_id = 0x9a40;
const uint16_t max_brightness = 0xd2f0;
const uint16_t min_brightness = 0x0190;

const std::vector<uint16_t> LGUltraFine::small_steps = {
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

const std::vector<uint16_t> LGUltraFine::big_steps = {
	0x0190, 0x021f, 0x02e2, 0x03eb,
	0x0553, 0x073c, 0x09d5, 0x0d5c,
	0x1227, 0x18aa, 0x2184, 0x2d8b,
	0x3de2, 0x5415, 0x7240, 0x9b3d,
	0xd2f0,
};

uint16_t LGUltraFine::next_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end) {
		auto mid = start + (end - start) / 2;
		if (steps[mid] > val) {
			end = (int) mid;
		}
		else {
			start = (int) mid;
		}
	}
	return steps[end];
}

uint16_t LGUltraFine::prev_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end) {
		auto mid = start + (end - start) / 2;
		if (steps[mid] >= val) {
			end = (int) mid;
		}
		else {
			start = (int)mid;
		}
	}
	return steps[start];
}

uint16_t LGUltraFine::get_brightness() {
	uint8_t data[7] = { 0 };
	int res = hid_get_feature_report(this->handle, data, sizeof(data));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls", hid_error(this->handle));
	}
	return data[1] + (data[2] << 8);
}

void LGUltraFine::set_brightness(uint16_t val) {
	uint8_t data[7] = {
		0x00,
		uint8_t(val & 0x00ff),
		uint8_t((val >> 8) & 0x00ff),
		0x00, 0x00, 0x00, 0x00 };
	int res = hid_send_feature_report(this->handle, data, sizeof(data));
	if (res < 0) {
		printf("Unable to set a feature report.\n");
		printf("%ls", hid_error(this->handle));
	}
}

LGUltraFine::LGUltraFine()
{
	//struct hid_device_info *devs, *cur_dev;
	if (hid_init())
		throw std::runtime_error("Cannot init hid device list");

	this->handle = hid_open(vendor_id, product_id, NULL);

	if (!this->handle)
		throw std::runtime_error("Unable to open device");
	
}


LGUltraFine::~LGUltraFine()
{
	hid_close(this->handle);
	hid_exit();
}
