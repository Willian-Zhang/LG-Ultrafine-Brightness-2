#pragma once
#include <vector>
#include <exception>
#include "hidapi.h"

class LGUltraFineException : public std::runtime_error {};
class LGConnectError : LGUltraFineException {};
class LGCommunicationError : LGUltraFineException {};

using std::vector;
class LGUltraFine{
private:
	hid_device* handle;
	inline uint16_t get_brightness();
	inline void  set_brightness(uint16_t val);
public:
	LGUltraFine();
	~LGUltraFine();

	void set_level_up(const float step=0.075);
	void set_level_down(const float step = 0.075);
	void set_level(const float level);
	float get_level();

public:
	static const std::vector<uint16_t> small_steps;

	static const std::vector<uint16_t> big_steps;
};


