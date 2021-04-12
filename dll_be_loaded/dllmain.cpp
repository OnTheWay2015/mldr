// Windows м╥нд╪Ч: 
#include <windows.h>
#include <iostream>

#include "dllmain.h"


void
testdll() {
		std::cout << "testdll() act!" << std::endl;
}



BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::cout << "DLL_PROCESS_ATTACH act!" << std::endl;
		break;
	case DLL_THREAD_ATTACH:
		std::cout << "DLL_THREAD_ATTACH act!" << std::endl;
		break;
	case DLL_THREAD_DETACH:
		std::cout << "DLL_THREAD_DETACH act!" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		std::cout << "DLL_PROCESS_DETACH act!" << std::endl;
		break;

	}
	return true;
}
