#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// ARCH BTW !!!!
float A, B, C;
int width = 160, height = 44;
float zbuff[160 * 44];
char buff[160 * 44];
int bgChar = ' ';
int distFromCam = 100;
float hzOffset;
float k1 = 40.0;

float UP_THE_SPEEEEDDD = 0.6;

float ooz; // one over z, btw
int xp, yp;
int idx;

typedef struct {
  float x, y, z;
} vetrex;

// i got autoformating to work!! check it out bellow.
float calcX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calcY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calcZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

// i3wm BTW !!!!

void calcFS(vetrex *cube, float cubeX, float cubeY, float cubeZ, int ch) {
  cube->x = calcX(cubeX, cubeY, cubeZ);
  cube->y = calcY(cubeX, cubeY, cubeZ);
  cube->z = calcZ(cubeX, cubeY, cubeZ) + distFromCam;

  ooz = 1 / cube->z; // literaly just one over z

  xp = (int)(width / 2 + hzOffset + k1 * ooz * cube->x * 2);
  yp = (int)(height / 2 + k1 * ooz * cube->y);

  idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (ooz > zbuff[idx]) {
      zbuff[idx] = ooz;
      buff[idx] = ch;
    }
  }
}

// i need new headphones
int main() {
  printf("\x1b[2J");
  while (1) {
    memset(buff, bgChar, width * height);
    memset(zbuff, 0, width * height * 4);
    float cubeWidth = 20;
    vetrex *cube;
    hzOffset = 20;
    for (float cubeX = -cubeWidth; cubeX < cubeWidth;
         cubeX += UP_THE_SPEEEEDDD) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += UP_THE_SPEEEEDDD) {
        calcFS(cube, cubeX, cubeY, -cubeWidth, '@');
        calcFS(cube, cubeWidth, cubeY, cubeX, '$');
        calcFS(cube, -cubeWidth, cubeY, -cubeX, '~');
        calcFS(cube, -cubeX, cubeY, cubeWidth, '#');
        calcFS(cube, cubeX, -cubeWidth, -cubeY, ';');
        calcFS(cube, cubeX, cubeWidth, cubeY, '+');
      }
    }

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buff[k] : 10);
    }

    // im like a rat, i eat the cables
    // i have a serious anxiety problem
    A += 0.05;
    B += 0.05;
    C += 0.01;
    usleep(8000 * 2);
  }
  return 0;
}
