#include <avr/io.h>
#include <LiquidCrystal.h>

#define MAX_GOAL_TIME 1.0 //Max time in seconds for a goal to be considered
#define MIN_GOAL_INTERVAL 1  //Minimum time between goals (integer)

#define PIN_GOAL1_FRONT 18
#define PIN_GOAL1_BACK 19
#define PIN_GOAL2_FRONT 20
#define PIN_GOAL2_BACK 21

#define PIN_LCD1 8
#define PIN_LCD2 9
#define PIN_LCD3 4
#define PIN_LCD4 5
#define PIN_LCD5 6
#define PIN_LCD6 7
#define PIN_LCD_BACKLIGHT 10
#define PIN_LCD_BUTTONS 0


#define GOAL1_COLOR "YELLOW"
#define GOAL2_COLOR "BLACK"

#define INT_GOAL1_FRONT 5
#define INT_GOAL1_BACK 4
#define INT_GOAL2_FRONT 3
#define INT_GOAL2_BACK 2

#define GOAL1_DISTANCE 0.0326
#define GOAL2_DISTANCE 0.0339

long goal1_fronttime, goal2_fronttime, last_score;

LiquidCrystal lcd(PIN_LCD1, PIN_LCD2, PIN_LCD3, PIN_LCD4, PIN_LCD5, PIN_LCD6);

void goal1_front()
{
  goal1_fronttime = micros();
  noInterrupts();
  Serial.print("goal1 front ");
  Serial.println(goal1_fronttime);
  interrupts();
}

void goal2_front()
{
  goal2_fronttime = micros();  
  noInterrupts();
  Serial.print("goal2 front ");
  Serial.println(goal2_fronttime);
  interrupts();
}

void goal1_back()
{
  long backtime;
  backtime = micros();
  noInterrupts();
  Serial.print("goal1 back  ");
  Serial.println(backtime);
  score_goal(1, backtime);
  goal1_fronttime=0;
  interrupts();
}

void goal2_back()
{
  long backtime;
  backtime = micros();
  noInterrupts();
  Serial.println("goal2 back  ");
  Serial.println(backtime);
  score_goal(2, backtime);
  goal2_fronttime=0;
  interrupts();
}

int score_goal(int goal, long backtime)
{
  float t, v, v_k, d;
  long fronttime;
  
  //Do not count if last goal was too recent (bouncing ball)
  if( (backtime-last_score) < (1000000*MIN_GOAL_INTERVAL))
  {
    return 0; 
  }

  fronttime = (goal==1)?goal1_fronttime:goal2_fronttime;
  
  //Do not count if the front sensor has not triggered yet
  if(fronttime==0)
  {
    return 0; 
  }
    
  d = (goal==1)?GOAL1_DISTANCE:GOAL2_DISTANCE;
  
  t = 0.000001 * (backtime - fronttime);

  if(t>MAX_GOAL_TIME)
  {
    return 0; 
  }

  v = d / t;
  
  v_k = v * 3.6;
  
  Serial.print((goal==1)?GOAL1_COLOR:GOAL2_COLOR);
  Serial.print(" GOAL: ");
  Serial.println(v_k);
  Serial.println(backtime - fronttime);
  
  last_score = backtime; 
  
  return 1;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing");
  
  noInterrupts();
  
  pinMode(PIN_LCD_BUTTONS, INPUT);
  pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
  digitalWrite(10, HIGH);
  lcd.begin(16,2);
  
  goal1_fronttime = 0;
  goal2_fronttime = 0;
  last_score = 0;

  
  pinMode(PIN_GOAL1_FRONT, INPUT);
  pinMode(PIN_GOAL1_BACK, INPUT);
  pinMode(PIN_GOAL2_FRONT, INPUT);
  pinMode(PIN_GOAL2_BACK, INPUT);
  
  
  attachInterrupt(INT_GOAL1_FRONT, goal1_front, RISING);
  attachInterrupt(INT_GOAL1_BACK, goal1_back, RISING);
  attachInterrupt(INT_GOAL2_FRONT, goal2_front, RISING);
  attachInterrupt(INT_GOAL2_BACK, goal2_back, RISING);

  lcd.setCursor(0,0);
  lcd.print("Ready.");

 
  interrupts();
  Serial.println("Done.");
 
  
}



void loop()
{
  
}
