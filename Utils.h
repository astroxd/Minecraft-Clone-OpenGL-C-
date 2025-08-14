#ifndef UTILS_H
#define UTILS_H

#include <chrono>

namespace Utils {
	static std::chrono::milliseconds GetMs() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
	}
}


#endif 
