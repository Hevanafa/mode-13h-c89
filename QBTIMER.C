#include<time.h>
#include<math.h>

// 13-02-2024

double TIMER() {
	return fmod(time(0), 86400.0);
}

int main() {
	printf("%.2f", TIMER());
}