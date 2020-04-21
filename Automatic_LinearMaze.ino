/*
  Analog Input 
  used for automatic reward delivery / linear maze / Buzsaki lab 
  By Farnaz Sharif Sept 2019
*/


int rightIRsensor = A0;    // Signal for the right Sensor
int leftIRsensor = A1;    // Signal for the left Sensor
const int rightTTL = 52;      // select the pin for the intan
const int leftTTL = 53;      // select the pin for the intan

boolean EnableWaterRight = true;
boolean EnableWaterLeft = false;
int CurrentRightInfraState = 0;
int CurrentLeftInfraState = 0;
int PreviousRightInfraState = 0;
int PreviousLeftInfraState = 0;

//Declare pin functions on Arduino for BigEasy driver (syringe pump motor)

#define dirLeft 2// 
#define stpLeft 3// 
#define MS3Left 4// 
#define MS2Left 5// 
#define MS1Left 6// 
#define ENLeft  7//

#define dirRight 8
#define stpRight 9
#define MS3Right 10
#define MS2Right 11
#define MS1Right 12
#define ENRight  13

char user_input;
int x;
int y;
int state;

//Declare pin functions on Arduino for BigEasy driver (syringe pump motor)
void setup() {
  pinMode(rightIRsensor, INPUT);
  pinMode(leftIRsensor, INPUT);
  pinMode(rightTTL, OUTPUT);
  pinMode(leftTTL, OUTPUT);
  pinMode(stpRight, OUTPUT);
  pinMode(dirRight, OUTPUT);
  pinMode(MS1Right, OUTPUT);
  pinMode(MS2Right, OUTPUT);
  pinMode(MS3Right, OUTPUT);
  pinMode(ENRight, OUTPUT);
  pinMode(stpLeft, OUTPUT);// # Farnaz
  pinMode(dirLeft, OUTPUT);// # Farnaz
  pinMode(MS1Left, OUTPUT);// # Farnaz
  pinMode(MS2Left, OUTPUT);// # Farnaz
  pinMode(MS3Left, OUTPUT);// # Farnaz
  pinMode(ENLeft, OUTPUT);// # Farnaz
  
  resetBEDPinsLeft(); //Set step, direction, microstep and enable pins to default states . #Farnaz what is this command?
  resetBEDPinsRight();
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  CurrentRightInfraState = analogRead(rightIRsensor);
  CurrentLeftInfraState = analogRead(leftIRsensor);
  
  //Serial.println(CurrentRightInfraState);
  if ((CurrentRightInfraState < 100) && (PreviousRightInfraState > 600))  // detect the animal is breaking the IR beam -> goes from Hi->Lo
  {
    if (EnableWaterRight == true)
    {
      Serial.println("Animal is detected on the right");
      EnableWaterRight = !EnableWaterRight;
      EnableWaterLeft = !EnableWaterLeft;
      // turn the ledPin on
      digitalWrite(rightTTL, HIGH);
      delay(10);
      digitalWrite(rightTTL, LOW);
      //start motor
      digitalWrite(ENRight, LOW);        //Pull enable pin low to set FETs active and allow motor control
      StepForwardDefaultRight(); //CONSIDER change it to slower water delivery!!! ASK BRIAN
      resetBEDPinsRight();
    }
  }
  
  if ((CurrentLeftInfraState < 100) && (PreviousLeftInfraState > 600)) // detect the animal is breaking the IR beam -> goes from Hi->Lo
  {
    if (EnableWaterLeft == true)
    {
      Serial.println("Animal is detected on the left");
      EnableWaterRight = !EnableWaterRight;
      EnableWaterLeft = !EnableWaterLeft;
      // turn the ledPin on
      digitalWrite(leftTTL, HIGH);
      delay(10);
      digitalWrite(leftTTL, LOW);
      //start motor
      digitalWrite(ENLeft, LOW);        //Pull enable pin low to set FETs active and allow motor control
      StepForwardDefaultLeft(); //CONSIDER change it to slower water delivery!!! ASK BRIAN
      resetBEDPinsLeft();
    }
   }
  
  PreviousRightInfraState = CurrentRightInfraState; //save the previous IR state
  PreviousLeftInfraState = CurrentRightInfraState; //save the previous IR state
}


//Reset Big Easy Driver pins to default states
void resetBEDPinsRight()
{
  digitalWrite(stpRight, LOW);
  digitalWrite(dirRight, LOW);
  digitalWrite(MS1Right, HIGH);
  digitalWrite(MS2Right, LOW);
  digitalWrite(MS3Right, LOW);
  digitalWrite(ENRight, HIGH);
}

//Default microstep mode function
void StepForwardDefaultRight()
{
  digitalWrite(dirRight, LOW); //Pull direction pin low to move "forward"
  for (x = 20; x < 100; x++) //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpRight, HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpRight, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}



//Reset Big Easy Driver pins to default states
void resetBEDPinsLeft()
{
  digitalWrite(stpLeft, LOW);
  digitalWrite(dirLeft, LOW);
  digitalWrite(MS1Left, HIGH);
  digitalWrite(MS2Left, LOW);
  digitalWrite(MS3Left, LOW);
  digitalWrite(ENLeft, HIGH);
}

//Default microstep mode function
void StepForwardDefaultLeft()
{
  digitalWrite(dirLeft, LOW); //Pull direction pin low to move "forward"
  for (x = 20; x < 100; x++) //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpLeft, HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stpLeft, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}
