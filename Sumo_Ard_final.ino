#include <Servo.h>
#include <string.h>

class Sumo_bot
{
  Servo motors[6];                                       // 0- Right foot           1- Left foot
  int offsets[6];                                        // 2- Right leg            3- Left leg
  int pos[6];                                            // 4- Right shoulder       5- Left shoulder

  public:
  Sumo_bot()
  {
    offsets[0]=94;         offsets[1]=84;
    offsets[2]=85;         offsets[3]=90;
    offsets[4]=72;         offsets[5]=150;
  }

  int get_pos(int id)
  {
    return pos[id];
  }
  void connec(int a,int b,int c,int d, int e,int f)                                       //takes pin no as arguments
  {
    motors[0].attach(a);            motors[1].attach(b);
    motors[2].attach(c);            motors[3].attach(d);
    motors[4].attach(e);            motors[5].attach(f);
  }

  void writee(int id,int new_pos)                                                         //sets angle from -90 to 90
  {
    motors[id].write(new_pos+offsets[id]);
    pos[id]=new_pos;
  }
  
  void init_pos()
  {
    set(0,0,0,0,0,0);
  }

  void set(int a,int b,int c,int d, int e,int f)                                          //sets all motors to respective angles
  {
    writee(0,a);            writee(1,b);
    writee(2,c);            writee(3,d);
    writee(4,e);            writee(5,f);
  }

};

Sumo_bot s;
char dataIn, determinant, det;

void ft_walk(int delayy,int tilt,int twist)          //works fine...see how to take one step at a time
{
  
  
  {
    s.set(tilt,s.get_pos(1),twist,twist,s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(s.get_pos(0),tilt,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(s.get_pos(0),-tilt,-twist,-twist,s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(-tilt,s.get_pos(1),s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
    delay(delayy);
  }
}

void bk_walk(int delayy,int tilt,int twist)         //works
{

  {
    s.set(tilt,s.get_pos(1),-twist,-twist,s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(s.get_pos(0),tilt,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(s.get_pos(0),-tilt,twist,twist,s.get_pos(4),s.get_pos(5));
    delay(delayy);
    s.set(-tilt,s.get_pos(1),s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
    delay(delayy);
  }
}

void turn(char* dir)              //turns 15 deg in dir direction
{                 //works
  int twist;
  if (!strcmp(dir,"left"))
    twist=15;
  else
    twist=-15;
  s.init_pos();
  s.set(20,s.get_pos(1),s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(400);
  s.set(s.get_pos(0),20,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(1000);
  s.set(s.get_pos(0),s.get_pos(1),twist,s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(1000);
  s.set(-20,-20,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(1000);
  s.set(s.get_pos(0),s.get_pos(1),0,s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(1000);
  s.set(0,0,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(1000);
  s.init_pos();
}

int check()
{
  if (Serial.available() > 0)
  {
    dataIn = Serial.read();
    if (dataIn == 'F')
      determinant = 'F';
    else if (dataIn == 'B')
      determinant = 'B';
    else if (dataIn == 'L')
      determinant = 'L';
    else if (dataIn == 'R')
      determinant = 'R';
    else if (dataIn == 'S')
      determinant = 'S';
    else if (dataIn == 'W'||dataIn == 'w')
      determinant = 'W';
    else if (dataIn == 'U'||dataIn == 'u')
      determinant = 'U';
    else
      determinant = 'N';
  }
  return determinant;
}

void wt_shift()
{
  s.set(20,s.get_pos(1),s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(400);
  s.set(s.get_pos(0),-20,s.get_pos(2),s.get_pos(3),s.get_pos(4),s.get_pos(5));
  delay(500);
}
void setup()
{
  s.connec(9,6,5,3,10,11);
  s.init_pos();
  Serial.begin(9600);
  delay(3000);
}

void loop()
{
  det = check(); 
  switch (det)
  {
    case 'F': // F, move forward
              Serial.print(det);
              ft_walk(500,15,30);
              det = check();
              break;

    case 'B': // B, move back
              Serial.print(det);
              bk_walk(500,15,30);
              det = check();
              break;

    case 'L':// L, move left
              Serial.print(det);
              turn("left");
              det = check();
              break;

    case 'R': // R, move right
              Serial.print(det);
              turn("right");
              det = check();
              break;

    case 'S': // S, stop
              Serial.print(det);
              det = check();
              break;

    case 'U': // U, weight shift
              wt_shift();
              break;

    case 'W': // W, Init pos
              s.init_pos();
              break;

    default : break;
  }
}
