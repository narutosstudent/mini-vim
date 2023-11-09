#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	const char *fileName = argv[1];
	FILE *file = fopen(fileName, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	int cursorPositonX = 0;
	int cursorPositonY = 0;

	// Start with an initial size and grow as needed
	size_t size = 1024;
	size_t used = 0;
	char *allCharacters = malloc(size);

	if (allCharacters == NULL)
	{
		perror("Error allocating memory");
		fclose(file);
		return 1;
	}

	int ch;
	while ((ch = fgetc(file)) != EOF)
	{
		// Resize the buffer if necessary
		if (used == size)
		{
			// Double the size
			size *= 2;
			char *newBuffer = realloc(allCharacters, size);
			if (newBuffer == NULL)
			{
				perror("Error reallocating memory");
				free(allCharacters);
				fclose(file);
				return 1;
			}
			allCharacters = newBuffer;
		}

		// Here increment happens in next iteration, this is like `allCharacters[used++] = ch`
		allCharacters[used++] = ch;
	}

	// Add a null terminator to make it a proper C string
	if (used == size)
	{
		// newBuffer to make sure realloc was successful
		char *newBuffer = realloc(allCharacters, size + 1);
		if (newBuffer == NULL)
		{
			perror("Error reallocating memory for null terminator");
			free(allCharacters);
			fclose(file);
			return 1;
		}
		allCharacters = newBuffer;
	}
	allCharacters[used] = '\0';

	// Use the string as needed
	printf("File content:\n%s", allCharacters);

	// Cleanup
	free(allCharacters);
	fclose(file);

	return 0;
}
