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
int S_Rd = 27;                              //Light Bar Red Indicator.
int S_Og = 25;                              //Light Bar Orange Indicator.
int S_Gn = 24;                              //Light Bar Green Incicator.
int S_Buzz = 26;                            //Light Bar Buzzer signal.

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
int Pulse_Delay = 2500;                      //Pulse Delay Period.
int Slave = 0;
String Mode;
int Trays_Count = 0;
static bool runOnlyOnce = false;
static bool runOnlyOnceL1 = false;


void setup()
{
  Serial.begin(9600);                    //Initiating Remote serial communication for Main System.
  //Serial1.begin(9600);                   /Initiating serial communication for LCD.
  //Serial2.begin(9600);                   //Initiating serial communication for Slave System.
  pinMode (M_Manual, INPUT);                 //Defining Pins Mode (Input).
  pinMode (M_Speed, INPUT);                  //Defining Pins Mode (Input).
  digitalWrite(M_Speed, LOW);
  pinMode (M_Enable, INPUT);                 //Defining Pins Mode (Input).
  pinMode (M_Reverse, INPUT);                //Defining Pins Mode (Input).
  pinMode (M_Up, INPUT);                     //Defining Pins Mode (Input).
  pinMode (M_Down, INPUT);                   //Defining Pins Mode (Input).
  pinMode (M_Enabled, OUTPUT);               //Defining Pins Mode (Output).
  pinMode (M_Manual_Override, OUTPUT);       //Defining Pins Mode (Output).
  digitalWrite(M_Manual_Override, LOW);
  pinMode (M_Dir, OUTPUT);                   //Defining Pins Mode (Output).
  pinMode (M_Pul, OUTPUT);                   //Defining Pins Mode (Output).
  pinMode (M_En, OUTPUT);                    //Defining Pins Mode (Output).
  digitalWrite(M_En, LOW);
  pinMode (M_CW, OUTPUT);                    //Defining Pins Mode (Output).
  digitalWrite(M_CW, LOW);
  pinMode (M_CCW, OUTPUT);                   //Defining Pins Mode (Output).
  digitalWrite(M_CCW, LOW);
  pinMode (M_Driver_Activate, OUTPUT);       //Defining Pins Mode (Output).
  pinMode (S_Rd, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Og, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Gn, OUTPUT);                    //Defining Pins Mode (Output).
  pinMode (S_Buzz, OUTPUT);                  //Defining Pins Mode (Output).
  pinMode (L_Top_Shift, INPUT);              //Defining Pins Mode (Input).
  pinMode (L_Tray_Shift, INPUT);             //Defining Pins Mode (Input).
  pinMode (L_SizeA_Shift, INPUT);            //Defining Pins Mode (Input).
  pinMode (L_SizeB_Shift, INPUT);            //Defining Pins Mode (Input).
  pinMode (B_Emergency, INPUT_PULLUP);              //Defining Pins Mode (Input).
  pinMode (B_RangeUp, INPUT_PULLUP);                //Defining Pins Mode (Input).
  pinMode (B_RangeDown, INPUT_PULLUP);              //Defining Pins Mode (Input).
  pinMode (Slave_Detect, INPUT);             //Defining Pins Mode (Input).
  pinMode (ESD_Ground, INPUT);               //Defining Pins Mode (Input).
  delay(500);
  Serial.println("MAS-100 Stacker & Destacker");
  delay(100);
  Serial.println("initializing....");
  initializing();
  delay(100);

  //--------------------------------------------------------------------------------------
  //Interrupt Functions.
  //--------------------------------------------------------------------------------------
//attachInterrupt(digitalPinToInterrupt(M_Manual_Override), Manual_Override, FALLING);         //Interrupt to Manual Override when Switch pressed.
}


void loop() 
{
if (digitalRead(M_Enable) == LOW)
  {
    Enable = !Enable;                         //Enabling or disabling the Motor By programming Oposite logic.
    digitalWrite(M_Driver_Activate, Enable);
    digitalWrite(M_En, Enable);
    digitalWrite(M_Enabled, !Enable);                 //Motor Enable LED to Follow M_Enable Variable Logic (Activate Low)
    delay(200);
  }
  
  else if (digitalRead(M_Reverse) == LOW) 
 {
  Reverse = !Reverse;
  digitalWrite(M_Dir, Reverse);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, Reverse);                //Motor CW LED to Follow Reverse.
  digitalWrite(M_CCW, !Reverse);              //Motor CCW LED to Follow Oposite of Reverse
  delay(200);
 }
 
 else if (digitalRead(M_Up) == LOW)
 {
  Accelerate();
 
  //digitalWrite(M_Pul, HIGH);
  //delayMicroseconds(Pulse_Delay);
  //digitalWrite(M_Pul, LOW);
  //delayMicroseconds(Pulse_Delay);
/*     
 else if (digitalRead(M_Manual_Override) == LOW)
    {
      
      Accelerate();
    }
*/
}
}

