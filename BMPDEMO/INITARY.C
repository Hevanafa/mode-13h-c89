#include<stdio.h>

// 13-02-2024

typedef unsigned char byte;

void init_ary(byte *ary, int size) {
	free(ary);
	ary = malloc(size * sizeof(byte));
	memset(ary, 0, size * sizeof(byte));
}

int main() {
	byte *buffer;
	int a;

	init_ary(buffer, 100);

	for (a = 0; a < 100; a++)
		buffer[a] = a;

	for (a = 0; a < 100; a++)
		printf("%d", buffer[a]);

	getch();
	return 0;
}
