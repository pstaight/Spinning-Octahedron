#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int width = 100, height = 30;
char buffer[100 * 30];
int idx;

typedef struct {
  float x, y, z;
} vec3;

vec3 triangle;

int main() {
  memset(buffer, ' ', width * height);
  for (int i = 0; i < height;
       i++) { // 1. create a variable named "i" and assing it the value 0, 2.
              // while "i" is smaller than the height of the screen -> increase
              // "i" by one
    for (int j = 0; j < i; j++) {
      if (i < height - j) {
        // empty btw
      } else if (i < width) {
        idx = width * i + j;
        buffer[idx] = '@';
      }
    }
  }
  printf("\x1b[H");
  for (int k = 0; k < width * height; k++) {
    putchar(k % width ? buffer[k] : 10);
  }
  return 0;
}
