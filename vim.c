#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char *ARROW_UP = "\033[A";
const char *ARROW_DOWN = "\033[B";
const char *ARROW_RIGHT = "\033[C";
const char *ARROW_LEFT = "\033[D";

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
	size_t usedSize = 0;
	size_t lineCount = 0;
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
		if (usedSize == size)
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
		allCharacters[usedSize++] = ch;

		// increment position of x for every character
		cursorPositonX++;

		bool isNewLine = ch == '\n';
		if (isNewLine)
		{
			lineCount++;
			cursorPositonY++;
			cursorPositonX = 0;
		}
	}

	// Add a null terminator to make it a proper C string
	if (usedSize == size)
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
	allCharacters[usedSize] = '\0';

	// Use the string as needed
	printf("File content:\n%s", allCharacters);

	// ensures that all output in the stream is sent to the console
	fflush(stdout);

	// Loop that waits user input, uses getchar, if q is pressed, exit, use usleep to sleep for 1ms to not use 100% cpu
	printf("Instructions: By pressing q, you exited the program\n");

	while (1)
	{
		// now we have to read multiple characters and also recognize arrow keys, up, down, left and right, we can not only read one character

		// we also have to prevent cursor from going out of bounds (with our cursor position x and y), we have to keep track of cursor position

		char *input = malloc(10);
		fgets(input, 3, stdin);

		if (strcmp(input, "q\n") == 0)
		{
			break;
		}
		else if (strcmp(input, "q\n") == 0)
		{
		}
		else if (strcmp(input, ARROW_UP) == 0)
		{
			if (cursorPositonY == 0)
			{
				continue;
			}
			cursorPositonY--;
		}
		else if (strcmp(input, ARROW_DOWN) == 0)
		{
			if (cursorPositonY == lineCount)
			{
				continue;
			}
			cursorPositonY++;
		}
		else if (strcmp(input, ARROW_LEFT) == 0)
		{
			if (cursorPositonX == 0)
			{
				continue;
			}

			cursorPositonX--;
		}
		else if (strcmp(input, ARROW_RIGHT) == 0)
		{
			if (cursorPositonX == strlen(allCharacters))
			{
				continue;
			}
			cursorPositonX++;
		}
		else
		{
			perror("Error reading input");
			free(allCharacters);
			fclose(file);
			return 1;
		}
	}

	// Cleanup
	free(allCharacters);
	fclose(file);

	return 0;
}
