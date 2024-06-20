#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int width = 100, height = 30;

typedef struct {
  float x, y, z;
} vec3;

vec3 triangle;

int main() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < i; j++) {
      if (i < height - j) {
        putchar(' ');
      } else if (i < width) {
        putchar('@');
      } else {
        putchar(' ');
      }
    }
    putchar('\n');
  }
  return 0;
}
