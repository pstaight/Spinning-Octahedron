#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;
int width = 160, height = 40;
float zbuff[160 * 40];
char buff[160 * 40];
float increaseSpeed = 0.06;
int xp, yp;
float k1 = 40;
int triangleWidht = 20;
float distFromCam = 100;

typedef struct {
  float x, y, z;
} vec3;

vec3 p1;
vec3 p2;
vec3 p3;

vec3 isHighest(vec3 p1, vec3 p2, vec3 p3) {
  if (p1.y > p2.y && p1.y > p3.y) {
    // the y value of the point "p1" is larger than every other point
    return p1;
  } else if (p2.y > p1.y && p2.y > p3.y) {
    // the y value of the point "p2" is larger than every other point
    return p2;
  } else if (p3.y > p1.y && p3.y > p2.y) {
    // the y value of the point "p3" is larger than every other point
    return p3;
  }
}

int main() {
  printf("\x1b[2J");
  p1.x = 10;
  p1.y = 20;
  p1.z = 30;

  p2.x = 10;
  p2.y = 30;
  p2.z = 30;

  p3.x = 10;
  p3.y = 40;
  p3.z = 30;

  while (1) {
    memset(buff, ' ', width * height);
    memset(zbuff, 0, width * height * 4);
    vec3 highestY = isHighest(p1, p2, p3);
    if (highestY.y == p1.y && highestY.x == p1.x && highestY.z == p1.z) {
      buff[30 * 20] = '@';
    } else if (highestY.y == p2.y && highestY.x == p2.x && highestY.z == p2.z) {
      buff[20 * 20] = '+';
    } else if (highestY.y == p3.y && highestY.x == p3.x && highestY.z == p3.z) {
      buff[40 * 20] = '!';
    } else {
      return 1;
    }
  }

  printf("\x1b[H");
  for (int k = 0; k < width * height; k++) {
    putchar(k % width ? buff[k] : 10);
  }
  return 0;
}
