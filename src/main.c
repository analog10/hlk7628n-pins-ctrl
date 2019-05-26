#include <main.h>

void usage (char* progname) {
	fprintf(stderr,
			"Usage: %s <module> <command> [<arguments>]\n\n"
			"Supported modules:\n"
			"  gpiomux\n\n"
			"  Commands:\n"
			"    get                list the current GPIO muxing configuration\n"
			"    set <group> <role> set the role for the specified GPIO signal group\n"
			"\n  refclk\n\n"
			"  Commands:\n"
			"    get                   display the current refclk setting\n"
			"    set <frequency (MHz)> set the refclk to the specified frequency if possible\n"
			"\n  agpio\n\n"
			"  Commands:\n"
			"    get                 display the current agpio setting\n"
			"    set <group> <state> set the state for the specified AGPIO group\n"
			"",
			progname
	);
}

int main(int argc, char **argv)
{
	int status	= EXIT_FAILURE;


	// read the command argument
	if (argc >= 2) {
		if (mmap_open() == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
		
		if (!strcmp(argv[1], "gpiomux") || !strcmp(argv[1], "pinmux")) {
			if (argc >= 5 && !strcmp(argv[2], "set")) {
				status = gpiomux_set(argv[3], argv[4]);
			} 
			else if (argc >= 3 && !strcmp(argv[2], "get")) {
				status = gpiomux_get();
			} 
			else {
				usage(*argv);
			}
		}
		else if (!strcmp(argv[1], "refclk")) {
			if (argc >= 4 && !strcmp(argv[2], "set")) {
				status = refclk_set(atoi(argv[3]));
			} 
			else if (argc >= 3 && !strcmp(argv[2], "get")) {
				status = refclk_get();
			} 
			else {
				usage(*argv);
			}
		}
		else if (!strcmp(argv[1], "agpio")) {
			if (argc >= 5 && !strcmp(argv[2], "set")) {
				status = agpio_set(argv[3], argv[4]);
			} 
			else if (argc >= 3 && !strcmp(argv[2], "get")) {
				status = agpio_get();
			} 
			else {
				usage(*argv);
			}
		}
		else {
			usage(*argv);
		}

		mmap_close();
	} 
	else {
		usage(*argv);
	}

	

	return status;
}
