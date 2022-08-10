#include "library/globals.h"

NTSTATUS drv_main(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registery_path)
{
	///////////////////////////////
	UNREFERENCED_PARAMETER(registery_path);
	///////////////////////////////

	///////////////////////////////
	PDEVICE_OBJECT dev_obj;
	NTSTATUS status = STATUS_SUCCESS;
	///////////////////////////////

	///////////////////////////////
	status = init_function();
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////
	UNICODE_STRING dev_name, sym_link;

	dev_name = ansi_to_unicode("\\Device\\NoScreen");
	sym_link = ansi_to_unicode("\\DosDevices\\NoScreen");
	///////////////////////////////

	///////////////////////////////
	status = IoCreateDevice(driver_obj, 0, &dev_name, file_device_mirrore, 0x00000100, 0, &dev_obj);
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////
	status = IoCreateSymbolicLink(&sym_link, &dev_name);
	if (!NT_SUCCESS(status))
		return status;
	///////////////////////////////

	///////////////////////////////
	SetFlag(dev_obj->Flags, DO_BUFFERED_IO);

	for (int t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
		driver_obj->MajorFunction[t] = unsupported_io;

	driver_obj->MajorFunction[IRP_MJ_CREATE] = create_io;
	driver_obj->MajorFunction[IRP_MJ_CLOSE] = close_io;
	driver_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ctl_io;
	driver_obj->DriverUnload = NULL;

	ClearFlag(dev_obj->Flags, DO_DEVICE_INITIALIZING);
	///////////////////////////////

	return status;
}

NTSTATUS entry_point(PVOID a1, PVOID a2)
{
	return IoCreateDriver(&ansi_to_unicode("\\Driver\\NoScreen"), &drv_main);
}