#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>

#define ANSI_COLOR_YELLOW	"\x1b[93m"
#define ANSI_COLOR_MAGENTA	"\x1b[95m"
#define ANSI_COLOR_WHITE	"\x1b[97m"
#define ANSI_COLOR_RESET	"\x1b[0m"

char *get_username();
char *get_hostname();
char *get_shell();
char *get_kernel();
char *search_release();
char *get_prettyname();

// ASCII art courtesy of Hayley Jane Wakenshaw of asciiart.eu
char *duck1 = "        ,~~.   ";
char *duck2 = "   ,   (  - )> ";
char *duck3 = "   )`~~'   (   ";
char *duck4 = "  (  .__)   )  ";
char *duck5 = "   `-.____,'   ";

char *who = "   Who: ";
char *os = "    OS: ";
char *kernelv = "Kernel: ";
char *ushell = " Shell: ";

// Gets username from passwd struct, otherwise uses default values
char *get_username()
{
	char *username;

	struct passwd *user = getpwuid(geteuid());

	if (user) {
		username = user->pw_name;
	} else {
		username = "unknown user";
	}

	return username;
}

// Gets user's shell from passwd struct, otherwise uses default values
char *get_shell()
{
	char *shell;

	struct passwd *user = getpwuid(geteuid());

	if (user) {
		shell = user->pw_shell;
	} else {
		shell = "unknown shell";
	}

	return shell;
}

// Gets hostname from utsname struct, otherwise uses default value
char *get_hostname()
{
	struct utsname system;
	uname(&system);

	char *hostname = strdup(system.nodename);

	if (!hostname) return "unknown hostname";

	return hostname;
}

// Gets kernel version from utsname struct, otherwise uses default value
char *get_kernel()
{
	struct utsname sys;
	uname(&sys);

	char *kernel = strdup(sys.release);

	if (!kernel) return "unknown kernel";

	return kernel;
}

// Searches /etc/os-release for the distro PRETTY_NAME and returns the line
// its on, or returns a default value
char *search_release()
{
	char line[1024];
	char *name;
	FILE *f;

	char *file = "/etc/os-release";
	char *search = "PRETTY_NAME";

	f = fopen(file, "r");
	if (f) {
		while (fgets(line, sizeof(line), f) != NULL) {
			if (strstr(line, search) != NULL) {
				name = (char *)malloc(sizeof(line) - 1);
				strncpy(name, line, sizeof(line) - 1);
				return name;
			}
		}
	}

	name = "something wild";

	return name;
}

// Upon retrieving the line the PRETTY_NAME is on, cleans it up before 
// returning it.
char *get_prettyname()
{
	char *prettyname = search_release();

	// If default value was used, just return it
	if (!strcmp(prettyname, "something wild")) return prettyname;

	// Removes first 13 characters, the "PRETTY_NAME='" 
	memmove(prettyname, prettyname + 13, strlen(prettyname) + 1 - 13);

	// Removes last 2 characters, closing quotation mark and newline character
	prettyname[strlen(prettyname) - 2] = '\0';

	return prettyname;
}

int main(void)
{
	char *username = get_username();
	char *hostname = get_hostname();
	char *shell = get_shell();
	char *kernel = get_kernel();
	char *prettyname = get_prettyname();

	// The user data is combined with the ascii art to form
	// a cute little fetch thing.	
	printf(
		ANSI_COLOR_YELLOW	"\n%s"	ANSI_COLOR_RESET
		ANSI_COLOR_YELLOW	"\n%s"	ANSI_COLOR_RESET
		ANSI_COLOR_MAGENTA 	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_WHITE 	"%s@%s" ANSI_COLOR_RESET
		ANSI_COLOR_YELLOW 	"\n%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_MAGENTA 	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_WHITE 	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_YELLOW 	"\n%s"	ANSI_COLOR_RESET
		ANSI_COLOR_MAGENTA	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_WHITE 	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_YELLOW 	"\n%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_MAGENTA	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_WHITE 	"%s" 	ANSI_COLOR_RESET
		"\n\n\n",
		duck1,
		duck2, who, username, hostname,
		duck3, os, prettyname,
		duck4, kernelv, kernel, 
		duck5, ushell, shell
	);

	free(kernel);
	free(hostname);
	free(prettyname);
	
	return EXIT_SUCCESS;
}