void Accelerate()
{
  
 for (int x = 0; x < 2100; x++)
    {
      digitalWrite(M_Pul, HIGH);
      delayMicroseconds(Pulse_Delay);
      digitalWrite(M_Pul, LOW);
      delayMicroseconds(Pulse_Delay);
      Pulse_Delay--;
    }
 //delay(1000);
for(int x = 0; x < 69540; x++) 
  {
    digitalWrite(M_Pul,HIGH); 
    delayMicroseconds(Pulse_Delay); 
    digitalWrite(M_Pul,LOW); 
    delayMicroseconds(Pulse_Delay); 
  }
for (int y = 0; y < 2100; y++)
    {
      digitalWrite(M_Pul, HIGH);
      delayMicroseconds(Pulse_Delay);
      digitalWrite(M_Pul, LOW);
      delayMicroseconds(Pulse_Delay);
      Pulse_Delay++;
    } 
}
   
   //if (runOnlyOnceL1 = false)
   //{
    //Level1();
   //}
//Idle();
//delay(100);
//}

//--------------------------------------------------------------------------------------
//Code Functions.
//--------------------------------------------------------------------------------------
void initializing()
{
  digitalWrite(S_Rd, LOW);                   //Default Status Light LED Is High (Off).
  delay(1000);
  digitalWrite(S_Og, LOW);                   //Default Status Light LED Is High (Off).
  delay(1000);
  digitalWrite(S_Gn, LOW);                   //Default Status Light LED Is High (Off).
  delay(1000);
  digitalWrite(S_Buzz, LOW);                 //Default Status Light Buzzer Is High (Off).
  delay(1000);
  digitalWrite(S_Rd, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Og, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Gn, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(S_Buzz, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_Driver_Activate, HIGH);
  delay(400);
  digitalWrite(M_Driver_Activate, LOW);
  digitalWrite(Slave_Detect, LOW);
  Slave = analogRead(Slave_Detect);
  digitalWrite(M_Manual_Override, LOW);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_Enabled, LOW);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, LOW);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CCW, LOW);                   //Default Status Light LED Is High (Off).
  delay(1000);
  digitalWrite(M_Manual_Override, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_Enabled, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CCW, HIGH);
  Serial.println("Machine Initialized Successfully......");
  Serial.println("Type Command (stacker, destacker, manual) to start operation....");
  delay(200);
}

void Level1()
{
  if (Serial.available())
  {
    Mode = Serial.readStringUntil('\n');
    Mode.trim();
    if (Mode.equals("stacker"))
    {
      Serial.print("Mode: ");
      Serial.println(Mode);
      //runOnlyOnceL1 = true;
      Stacker();
    }

    else if (Mode.equals("destacker"))
    {
     Serial.print("Mode: ");
     Serial.println(Mode);
     //runOnlyOnceL1 = true;
     Destacker(); 
    }

    else if (Mode.equals("manual"))
    {
     Serial.print("Mode: ");
     Serial.println(Mode);
     //runOnlyOnceL1 = true; 
     Manual_Override();
    }

    else
    {
      Serial.println("Bad Command, Try again...");
    }
    Serial.print("Mode: ");
    Serial.println(Mode);
   // runOnlyOnceL1 = true;
  }
//Level1();
}
void Stacker()
{
  Stacker_Preset();

}
void Destacker()
{

}
void Manual_Override()
{
  //digitalWrite(M_Manual_Override, LOW);  
  //Pulse_Delay = map((analogRead(M_Speed)),0,1023,2500,300);
  //delay(1000);
  Serial.print("\n");
  Serial.print(Pulse_Delay);
  if (digitalRead(M_Enable) == LOW)
  {
    Enable = !Enable;                         //Enabling or disabling the Motor By programming Oposite logic.
    digitalWrite(M_Driver_Activate, Enable);
    digitalWrite(M_En, Enable);
    digitalWrite(M_Enabled, !Enable);                 //Motor Enable LED to Follow M_Enable Variable Logic (Activate Low)
    delay(100);
  }
 else if(digitalRead(M_Reverse) == LOW)
 {
  Reverse = !Reverse;
  digitalWrite(M_Dir, Reverse);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, Reverse);                //Motor CW LED to Follow Reverse.
  digitalWrite(M_CCW, !Reverse);              //Motor CCW LED to Follow Oposite of Reverse
  delay(200);
 }
