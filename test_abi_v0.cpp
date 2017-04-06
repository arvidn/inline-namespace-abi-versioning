#include <iostream>
#include "v0.hpp"

using namespace test;

int main()
{
	int const ret = test_function();
	if (ret != 0) {
		std::cerr << "FAILED: " << ret << " != 0" << std::endl;
		return -1;
	}
	else {
		std::cerr << "OK: " << ret << std::endl;
		return 0;
	}
}

