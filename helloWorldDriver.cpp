// Import the necessary header files for Windows kernel development
extern "C" {
#include <ntddk.h>
}

// Function to be called when the driver is unloaded
// +-------------------+
// |   UnloadDriver    |
// +-------------------+
//           |
//           V
// +-------------------+
// |   DbgPrintEx      |
// +-------------------+
extern "C" VOID UnloadDriver(_In_ PDRIVER_OBJECT DriverObject)
{
    // Suppress unused parameter warning
    UNREFERENCED_PARAMETER(DriverObject);

    // Print "Goodbye, World!" to the debugger
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Goodbye, World!\n");
}

// Entry point for the driver
// +----------------+
// |  DriverEntry   |
// +----------------+
//          |
//          V
// +----------------+
// |  DbgPrintEx    |
// +----------------+
//          |
//          V
// +----------------+
// | DriverUnload   |
// +----------------+
extern "C" NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT   DriverObject,  // Driver object for this driver
    _In_ PUNICODE_STRING  RegistryPath   // Registry path string
)
{
    // Suppress unused parameter warning
    UNREFERENCED_PARAMETER(RegistryPath);

    // Print "Hello, World!" to the debugger
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Hello, World!\n");

    // Set the driver unload function
    DriverObject->DriverUnload = UnloadDriver;

    // Return a success status
    return STATUS_SUCCESS;
}
