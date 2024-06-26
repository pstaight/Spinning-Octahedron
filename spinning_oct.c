#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


float A, B, C;

float octWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceFromCam = 100;
float horizontalOffset;
float K1 = 40;

float incrementSpeed = 0.6;

typedef struct {
  float x, y, z;
} vec3;

vec3 oct;

float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
  return (j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
          j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C));
}

float calculateY(int i, int j, int k) {
  return (j * cos(A) * cos(C) + k * sin(A) * cos(C) -
          j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
          i * cos(B) * sin(C));
}

float calculateZ(int i, int j, int k) {
  return (k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B));
}

void calculateForSurface(float octX, float octY, float octZ, int ch) {
  oct.x = calculateX(octX, octY, octZ);
  oct.y = calculateY(octX, octY, octZ);
  oct.z = calculateZ(octX, octY, octZ) + distanceFromCam;

  ooz = 1 / oct.z;

  xp = (int)(width / 2 + horizontalOffset + K1 * ooz * oct.x * 2);
  yp = (int)(height / 2 + K1 * ooz * oct.y);

  idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

int main() {
  printf("\x1b[2J");
  while (1) {
    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0, width * height * 4);

    // first oct
    octWidth = 20;
    horizontalOffset = -2 * octWidth;

    for (float octX = -octWidth; octX < octWidth; octX += incrementSpeed) {
      for (float octY = -octWidth; octY < octWidth; octY += incrementSpeed) {
        calculateForSurface(octX, octY, -octWidth, '@'); // lots of this
        calculateForSurface(octWidth, octY, octX, '$');
        calculateForSurface(-octWidth, octY, -octX, '~');
        calculateForSurface(-octX, octY, octWidth, '#');
        calculateForSurface(octX, -octWidth, -octY, ';'); // doesnt showup alot
        calculateForSurface(octX, octWidth, octY, '+');
      }
    }

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }

    A += 0.1;
    B += 0.0;
    C += 0.0;

    usleep(8000 * 2);
  }

  return 0;
}