while(digitalRead(M_Up) == LOW)
 {
  digitalWrite(M_Dir, HIGH);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, LOW);                //Motor CW LED to Follow Reverse.
  digitalWrite(M_CCW, HIGH);              //Motor CCW LED to Follow Oposite of Reverse
  digitalWrite(M_Pul, HIGH);
  delayMicroseconds(Pulse_Delay);
  digitalWrite(M_Pul, LOW);
  delayMicroseconds(Pulse_Delay);
 }
while(analogRead(M_Down) == LOW)
 {
  digitalWrite(M_Dir, LOW);                   //Default Status Light LED Is High (Off).
  digitalWrite(M_CW, HIGH);                //Motor CW LED to Follow Reverse.
  digitalWrite(M_CCW, LOW);              //Motor CCW LED to Follow Oposite of Reverse
  digitalWrite(M_Pul, HIGH);
  delayMicroseconds(Pulse_Delay);
  digitalWrite(M_Pul, LOW);
  delayMicroseconds(Pulse_Delay);
 }
 delay(200);
 Manual_Override();
}


void Stacker_Preset()
{
  while ((digitalRead(L_SizeA_Shift) == HIGH) && ((digitalRead(B_RangeDown) == HIGH)))
    {
    digitalWrite(M_Driver_Activate, HIGH);
    digitalWrite(M_Enabled, LOW);
    digitalWrite(M_Dir, HIGH);
    digitalWrite(M_CW, LOW);
    digitalWrite(M_CCW, HIGH);
    for (int x = 0; x < 2430; x++)
    {
      digitalWrite(M_Pul, HIGH);
      delayMicroseconds(Pulse_Delay);
      digitalWrite(M_Pul, LOW);
      delayMicroseconds(Pulse_Delay);
      Pulse_Delay--;
    }
    digitalWrite(M_Pul, HIGH);
    delayMicroseconds(Pulse_Delay);
    digitalWrite(M_Pul, LOW);
    delayMicroseconds(Pulse_Delay);
    }
  
  if((digitalRead(L_SizeA_Shift) == LOW) && ((digitalRead(B_RangeDown) == HIGH)))
    {
      Stacker_Preset_S();
    }
}

void Stacker_Preset_S()
{
  Pulse_Delay = 2500;
  while (digitalRead(B_RangeDown) == HIGH)
  {
    digitalWrite(M_Pul, HIGH);
    delayMicroseconds(Pulse_Delay);
    digitalWrite(M_Pul, LOW);
    delayMicroseconds(Pulse_Delay);
  }
  if (digitalRead(B_RangeDown) == LOW)
  {
    digitalWrite(M_Dir, LOW);
    digitalWrite(M_CW, HIGH);
    digitalWrite(M_CCW, LOW);
    for (int y = 0; y < 300; y++)
    {
      digitalWrite(M_Pul, HIGH);
      delayMicroseconds(Pulse_Delay);
      digitalWrite(M_Pul, LOW);
      delayMicroseconds(Pulse_Delay);
    }
   Next_Trigger();
  }
}
  void Next_Trigger()
  {
    while((digitalRead(L_Top_Shift) == HIGH) && (digitalRead(B_RangeDown) == HIGH))
    {
      digitalWrite(M_Pul, HIGH);
      delayMicroseconds(Pulse_Delay);
      digitalWrite(M_Pul, LOW);
      delayMicroseconds(Pulse_Delay);
    }
   
    while((digitalRead(L_Top_Shift) == LOW) && (digitalRead(B_RangeDown) == HIGH))
    {
       Idle();
    }  
    if (digitalRead(B_RangeDown) == LOW)
    {
      Mode_End();
    }
  }
  
 void Mode_End()
 {
  
 }
void Idle()
{
   if (runOnlyOnce == false) 
   {
      Trays_Count++;
      runOnlyOnce = true;
   }
 Serial.println("Processed Trays Count =");
 Serial.print(Trays_Count);
 delay(1000);
}
 void Trays_Processed()
 {
      Trays_Count++;
      Serial.println("Processed Trays Count =");
      Serial.print(Trays_Count);
 }
