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
  while (1) {
    memset(buff, ' ', width * height);
    memset(zbuff, 0, width * height * 4);
    isHighest(p1, p2, p3);
  }
  return 0;
}
