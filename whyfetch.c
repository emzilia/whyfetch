#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_YELLOW	"\x1b[93m"
#define ANSI_COLOR_MAGENTA	"\x1b[95m"
#define ANSI_COLOR_WHITE	"\x1b[97m"
#define ANSI_COLOR_RESET	"\x1b[0m"

char *get_commandout(char *usercommand);

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

// User data is acquired by saving the output of serveral commands,
// the command output is piped in and null terminated
// before being returned.
char *get_commandout(char *usercommand)
{
	const int bufferSize = 64;
	char buffer[bufferSize];
	char *output = NULL;
	size_t outputSize = 0;

	FILE *p = popen(usercommand, "r");
	if (p == NULL) return NULL;
	
	while (fgets(buffer, bufferSize, p) != NULL) {
		size_t fragmentSize = strlen(buffer);
		char *newOutput = realloc(
			output, outputSize + fragmentSize + 1
		);
		output = newOutput;
		memcpy(output + outputSize, buffer, fragmentSize);
		outputSize += fragmentSize;
	}

	int status = pclose(p);
	if (status == -1) free(output);

	char *finalOutput = realloc(output, outputSize + 1);
	finalOutput[outputSize] = '\0';
	finalOutput[strcspn(finalOutput, "\n")] = 0;

	return finalOutput;

}

int main(void)
{
	// Command output is captured.
	char *userhost = get_commandout("echo $USER@$HOSTNAME");
	char *prettyname = get_commandout(
		"grep PRETTY /etc/os-release | cut -d '\"' -f2"
	);
	char *kernelv = get_commandout("uname -r");
	char *usershell = get_commandout("echo $SHELL");
	
	// The user data is combined with the ascii art to form
	// a cute little fetch thing.	
	printf(
		ANSI_COLOR_YELLOW	"\n%s"	ANSI_COLOR_RESET
		ANSI_COLOR_YELLOW	"\n%s"	ANSI_COLOR_RESET
		ANSI_COLOR_MAGENTA 	"%s" 	ANSI_COLOR_RESET
		ANSI_COLOR_WHITE 	"%s" 	ANSI_COLOR_RESET
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
		duck2, who, userhost,
		duck3, os, prettyname,
		duck4, kernel, kernelv, 
		duck5, shell, usershell
	);
	
	return 0;
}
