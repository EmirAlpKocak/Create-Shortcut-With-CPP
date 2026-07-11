// AUTHOR: EmirAlpKocak
// GitHub: https://github.com/EmirAlpKocak
// Website: https://deuteriumsoftware.com
// Creating shortcuts on C++ using Windows API COM

// Required includes
#include <iostream>
#include <Windows.h>
#include <objbase.h>
#include <ShlObj.h>
#include <ObjIdl.h>

// In some environments you may need these
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

int main()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED); // initialize COM
	if (FAILED(hr)) // if failed
	{
		std::cout << "Error: " << hr << std::endl;
		return 1; // exit with code 1
	}

	IShellLink* pShellLink = nullptr; // create an IShellLink pointer
	hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink); // create instance

	if (SUCCEEDED(hr)) // on success
	{
		// CODE FOR CREATING SHORTCUT

		pShellLink->SetPath(L"C:\\Windows\\system32\\cmd.exe"); // Target path of the shortcut
		pShellLink->SetRelativePath(L"C:\\Users\\Emir\\Desktop\\Test.lnk", 0); // Relative path of the shortcut
		IPersistFile* pPersistFile = nullptr; // we create a IPersistFile
		hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile); // query interface

		// EXTRA SHORTCUT FEATURES:
		
		//pShellLink->SetIconLocation(L"C:\\Users\\Emir\\Desktop\\main.ico", 0); // for icon from an ico file

		pShellLink->SetIconLocation(L"C:\\Windows\\system32\\imageres.dll", 42); // take icon from imageres.dll. 9 is the icon index
		// DLL files can contain more than one icons, so you can use that integer for the index. You can use IconViewer to see the icons on a DLL (or exe etc.)
		
		pShellLink->SetDescription(L"This shortcut is created by a C++ program!"); // Description (comment)
		pShellLink->SetShowCmd(7); // this makes the shortcut program start as minimized
		// value 1: SW_SHOWNORMAL : Normal window
		// value 3: SW_SHOWMAXIMIZED : Maximized
		// value 7: SW_SHOWMINNOACTIVE : Minimized


		WORD hotKey = (HOTKEYF_CONTROL | HOTKEYF_ALT) << 8 | 'D'; // Set hotKey variable for our shortcut.
		// Tip: Normally, Windows only allows CTRL + ALT + KEY hotkeys when you create a shortcut normally.
		// With this code, you can use any key you want.
		// For example: WORD hotKey = 'A'

		pShellLink->SetHotkey(hotKey); // Add the hotkey to the shortcut.

		pShellLink->SetWorkingDirectory(L"C:\\Windows\\system32\\drivers\\etc"); // This makes the shortcut start on a specific folder
		pShellLink->SetArguments(L"/c msg * \"Hello!\""); // You can add arguments

		// Following code is a method that allows you to point special folders (virtual folders) in Windows
		// For example: Recycle Bin
		// Set FOLDERID to one of the special folders in Windows.
		// Tip: I recommend you to explore this, there are a lot of strange special folders in Windows.
		// Did you know that Send to menu at right click is a folder? FOLDERID_SendTo
		// Following code is commented out because we declared shortcut path at top. You can safely uncomment it.

		/*PIDLIST_ABSOLUTE pidl = nullptr; // create the variable to hold our special folder
		if (SUCCEEDED(SHGetKnownFolderIDList(FOLDERID_SendTo, 0, nullptr, &pidl))) // on success
		{
			pShellLink->SetIDList(pidl); // Set the folder.
			CoTaskMemFree(pidl); // Free the memory !
		}*/

		// ======================================================================

		// FOLLOWING CODE IS USED FOR MAKING YOUR SHORTCUT RUN AS ADMINISTRATOR
		// YOU CAN SAFELY REMOVE IT IF YOU DON'T USE

		IShellLinkDataList* pDataList = nullptr; // Create an IShellLinkDataList pointer
		hr = pShellLink->QueryInterface(IID_IShellLinkDataList, (void**)&pDataList); // query interface again for pDataList
		if (SUCCEEDED(hr)) // on success
		{
			DWORD flags = 0; // variable for storing flags
			flags |= SLDF_RUNAS_USER; // flag for run as administrator
			pDataList->SetFlags(flags); // set flags
			pDataList->Release(); // free the memory
		}

		if (SUCCEEDED(hr)) // on success of queryinterface
		{
			pPersistFile->Save(L"C:\\Users\\Emir\\Desktop\\sws.lnk", TRUE); // create the shortcut
			pPersistFile->Release(); // free the memory
			std::cout << "Shortcut created successfully." << std::endl;
		}
		else // on failure
		{
			std::cout << "Error: " << hr << std::endl;
			CoUninitialize(); // uninitialize COM
			return 1;
		}

	}
	else // on failure
	{
		std::cout << "Error: " << hr << std::endl;
		CoUninitialize(); // uninitialize COM
		return 1;
	}

	pShellLink->Release(); // free the memory
	CoUninitialize(); // uninitialize COM
	return 0;
}