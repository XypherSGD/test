#include <ntddk.h>

#define IOCTL_READ  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

typedef struct _RW_REQUEST { PVOID address; PVOID buffer; SIZE_T size; } RW_REQUEST, *PRW_REQUEST;
static PDEVICE_OBJECT g_Device = NULL;

NTSTATUS Ioctl(PDEVICE_OBJECT Dev, PIRP Irp)
{
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;
    ULONG len = 0;

    if (stack->MajorFunction == IRP_MJ_DEVICE_CONTROL)
    {
        PRW_REQUEST req = (PRW_REQUEST)Irp->AssociatedIrp.SystemBuffer;
        switch (stack->Parameters.DeviceIoControl.IoControlCode)
        {
        case IOCTL_READ:
            if (MmIsAddressValid(req->address))
                RtlCopyMemory(req->buffer, req->address, req->size);
            else status = STATUS_INVALID_PARAMETER;
            len = sizeof(RW_REQUEST);
            break;
        case IOCTL_WRITE:
            if (MmIsAddressValid(req->address))
                RtlCopyMemory(req->address, req->buffer, req->size);
            else status = STATUS_INVALID_PARAMETER;
            len = sizeof(RW_REQUEST);
            break;
        default:
            status = STATUS_INVALID_DEVICE_REQUEST;
        }
    }
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = len;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

VOID Unload(PDRIVER_OBJECT Driver)
{
    UNICODE_STRING sym = RTL_CONSTANT_STRING(L"\\??\\SoftAssistDev");
    IoDeleteSymbolicLink(&sym);
    if (g_Device) IoDeleteDevice(g_Device);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, PUNICODE_STRING Registry)
{
    UNICODE_STRING name = RTL_CONSTANT_STRING(L"\\Device\\SoftAssistDev");
    UNICODE_STRING sym  = RTL_CONSTANT_STRING(L"\\??\\SoftAssistDev");

    NTSTATUS st = IoCreateDevice(Driver, 0, &name, FILE_DEVICE_UNKNOWN, 0, FALSE, &g_Device);
    if (!NT_SUCCESS(st)) return st;
    IoCreateSymbolicLink(&sym, &name);

    Driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Ioctl;
    Driver->MajorFunction[IRP_MJ_CREATE] = Ioctl;
    Driver->MajorFunction[IRP_MJ_CLOSE]  = Ioctl;
    Driver->DriverUnload = Unload;
    return STATUS_SUCCESS;
}
