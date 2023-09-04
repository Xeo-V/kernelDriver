extern "C" {
#include <ntddk.h>
}

// Forward declaration of the IRP dispatch functions
NTSTATUS UnsupportedDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);
NTSTATUS CreateCloseDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);

// ASCII Diagram for Driver Unload Flow
// +------------------+
// |  UnloadDriver    |
// +------------------+
//          |
//          |  (Unload)
//          V
// +------------------+
// |    DbgPrint      |
// +------------------+

// Unload function for the driver
extern "C" VOID UnloadDriver(_In_ PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);  // Suppress unused parameter warning
    DbgPrint("Driver Unloaded\n");  // Print message to the debugger
}

// ASCII Diagram for Driver Entry Flow
// +------------------+
// |   DriverEntry    |
// +------------------+
//          |
//          |  (Initialize)
//          V
// +------------------+
// |   IRP Table      |
// +------------------+
//          |
//          V
// +------------------+
// |  Create Device   |
// +------------------+

// Entry point for the driver
extern "C" NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT   DriverObject,  // Driver object for this driver
    _In_ PUNICODE_STRING  RegistryPath   // Registry path string
)
{
    UNREFERENCED_PARAMETER(RegistryPath);  // Suppress unused parameter warning
    
    // Initialize the driver unload function
    DriverObject->DriverUnload = UnloadDriver;

    // Initialize the IRP (I/O Request Packet) dispatch table
    for (UINT i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {
        DriverObject->MajorFunction[i] = UnsupportedDispatch;
    }
    DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCloseDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CreateCloseDispatch;

    // Create device object and symbolic link
    PDEVICE_OBJECT DeviceObject = nullptr;
    UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\MyDevice");
    UNICODE_STRING SymLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\MyDevice");
    NTSTATUS status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
    if (!NT_SUCCESS(status)) {
        return status;
    }
    IoCreateSymbolicLink(&SymLinkName, &DeviceName);

    DbgPrint("Driver Loaded\n");  // Print message to the debugger
    return STATUS_SUCCESS;
}

// Unsupported IRP dispatch function
NTSTATUS UnsupportedDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);  // Suppress unused parameter warning
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_NOT_SUPPORTED;
}

// Create and Close IRP dispatch function
NTSTATUS CreateCloseDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);  // Suppress unused parameter warning
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}
