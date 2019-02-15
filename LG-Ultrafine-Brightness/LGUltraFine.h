#pragma once
#include <vector>
#include "hidapi.h"
using std::vector;

class LGUltraFine{
private:
	hid_device* handle;
public:
	LGUltraFine();
	~LGUltraFine();
	uint16_t next_step(uint16_t val, const vector<uint16_t> &steps);
	uint16_t prev_step(uint16_t val, const vector<uint16_t> &steps);
	uint16_t get_brightness();
	void set_brightness(uint16_t val);

public:
	static const std::vector<uint16_t> small_steps;

	static const std::vector<uint16_t> big_steps;
};


