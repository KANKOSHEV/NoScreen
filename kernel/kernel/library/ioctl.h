#pragma once

///////////////////////////////////////////////////////////
#define offset_io_mirrore   0x2056
#define file_device_mirrore 0x3138
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
#define ioctl_protect_sprite_content (ULONG)CTL_CODE(file_device_mirrore, offset_io_mirrore + 0x0010, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
NTSTATUS ctl_io(PDEVICE_OBJECT device_obj, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_obj);
	NTSTATUS status = STATUS_SUCCESS;

	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	PIO_STACK_LOCATION stack_location = IoGetCurrentIrpStackLocation(irp);
	PVOID              system_buffer = irp->AssociatedIrp.SystemBuffer;
	ULONG              input_buffer_length = stack_location->Parameters.DeviceIoControl.InputBufferLength;
	ULONG              output_buffer_length = stack_location->Parameters.DeviceIoControl.OutputBufferLength;

	if (stack_location)
	{
		const auto ctl_code = stack_location->Parameters.DeviceIoControl.IoControlCode;
		switch (ctl_code)
		{
		case ioctl_protect_sprite_content:
		{
			if (input_buffer_length >= sizeof(protect_sprite_content) && system_buffer)
				irp->IoStatus.Status = protect_sprite_content_fn((pprotect_sprite_content)system_buffer);
			else
				irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			break;
		}

		default:
			irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			break;
		}
	}

	status = irp->IoStatus.Status;
	IofCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS unsupported_io(PDEVICE_OBJECT device_obj, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_obj);
	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IofCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS create_io(PDEVICE_OBJECT device_obj, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_obj);
	IofCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS close_io(PDEVICE_OBJECT device_obj, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_obj);
	IofCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS read_write_io(PDEVICE_OBJECT device_obj, PIRP irp)
{
	UNREFERENCED_PARAMETER(device_obj);
	return irp->IoStatus.Status;
}
///////////////////////////////////////////////////////////
