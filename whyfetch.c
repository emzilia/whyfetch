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

char *search_file(char *search, char *file);
char *get_prettyname();

// ASCII art courtesy of Hayley Jane Wakenshaw of asciiart.eu
char *duck1 = "        ,~~.   ";
char *duck2 = "   ,   (  - )> ";
char *duck3 = "   )`~~'   (   ";
char *duck4 = "  (  .__)   )  ";
char *duck5 = "   `-.____,'   ";

char *who = "   Who: ";
char *os = "    OS: ";
char *kernel = "Kernel: ";
char *shell = " Shell: ";

// Searches a file for a keyword and returns the line of text its found on
char *search_file(char *search, char *file)
{
	char line[1024];
	char *name;
	FILE *f;

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

	// Default if nothing is found or file couldn't be opened
	name = (char *)malloc(14*sizeof(char));
	name = "something wild";
	return name;
}

// Searches /etc/os-release for the 'pretty name', cleans it before returning
char *get_prettyname()
{
	char *prettyname = search_file("PRETTY_NAME", "/etc/os-release");

	// Removes first 13 characters, the "PRETTY_NAME='" 
	memmove(prettyname, prettyname + 13, strlen(prettyname) + 1 - 13);

	// Removes last 2 characters, closing quotation mark and newline character
	prettyname[strlen(prettyname) - 2] = '\0';

	return prettyname;
}

int main(void)
{
	// Get hostname and kernel version with utsname struct
	struct utsname user;
	if (uname(&user) < 0) return EXIT_FAILURE;
	char *hostname = user.nodename;
	char *kernelv = user.release;

	// Get username and user's shell with passwd struct
	char *username = getpwuid(geteuid())->pw_name;
	char *usershell = getpwuid(geteuid())->pw_shell;

	// Get prettyname from /etc/os-release
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
		duck4, kernel, kernelv, 
		duck5, shell, usershell
	);
	
	return EXIT_SUCCESS;
}
