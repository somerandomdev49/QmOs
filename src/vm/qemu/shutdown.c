#include <qmos/port_io.h>
#include <qmos/serial.h>
#include <qmos/qmc/stdio.h>

void _qmos_shutdown_emulator()
{
	writes_serial("Shutting down...");
	fputs("Shutting down...\n", stdout);
	outw(0x604, 0x2000); // QEMU-specific!
}
