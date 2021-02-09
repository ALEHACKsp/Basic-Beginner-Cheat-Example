#include "main.h"

//To Learn c++ there are some great websites like this -https://www.learncpp.com/
//For some great resources on where to start -https://guidedhacking.com/threads/ghb0-game-hacking-bible-introduction.14450/

void HealthToggle()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
			InfiniteHealth = !InfiniteHealth;//This will make a toggle for our infinite health!
	}
}

int main()//Our entry, this get called when we run the program
{
	start:			//Making a area to refer to if we can't find the HWND

	HWND hwnd = FindWindow(NULL, "AssaultCube");//Get the HWND of AssualtCube
	while (!hwnd)//while the HWND of Assualt Cube is NULL
	{
		printf("Failed to get HWND to Assualt Cube... retrying\n");
		Sleep(200);//Sleep for 200ms
		goto start;//Go to the top of our code to retry getting the HWND
	}


	GetWindowThreadProcessId(hwnd, &ProcID);
	if (!ProcID)
	{
		printf("couldn't find the ProcessID of Assualt Cube\n");
	}

	std::thread HlthToggle(HealthToggle);//Creates our thread for toggling infinite health
	ACHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcID);//Get the handle and open access to Assualt Cube
	std::cout << "Press INSERT on your keyboard to enable infinite health or END to close the cheat" << std::endl;

	while (true)
	{
		if (InfiniteHealth)//if we have our InfiniteHealth bool to 'true'
		{
			auto LocalPlayer = RPM<LPVOID*>(ACHandle, Offsets::LocalPlayer);//Get The LocalPlayer
			int LocalHealth = RPM<int>(ACHandle, Offsets::PlayerHP);//Get the Local Player Health as an int
			WPM(ACHandle, LocalHealth, 999);//Setting health to 999
			std::cout << "LocalPlayer Offset: " << LocalPlayer << std::endl;
			std::cout << "Player Health: " << LocalHealth << std::endl;
		}
		if (GetAsyncKeyState(VK_END) & 1)
			break;//Breaks out of our loop and closes the application
	}

	return 0;//this 'return 0' is optional, with haveing main as an 'int' it would be added automaticly if it wasnt there.
}