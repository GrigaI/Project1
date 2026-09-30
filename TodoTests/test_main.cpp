#include <gtest/gtest.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}