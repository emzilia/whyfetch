#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

char *get_username(void);
char *get_hostname(void);
char *get_prettyname(void);
char *get_shell(void);
char *get_kernelv(void);
char *combine_userhost(char *username, char *hostname);
char *combine_ascii(char* ascii, char* data);

// ASCII art courtesy of Hayley Jane Wakenshaw of asciiart.eu
char *duck1 = "\n       ,~~.	   	";
char *duck2 = "  ,   (  - )>	   Who: ";
char *duck3 = "  )`~~'   (	    OS: ";
char *duck4 = " (  .__)   )	Kernel: ";
char *duck5 = "  `-.____,' 	 Shell: ";

// User data is acquired by saving the output of serveral commands,
// detailed below. The command output is piped in and null terminated
// before being returned.
char * get_commandout(char *usercommand)
{
	const int bufferSize = 32;
	char buffer[bufferSize];
	char *result = NULL;
	size_t resultSize = 0;

	FILE *p = popen(usercommand, "r");
	if (p == NULL) return NULL;
	
	while (fgets(buffer, bufferSize, p) != NULL) {
		size_t fragmentSize = strlen(buffer);
		char *newResult = realloc(
			result, resultSize + fragmentSize + 1
		);
		result = newResult;
		memcpy(result + resultSize, buffer, fragmentSize);
		resultSize += fragmentSize;
	}

	int status = pclose(p);
	if (status == -1) free(result);

	char*finalResult = realloc(result, resultSize + 1);
	finalResult[resultSize] = '\0';

	finalResult[strcspn(finalResult, "\n")] = 0;

	return finalResult;

}

// Both the username and hostname are combined with a @ sign separating them.
char *combine_userhost(char *username, char *hostname)
{
	char *atsign = "@";

	size_t resultSize = (
		strlen(username) + strlen(atsign) + strlen(hostname) + 1
	);

	char *result = (char*)malloc(resultSize);

	sprintf(result, "%s%s%s", username, atsign, hostname);

	return result;
}

// ASCII art is combined with the previous data, so it's evenly spaced.
char *combine_ascii(char* ascii, char* data)
{
	size_t resultSize = (
		strlen(ascii) + strlen(data) + 1
	);

	char *resultDuck = (char*)malloc(resultSize);

	sprintf(resultDuck, "%s%s", ascii, data);

	return resultDuck;

}

int main(void)
{
	char *prettycommand = (
		"cat /etc/os-release | grep PRETTY | cut -d '\"' -f2"
	);

	
	char *username = get_commandout("echo $USER");
	char *hostname = get_commandout("hostname");

	char *userhost = combine_userhost(username, hostname);
	char *prettyname = get_commandout(prettycommand);
	char *kernelv = get_commandout("uname -r");
	char *usershell = get_commandout("echo $SHELL");

	char *resultDuck2 = combine_ascii(duck2, userhost);
	char *resultDuck3 = combine_ascii(duck3, prettyname);
	char *resultDuck4 = combine_ascii(duck4, kernelv);
	char *resultDuck5 = combine_ascii(duck5, usershell);
	
	
	printf(
		"%s\n%s\n%s\n%s\n%s\n",
		duck1, resultDuck2, resultDuck3,
		resultDuck4, resultDuck5
	);
	puts("\n");

    return 0;
}
