// WDTPetApp.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "wdtDLL.h"
#include <Windows.h>
#include <conio.h>
#include <process.h>

//reads time every second and prints timeout remaining
void countThread(void *)
{
	unsigned int reading;
	while (1)
	{
		if (ReadTimerValue(&reading))
			std::cout<<"Failed to read\n";

		std::cout << "Current timeout: "<<reading<<"\n";
		Sleep(1000); //1 second sleep
	}			
}


int _tmain(int argc, _TCHAR* argv[])
{
	int errCode = 0;
	unsigned int timeout = 30;
	int timeunit = SECONDS;

	//Pet app --- Require key press to reset timer to 30 seconds. If key press is q then it will close the driver and quit.
	try
	{
		//Get access to driver, and check status
		if (errCode = InitializeSession()) 
		{
			std::cout<<"Failed to open device\n";
			throw errCode;
		}

		//enable WDT in seconds mode with a timeout of 30, and check success
		if (errCode = EnableTimer(timeout, timeunit)) 
			{ 
				std::cout << "Failed to start WDT with the following timeout and min_sec args " << timeout << " " << timeunit << "\n"; 
				throw errCode;
			}
		else
			{
				std::cout<<"Set timer to "<<timeout<<" seconds\n";
			}


		_beginthread(countThread, 0, NULL);


		//Loops writing 30 second timeout and reading timeout.
		//Waits for keypress to write 30 second timeout
		//q closes the driver and breaks loop
		while (1)
		{
			if (errCode = WriteTimerValue(timeout))
			{
				std::cout<<"Failed to write time to driver\n";
				throw errCode;
			}

			std::cout<<"Press key to reset timer to "<<timeout<<" seconds. Press q to close WDT and quit application.\n";
			
			_kbhit();
			if (_getch() == 'q')
			{
				if (errCode = CloseSession()) 
				{
					std::cout<<"Failed to close Driver.\n";
					throw errCode;
				}
				std::cout<<"WDT closed. Exiting\n";
				break;
			}

		}

	}
	catch (int err)
	{
		switch (err) 
		{
			case DRIVER_ERROR:
			{
				std::cout << "Failed to communicate with driver\n";
				break;
			}
			case INVALID_HANDLE:
			{
				std::cout << "Failed to obtain handle to driver\n";
				break;
			}
			case INVALID_PARAMETER:
			{
				std::cout << "Bad parameter\n";
				break;
			}
			default :
				std::cout << "Unkown error!\n";
		}
		system("pause");
	}
	return 0;
}

