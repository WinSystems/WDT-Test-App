// WDTPetApp.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "wdtDLL.h"
#include <Windows.h>
#include <conio.h>
#include <process.h>

//reads time every second and prints timeout remaining
void countThread(void *par){
	unsigned int reading;
	while (1){
		if (ReadTimerValue(&reading))
			printf("Failed to read");
		printf("Current timeout: %u\n", reading);
		Sleep(1000);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	int errCode = 0;
	unsigned int timeout = 30;
	int timeunit = SECONDS;
	unsigned int reading;

	//Pet app --- Require key press to reset timer to 30 seconds. If key press is q then it will close the driver and quit.
	try{
		//Get access to driver, and check status
		if (errCode = InitializeSession()) {
			printf("Failed to open device\n");
			throw errCode;
		}

		//enable WDT in seconds mode with a timeout of 30, and check success
		if (errCode = EnableTimer(timeout, timeunit)) { 
			printf("Failed to start WDT with the following timeout and min_sec args %c %d\n", timeout, timeunit);
			throw errCode;
		}
		else{
			printf("Set timer to %d seconds\n", timeout);
		}


		_beginthread(countThread, 0, NULL);


		//Loops writing 30 second timeout and reading timeout.
		//Waits for keypress to write 30 second timeout
		//q closes the driver and breaks loop
		while (1){
			if (errCode = WriteTimerValue(timeout)){
				printf("Failed to write time to driver\n");
				throw errCode;
			}
			if (errCode = ReadTimerValue(&reading)) {
				printf("Failed to read current timeout size\n");
				throw errCode;
			}
			printf("Current nonthread timeout: %u\n", reading);
			printf("Press key to reset timer to %d seconds. Press q to close WDT and quit application.\n", reading, timeout);
			_kbhit();
			if (_getch() == 'q'){
				if (errCode = CloseSession()) {
					printf("Failed to close Driver.\n");
					throw errCode;
				}
				printf("WDT closed. Exiting\n");
				break;
			}

		}

	}
	catch (int err){
		switch (err) {
		case DRIVER_ERROR: std::cout << "Failed to communicate with driver\n";
			break;
		case INVALID_HANDLE: std::cout << "Failed to obtain handle to driver\n";
			break;
		case INVALID_PARAMETER: std::cout << "Bad parameter\n";
			break;
		}
		system("pause");
	}
	return 0;
}

