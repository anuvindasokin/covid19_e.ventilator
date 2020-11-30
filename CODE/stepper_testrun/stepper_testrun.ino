#define EN        8  
#define X_DIR     5 //Direction pin
#define X_STP     2 //Step pin

int delayTime=30; //Delay between each pause in uS
int stps=6400;// Steps to move

void step(boolean dir, byte dirPin, byte stepperPin, int steps)

{
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime); 
  }
}

void setup(){
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}

void loop(){

  step(false, X_DIR, X_STP, stps); //X, Clockwise
  delay(100);
  step(true, X_DIR, X_STP, stps); //X, Counterclockwise
  delay(100);
}
