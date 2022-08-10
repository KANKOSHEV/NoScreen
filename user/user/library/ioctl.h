#pragma once

///////////////////////////////////////////////////////////
#define drv_device_file L"\\\\.\\NoScreen" 
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
HANDLE   h_driver = INVALID_HANDLE_VALUE;
NTSTATUS load_status = STATUS_NOT_FOUND;
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
#define offset_io_mirrore   0x2056
#define file_device_mirrore 0x3138
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
#define ioctl_protect_sprite_content (ULONG)CTL_CODE(file_device_mirrore, offset_io_mirrore + 0x0010, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
typedef struct _protect_sprite_content
{
	uint32_t value;
	uint64_t window_handle;
} protect_sprite_content, *pprotect_sprite_content;
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
bool service_is_load()
{
	h_driver = CreateFileW(drv_device_file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	return h_driver != INVALID_HANDLE_VALUE;
}

NTSTATUS send_service(ULONG ioctl_code, LPVOID io, DWORD size)
{
	if (h_driver == INVALID_HANDLE_VALUE)
		return STATUS_DEVICE_DOES_NOT_EXIST;

	if (!DeviceIoControl(h_driver, ioctl_code, io, size, nullptr, 0, NULL, NULL))
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;
}

NTSTATUS protect_sprite_content_ex(HWND window_handle, uint32_t value)
{
	protect_sprite_content req = { 0 };

	req.window_handle = reinterpret_cast<uint64_t>(window_handle);
	req.value = value;

	return send_service(ioctl_protect_sprite_content, &req, sizeof(req));
}
///////////////////////////////////////////////////////////


