// Test.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "wdtDLL.h"
#include <Windows.h>
#include <conio.h>


/*
Tests that driver error is thrown for all functions when WDT is uninitialized
*/
int testUninit(){ 
	unsigned int reading;
	int errCode = EnableTimer(0, 0); //checking fail codes for driver not initialized
	if (errCode != DRIVER_ERROR){
		std::cout << "Expected driver error for enable. Recieved error: " << errCode << "\n";
		return 1;
	}
	errCode = ReadTimerValue(&reading);
	if (errCode != DRIVER_ERROR){
		std::cout << "Expected driver error for read. Recieved error: " << errCode << "\n";
		return 1;
	}
	errCode = WriteTimerValue(0);
	if (errCode != DRIVER_ERROR){
		std::cout << "Expected driver error for read. Recieved error: " << errCode << "\n";
		return 1;
	}
	errCode = CloseSession();
	if (errCode != DRIVER_ERROR){
		std::cout << "Expected driver error for read. Recieved error: " << errCode << "\n";
		return 1;
	}
	return 0;
}

/*
Test Bounds checking for Write and Enable
*/
int testBounds(){
	int errCode=0;

	if (errCode = InitializeSession()) {
		printf("Failed to open device\n");
		return 1;
	}
	if (errCode = EnableTimer(257, 0)) {
		if (errCode != INVALID_PARAMETER){
			std::cout << "Expected bad parameter for enabletimer with bad timeout. Recieved error: " << errCode << "\n";
			CloseSession();
			return 1;
		}
	}
	if (errCode = EnableTimer(0, 2)) {
		if (errCode != INVALID_PARAMETER){
			std::cout << "Expected bad parameter for enabletimer with bad time unit. Recieved error: " << errCode << "\n";
			CloseSession();
			return 1;
		}
	}
	if (errCode = WriteTimerValue(257)){
		if (errCode != INVALID_PARAMETER){
			std::cout << "Expected bad parameter for writetimer with bad timeout. Recieved error: " << errCode << "\n";
			CloseSession();
			return 1;
		}
	}
	if (errCode = ReadTimerValue(NULL)) {
		if (errCode != INVALID_PARAMETER){
			std::cout << "Expected bad parameter for enabletimer with bad time unit. Recieved error: " << errCode << "\n";
			CloseSession();
			return 1;
		}
	}
	CloseSession();

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int errCode= 0;
	unsigned int timeout = 30;
	int timeunit = 0;
	unsigned int reading = 0;


/*	if (testUninit()){
		system("pause");
		return 1;
	}
*/
	if (testBounds()){
		system("pause");
		return 1;
	}

	

	//Pet app --- Require key press to reset timer to 30 seconds. If key press is q then it will close the driver and quit.
	try{
			if (errCode = InitializeSession()) {
				printf("Failed to open device\n");
				throw errCode;
			}

			if (errCode = EnableTimer(timeout, 0)) { //starting timer in seconds
				printf("Failed to start WDT with the following timeout and min_sec args %c %d\n", timeout, timeunit);
				throw errCode;
			}
            
			if (errCode = DisableTimer())
			{
				printf("Failed to disable timer\n");
				throw errCode;
			}

			if (errCode = ReadTimerValue(&reading)) {
				printf("Failed to read current timeout size\n");
				throw errCode;
			}
			if (reading)
			{
				printf("Disable timer failed to function correctly");
			}

			if (errCode = EnableTimer(timeout, 0)) { //starting timer in seconds
				printf("Failed to start WDT with the following timeout and min_sec args %c %d\n", timeout, timeunit);
				throw errCode;
			}
			else{
				printf("Set timer to %d seconds\n", timeout);
			}


			Sleep(2000);	//sleep to verify read

			if (errCode = ReadTimerValue(&reading)) {
				printf("Failed to read current timeout size\n");
				throw errCode;
			}
			else {
				printf("Testing read functionality. Should be two seconds less than starting value\n");
				printf("Value read was : %d\n", reading);
			}

			if (reading != timeout - 2){						//Verifying read works. reading should be 2 seconds less
				printf("Timer not counting down correctly\n");
				return 1;
			}

			while (1){
				if (errCode = WriteTimerValue(timeout)){
					printf("Failed to write time to driver\n");
					throw errCode;
				}
				if (errCode = ReadTimerValue(&reading)) {
					printf("Failed to read current timeout size\n");
					throw errCode;
				}
				printf("%d Seconds til shutdown. Press key to reset timer to %d seconds. Press q to close WDT and quit application.\n", reading, timeout);
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

