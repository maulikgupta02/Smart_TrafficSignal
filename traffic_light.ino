
#define s1 2
#define s2 3
#define s3 4
#define s4 5
#define p1 6
#define p2 7
#define p3 8
#define p4 9
#define r1 10
#define r2 11
#define r3 12
#define r4 13
#define g1 14
#define g2 15
#define g3 16
#define g4 17


char sensor[] = {s1, s2, s3, s4};
char button[] = {p1, p2, p3, p4};
char red[] = {r1, r2, r3, r4};
char green[] = {g1, g2, g3, g4};
int light_delay[] = {6000, 6000, 6000, 6000};




//SAFETY CLEARANCE
void blinkit(int k, int t)
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(green[t], LOW);
    digitalWrite(red[k], LOW);
    delay(200);
    digitalWrite(green[t], HIGH);
    digitalWrite(red[k], HIGH);
    delay(200);
  }
}




//EMERGENCY OVER-RIDE
int emergency(int p)
{
  int flag=0;
  for (int i = 0; i < 4; i++)
  {
    if (digitalRead(button[i]) == 1)
    {
      flag=1;
      blinkit(i, p); //Safety Time (Since we don't have yellow light)
      for (int j = 0; j < 4; j++)
      {
        digitalWrite(red[j],1);
        digitalWrite(green[j],0);
      }
      digitalWrite(red[i],0);
      digitalWrite(green[i],1);

      while (1)
      {
      int x=digitalRead(button[i]);
      if (x)
      {
      digitalWrite(red[i],0);
      digitalWrite(green[i],1);
      }
      else
      {
      break;
      }
      }
    }
  }
  return flag;
}



//LIGHT DURATION
void wait(int k, int p)
{

  for (int i = 200; i <= k; i += 200)
{
  delay(200);
  if (emergency(p))
  {
    break;
  }
  traffic_check(light_delay);
}

}




//HIGH-TRAFFIC ALERT
void traffic_check(int light_delay[])
{
  for (int i = 0; i < 4; i++)
  {
    if (digitalRead(sensor[i]))
    {
      light_delay[i] = 9000;
    }
    else
    {
      light_delay[i] = 6000;
    }
  }
}



void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 4; i++)
  {
    pinMode(sensor[i], INPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(button[i], INPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(red[i], OUTPUT);
  } for (int i = 0; i < 4; i++)
  {
    pinMode(green[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(red[i], HIGH);
    digitalWrite(green[i], HIGH);
  }
  delay(1500); //Initial Check
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(red[i], HIGH);
    digitalWrite(green[i], LOW);
  }
}



void loop()
{
  for (int h = 0; h < 4; h++)
    {
      Serial.println(light_delay[h]);
    }
  for (int i = 0; i < 4; i++)
  {
    emergency(i);
    traffic_check(light_delay);
    for (int l = 0; l < 4; l++ &&l !=i)
  {
    digitalWrite(red[l], HIGH);
    digitalWrite(green[l], LOW);
  }
    digitalWrite(red[i], LOW);
    digitalWrite(green[i], HIGH);
    wait(light_delay[i], i);
  }

}
