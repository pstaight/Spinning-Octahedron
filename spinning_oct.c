#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// merge test

float A, B, C;

float octWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
float far = 100.0;
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

void drawPoint(vec3 p, char paint) {
  int xp, yp, idx;
  xp = (int)(p.x + horizontalOffset + width / 2);
  yp = (int)(p.y + height / 2);
  //debug
  //printf("xp:%d; yp:%d; zp:%f\n",xp,yp,p.z);
  idx = xp + yp * width;
  //debug
  //printf("idx:%d; size:%d; p.z:%f; zbuff:%f\n",idx,width*height,p.z,zBuffer[idx]);
  if (idx >= 0 && idx < width * height) {
    if (p.z+far > zBuffer[idx]) {
      zBuffer[idx] = p.z+far;
      buffer[idx] = paint;
    }
  }
}

void drawTriangle(vec3 p1, vec3 p2, vec3 p3, char paint) {
  vec3 p1_rotated,p2_rotated,p3_rotated;
  vec3 top, mid, bot;
  int i,j,k;
  float m1,m2,m3,n1,n2,n3,b1,b2,b3;
  float start,end,step=0.5;
  vec3 p;
  p1_rotated.x = calculateX(p1.x, p1.y, p1.z);
  p1_rotated.y = calculateY(p1.x, p1.y, p1.z);
  p1_rotated.z = calculateZ(p1.x, p1.y, p1.z);

  p2_rotated.x = calculateX(p2.x, p2.y, p2.z);
  p2_rotated.y = calculateY(p2.x, p2.y, p2.z);
  p2_rotated.z = calculateZ(p2.x, p2.y, p2.z);

  p3_rotated.x = calculateX(p3.x, p3.y, p3.z);
  p3_rotated.y = calculateY(p3.x, p3.y, p3.z);
  p3_rotated.z = calculateZ(p3.x, p3.y, p3.z);
  //debug
  // printf("p1_rotated:<%f;%f;%f>\n",p1_rotated.x,p1_rotated.y,p1_rotated.z);
  // printf("p2_rotated:<%f;%f;%f>\n",p2_rotated.x,p2_rotated.y,p2_rotated.z);
  // printf("p3_rotated:<%f;%f;%f>\n",p3_rotated.x,p3_rotated.y,p3_rotated.z);
  if (p1_rotated.y >= p2_rotated.y){
    if( p1_rotated.y >= p3_rotated.y) {
      // the y value of the point "p1" is larger than every other point
      top = p1_rotated;
      if (p2_rotated.y >= p3_rotated.y) {
        mid = p2_rotated;
        bot = p3_rotated;
      }
      else {
        mid = p3_rotated;
        bot = p2_rotated;
      }
    }
    else{
      // the y value of the point "p3" is larger than every other point
      top = p3_rotated;
      mid = p1_rotated;
      bot = p2_rotated;
    }
  }
  else if (p2_rotated.y >= p3_rotated.y) {
    // the y value of the point "p2" is larger than every other point
    top = p2_rotated;
    if (p1_rotated.y >= p3_rotated.y) {
      mid = p1_rotated;
      bot = p3_rotated;
    } else {
      mid = p3_rotated;
      bot = p1_rotated;
    }
  } else {
    top=p3_rotated;
    mid=p2_rotated;
    bot=p1_rotated;
  }
  // TO_DID: Fill in the triangle instead of just drawing its 3 points.
  float tm_m = (top.x-mid.x)/(top.y-mid.y), tm_b = top.x - tm_m*top.y; 
  float mt_m = (mid.x-top.x)/(mid.y-top.y), mt_b = top.x - mt_m*top.y; 
  float tb_m = (top.x-bot.x)/(top.y-bot.y), tb_b = top.x - tb_m*top.y; 
  float bt_m = (bot.x-top.x)/(bot.y-top.y), bt_b = top.x - bt_m*top.y; 
  float mb_m = (mid.x-bot.x)/(mid.y-bot.y), mb_b = mid.x - mb_m*mid.y; 
  float bm_m = (bot.x-mid.x)/(bot.y-mid.y), bm_b = mid.x - bm_m*mid.y; 

  // #define FLT_MAX 3.402823466e+38F 
  // if(top.y!=mid.y){
  //   m1 = (top.x-mid.x)/(top.y-mid.y);
  //   b1 = top.x - m1*top.y;
  //   //debug   
  //   if(mid.x-0.001 > m1*mid.y + b1 || mid.x+0.001 < m1*mid.y + b1)printf("mid.x:%f\tright:%f\n",mid.x,m1*mid.y + b1);
  // }
  // else{
  //   m1 = FLT_MAX;
  //   b1 = top.x;
  // }
  // if(top.y!=bot.y){
  //   m2 = (top.x-bot.x)/(top.y-bot.y);
  //   b2 = top.x-m2*top.y;
  //   //debug
  //   if(bot.x-0.001 > m2*bot.y + b2 || bot.x+0.001 < m2*bot.y + b2)printf("bot.x:%f\tright:%f\n",bot.x,m2*bot.y + b2);
  // }
  // else{
  //   m2 = FLT_MAX;
  //   b2 = top.x;
  // }
  // if(mid.y!=bot.y){
  //   m3 = (mid.x-bot.x)/(mid.y-bot.y);
  //   b3 = mid.x-m3*mid.y;
  // }
  // else{
  //   m3=FLT_MAX;
  //   b3=mid.x;
  // }
  p.z=(top.z+mid.z+bot.z)/3.0;
  //debug
  //printf("here1\n");
    // printf("bot:<%f;%f;%f>\n",bot.x,bot.y,bot.z);
    // printf("mid:<%f;%f;%f>\n",mid.x,mid.y,mid.z);
    // printf("top:<%f;%f;%f>\n",top.x,top.y,top.z);    
    // printf("top-mid slope:%f; top-mid b:%f\n",m1,b1);    
    // printf("top-bot slope:%f; top-bot b:%f\n",m2,b2);
    // printf("bot-mid slope:%f; bot-mid b:%f\n\n\n",m3,b3);

  //draw the lower half of the triagle:
  //  start at the line bot-mid, end at the line bot-top
  for(p.y=bot.y;p.y<mid.y;p.y+=step){
    start=bm_m*p.y + bm_b;
    end =bt_m*p.y + bt_b;
    // if(m3!=FLT_MAX) start=m3*p.y + b3;
    // else if(m1!=FLT_MAX)start = m1*p.y + b1;
    // else start = b1;
    // if(m2!=FLT_MAX)end=m2*p.y + b2;
    // else if(m2!=FLT_MAX)end=m2*p.y + b2;
    // else end = b1;
    //debug
    // printf("bottom part:\n");
    // printf("m1:top-mid: %f * %f + %f\n",m1,p.y,b1);        
    // printf("m2:top-bot: %f * %f + %f\n",m2,p.y,b2);
    // printf("m3:bot-mid: %f * %f + %f\n",m3,p.y,b3);
    // printf("start:%f ,end:%f\n\n\n",start,end);
    if(start<=end)for(p.x=start;p.x<=end;p.x+=step){
      //debug
      //printf("<%f;%f;%f>\n",p.x,p.y,p.z);
      drawPoint(p,paint);
    }
    else for(p.x=end;p.x<start;p.x+=step){
      //debug
      //printf("<%f;%f;%f>\n",p.x,p.y,p.z);
      drawPoint(p,paint);
    }
  }
  for(p.y=mid.y;p.y<=top.y;p.y+=step){
    start=mt_m*p.y +mt_b;
    end=bt_m*p.y + bt_b;
    // if(m2!=FLT_MAX)start=m2*p.y + b2;
    // else if(m2!=FLT_MAX)start = m2*p.y + b2;
    // else start = b2;
    // if(m1!=FLT_MAX)end=m1*p.y + b1;
    // else if(m3!=FLT_MAX)end=m3*p.y + b3;
    // else end = b2;
    // //debug
    // printf("top part:\n");
    // printf("m1:top-mid: %f * %f + %f\n",m1,p.y,b1);        
    // printf("m2:top-bot: %f * %f + %f\n",m2,p.y,b2);
    // printf("m3:bot-mid: %f * %f + %f\n",m3,p.y,b3);
    // printf("start:%f ,end:%f\n\n\n",start,end);
    if(start<=end)for(p.x=start;p.x<end;p.x+=step){
      //debug
      //printf("here3");
      drawPoint(p,paint);
    }
    else for(p.x=end;p.x<start;p.x+=step)drawPoint(p,paint);
  }  
}

