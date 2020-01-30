////////////////////////////////////////////////////////////////
// Bibliothek zur Ansteuerung von drei A4988-Treibern         //
// Kompatibel zur Arduino-Plattform                           //
// Stand 30/01/2020 - Entwickelt von Finn Liebner             //
////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "Tri-A4988.h"

////////////////////////////////////////////////////////////////

#define DIR_X  2
#define STP_X  3
#define DIR_Y  4
#define STP_Y  5
#define DIR_Z  6
#define STP_Z  7
#define MS0    8
#define MS1    9
#define MS2   10

////////////////////////////////////////////////////////////////

void KL::Init(void)
{
  pinMode(DIR_X, OUTPUT);
  pinMode(STP_X, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(STP_Y, OUTPUT);
  pinMode(DIR_Z, OUTPUT);
  pinMode(STP_Z, OUTPUT);
  pinMode(MS0,   OUTPUT);
  pinMode(MS1,   OUTPUT);
  pinMode(MS2,   OUTPUT);

  digitalWrite(DIR_X, 0);
  digitalWrite(STP_X, 0);
  digitalWrite(DIR_Y, 0);
  digitalWrite(STP_Y, 0);
  digitalWrite(DIR_Z, 0);
  digitalWrite(STP_Z, 0);
  digitalWrite(MS0,   0);
  digitalWrite(MS1,   0);
  digitalWrite(MS2,   0);
}

////////////////////////////////////////////////////////////////

unsigned int period = 2000;

void KL::Interval(unsigned int v)
{
  period = v;
}

////////////////////////////////////////////////////////////////

const bool MSLUT[6][3] =
{
  0, 0, 1, 0, 1, 1,
  0, 0, 0, 1, 1, 1,
  0, 0, 0, 0, 0, 1
};

void KL::Microsteps(byte res)
{
  byte n = 0;

  switch(res)
  {
    case  1: n = 1; break;
    case  2: n = 2; break;
    case  4: n = 3; break;
    case  8: n = 4; break;
    case 16: n = 5; break;
  }

  digitalWrite(MS0, MSLUT[n][0]);
  digitalWrite(MS1, MSLUT[n][1]);
  digitalWrite(MS2, MSLUT[n][2]);
}

////////////////////////////////////////////////////////////////

void KL::Rotate(int x, int y, int z)
{
  unsigned long r[4];
  unsigned long t[3];
  bool s[3] = {1, 1, 1};
  int stp[3] = {x, y, z};
  unsigned int cnt[2] = {0, 0};

  if(stp[0] < 0)
  {
    digitalWrite(DIR_X, 0);
    stp[0] = stp[0] * (-1);
  }
  else
  {
    digitalWrite(DIR_X, 1);
  }

  if(stp[1] < 0)
  {
    digitalWrite(DIR_Y, 0);
    stp[1] = stp[1] * (-1);
  }
  else
  {
    digitalWrite(DIR_Y, 1);
  }

  if(stp[2] < 0)
  {
    digitalWrite(DIR_Z, 0);
    stp[2] = stp[2] * (-1);
  }
  else
  {
    digitalWrite(DIR_Z, 1);
  }

  if((stp[0] > stp[1]) && (stp[0] > stp[2]))
  {
    r[0] = period;
    r[1] = (stp[0] * period) / stp[1];
    r[2] = (stp[0] * period) / stp[2];
  }
  else if((stp[1] > stp[2]) && (stp[1] > stp[3]))
  {
    r[0] = (stp[0] * period) / stp[0];
    r[1] = period;
    r[2] = (stp[0] * period) / stp[2];
  }
  else
  {
    r[0] = (stp[0] * period) / stp[0];
    r[1] = (stp[0] * period) / stp[1];
    r[2] = period;
  }

  cnt[0] = (stp[0] + stp[1] + stp[2]) * 2;

  r[3] = micros();
  t[0] = r[0] + r[3];
  t[1] = r[1] + r[3];
  t[2] = r[2] + r[3];

  while(cnt[0] != cnt[1])
  {
    if(t[0] <= micros())
    {
      digitalWrite(STP_X, s[0]);
      t[0] = t[0] + r[0];
      s[0] = !s[0];
      cnt[1]++;
    }

    if(t[1] <= micros())
    {
      digitalWrite(STP_Y, s[1]);
      t[1] = t[1] + r[1];
      s[1] = !s[1];
      cnt[1]++;
    }

    if(t[2] <= micros())
    {
      digitalWrite(STP_Z, s[2]);
      t[2] = t[2] + r[2];
      s[2] = !s[2];
      cnt[1]++;
    }
  }

  digitalWrite(STP_X, 0);
  digitalWrite(STP_Y, 0);
  digitalWrite(STP_Z, 0);
}

////////////////////////////////////////////////////////////////
