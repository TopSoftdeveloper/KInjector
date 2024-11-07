#pragma once
#ifndef __API_H__
#define __API_H__
#include <Windows.h>
//#include <winternl.h>
#include <shlwapi.h> // For SHRegGetPathW
#include <fstream>
#include <string>
#include <iostream>
#include <shobjidl_core.h>
#include <regex>
#include <atlcomcli.h>  // for COM smart pointers
#include <atlbase.h>    // for COM smart pointers
#include "resource.h"

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Shell32.lib")
#define SEARCH_PIPE_NAME "\\\\.\\pipe\\SEARCHDEBUGLOG"
HANDLE g_hPipe = NULL;

std::string WStringToString(const std::wstring& wstr)
{
	std::string str;
	size_t size;
	str.resize(wstr.length());
	wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
	return str;
}

void writeLog(const char* message) {
	OutputDebugStringA(message);
}

void writeLog(int value) {
	std::string message = std::to_string(value);
	OutputDebugStringA(message.c_str());
}

void writeLog(const wchar_t* message) {
	OutputDebugStringW(message);
}

void notify(std::wstring value) {
	//OutputDebugStringA(__FUNCTION__);
	std::string stringvalue = WStringToString(value);
	OutputDebugStringA(stringvalue.c_str());
	if (g_hPipe != NULL)
	{
		WriteFile(g_hPipe, stringvalue.c_str(), stringvalue.length(), NULL, NULL);
	}
}

bool CreateRegistryKeyAndSetValues(HKEY hKeyParent, const std::string& subKey, const std::string& valueName1, const std::string& valueData1) {
	HKEY hKey;
	LONG lResult;
	DWORD dwDisposition;
	writeLog("1");
	writeLog(subKey.c_str());
	writeLog(valueName1.c_str());
	writeLog(valueData1.c_str());
	// Create or open the registry key
	//lResult = RegCreateKeyEx(hKeyParent, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
	//writeLog("11");

	//if (lResult != ERROR_SUCCESS) {
	//	return false;
	//}
	//writeLog("2");
	//
	//// Set the first string value
	//lResult = RegSetValueEx(hKey, valueName1.c_str(), 0, REG_SZ, (const BYTE*)valueData1.c_str(), valueData1.length());
	//if (lResult != ERROR_SUCCESS) {
	//	RegCloseKey(hKey);
	//	return false;
	//}
	//writeLog("3");
	//
	//// Close the registry key
	//RegCloseKey(hKey);
	//writeLog("4");

	return true;
}

typedef LONG NTSTATUS;
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

