//****************************************************************************
//	
//	Copyright 2017 by WinSystems Inc.
//
//****************************************************************************
//
//	Name	 : wdtDLL.h
//
//	Project	 : WDT Windows DLL
//
//	Author	 : Paul DeMetrotion
//
//****************************************************************************
//
//	  Date		  Rev	                Description
//	--------    -------	   ---------------------------------------------
//	04/14/17	  1.0		Original Release of DLL	
//
//****************************************************************************

#ifndef _WDT_DLL_H_
#define _WDT_DLL_H_
#include <iostream>

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C"
{
    DECLDIR int InitializeSession();
    DECLDIR int ReadTimerValue(unsigned int *readValue);
    DECLDIR int WriteTimerValue(unsigned int writeValue);
    DECLDIR int EnableTimer(unsigned int timeoutValue, int min_sec);
    DECLDIR int DisableTimer();
    DECLDIR int CloseSession();
}

typedef enum {
    SUCCESS = 0,
    DRIVER_ERROR,
    INVALID_HANDLE,
    INVALID_PARAMETER
} ErrorCodes;

typedef enum {
    SECONDS = 0,
    MINUTES
} Units;

#endif