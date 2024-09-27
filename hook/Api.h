#pragma once
#ifndef __API_H__
#define __API_H__
#include <Windows.h>
#include <winternl.h>
#include <shlwapi.h> // For SHRegGetPathW
#include <fstream>
#include <string>
#include <iostream>
#include <shobjidl_core.h>

#pragma comment(lib, "Shlwapi.lib")

void writeLog(const char* message) {
	const std::string path = "C:\\Users\\KDark\\Downloads\\test\\log.txt";

	std::ofstream logFile;
	logFile.open(path, std::ios::app); // Open in append mode
	if (logFile.is_open()) {
		logFile << message << std::endl;
		logFile.flush();
		logFile.close();
	}
	else {
		std::cerr << "Unable to open file for logging." << std::endl;
	}
}

void writeLog(const wchar_t* message) {
	const std::wstring path = L"C:\\Users\\KDark\\Downloads\\test\\log.txt";

	std::wofstream logFile;
	logFile.open(path, std::ios::app); // Open in append mode
	if (logFile.is_open()) {
		logFile << message << std::endl;
		logFile.flush();
		logFile.close();
	}
	else {
		std::wcerr << L"Unable to open file for logging." << std::endl;
	}
}

HKEY g_searchkey = NULL;
wchar_t g_pszPath[MAX_PATH] = {0};

typedef DWORD(WINAPI* TypeIsMSSearchEnabled)(
	LPWSTR lpMachineName,
	BOOL* flag
	);
TypeIsMSSearchEnabled originalISMSSearchEnabled = NULL;

typedef DWORD(WINAPI* TypeSHCreateScope) (
	DWORD a1,
	IID* riid,
	void** ppv
	);
TypeSHCreateScope originalSHCreateScope = NULL;

typedef DWORD(WINAPI* TypeSHCreateAutoList) (
	int a1,
	int a2,
	IID* riid,
	void** ppv
	);
TypeSHCreateAutoList originalSHCreateAutoList = NULL;

typedef DWORD(WINAPI* TypeSHCreateScopeItemFromShellItem) (
	IUnknown* punk,
	int a2,
	int a3,
	int a4,
	IID* riid,
	void** a6
	);
TypeSHCreateScopeItemFromShellItem originalSHCreateScopeItemFromShellItem = NULL;

typedef DWORD(WINAPI* TypeCreateDefaultProviderResolver) (
	IID* riid,
	void** ppv
	);
TypeCreateDefaultProviderResolver originalCreateDefaultProviderResolver = NULL;

typedef DWORD(WINAPI* TypeSHCreateAutoListWithID) (
	int a1,
	int a2,
	int a3,
	int a4,
	char a5,
	IID* a6,
	void* a7
	);
TypeSHCreateAutoListWithID originSHCreateAutoListWithID = NULL;

typedef LONG(WINAPI* RegSetValueExWType)(
	HKEY hKey,
	LPCWSTR lpValueName,
	DWORD Reserved,
	DWORD dwType,
	const BYTE* lpData,
	DWORD cbData
	);
RegSetValueExWType OriginalRegSetValueExW = nullptr;

// Typedef for the original RegCreateKeyExW function
typedef LONG(WINAPI* RegCreateKeyExWType)(
	HKEY hKey,
	LPCWSTR lpSubKey,
	DWORD Reserved,
	LPWSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
	);
RegCreateKeyExWType OriginalRegCreateKeyExW = nullptr;

typedef INT(WINAPI* TypeCreateResultSetFactory) (
	int a1,
	int a2,
	IID* riid,
	void** ppv
	);
TypeCreateResultSetFactory originalCreateResultSetFactory = nullptr;


typedef INT(WINAPI* TypeCreateSingleVisibleInList) (
	LPCWSTR pszStr1,
	int a2,
	void** a3
	);
TypeCreateSingleVisibleInList originalCreateSingleVisibleInList = nullptr;

typedef HRESULT(WINAPI* TypeGetScopeFolderType) (
	int* a1,
	DWORD* a2
	);
TypeGetScopeFolderType originalGetScopeFolderType = nullptr;

typedef int (WINAPI* TypeIsShellItemInSearchIndex) (
	int a1,
	int a2,
	DWORD* a3,
	int a4
	);
TypeIsShellItemInSearchIndex originalIsShellItemInSearchIndex = nullptr;

typedef int (WINAPI* TypeSEARCH_WriteAutoListContents) (
	int a1,
	int a2
	);
TypeSEARCH_WriteAutoListContents originalSEARCH_WriteAutoListContents;

typedef BOOL(WINAPI* PathIsDirectoryW_t)(LPCWSTR pszPath);
PathIsDirectoryW_t TruePathIsDirectoryW = nullptr;

DWORD HookedISMSSearchEnabled(
	LPWSTR lpMachineName,
	BOOL* flag
)
{
	writeLog(__FUNCTION__);
	return originalISMSSearchEnabled(lpMachineName, flag);
}

DWORD HookedSHCreateScope(
	DWORD a1,
	IID* riid,
	void** ppv
)
{
	writeLog(__FUNCTION__);
	return originalSHCreateScope(a1, riid, ppv);
}