typedef struct _IO_STATUS_BLOCK
{
	NTSTATUS Status;
	ULONG Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, * PANSI_STRING;

typedef enum class _FILE_INFORMATION_CLASS
{
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,                   // 2
	FileBothDirectoryInformation,                   // 3
	FileBasicInformation,                           // 4
	FileStandardInformation,                        // 5
	FileInternalInformation,                        // 6
	FileEaInformation,                              // 7
	FileAccessInformation,                          // 8
	FileNameInformation,                            // 9
	FileRenameInformation,                          // 10
	FileLinkInformation,                            // 11
	FileNamesInformation,                           // 12
	FileDispositionInformation,                     // 13
	FilePositionInformation,                        // 14
	FileFullEaInformation,                          // 15
	FileModeInformation,                            // 16
	FileAlignmentInformation,                       // 17
	FileAllInformation,                             // 18
	FileAllocationInformation,                      // 19
	FileEndOfFileInformation,                       // 20
	FileAlternateNameInformation,                   // 21
	FileStreamInformation,                          // 22
	FilePipeInformation,                            // 23
	FilePipeLocalInformation,                       // 24
	FilePipeRemoteInformation,                      // 25
	FileMailslotQueryInformation,                   // 26
	FileMailslotSetInformation,                     // 27
	FileCompressionInformation,                     // 28
	FileObjectIdInformation,                        // 29
	FileCompletionInformation,                      // 30
	FileMoveClusterInformation,                     // 31
	FileQuotaInformation,                           // 32
	FileReparsePointInformation,                    // 33
	FileNetworkOpenInformation,                     // 34
	FileAttributeTagInformation,                    // 35
	FileTrackingInformation,                        // 36
	FileIdBothDirectoryInformation,                 // 37
	FileIdFullDirectoryInformation,                 // 38
	FileValidDataLengthInformation,                 // 39
	FileShortNameInformation,                       // 40
	FileIoCompletionNotificationInformation,        // 41
	FileIoStatusBlockRangeInformation,              // 42
	FileIoPriorityHintInformation,                  // 43
	FileSfioReserveInformation,                     // 44
	FileSfioVolumeInformation,                      // 45
	FileHardLinkInformation,                        // 46
	FileProcessIdsUsingFileInformation,             // 47
	FileNormalizedNameInformation,                  // 48
	FileNetworkPhysicalNameInformation,             // 49
	FileIdGlobalTxDirectoryInformation,             // 50
	FileIsRemoteDeviceInformation,                  // 51
	FileUnusedInformation,                          // 52
	FileNumaNodeInformation,                        // 53
	FileStandardLinkInformation,                    // 54
	FileRemoteProtocolInformation,                  // 55

	FileRenameInformationBypassAccessCheck,         // 56
	FileLinkInformationBypassAccessCheck,           // 57

	FileVolumeNameInformation,                      // 58
	FileIdInformation,                              // 59
	FileIdExtdDirectoryInformation,                 // 60
	FileReplaceCompletionInformation,               // 61
	FileHardLinkFullIdInformation,                  // 62
	FileIdExtdBothDirectoryInformation,             // 63
	FileDispositionInformationEx,                   // 64
	FileRenameInformationEx,                        // 65
	FileRenameInformationExBypassAccessCheck,       // 66
	FileDesiredStorageClassInformation,             // 67
	FileStatInformation,                            // 68
	FileMemoryPartitionInformation,                 // 69
	FileStatLxInformation,                          // 70
	FileCaseSensitiveInformation,                   // 71
	FileLinkInformationEx,                          // 72
	FileLinkInformationExBypassAccessCheck,         // 73
	FileStorageReserveIdInformation,                // 74
	FileCaseSensitiveInformationForceAccessCheck,   // 75
	FileKnownFolderInformation,                     // 76

	FileMaximumInformation
} FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;

typedef struct _FILE_BOTH_DIRECTORY_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaInformationLength;
	UCHAR         AlternateNameLength;
	WCHAR         AlternateName[12];
	WCHAR         FileName[1];
} FILE_BOTH_DIR_INFORMATION, * PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_DIRECTORY_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	WCHAR         FileName[1];
} FILE_DIRECTORY_INFORMATION, * PFILE_DIRECTORY_INFORMATION;


typedef struct _FILE_FULL_DIR_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	WCHAR         FileName[1];
} FILE_FULL_DIR_INFORMATION, * PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_FULL_DIR_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	LARGE_INTEGER FileId;
	WCHAR         FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, * PFILE_ID_FULL_DIR_INFORMATION;

typedef struct _FILE_ID_BOTH_DIR_INFORMATION
{
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	LARGE_INTEGER FileId;
	WCHAR         FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, * PFILE_ID_BOTH_DIR_INFORMATION;

typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	LARGE_INTEGER FileId;
	GUID          LockingTransactionId;
	ULONG         TxInfoFlags;
	WCHAR         FileName[1];
} FILE_ID_GLOBAL_TX_DIR_INFORMATION, * PFILE_ID_GLOBAL_TX_DIR_INFORMATION;

typedef struct _FILE_ID_EXTD_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	ULONG         ReparsePointTag;
	FILE_ID_128   FileId;
	WCHAR         FileName[1];
} FILE_ID_EXTD_DIR_INFORMATION, * PFILE_ID_EXTD_DIR_INFORMATION;

