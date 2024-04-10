#include <iostream>
#include <math.h>
#include <cstring>

#define pi 3.142
#define thetaOffset 0.07
#define phiOffset 0.02
#define scrWidth 80
#define scrHeight 22
#define bufferLength (scrHeight * scrWidth)
#define R1 1
#define R2 2
#define K2 5
#define K1 15

int main(){
  const char chars[] = ".,-~:;=!*#$@";
  
  float A = 0;
  float B = 0;
  float theta;
  float phi;

  float sinA, cosA;
  float sinB, cosB;
  float sinTheta, cosTheta;
  float sinPhi, cosPhi;

  float cX, cY;

  float x, y, z;

  float lum, depth;
  int lumIndex;

  int xp, yp, point;

  float zBuffer[bufferLength];
  char buffer[bufferLength];

  while(1){
    std::memset(zBuffer, 0, bufferLength * sizeof(float));
    std::memset(buffer, ' ', bufferLength);

    for(theta = 0; theta < 2*pi; theta += thetaOffset){
      for(phi = 0; phi < 2*pi; phi += phiOffset){
        sinA = sin(A);
        cosA = cos(A);
        sinB = sin(B);
        cosB = cos(B);
        sinTheta = sin(theta);
        cosTheta = cos(theta);
        sinPhi = sin(phi);
        cosPhi = cos(phi);

        cX = R2 + R1 * cosTheta;
        cY = R1 * sinTheta;

        // x = cX * (cosB * cosPhi + sinA * sinB * sinPhi) - (cosA * sinB * sinTheta);
        // y = cX * (cosPhi * sinB - cosB * sinA * sinPhi) + (cosA * cosB * sinTheta);
        x = cX * (cosB * cosPhi + sinA * sinB * sinPhi) - cY * cosA * sinB; 
        y = cX * (sinB * cosPhi - sinA * cosB * sinPhi) + cY * cosA * cosB;
        z = K2 + cosA * cX * sinPhi + cY  * sinA; 

        depth = 1 / z;

        xp = (int) (scrWidth/2 + K1*depth*x);
        yp = (int) (scrHeight/2 - K1*depth*y);

        point = xp + yp * scrWidth;

        lum = (cosPhi * cosTheta * sinB) - (cosA * cosTheta * sinPhi) - 
              (sinA * sinTheta) + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);

        if(lum > 0 && depth > zBuffer[point]){
          lumIndex = (int) 8 * lum;
          zBuffer[point] = depth;
          buffer[point] = chars[lumIndex];
        }
      }
    }

    std::cout << "\x1b[H";

    for(int i = 0; i < bufferLength; i++){
        if(i % scrWidth == 0){
          std::cout << std::endl;
        }else{
          std::cout << buffer[i];
        }
    }

    A += 0.001;
    B += 0.001;

  }
}