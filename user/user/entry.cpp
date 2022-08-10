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

	HWND window_handle = FindWindowW(0, L"Cheat Engine 7.4");

	if (!window_handle)
	{
		error_message("error: target window not found");
	}

	NTSTATUS status = protect_sprite_content_ex(window_handle, WDA_EXCLUDEFROMCAPTURE);

	if (status == 0)
	{
		cout << "success: hide target window from screenshots" << endl;
	}
	else
	{
		error_message("error");
	}

	cout << endl;
	system("pause");
}