typedef struct _FILE_ID_EXTD_BOTH_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	ULONG         ReparsePointTag;
	FILE_ID_128   FileId;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	WCHAR         FileName[1];
} FILE_ID_EXTD_BOTH_DIR_INFORMATION, * PFILE_ID_EXTD_BOTH_DIR_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_NAMES_INFORMATION, * PFILE_NAMES_INFORMATION;

typedef VOID(NTAPI* PIO_APC_ROUTINE)(
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG Reserved);



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

typedef NTSTATUS(NTAPI* NtQueryDirectoryFile)(
	IN HANDLE FileHandle,
	IN HANDLE Event,
	IN PIO_APC_ROUTINE ApcRoutine,
	IN PVOID ApcContext,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PUNICODE_STRING FileName,
	IN BOOLEAN RestartScan);

typedef NTSTATUS(NTAPI* NtQueryDirectoryFileEx)(
	IN HANDLE FileHandle,
	IN HANDLE Event,
	IN PIO_APC_ROUTINE ApcRoutine,
	IN PVOID ApcContext,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN ULONG QueryFlags,
	IN PUNICODE_STRING FileName);

NtQueryDirectoryFile TrueNtQueryDirectoryFile = NULL;
NtQueryDirectoryFileEx TrueNtQueryDirectoryFileEx = NULL;

typedef NTSTATUS(NTAPI* TypeNtOpenFile)(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	void*				 ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	ULONG              ShareAccess,
	ULONG              OpenOptions
);

TypeNtOpenFile TrueNtOpenFile = NULL;

typedef HRESULT(WINAPI* TypeSHOpenFolderAndSelectItems) (
	PCIDLIST_ABSOLUTE     pidlFolder,
	UINT                  cidl,
	PCUITEMID_CHILD_ARRAY apidl,
	DWORD                 dwFlags
	);
TypeSHOpenFolderAndSelectItems TrueSHOpenFolderAndSelectItems = NULL;

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

std::wstring extractLocation(const std::wstring& searchUri) {
	// Constants for search parameters
	const std::wstring locationParam = L"location:";

	// Find the start of the location parameter
	size_t locationStart = searchUri.find(locationParam);
	if (locationStart == std::wstring::npos) {
		throw std::runtime_error("Location parameter not found in the search URI");
	}

	// Extract the encoded location (everything after "location:")
	std::wstring encodedLocation = searchUri.substr(locationStart + locationParam.length());

	// Find the end of the location (next '&' or end of string)
	size_t locationEnd = encodedLocation.find(L'&');
	if (locationEnd != std::wstring::npos) {
		encodedLocation = encodedLocation.substr(0, locationEnd);
	}

	// URL decode the location
	std::wstring decodedLocation;
	decodedLocation.reserve(encodedLocation.length());  // Preallocate for efficiency

	for (size_t i = 0; i < encodedLocation.length(); ++i) {
		if (encodedLocation[i] == L'%' && i + 2 < encodedLocation.length()) {
			// Handle URL-encoded characters
			wchar_t hex[3] = { encodedLocation[i + 1], encodedLocation[i + 2], L'\0' };
			wchar_t ch = static_cast<wchar_t>(std::wcstol(hex, nullptr, 16));
			decodedLocation += ch;
			i += 2;
		}
		else if (encodedLocation[i] == L'+') {
			// Handle space ('+' in URL encoding represents a space)
			decodedLocation += L' ';
		}
		else {
			// Regular character
			decodedLocation += encodedLocation[i];
		}
	}

	return decodedLocation;
}

std::wstring extractCrumbValue(const std::wstring& input) {
	std::wstring crumbKey = L"System.Generic.String%3A";
	std::wstring delimiter = L"&crumb=location:";

	// Find the position of the "&crumb=" substring
	size_t startPos = input.find(crumbKey);
	if (startPos != std::wstring::npos) {
		// Move the start position to the end of "&crumb="
		startPos += crumbKey.length();

		// Find the position of the "String%3" substring
		size_t endPos = input.find(delimiter, startPos);
		if (endPos != std::wstring::npos) {
			// Extract the substring between startPos and endPos
			return input.substr(startPos, endPos - startPos);
		}
	}

	// Return an empty string if not found
	return L"";
}

