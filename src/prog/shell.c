#include <qmos/memory/memory.h>
#include <qmos/prog/shell.h>
#include <qmos/qmc/stdlib.h>
#include <qmos/qmc/string.h>
#include <qmos/qmc/qm/sh.h>
#include <qmos/qmc/stdio.h>
#include <qmos/qmc/proc.h>
#include <qmos/kernel.h>
#include <qmos/serial.h>

// TODO: Support ANSI color escapes.
#include <qmos/textmode.h>

#define MAX_CMD_TABLE 128
const size_t qmosh_max_command_count = MAX_CMD_TABLE;
size_t qmosh_command_count = 0;
qmosh_command_t _COMMANDS[MAX_CMD_TABLE];
qmosh_command_t *qmosh_command_table = _COMMANDS;

static bool should_exit = false;

static int cmd_len = 0;
static char cmd[256] = {0};

static int tok_count = 0;
static char toks[64][128] = {0};

static int cmd_idx = 0;

static const char *SHCFG_PROMPT_DEFAULT = "%s# ";
static char shcfg_prompt[128];

static void rem_space()
{
	while(cmd[cmd_idx] == ' ') ++cmd_idx;
}

static void read_command()
{
	memset(cmd, 0, 256);
	printf(shcfg_prompt, get_this_proc()->__name); fflush(stdout);
	fgetline(cmd, 256, stdin);
	cmd_len = strlen(cmd);

	tok_count = 0;
	cmd_idx = 0;
	int tok_idx = 0;
	int mode = 0; // 0 - normal, 1 - quote
	outer_loop: while(cmd_idx < cmd_len)
	{
		int i = 0;
		if(mode == 1)
		{
			while(cmd[cmd_idx] != '"' && cmd_idx < cmd_len && tok_idx < 63 && i < 127)
			{
				toks[tok_idx][i++] = cmd[cmd_idx++];
			}
			toks[tok_idx][i] = 0;
			++tok_idx;
			mode = 0;
			++cmd_idx; // skip ending quote
			writes_serial("mode 1 end");
		}
		if(mode == 0)
		{
			rem_space();
			while(cmd[cmd_idx] != ' ' && cmd_idx < cmd_len && tok_idx < 63 && i < 127)
			{
				if(cmd[cmd_idx] == '"') { mode = 1; goto outer_loop; }
				if(cmd[cmd_idx] == '\\') cmd_idx += 1;
				toks[tok_idx][i++] = cmd[cmd_idx++];
			}
			toks[tok_idx][i] = 0;
			++tok_idx;
		}
	}
	tok_count = tok_idx;
}

// TODO: Can there be other checks?
static bool is_path_like(const char *s) { return strchr(s, '/'); }
static bool is_cmd_special(const char *cmd) { return cmd[0] == '`' || cmd[0] == '~'; }
static char get_cmd_color(char prefix)
{
	switch(prefix)
	{
	case '`': return VGA_COLOR_LIGHT_BLUE;
	case '~': return VGA_COLOR_LIGHT_GREEN;
	default: return VGA_COLOR_LIGHT_GREY;
	}
}

static qmosh_command_t *cmd_get(const char *name)
{
	for(size_t i = 0; i < qmosh_command_count; ++i)
	{
		const char *x = qmosh_command_table[i].name;
		bool is_special = is_cmd_special(x);
		if(strcmp(is_special ? x + 1 : x, name) == 0)
			return &qmosh_command_table[i];
	}
	return NULL;
}

// TODO: This is a crude version, we need to do proper parsing
// TODO: Redirecting output:
// TODO: 	Save the proc state and have custom FILEBUFs which redirect to where specified.
// TODO: Much more...

