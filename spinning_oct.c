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

void drawPoint(vec3 p1,char paint){
  vec3 p1_rotated;
  float ooz;
  int xp,yp,idx;
  p1_rotated.x = calculateX(p1.x, p1.y, p1.z);
  p1_rotated.y = calculateY(p1.x, p1.y, p1.z);
  p1_rotated.z = calculateZ(p1.x, p1.y, p1.z) + distanceFromCam;
        
  ooz = 1 / p1_rotated.z;
  xp = (int)(p1_rotated.x + horizontalOffset + width / 2);
  yp = (int)(p1_rotated.y + height / 2);

  idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = paint;
    }
  }
}

void drawTriangle(vec3 p1, vec3 p2, vec3 p3, char paint){
  vec3 top, mid, bot;
    if (p1.y > p2.y && p1.y > p3.y) {
    // the y value of the point "p1" is larger than every other point
    top = p1;
    if(p2.y > p3.y){mid=p2;bot=p3;}
    else{mid=p3;bot=p2;}
  } else if (p2.y > p1.y && p2.y > p3.y) {
    // the y value of the point "p2" is larger than every other point
    top = p2;
    if(p1.y > p3.y){mid=p1;bot=p3;}
    else{mid=p3;bot=p1;}
  } else if (p3.y > p1.y && p3.y > p2.y) {
    // the y value of the point "p3" is larger than every other point
    top = p3;
    if(p2.y > p1.y){mid=p2;bot=p1;}
    else{mid=p1;bot=p2;}
  }
  //todo: fill in the triangle instead of just drawing its 3 points.
  drawPoint(top,paint);
  drawPoint(mid,paint);
  drawPoint(bot,paint);
}

int main() {
  printf("\x1b[2J");
  vec3 p1,p2,p3,p4,p5,p6;
  octWidth = 20;
  //top and bottom
  p1.x = 0; p1.y = octWidth; p1.z = 0;
  p6.x = 0; p6.y =-octWidth; p6.z = 0;

  //left and right
  p2.x = octWidth; p2.y = 0; p2.z = 0;
  p4.x =-octWidth; p2.y = 0; p2.z = 0;

  //front and back
  p3.x = 0; p3.y = 0; p3.z = octWidth;
  p5.x = 0; p5.y = 0; p5.z =-octWidth;

  while (1) {
    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0, width * height * 4);

    // first oct
    octWidth = 20;
    horizontalOffset = -2 * octWidth;
    drawTriangle(p1,p2,p3,'@');
    drawTriangle(p1,p3,p4,'$');
    drawTriangle(p1,p4,p5,'~');
    drawTriangle(p1,p5,p2,'#');
    drawTriangle(p6,p2,p3,';');
    drawTriangle(p6,p3,p4,'+');
    drawTriangle(p6,p4,p5,'M');
    drawTriangle(p6,p5,p2,'|');

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }

    A += 0.02;
    B += 0.03;
    C += 0.001;

    usleep(8000 * 2);
  }

  return 0;
}
