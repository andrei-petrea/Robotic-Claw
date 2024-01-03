#include <IRremote.h>
#include <Servo.h>

// addresses for keys on IR remote control
#define left1 0x8C22657B
#define left2 0xFF10EF
#define right1 0x449E79F
#define right2 0xFF5AA5
#define claw_open1 0x511DBB
#define claw_open2 0xFF629D
#define claw_close1 0xA3C8EDDB
#define claw_close2 0xFFA857
#define up1 0x3D9AE3F7
#define up2 0xFF18E7
#define down1 0x1BC0157B
#define down2 0xFF4AB5
#define forward1 0x52A3D41F
#define forward2 0xFF22DD
#define backward1 0x20FE4DBB
#define backward2 0xFFC23D
#define continuous_motion1 0xFFFFFFFF
#define continuous_motion2 0xFFFF
#define continuous_motion3 0xFF

int IRpin = 11;
int serv1 = 3; // servo for claw
int serv2 = 9; // servo for moving left to right
int serv3 = 5; // servo for going up and down
int serv4 = 6; // servo for going forward and backwards

Servo servo1;  // servo for claw
Servo servo2;  // servo for moving left to right
Servo servo3;  // servo for going up and down
Servo servo4;  // servo for going forward and backwards

IRrecv irrecv(IRpin);
decode_results results;

struct angle_motion {
  int val;
  bool direct;
};

angle_motion claw;
angle_motion updown;
angle_motion forback;
angle_motion side;

int last_state = 0;  // remember which button was pressed last

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();   // Start the receiver
  servo1.attach(serv1);  // Attaching the servo motors
  
  servo2.attach(serv2);
  servo3.attach(serv3);
  servo4.attach(serv4);
}

void loop()
{  
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX); // Print the Serial 'results.value'
    irrecv.resume(); // Receive the next value

    if (results.value == left1 || results.value == left2)
    {
      side.direct = false;
      side.val -= 3;
      last_state = 2;
    } else if (results.value == right1 || results.value == right2)
    {
      side.direct = true;
      side.val += 3;
      last_state = 2;
    } else if (results.value == claw_open1 || results.value == claw_open2) 
    {
      claw.direct = true;
      claw.val += 3;
      last_state = 1;
    } else if (results.value == claw_close1 || results.value == claw_close2) 
    {
      claw.direct = false;
      claw.val -= 3;
      last_state = 1;
    } else if (results.value == up1 || results.value == up2) 
    {
      updown.direct = true;
      updown.val += 3;
      last_state = 3;
    } else if (results.value == down1 || results.value == down2) 
    {
      updown.direct = false;
      updown.val -= 3;
      last_state = 3;
    } else if (results.value == forward1 || results.value == forward2) 
    {      
      forback.direct = true;
      forback.val += 3; 
      last_state = 4;     
    } else if (results.value == backward1 || results.value == backward2) 
    {
      forback.direct = false;
      forback.val -= 3; 
      last_state = 4;
    } else if (results.value == continuous_motion1 || results.value == continuous_motion2 || results.value == continuous_motion3)
    {
      if (last_state == 1 && claw.direct == true) claw.val += 3;
      else if (last_state == 1 && claw.direct == false) claw.val -= 3; 

      if (last_state == 2 && side.direct == true) side.val += 3;
      else if (last_state == 2 && side.direct == false) side.val -= 3; 

      if (last_state == 3 && updown.direct == true) updown.val += 3;
      else if (last_state == 3 && updown.direct == false) updown.val -= 3; 
      
      if (last_state == 4 && forback.direct == true) forback.val += 3;
      else if (last_state == 4 && forback.direct == false) forback.val -= 3; 
    }

  if (last_state == 1) servo1.write(claw.val);
  else if (last_state == 2) servo2.write(side.val);
  else if (last_state == 3) servo3.write(updown.val);
  else if (last_state == 4) servo4.write(forback.val);

  }
}

// 1 = E318261B
// 2 = 511DBB
// 3 = EE886D7F
// 4 = 52A3D41F
// 5 = D7E84B1B
// 6 = 20FE4DBB
// 7 = F076C13B
// 8 = A3C8EDDB
// 9 = E5CFBD7F
// 0 = 97483BFB
// * = C101E57B
// # = F0C41643
// UP = 3D9AE3F7
// DOWN = 1BC0157B
// LEFT = 8C22657B
// RIGHT = 449E79F
// OK = 488F3CBB
