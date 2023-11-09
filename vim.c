#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
	bool hasTwoArguments = argc == 2;
	char *allCharacters = malloc(sizeof(char) * 1000);

	if (hasTwoArguments)
	{
		char *fileName = argv[1];
		FILE *file = fopen(fileName, "r");
		if (file == NULL)
		{
			perror("Error: File not found");
			return 1;
		}

		allCharacters = fgetc(file);
	}
	else
	{
		perror("Error: Invalid number of arguments");
		return 1;
	}

	return 0;
}
