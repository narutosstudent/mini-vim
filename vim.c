#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Function to read arrow key input
bool readArrowKeyInput(int *cursorPositionX, int *cursorPositionY, size_t lineCount, size_t totalLength)
{
	int ch1, ch2, ch3;
	ch1 = getchar();
	if (ch1 == EOF || ch1 != '\033')
	{
		return false; // Not an escape sequence
	}

	ch2 = getchar();
	if (ch2 != '[')
	{
		return false; // Not an arrow key sequence
	}

	ch3 = getchar(); // This should be one of A, B, C, or D
	switch (ch3)
	{
	case 'A': // Up
		if (*cursorPositionY > 0)
			*cursorPositionY -= 1;
		break;
	case 'B': // Down
		if (*cursorPositionY < lineCount)
			*cursorPositionY += 1;
		break;
	case 'C': // Right
		if (*cursorPositionX < totalLength)
			*cursorPositionX += 1;
		break;
	case 'D': // Left
		if (*cursorPositionX > 0)
			*cursorPositionX -= 1;
		break;
	default:
		return false; // Not an arrow key
	}

	return true;
}

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
	size_t usedCharactersSize = 0;
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
		if (usedCharactersSize == size)
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
		allCharacters[usedCharactersSize++] = ch;

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
	if (usedCharactersSize == size)
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
	allCharacters[usedCharactersSize] = '\0';

	// ensures that all output in the stream is sent to the console
	fflush(stdout);

	while (1)
	{
		int ch = getchar();
		if (ch == 'q')
		{
			break;
		}
		else if (ch == '\033')
		{
			// Put back the escape character and handle arrow key input
			ungetc(ch, stdin);
			if (!readArrowKeyInput(&cursorPositonX, &cursorPositonY, lineCount, strlen(allCharacters)))
			{
				printf("Invalid input!\n");
			}
		}
		else
		{
			printf("Unknown command\n");
		}

		// Clear the screen
		system("clear");  // Use "cls" on Windows
		printf("\033[H"); // This ANSI escape code moves the cursor to the top-left corner

		// Redraw the entire buffer
		printf("%s", allCharacters);

		// Print the cursor position
		printf("\nCursor Position: (%d, %d)\n", cursorPositonX, cursorPositonY);

		usleep(1000);
	}

	// Cleanup
	free(allCharacters);
	fclose(file);

	return 0;
}