std::wstring extractSearchLocation(const std::wstring& input) {
	// Constants for string matching
	const std::wstring PREFIX = L"search-ms";
	const std::wstring LOCATION_MARKER = L"crumb=&crumb=location:";

	// Check if input starts with "search-ms"
	if (input.length() < PREFIX.length() || input.substr(0, PREFIX.length()) != PREFIX) {
		return L""; // Return empty string if prefix doesn't match
	}

	// Find the location marker
	size_t locationStart = input.find(LOCATION_MARKER);
	if (locationStart == std::wstring::npos) {
		return L""; // Return empty string if location marker not found
	}

	// Extract the encoded path (everything after the location marker)
	std::wstring encodedPath = input.substr(locationStart + LOCATION_MARKER.length());

	// Decode the URL-encoded path
	std::wstring decodedPath;
	decodedPath.reserve(encodedPath.length()); // Reserve space for efficiency

	for (size_t i = 0; i < encodedPath.length(); ++i) {
		if (encodedPath[i] == L'%' && i + 2 < encodedPath.length()) {
			// Handle URL encoding (e.g., %3A -> :)
			wchar_t hex1 = encodedPath[i + 1];
			wchar_t hex2 = encodedPath[i + 2];

			// Convert hex characters to integer
			int value = 0;

			// Process first hex digit
			if (hex1 >= L'0' && hex1 <= L'9') value = (hex1 - L'0') << 4;
			else if (hex1 >= L'A' && hex1 <= L'F') value = (hex1 - L'A' + 10) << 4;
			else if (hex1 >= L'a' && hex1 <= L'f') value = (hex1 - L'a' + 10) << 4;

			// Process second hex digit
			if (hex2 >= L'0' && hex2 <= L'9') value |= (hex2 - L'0');
			else if (hex2 >= L'A' && hex2 <= L'F') value |= (hex2 - L'A' + 10);
			else if (hex2 >= L'a' && hex2 <= L'f') value |= (hex2 - L'a' + 10);

			decodedPath += static_cast<wchar_t>(value);
			i += 2; // Skip the next two characters
		}
		else {
			decodedPath += encodedPath[i];
		}
	}

	return decodedPath;
}

