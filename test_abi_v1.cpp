#include <iostream>
#include "v1.hpp"

using namespace test;

int main()
{
	int const ret = test_function();
	if (ret != 1) {
		std::cerr << "FAILED: " << ret << " != 1" << std::endl;
		return -1;
	}
	else {
		std::cerr << "OK: " << ret << std::endl;
		return 0;
	}
}


