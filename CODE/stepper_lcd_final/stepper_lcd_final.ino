
//stepper_lcd_final @anuvindasok.com
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>
AccelStepper stepper(4, 3, 4, 5, 6); //4 wire stepper, blue, red, green, black
//LiquidCrystal lcd(6, 7, 5, 4, 3, 2);  //RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  //RS, E, D4, D5, D6, D7
Bounce button = Bounce();


const int fullStepCount = 200;
int stepPos = 0;
int stepCounter = 0;


int FstepperCounter = 0; //Counts the forward rotations
int BstepperCounter = 0; //Counts the backward rotations


int buttonPin = 2;
boolean buttonOn = false;

int forwardCount = 0;
int backwardCount = 0;
int currentCounter = 0;

int buzzer = 13;
//String directionText = "";

//Twister State: 0 = OFF, 1 = FORWARD, 2 = BACKWARD, 3 = FINISHED
int twisterState = 0;

//Poteniometer Settings
int potOne = 0;
int potTwo = 1;
int potOneVal = 0;
int potTwoVal = 0;
int mapPotOneVal = 0;
int mapPotTwoVal = 0;

void setup() {
  Serial.begin(9600);

  stepper.setMaxSpeed(500);
  stepper.setAcceleration(200);
  button.attach(buttonPin);
  button.interval(5);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  delay(2000);

  lcd.clear();
  lcd.print("Tetrode Twister");
  lcd.setCursor(0, 1);
  lcd.print("Version 2.0");
  delay(2000);

  lcd.clear();
  lcd.print("Live Free");
  lcd.setCursor(0, 1);
  lcd.print("Or Die Hard!");
  delay(2000);
  lcd.clear();

  pinMode(buzzer, OUTPUT);
}

unsigned long prev = 0;
int inter = 200;


void loop() {

  updatePots();
  updateLCD();



  updateStepper();
}

void updateStepper() {

  stepper.run();
  button.update();

  boolean newState = button.rose();

  if (newState == true) {
    buttonOn = !buttonOn;
    twisterState = 1;
  }

  if (buttonOn == false) {
    twisterState = 0;
  }

  if (twisterState == 1) {
    stepper.move(200);
    stepPos = abs(stepper.currentPosition());

    stepCounter = stepPos % fullStepCount;
    if (stepCounter == 100) {
      currentCounter += 1;

      //delay(10);
    }
    if (currentCounter > forwardCount) {
      twisterState = 2;
      currentCounter = 0;
    }
  }

  else if (twisterState == 2) {
    stepper.move(-200);
    stepPos = abs(stepper.currentPosition());

    stepCounter = stepPos % fullStepCount;

    if (stepCounter == 100) {
      currentCounter += 1;

      //delay(10);
    }
    if (currentCounter > forwardCount) {
      twisterState = 3;
      currentCounter = 0;
    }
  } else if (twisterState == 0 || twisterState == 3) {
    stepper.move(0);
  }


  Serial.println(stepCounter);
  Serial.print("\t");
  Serial.print(FstepperCounter);
  Serial.print("\t");
  Serial.print(BstepperCounter);
  Serial.print(currentCounter);
  Serial.print("\t");
  Serial.println(twisterState);

  delay(10);
}

void updateLCD() {

  //if (twisterState < 3) {
  if (twisterState == 0) {
    lcd.setCursor(0, 0);
    lcd.print("FOR:");
    lcd.setCursor(4, 0);
    lcd.print("   ");
    lcd.setCursor(4, 0);
    lcd.print(forwardCount);

    lcd.setCursor(8, 0);
    lcd.print("REV:");
    lcd.setCursor(12, 0);
    lcd.print("   ");
    lcd.setCursor(12, 0);
    lcd.print(backwardCount);
  } else if ( twisterState == 1 || twisterState == 2) {

    lcd.setCursor(0, 0);
    lcd.print("FOR:");
    lcd.setCursor(4, 0);
    lcd.print(forwardCount);

    lcd.setCursor(8, 0);
    lcd.print("REV:");
    lcd.setCursor(12, 0);
    lcd.print(backwardCount);


    lcd.setCursor(0, 1);
    lcd.print("COUNT:");

    lcd.setCursor(6, 1);
    lcd.print(currentCounter);

    lcd.setCursor(9, 1);
    lcd.print("STATE:");
    lcd.setCursor(15, 1);
    lcd.print(twisterState);

  } else if (twisterState == 3) {
    delay(2000);
    lcd.clear();
    lcd.print("TWISTER FINISHED");
  }

}

void updatePots() {

  if (twisterState == 0) {
    potOneVal = analogRead(potOne);
    potTwoVal = analogRead(potTwo);
    mapPotOneVal = map(potOneVal, 0, 1023, 0, 100);
    mapPotTwoVal = map(potTwoVal, 0, 1023, 0, 100);
  }
  forwardCount = mapPotOneVal;
  backwardCount = mapPotTwoVal;
}
