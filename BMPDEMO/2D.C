#include<stdio.h>

// 12-02-2024

typedef unsigned char byte;

void two_d(byte** target) {
	int rows = 5, cols = 7;
	int a, b;
	int count = 0;

	free(target);

	// Stackoverflow: /questions/59811874
	**target = malloc(rows * sizeof(*target));
	for (a = 0; a < rows; a++)
		target[a] = (byte*) malloc(cols * sizeof(*target[a]));


	for (b = 0; b < rows; b++)
	for (a = 0; a < cols; a++) {
		count++;
		target[b][a] = count;
	}
}


int main() {
	int a, b;

	byte** example;
	two_d(example);

	printf("Success?\n");

	for (b = 0; b < 5; b++) {
		printf("row %d: ", b);

		for (a = 0; a < 7; a++)
			printf("%d ", example[b][a]);

		printf("\n");
	}

	getch();

	return 0;
}