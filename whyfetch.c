#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_commandout(char *usercommand);
char *combine_ascii(char *ascii, char *data);

// ASCII art courtesy of Hayley Jane Wakenshaw of asciiart.eu
char *duck1 = "        ,~~.            ";
char *duck2 = "   ,   (  - )>     Who: ";
char *duck3 = "   )`~~'   (        OS: ";
char *duck4 = "  (  .__)   )   Kernel: ";
char *duck5 = "   `-.____,'     Shell: ";

// User data is acquired by saving the output of serveral commands,
// detailed below. The command output is piped in and null terminated
// before being returned.
char *get_commandout(char *usercommand)
{
	const int bufferSize = 32;
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

// ASCII art is combined with the previous data, so it's evenly spaced.
char *combine_ascii(char *ascii, char *data)
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
	// Command output is captured.
	char *userhost = get_commandout("echo $USER@$HOSTNAME");
	// There's definitely a way to do this in two commands.
	char *prettyname = get_commandout(
		"cat /etc/os-release | grep PRETTY | cut -d '\"' -f2"
	);
	char *kernelv = get_commandout("uname -r");
	char *usershell = get_commandout("echo $SHELL");
	
	// User data is combined with the ASCII art so it can be to the side
	// of the text.
	char *resultDuck2 = combine_ascii(duck2, userhost);
	char *resultDuck3 = combine_ascii(duck3, prettyname);
	char *resultDuck4 = combine_ascii(duck4, kernelv);
	char *resultDuck5 = combine_ascii(duck5, usershell);
	
	// The result duck is combined with the original duck to form
	// a cute little fetch thing.	
	printf(
		"\n%s\n%s\n%s\n%s\n%s\n\n\n",
		duck1, resultDuck2, resultDuck3,
		resultDuck4, resultDuck5
	);

	return 0;
}
