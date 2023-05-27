#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

char *duck1 = "\n       ,~~.	   Who: ";
char *duck2 = "  ,   (  - )>	    OS: ";
char *duck3 = "  )`~~'   (	Kernel: ";
char *duck4 = " (  .__)   )	 Shell: ";
char *duck5 = "  `-.____,' 	\n";

char *get_username(void)
{
	char *username = getenv("USER");
	return username;
}

char *get_hostname(void)
{
	char *hostname = getenv("HOSTNAME");
	return hostname;
}

char *get_prettyname(void)
{
	const int bufferSize = 32;
	char buffer[bufferSize];
	char *result = NULL;
	size_t resultSize = 0;

	const char *command = (
		"cat /etc/os-release | grep PRETTY | cut -d '\"' -f2"
		);
	
	FILE *p = popen(command, "r");
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
char *get_shell(void)
{
    char *shell = getenv("SHELL");
    return shell;
} 

char *get_kernelv(void)
{
	const int bufferSize = 32;
	char buffer[bufferSize];
	char *result = NULL;
	size_t resultSize = 0;

	FILE *p = popen("uname -r", "r");
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

char *combine_userhost(char *username, char *hostname)
{
	const char *atsign = "@";

	size_t resultSize = (
		strlen(username) + 1 + strlen(hostname) + 1
	);

	char *result = (char*)malloc(resultSize);

	sprintf(result, "%s%s%s", username, atsign, hostname);

	return result;
}

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
	char *username = get_username();
	char *hostname = get_hostname();

	char *userhost = combine_userhost(username, hostname);
	char *prettyname = get_prettyname();
	char *kernelv = get_kernelv();
	char *usershell = get_shell();

	char *resultDuck1 = combine_ascii(duck1, userhost);
	char *resultDuck2 = combine_ascii(duck2, prettyname);
	char *resultDuck3 = combine_ascii(duck3, kernelv);
	char *resultDuck4 = combine_ascii(duck4, usershell);
	//char *resultDuck5 = combine_ascii(duck1, userhost);
	
	
	printf(
		"%s\n%s\n%s\n%s\n%s\n",
		resultDuck1, resultDuck2, resultDuck3,
		resultDuck4, duck5
	);

}
