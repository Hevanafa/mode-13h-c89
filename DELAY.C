#include<stdio.h>  // printf
#include<time.h>   // clock(), clock_t

// Less than 1 second delay implementation like QBASIC.
// 15-02-2024

void delay(long ms) {
  double s = ms / 1000.0;
  clock_t start_t = clock();
  while ((double)(clock() - start_t) / CLOCKS_PER_SEC < s);
}

void sleep(double s) {
  clock_t start_t = clock();
  while ((double)(clock() - start_t) / CLOCKS_PER_SEC < s);
}

int main() {
  printf("Starting...\n");
  sleep(1);
  printf("Slept for 1 second.\n");
  delay(1000);
  printf("Delayed for 1 second.\n");
  return 0;
}
