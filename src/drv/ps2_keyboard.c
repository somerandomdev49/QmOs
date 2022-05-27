#include <qmos/port_io.h>
#include <qmos/cpu/cpu.h>
#include <qmos/cpu/irq.h>
#include <qmos/serial.h>

#define SC_MAX 57
const char *sc_name[] =
{
	"ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
	"R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
	"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
	"LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
	"/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

// const char sc_ascii[] =
// {
// 	'?', '\e', '1', '2', '3', '4', '5', '6',     
//     '7', '8', '9', '0', '-', '=', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 
// 	'U', 'I', 'O', 'P', '[', ']', '\n', '?', 'A', 'S', 'D', 'F', 'G', 
// 	'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
// 	'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '
// };

const char sc_ascii_upper[] = "?\e!@#$%^&*()_+\b\tQWERTYUIOP{}\n?ASDFGHJKL:\"`?\\ZXCVBNM<>???? ";
const char sc_ascii_lower[] = "?\e1234567890-=\b\tqwertyuiop[]\n?asdfghjkl;'`?\\zxcvbnm,./??? ";


static uint8_t did_receive_key;
static int key_scancode;

enum {
	MOD_LSHIFT = 1 << 1, MOD_LCTRL = 1 << 2, MOD_LALT = 1 << 3,
	MOD_RSHIFT = 1 << 4, MOD_RCTRL = 1 << 5, MOD_RALT = 1 << 6,
};

static uint8_t mods = 0;

static void callback(qmos_registers_t regs)
{
	(void)regs;
	// writes_serial("PS/2 Keyboard Driver: Callback!");
	uint8_t scancode = inb(0x60);

	if(scancode == 0x2a) { mods |= MOD_LSHIFT; return; }
	if(scancode == 0xaa) { mods &=~MOD_LSHIFT; return; }
	if(scancode == 0x1d) { mods |= MOD_LCTRL ; return; }
	if(scancode == 0x9d) { mods &=~MOD_LCTRL ; return; }
	if(scancode == 0x38) { mods |= MOD_LALT  ; return; }
	if(scancode == 0xb8) { mods &=~MOD_LALT  ; return; }
	if(scancode == 0x36) { return; }
	if(scancode == 0xb6) { return; }

	if(scancode > SC_MAX) return;

	did_receive_key = 1;
	key_scancode = scancode;
}

void qmos_driver__ps2_keyboard_init()
{
	writes_serial("PS/2 Keyboard Driver:");
	did_receive_key = 0;
	key_scancode = 0;
	qmos_reg_int_handler(IRQ1, &callback);
	writes_serial("PS/2 Keyboard Driver Initialized!");
}

void qmos_driver__ps2_keyboard_deinit() {}

int qmos_driver__ps2_keyboard_getc()
{
	// writes_serial("PS/2 Keyboard Driver: Waiting for key");
	while(!did_receive_key) asm volatile("hlt"); // wait for the handler to interrupt
	// writes_serial("PS/2 Keyboard Driver: Done waiting for key");
	did_receive_key = 0;
	// char c = sc_ascii[key_scancode];
	// if(c >= 'A' && c <= 'Z')
	// {
	// 	if(mods & MOD_LSHIFT) return c;
	// 	else return c + ('a' - 'A');
	// }
	return mods & MOD_LSHIFT ? sc_ascii_upper[key_scancode] : sc_ascii_lower[key_scancode];
}
