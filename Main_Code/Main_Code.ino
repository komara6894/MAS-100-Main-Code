//Karim Omara
//komara6894@sdsu.edu
//EE496A Senior Design B.
//MAS-100 Control System.

//-------------------------------------------------------------------------------------
//Motor Control
//-------------------------------------------------------------------------------------
int M_Speed = A0;                           //Speed Refrence for the Motor.
int M_Enable = A1;                          //Enable Switch for the Motor.
int M_Reverse = A2;                         //Reverse Direction Switch for the Motor.
int M_Manual = A3;                          //Motor Manual Mode Activate.
int M_Up = A4;                              //Motor Manual Mode Move Up.
int M_Down = A5;                            //Motor Manual Mode Move Down.
int M_En = 23;                              //Enable Signal for Motor Driver.
int M_Dir = 22;                             //Direction Signal for Motor Driver.
int M_Pul = 2;                              //Pulse Signal for Motor Driver (PWM cabable).
int M_Driver_Activate = 40;                 //Stepper Motor Driver Power relay Signal.

//--------------------------------------------------------------------------------------
//Motor Indicators
//--------------------------------------------------------------------------------------
int M_Manual_Override = 41;                 //LED Indicator for Manual Mode Override Enable;
int M_Enabled = 28;                         //LED Indicator for Motor Enable.
int M_CW = 29;                              //LED Indicator for CW Direction.
int M_CCW = 30;                             //LED Indicator for CCW Direction.

//--------------------------------------------------------------------------------------
//Light Bar indicators
//--------------------------------------------------------------------------------------
int S_Rd = 24;                              //Light Bar Red Indicator.
int S_Og = 25;                              //Light Bar Orange Indicator.
int S_Gn = 26;                              //Light Bar Green Incicator.
int S_Buzz = 27;                            //Light Bar Buzzer signal.

//--------------------------------------------------------------------------------------
//Laser Light Sensors
//--------------------------------------------------------------------------------------
int L_Top_Shift = 31;                       //Laser Sensor Tower Top.
int L_Tray_Shift = 32;                      //Laser Sensor Tray Present.
int L_SizeA_Shift = 33;                     //Laser Sensor Tray Size A.
int L_SizeB_Shift = 34;                     //Laser Sensor Tray Size B.

//--------------------------------------------------------------------------------------
//Tower Mechanical Switches
//--------------------------------------------------------------------------------------
int B_Emergency = 35;                       //Emergency Switch Signal.
int B_RangeUp = 36;                         //Top Range Limit Switch.
int B_RangeDown = 37;                       //Bottom Range Limit Switch.

//--------------------------------------------------------------------------------------
//Interfaces Communications
//--------------------------------------------------------------------------------------
int LCD_RX = 18;                            //LCD Communication Port TX.
int LCD_TX = 19;                            //LCD Communication Port RX.
int Remote_TX = 0;                          //Remote Coms for System TX.
int Remote_RX = 1;                          //Remote Coms for System RX.
int Slave_RX = 16;                          //Slave system Coms TX.
int Slave_TX = 17;                          //Slave system Coms RX.
int Slave_Detect = 38;                      //Slave System detect signal.

//--------------------------------------------------------------------------------------
//ESD Monitor System
//--------------------------------------------------------------------------------------
int ESD_Ground = 39;                        //Tower Ground fail.

//--------------------------------------------------------------------------------------
//Function Variables
//--------------------------------------------------------------------------------------
boolean Enable = LOW;                       //Defualt State for Enable Pin is High to make the Motor Disabled by default.
boolean Reverse = LOW;                      //Defualt State for Direction Pin is Low to make the Motor Direction CW by defauult.
int Pulse_Delay = 500;                      //Pulse Delay Period.


