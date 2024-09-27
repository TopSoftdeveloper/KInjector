// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "detours.h"
#include "Api.h"

static BOOL InstallHook(LPCSTR dll, LPCSTR function, LPVOID* originalFunction, LPVOID hookedFunction)
{
	HMODULE module = GetModuleHandleA(dll);
	*originalFunction = (LPVOID)GetProcAddress(module, function);

	if (*originalFunction)
	{
		DetourAttach(originalFunction, hookedFunction);
		return true;
	}

	return false;
}

void mainhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//InstallHook("windows.storage.search.dll", "CreateDefaultProviderResolver", (LPVOID*)& originalCreateDefaultProviderResolver, HookedCreateDefaultProviderResolver);
	//InstallHook("windows.storage.search.dll", "CreateResultSetFactory", (LPVOID*)& originalCreateResultSetFactory, HookCreateResultSetFactory);
	//InstallHook("windows.storage.search.dll", "CreateSingleVisibleInList", (LPVOID*)& originalCreateSingleVisibleInList, HookCreateSingleVisibleInList);
	//InstallHook("windows.storage.search.dll", "GetScopeFolderType", (LPVOID*)& originalGetScopeFolderType, HookGetScopeFolderType);
	//InstallHook("windows.storage.search.dll", "IsMSSearchEnabled", (LPVOID*)& originalISMSSearchEnabled, HookedISMSSearchEnabled);
	//InstallHook("windows.storage.search.dll", "IsShellItemInSearchIndex", (LPVOID*)& originalIsShellItemInSearchIndex, HookIsShellItemInSearchIndex);
	//InstallHook("windows.storage.search.dll", "SEARCH_WriteAutoListContents", (LPVOID*)& originalSEARCH_WriteAutoListContents, HookSEARCH_WriteAutoListContents);
	//InstallHook("windows.storage.search.dll", "SHCreateAutoList", (LPVOID*)& originalSHCreateAutoList, HookedSHCreateAutoList);
	//InstallHook("windows.storage.search.dll", "SHCreateAutoListWithID", (LPVOID*)& originSHCreateAutoListWithID, HookedSHCreateAutoListWithID);
	//InstallHook("windows.storage.search.dll", "SHCreateScope", (LPVOID*)& originalSHCreateScope, HookedSHCreateScope);
	InstallHook("windows.storage.search.dll", "SHCreateScopeItemFromShellItem", (LPVOID*)& originalSHCreateScopeItemFromShellItem, HookedSHCreateScopeItemFromShellItem);
	InstallHook("KERNELBASE.dll", "RegCreateKeyExW", (LPVOID*)& OriginalRegCreateKeyExW, HookedRegCreateKeyExW);
	InstallHook("KERNELBASE.dll", "RegSetValueExW", (LPVOID*)& OriginalRegSetValueExW, HookedRegSetValueExW);
	InstallHook("SHLWAPI.dll", "PathIsDirectoryW", (LPVOID*)& TruePathIsDirectoryW, DetourPathIsDirectoryW);

	DetourTransactionCommit();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		mainhook();
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