static void fs() { printf("QMOS Does not have a filesystem currently.\n"); fflush(stdout); }
static int do_command()
{

	read_command();
	// printf("Read '%s'\n", cmd); fflush(stdout);
	if(tok_count == 0) return 1; // Do nothing
	if(is_path_like(toks[0])) fs();


	if(strcmp(toks[0], "help") == 0)
	{
		const int HELP_ROW_COUNT = 5;
		for(size_t i = 0; i < qmosh_command_count; i += HELP_ROW_COUNT)
		{
			for(size_t n = 0; n < HELP_ROW_COUNT && i + n < qmosh_command_count; ++n)
			{
				const char *x = qmosh_command_table[i + n].name;
				fflush(stdout); terminal_set_fg(get_cmd_color(x[0]));

				bool is_special = is_cmd_special(x);
				printf("%s ", is_special ? x + 1 : x);

				fflush(stdout); terminal_set_fg(VGA_COLOR_LIGHT_GREY);
			}
			fputc('\n', stdout);
		}
		return 0;
	}     
	else if(strcmp(toks[0], "exit") == 0)
	{
		should_exit = 1;
		return 0;
	}
	else if(strcmp(toks[0], "shcfg") == 0)
	{
		if(tok_count < 2)
		{
		__usage_shcfg__label:
			printf("Usage:\n  shcfg <option> <string>\nOptions:\n  'prompt': default value \"%s\"\n", SHCFG_PROMPT_DEFAULT);
		}
		else
		{
			if(strcmp(toks[1], "view") == 0)
			{
				printf("Shell Config:\n  prompt: \"%s\"\n", shcfg_prompt);
			}
			else if(strcmp(toks[1], "prompt") == 0)
			{
				if(tok_count < 3)
				{
					fputs("No value provided!\n", stdout);
					goto __usage_shcfg__label;
				}

				size_t offset = 0;
				for(int i = 0; i < tok_count - 2; ++i)
				{
					strncpy(shcfg_prompt + offset, toks[2 + i], 128 - offset);
					offset += strlen(toks[2 + i]);
				}
			}
			else if(strcmp(toks[1], "reset") == 0)
			{
				if(tok_count < 3)
				{
					fputs("No value provided!\n", stdout);
					goto __usage_shcfg__label;
				}

				if(strcmp(toks[2], "prompt") == 0) strncpy(shcfg_prompt, SHCFG_PROMPT_DEFAULT, 128);
				else goto __unknown_shcfg__label;
			}
			else
			{
			__unknown_shcfg__label:
				printf("Unknown config: '%s'\n", toks[2]);
			}
			
		}
		return 0;
	}
	else
	{
		qmosh_command_t *c = cmd_get(toks[0]);
		if(!c)
		{
			printf("No such command: '%s'.\n", toks[0]);
			return 1;
		}

		// printf("%d tokens at 0x%x\n", tok_count, toks);
		// for(int i = 0; i < tok_count; ++i)
		// 	printf("Token %d/%d '%s'\n", i+1, tok_count, toks[i]);
		// fflush(stdout);
		// // TODO: Copy toks...?
		// printf("argv0 = '%s', handler = 0x%x\n", toks[0], c->handler);
		// fflush(stdout);

		// Convert from 2d array to array of char pointers.
		char *toks_ptr_arr[tok_count];
		for(int i = 0; i < tok_count; ++i) toks_ptr_arr[i] = (char*)&toks[i];

		int r = c->handler(tok_count, (char**)toks_ptr_arr);
		if(r == ESHNL && r != ENOSHNL) fputc('\n', stdout);
		fflush(stdout);
		return r;
	}
}

void shell_main()
{
	// qmosh_command_table = qmos_alloc(qmosh_max_command_count * sizeof(qmosh_command_t));
	// printf("qmosh_command_table = 0x%x\n", qmosh_command_table);
	// qmosh_command_table[0].name = "test";

	// printf(".shellCommandReg 0x%x-0x%x\n",
	// 	(uint32_t)&__SEC_BEGIN_KERNEL_CALL_START, (uint32_t)&__SEC_END_KERNEL_CALL_START);

	strncpy(shcfg_prompt, SHCFG_PROMPT_DEFAULT, 128);
	fflush(stdout);
	typedef void (*I)();
	for(uint32_t i = (uint32_t)&__SEC_BEGIN_KERNEL_CALL_START;
		i < (uint32_t)&__SEC_END_KERNEL_CALL_START;
		i += 4)
	{
		(*(I*)i)();
	}

	// Built-ins.
	qmosh_add_command("`exit", NULL);
	qmosh_add_command("`shcfg", NULL);
	qmosh_add_command("`help", NULL);

	while(!should_exit) do_command();
}

void qmosh_add_command(const char *name, qmosh_command_handler_func handler)
{
	// printf("Adding command %s at 0x%x!\n", name, handler);
	fflush(stdout);
	if(qmosh_command_count >= qmosh_max_command_count) return; // TODO: print error
	// printf("command_table[%d]\n", qmosh_command_count);
	qmosh_command_table[qmosh_command_count++] = (qmosh_command_t){ name, handler };
}