void setup()
{
  Serial.begin(115200);                    //Initiating Remote serial communication for Main System.
  //Serial1.begin(115200);                   /Initiating serial communication for LCD.
  //Serial2.begin(115200);                   //Initiating serial communication for Slave System.  
  pinMode (M_Manual, INPUT);                 //Defining Pins Mode (Input).
  pinMode (M_Speed, INPUT);                  //Defining Pins Mode (Input).
  pinMode (M_Enable, INPUT);                 //Defining Pins Mode (Input).
  pinMode (M_Reverse, INPUT);                //Defining Pins Mode (Input).
  pinMode (M_Up, INPUT);                     //Defining Pins Mode (Input).
  pinMode (M_Down, INPUT);                   //Defining Pins Mode (Input).
  pinMode (M_Enabled, OUTPUT);               //Defining Pins Mode (Output).
  pinMode (M_Manual_Override, OUTPUT);       //Defining Pins Mode (Output).
  pinMode (M_Dir, OUTPUT);                   //Defining Pins Mode (Output).
  pinMode (M_Pul, OUTPUT);                   //Defining Pins Mode (Output).
  pinMode (M_En, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (M_CW, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (M_CCW, OUTPUT);                   //Defining Pins Mode (Output).
  pinMode (M_Driver_Activate, OUTPUT);       //Defining Pins Mode (Output).
  pinMode (S_Rd, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Og, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Gn, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Buzz, OUTPUT);                  //Defining Pins Mode (Output).
  pinMode (L_Top_Shift, INPUT);              //Defining Pins Mode (Input).
  pinMode (L_Tray_Shift, INPUT);             //Defining Pins Mode (Input).
  pinMode (L_SizeA_Shift, INPUT);            //Defining Pins Mode (Input).
  pinMode (L_SizeB_Shift, INPUT);            //Defining Pins Mode (Input).
  pinMode (B_Emergency, INPUT);              //Defining Pins Mode (Input).
  pinMode (B_RangeUp, INPUT);                //Defining Pins Mode (Input).
  pinMode (B_RangeDown, INPUT);              //Defining Pins Mode (Input).
  pinMode (Slave_Detect, INPUT);             //Defining Pins Mode (Input).
  pinMode (ESD_Ground, INPUT);               //Defining Pins Mode (Input).
  
  //--------------------------------------------------------------------------------------
  //Interrupt Functions.
  //--------------------------------------------------------------------------------------
  attachInterrupt(digitalPinToInterrupt(M_Manual_Override), Manual_Override, FALLING);         //Interrupt to Manual Override when Switch pressed.
  attachInterrupt(digitalPinToInterrupt(M_Enable), Enable_Motor, FALLING);                     //Interrupt to Enable Motor when Switch pressed.
  attachInterrupt(digitalPinToInterrupt(M_Reverse), Reverse_Motor, FALLING);                   //Interrupt to Reverse Motor when Switch pressed.
  attachInterrupt(digitalPinToInterrupt(M_Up), Move_Up, FALLING);                              //Interrupt to Move Motor Up when Switch pressed.
  attachInterrupt(digitalPinToInterrupt(M_Down), Move_Down, FALLING);                          //Interrupt to Move Motor Down when Switch pressed.

  //--------------------------------------------------------------------------------------
  //Default Values for LED Indicators.
  //--------------------------------------------------------------------------------------
  digitalWrite(M_En, Enable);                 //Motor Enable LED to Follow M_Enable Variable Logic (Activate Low).
  digitalWrite(M_CW, Reverse);                //Motor CW LED to Follow Reverse.
  digitalWrite(M_CCW, !Reverse);              //Motor CCW LED to Follow Oposite of Reverse.
  digitalWrite(S_Rd, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Og, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Gn, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Buzz, HIGH);                 //Default Status Light Buzzer Is High (Off).

}  
void loop()
{ 
  Pulse_Delay = map((analogRead(M_Speed)),0,1023,2500,35);
  digitalWrite(M_Dir, Reverse);
  digitalWrite(M_En, !Enable);
  digitalWrite(M_CW,Reverse);
  digitalWrite(M_CCW,!Reverse);
  digitalWrite(M_Pul, HIGH);
  delayMicroseconds(Pulse_Delay);
  digitalWrite(M_Pul, LOW);
}

//--------------------------------------------------------------------------------------
//Code Functions.
//--------------------------------------------------------------------------------------
void Manual_Override()
{
  
}

void Enable_Motor()
{
  Enable = !Enable;                         //Enabking or disabling the Motor By programming Oposite logic.
}

void Reverse_Motor()
{
  Reverse = !Reverse;                       //Revesing the Motor Direction By programming Oposite logic.
}

void Move_Up()
{
  boolean Reverse = LOW;                      
  digitalWrite(M_Dir, Reverse);
  digitalWrite(M_Pul, HIGH);
  delayMicroseconds(Pulse_Delay);
  digitalWrite(M_Pul, LOW);
}

void Move_Down()
{ 
  boolean Reverse = LOW;                      
  digitalWrite(M_Dir, Reverse);
  digitalWrite(M_Pul, HIGH);
  delayMicroseconds(Pulse_Delay);
  digitalWrite(M_Pul, LOW);
}