DWORD WINAPI CreatePipe(LPVOID lpParam)
{
	//create pipe
	g_hPipe = CreateNamedPipe(SEARCH_PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
		PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
	if (g_hPipe == INVALID_HANDLE_VALUE) {
		//printf("Failed to create the keyboard named pipe.\n");
		return 0;
	}

	BOOL fConnected = false;
	while (!fConnected)
	{
		fConnected = ConnectNamedPipe(g_hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		Sleep(1000);
	}
	OutputDebugStringA("Pipe is created");
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

	LPITEMIDLIST current_folder;
	HRESULT hres = SHGetIDListFromObject((IUnknown*)punk, &current_folder);
	CComHeapPtr<wchar_t> pSearch;
	CComHeapPtr<wchar_t> pPath;

	//win11
	//::SHGetNameFromIDList(current_folder, SIGDN_PARENTRELATIVEPARSING, &pSearch);
	//writeLog(static_cast<LPWSTR>(pSearch));

	//win10
	::SHGetNameFromIDList(current_folder, SIGDN_PARENTRELATIVEFORADDRESSBAR, &pSearch);
	std::wstring input = static_cast<LPWSTR>(pSearch);
	writeLog(input.c_str());
	std::wstring searchword = extractCrumbValue(input);
	if (searchword.length() > 0)
	{
		std::wstring searchpath = extractLocation(input);
		notify(searchword + L"&" + searchpath);
		
		const std::wstring prefix = L"C:\\";
		if (searchpath.compare(0, prefix.length(), prefix) == 0)
		{
			punk = NULL;
		}
	}

	result = originalSHCreateScopeItemFromShellItem(punk, a2, a3, a4, riid, a6);

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
		//MessageBoxW(NULL, binaryDataAsWString.c_str(), L"search value", MB_OK | MB_ICONINFORMATION);
		writeLog(binaryDataAsWString.c_str());
		writeLog(g_pszPath);
		
		//MessageBoxW(NULL, g_pszPath, L"search path", MB_OK | MB_ICONINFORMATION);
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
	//writeLog(__FUNCTION__);
	//writeLog(pszPath);
	wcscpy(g_pszPath, pszPath);
	return TruePathIsDirectoryW(pszPath);
}

BOOL RegexCompare(PWCHAR FileName, int NumCharacters)
{
	std::wstring ws(FileName, FileName + NumCharacters);
	//std::wstring regVal = RegistryGetString(HKEY_CURRENT_USER, REG_SUBKEY_NAME, REG_VALUE_NAME_REGEX);
	std::wstring regVal;

	if (regVal.empty())
		return FALSE;

	return std::regex_match(ws, std::wregex(regVal));
}

template <class fiType>
void CheckAndModifyMatchingDetails(fiType FileInformation)
{
	for (fiType OldFileInformation = NULL; OldFileInformation != FileInformation;
		FileInformation = (fiType)((char*)FileInformation + FileInformation->NextEntryOffset))
	{

		// Save current pointer address
		OldFileInformation = FileInformation;

		

		// Ignore "." and ".."
		if (FileInformation->FileNameLength == 2 && !memcmp(FileInformation->FileName, L".", 2) ||
			FileInformation->FileNameLength == 4 && !memcmp(FileInformation->FileName, L"..", 4))
		{
			//writeLog(FileInformation->);
			continue;
		}

		// Hide files if they match the loaded regular expression list
		if (RegexCompare(FileInformation->FileName, FileInformation->FileNameLength / sizeof(WCHAR)))
			FileInformation->FileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	}
}

BOOL ModifyFileInformation(FILE_INFORMATION_CLASS FileInformationClass, PVOID FileInformation)
{
	switch (FileInformationClass)
	{
	case FILE_INFORMATION_CLASS::FileDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_DIRECTORY_INFORMATION>((PFILE_DIRECTORY_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileIdBothDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_ID_BOTH_DIR_INFORMATION>((PFILE_ID_BOTH_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileBothDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_BOTH_DIR_INFORMATION>((PFILE_BOTH_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileIdFullDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_ID_FULL_DIR_INFORMATION>((PFILE_ID_FULL_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileFullDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_FULL_DIR_INFORMATION>((PFILE_FULL_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileIdGlobalTxDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_ID_GLOBAL_TX_DIR_INFORMATION>((PFILE_ID_GLOBAL_TX_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileIdExtdDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_ID_EXTD_DIR_INFORMATION>((PFILE_ID_EXTD_DIR_INFORMATION)FileInformation);
		break;
	case FILE_INFORMATION_CLASS::FileIdExtdBothDirectoryInformation:
		CheckAndModifyMatchingDetails<PFILE_ID_EXTD_BOTH_DIR_INFORMATION>((PFILE_ID_EXTD_BOTH_DIR_INFORMATION)FileInformation);
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

std::string GetFullPathFromHandle(HANDLE hFile) {
	// Check if the handle is valid
	if (hFile == INVALID_HANDLE_VALUE) {
		return "Invalid handle";
	}

	// Buffer to hold the path
	char buffer[MAX_PATH];
	DWORD result = GetFinalPathNameByHandleA(hFile, buffer, sizeof(buffer), FILE_NAME_NORMALIZED);

	if (result == 0) {
		// Handle error
		return "Error getting path: " + std::to_string(GetLastError());
	}

	return std::string(buffer);
}

NTSTATUS NTAPI NewNtQueryDirectoryFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine,
	PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length,
	FILE_INFORMATION_CLASS FileInformationClass, BOOLEAN ReturnSingleEntry, PUNICODE_STRING FileName,
	BOOLEAN RestartScan)
{
	OutputDebugStringA((const char*)"Hooked NewNtQueryDirectoryFile\n");

	NTSTATUS ret = TrueNtQueryDirectoryFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, Length, FileInformationClass, ReturnSingleEntry, FileName, RestartScan);

	if (ret != STATUS_SUCCESS)
		return ret;
	std::string fullPath = GetFullPathFromHandle(FileHandle);
	writeLog(fullPath.c_str());
	// Modify NewNtQueryDirectoryFile file attributes
	ModifyFileInformation(FileInformationClass, FileInformation);

	return ret;
}

NTSTATUS NTAPI NewNtQueryDirectoryFileEx(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine,
	PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length,
	FILE_INFORMATION_CLASS FileInformationClass, ULONG QueryFlags, PUNICODE_STRING FileName)
{
	OutputDebugStringA("Hooked NewNtQueryDirectoryFileEx\n");

	NTSTATUS ret = TrueNtQueryDirectoryFileEx(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, Length, FileInformationClass, QueryFlags, FileName);

	if (ret != STATUS_SUCCESS)
		return ret;

	std::string fullPath = GetFullPathFromHandle(FileHandle);
	writeLog(fullPath.c_str());

	// Modify NewNtQueryDirectoryFileEx file attributes
	ModifyFileInformation(FileInformationClass, FileInformation);

	return ret;
}

NTSTATUS NewNtOpenFile(PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	void* ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	ULONG              ShareAccess,
	ULONG              OpenOptions)
{
	NTSTATUS result;
	result = TrueNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
	if (result != STATUS_SUCCESS)
		return result;

	OutputDebugStringA("Hooked NewNtOpenFile\n");
	std::string fullPath = GetFullPathFromHandle(*FileHandle);
	writeLog(fullPath.c_str());
	return result;
}

HRESULT NewSHOpenFolderAndSelectItems(
	PCIDLIST_ABSOLUTE     pidlFolder,
	UINT                  cidl,
	PCUITEMID_CHILD_ARRAY apidl,
	DWORD                 dwFlags
)
{
	writeLog(__FUNCDNAME__);
	return TrueSHOpenFolderAndSelectItems(pidlFolder, cidl, apidl, dwFlags);
}

typedef HRESULT(WINAPI* SHGetIDListFromObject_t)(IUnknown* punk, PIDLIST_ABSOLUTE* ppidl);
SHGetIDListFromObject_t TrueSHGetIDListFromObject = nullptr;

HRESULT WINAPI HookSHGetIDListFromObject(IUnknown* punk, PIDLIST_ABSOLUTE* ppidl) {
	// Call the original function
	HRESULT result = TrueSHGetIDListFromObject(punk, ppidl);

	// Check if the function succeeded and log the PIDL if available
	if (SUCCEEDED(result) && ppidl && *ppidl) {
		CComHeapPtr<wchar_t> pSearch;
		CComHeapPtr<wchar_t> pPath;
		::SHGetNameFromIDList(*ppidl, SIGDN_PARENTRELATIVEFORADDRESSBAR, &pSearch);
		std::wstring input = static_cast<LPWSTR>(pSearch);
		std::wstring searchword = extractCrumbValue(input);
		if (searchword.length() > 0)
		{
			//writeLog(input.c_str());
			std::wstring searchpath = extractLocation(input);
			//writeLog(searchword.c_str());
			//writeLog(searchpath.c_str());
			notify(searchword + L"&" + searchpath);
			const std::wstring prefix = L"C:\\";
			if (searchpath.compare(0, prefix.length(), prefix) == 0)
			{
				ppidl = NULL;
				return E_UNEXPECTED;
			}
		}

		//check if this is search context. some keywords are not showed
		std::wstring specialsearchpath = extractSearchLocation(input);
		if (specialsearchpath.length() > 0)
		{
			//writeLog(input.c_str());
			//writeLog(specialsearchpath.c_str());
			notify(L"speical context &" + specialsearchpath);
			const std::wstring prefix = L"C:\\";
			if (specialsearchpath.compare(0, prefix.length(), prefix) == 0)
			{
				ppidl = NULL;
				return E_UNEXPECTED;
			}
		}
	}
	else {
		OutputDebugStringA("SHGetIDListFromObject failed or PIDL not obtained.\n");
	}

	return result;
}
#endif