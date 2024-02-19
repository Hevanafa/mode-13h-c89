#include<conio.h>

typedef unsigned char byte;

void init_buffer(byte *target) {
	int a;
	free(target);

	*target = malloc(100 * 200 * sizeof(byte));
	for (a = 0; a < 100 * 200; a++)
		target[a] = 0;
	// memset(target, 0, 320 * 200 * sizeof(*target));
}

int main() {
	byte *buffer;
	int a, b, count = 0;

	// apparently 320x200 is too big for the int pointer
	buffer = (byte*) malloc(200 * sizeof(byte));
	memset(buffer, 0, 200 * sizeof(byte));
	printf("Success?");

	// init_buffer(buffer);

	for (b = 0; b < 100; b++)
	for (a = 0; a < 100; a++) {
		count++;
		buffer[b * 200 + a] = count;
	}

	for (a = 0; a < 100; a++)
		printf("%d ", buffer[a]);

	getch();

	return 0;
}
