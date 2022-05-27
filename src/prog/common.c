#include <qmos/prog/shell.h>
#include <qmos/qmc/stdio.h>
#include <qmos/textmode.h>
#include <qmos/serial.h>
#include <qmos/kernel.h>
#include <qmos/driver.h>

typedef void (*I)();

//=********************* info *********************=//

static int main_info(int argc, char **argv)
{ (void)argc; (void)argv;
	extern textmode_driver_t *textmode_driver_current;

	printf("%s Kernel:\n"
	       "  Version: %d.%d.%d\n",
		qmos_kernel_info.name,
		VERSION_UNPACK(qmos_kernel_info.version));

	textmode_driver_t *drv = textmode_driver_current;
	printf("Textmode Driver:\n"
	       "  Name: %s\n"
		   "  Version: %d.%d.%d\n",
		drv->d.name,
		VERSION_UNPACK(drv->d.version));

	return 0;
}

static void init_info() { qmosh_add_command("sysinfo", &main_info); }
I __attribute__((section(".shellCommandReg"))) __qmosh_prog_info_init = &init_info;

//=********************* echo *********************=//

static int main_echo(int argc, char **argv)
{
	for(int i = 1; i < argc; ++i, fputc(' ', stdout))
		fputs(argv[i], stdout);

	fputc('\n', stdout);
	return 0;
}

static void init_echo() { qmosh_add_command("echo", &main_echo); }
I __attribute__((section(".shellCommandReg"))) __qmosh_prog_echo_init = &init_echo;

//=********************* hello *********************=//

static int main_hello(int argc, char **argv)
{
	if(argc == 2)
		printf("Hello, %s\n", argv[1]);
	else
		printf("Hello, World!\n");
	return 0;
}

static void init_hello() { qmosh_add_command("hello", &main_hello); }
I __attribute__((section(".shellCommandReg"))) __qmosh_prog_hello_init = &init_hello;


//=********************* clear *********************=//

static int main_clear(int argc, char **argv)
{ (void)argc; (void)argv;
	terminal_clear();
	return 0;
}

static void init_clear() { qmosh_add_command("clear", &main_clear); }
I __attribute__((section(".shellCommandReg"))) __qmosh_prog_clear_init = &init_clear;

//=********************* shutdown *********************=//

static int main_shutdown(int argc, char **argv)
{ (void)argc; (void)argv;
	fprintf(stdout, "Are you sure? [y/N] ");
	fflush(stdout);
	char c = fgetc(stdin);
	if(c == 'y' || c == 'Y')
	{
		extern void _qmos_shutdown_emulator();
		_qmos_shutdown_emulator();
	}
	else fprintf(stdout, "Shutdown cancelled.\n");
	return 0;
}

static void init_shutdown() { qmosh_add_command("~shutdown", &main_shutdown); }
I __attribute__((section(".shellCommandReg"))) __qmosh_prog_shutdown_init = &init_shutdown;


