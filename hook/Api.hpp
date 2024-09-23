#pragma once
#ifndef __API_H__
#define __API_H__
#include <Windows.h>

typedef DWORD(WINAPI* TypeIsMSSearchEnabled)(
	LPWSTR lpMachineName,
	BOOL* flag
	);
TypeIsMSSearchEnabled originalISMSSearchEnabled = NULL;


DWORD HookedISMSSearchEnabled(
	LPWSTR lpMachineName,
	BOOL* flag
)
{
	MessageBoxA(NULL,  "Event is triggered", "WOW", MB_OK);
	return originalISMSSearchEnabled(lpMachineName, flag);
}
#endif