int main() {
  printf("\x1b[2J");
  vec3 p1, p2, p3, p4, p5, p6;
  octWidth = 20;
  // top and bottom
  p1.x = 0;
  p1.y = octWidth;
  p1.z = 0;
  p6.x = 0;
  p6.y = -octWidth;
  p6.z = 0;

  // left and right
  p2.x = octWidth;
  p2.y = 0;
  p2.z = 0;
  p4.x = -octWidth;
  p4.y = 0;
  p4.z = 0;

  // front and back
  p3.x = 0;
  p3.y = 0;
  p3.z = octWidth;
  p5.x = 0;
  p5.y = 0;
  p5.z = -octWidth;

  while (1) {
    memset(buffer, ' ', width * height);
    memset(zBuffer, 0, width * height * 4);

    // first oct
    octWidth = 20;
    horizontalOffset = -2 * octWidth;
    drawTriangle(p1, p2, p3, '@');
    drawTriangle(p1, p3, p4, '$');
    drawTriangle(p1, p4, p5, '~');
    drawTriangle(p1, p5, p2, '#');
    drawTriangle(p6, p2, p3, ';');
    drawTriangle(p6, p3, p4, '+');
    drawTriangle(p6, p4, p5, 'M'); //little extra
    drawTriangle(p6, p5, p2, '|');

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
       putchar(k % width ? buffer[k] : 10);
    }

    A += 0.05;
    B += 0.06;
    C += 0.07;

    usleep(8000 * 2);
  }

  return 0;
}
