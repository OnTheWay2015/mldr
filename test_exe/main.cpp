#include <iostream>
#include <windows.h>


int main(int argc, char* argv[]) 
{

  auto hi = GetModuleHandle(NULL);
	std::cout << "test_exe main act! hins:" <<  hi << std::endl;

	for (int i = 0; i < argc;i++) {
		std::cout << "argc["<<i<<"]:" <<  argv[i] << std::endl;
	}
  return 0;

}


