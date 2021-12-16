#include "library/globals.h"

void error_message(const char* msg)
{
	cout << msg << endl;
	Sleep(2000);
	ExitProcess(EXIT_SUCCESS);
}

int main()
{
	SetConsoleTitleW(L"");
	
	if (!service_is_load())
	{
		error_message("error: driver is not loaded");
	}

	HWND window_handle = FindWindowW(0, L"Cheat Engine 7.2");

	if (!window_handle)
	{
		error_message("error: target window not found");
	}

	NTSTATUS status = change_protect_window_ex(window_handle, WDA_EXCLUDEFROMCAPTURE);

	if (status == 0)
	{
		cout << "success: hide target window" << endl;
	}
	else
	{
		error_message("error: hide target window");
	}

	cout << endl;
	system("pause");
}