DWORD HookedSHCreateAutoList(
	int a1,
	int a2,
	IID* riid,
	void** ppv
)
{
	writeLog(__FUNCTION__);
	return originalSHCreateAutoList(a1, a2, riid, ppv);
}

DWORD HookedSHCreateScopeItemFromShellItem(
	IUnknown* punk,
	int a2,
	int a3,
	int a4,
	IID* riid,
	void** a6
)
{
	DWORD result;
	punk = NULL;

	//LPITEMIDLIST current_folder;
	//HRESULT hres = SHGetIDListFromObject((IUnknown*)punk, &current_folder);

	//writeLog(__FUNCTION__);
	//PIDLIST_ABSOLUTE parent_pidl, pidl;
	//punk = NULL;
	result = originalSHCreateScopeItemFromShellItem(punk, a2, a3, a4, riid, a6);
	//COMBOBOXEXITEMW* item = (COMBOBOXEXITEMW*)*a6;
	//item->mask |= CBEIF_IMAGE;

	return result;
}

HRESULT HookedCreateDefaultProviderResolver(
	IID* riid,
	void** ppv
)
{
	writeLog(__FUNCTION__);
	return originalCreateDefaultProviderResolver(riid, ppv);
}

DWORD HookedSHCreateAutoListWithID(
	int a1,
	int a2,
	int a3,
	int a4,
	char a5,
	IID* a6,
	void* a7
)
{
	writeLog(__FUNCTION__);
	return  originSHCreateAutoListWithID(a1, a2, a3, a4, a5, a6, a7);
}

std::wstring ConvertBinaryToWString(const BYTE* lpData, DWORD cbData)
{
	std::wstring result;

	// Iterate over the data in pairs of bytes (2 bytes = 1 wchar_t)
	for (DWORD i = 0; i < cbData; i += 2)
	{
		// Ensure there are at least two bytes left to form a wchar_t
		if (i + 1 < cbData)
		{
			// Combine low and high bytes into a wchar_t
			wchar_t wChar = (lpData[i + 1] << 8) | lpData[i];
			result += wChar;
		}
	}

	return result;
}
LONG WINAPI HookedRegSetValueExW(
	HKEY hKey,
	LPCWSTR lpValueName,
	DWORD Reserved,
	DWORD dwType,
	const BYTE* lpData,
	DWORD cbData
)
{
	if (g_searchkey == hKey && dwType == REG_BINARY && lpData != nullptr && cbData > 0)
	{
		std::wstring binaryDataAsWString = ConvertBinaryToWString(lpData, cbData);
		MessageBoxW(NULL, binaryDataAsWString.c_str(), L"search value", MB_OK | MB_ICONINFORMATION);

		MessageBoxW(NULL, g_pszPath, L"search path", MB_OK | MB_ICONINFORMATION);
	}
	

	// Call the original function
	return OriginalRegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

// Hooked function for RegCreateKeyExW
LONG WINAPI HookedRegCreateKeyExW(
	HKEY hKey,
	LPCWSTR lpSubKey,
	DWORD Reserved,
	LPWSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
)
{
	LONG Result;
	Result = OriginalRegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
	
	// Check if lpSubKey contains "WordWheelQuery"
	if (lpSubKey != nullptr && wcsstr(lpSubKey, L"WordWheelQuery") != nullptr)
	{
		//MessageBoxW(NULL, L"Registry key being created contains WordWheelQuery!", L"Registry Hook", MB_OK | MB_ICONINFORMATION);
		g_searchkey = *phkResult;
	}

	return Result;
}

INT HookCreateResultSetFactory(
	int a1,
	int a2,
	IID* riid,
	void** ppv
)
{
	writeLog(__FUNCTION__);
	return originalCreateResultSetFactory(a1, a2, riid, ppv);
}

INT HookCreateSingleVisibleInList(
	LPCWSTR pszStr1,
	int a2,
	void** a3
)
{
	writeLog(__FUNCTION__);
	return originalCreateSingleVisibleInList(pszStr1, a2, a3);
}

HRESULT HookGetScopeFolderType(
	int* a1,
	DWORD* a2
)
{
	writeLog(__FUNCTION__);
	return originalGetScopeFolderType(a1, a2);
}

int HookIsShellItemInSearchIndex(
	int a1,
	int a2,
	DWORD* a3,
	int a4
)
{
	writeLog(__FUNCTION__);
	return originalIsShellItemInSearchIndex(a1, a2, a3, a4);
}

int HookSEARCH_WriteAutoListContents(
	int a1,
	int a2
)
{
	writeLog(__FUNCTION__);
	return originalSEARCH_WriteAutoListContents(a1, a2);
}

BOOL WINAPI DetourPathIsDirectoryW(LPCWSTR pszPath) {
	// Log or modify the path before calling the original function

	// Call the original PathIsDirectoryW function
	writeLog(__FUNCTION__);
	writeLog(pszPath);
	wcscpy(g_pszPath, pszPath);
	return TruePathIsDirectoryW(pszPath);
}
#endif