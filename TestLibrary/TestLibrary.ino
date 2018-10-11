#include <LiquidCrystal.h>
#include <ACS712.h>
#include <Keypad.h>
unsigned long startMillis;
unsigned long endMillis;
double energy = 0;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[ROWS] = {3, 2, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 6, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // set Key pad 
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); // set LCD
// We have 5 amps version sensor connected to A0 pin of arduino
// Replace with your version if necessary
ACS712 sensor(ACS712_05B, A0);


void setup(){
  Serial.begin(9600);
  
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  int zero = sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);
  lcd.begin(16, 2);
  pinMode(13, OUTPUT);
  startMillis = millis();
}

void loop(){
    // Read current from sensor
  float I1 = 0;  
  for(int i = 0; i < 1000; i++) {
    I1 = I1 + (.0264 * analogRead(A0) -13.51);
    delay(1);
  }
  I1 = I1 /1000;
 
  Serial.println("Total Current: ");
  // Send it to serial
  Serial.println(String("I = ") + I1 + " A");
  double power = I1 * 5;
  endMillis = millis();
  unsigned long totalTime = totalTime+(endMillis - startMillis);
//  Serial.print("Time ");
//  Serial.println(totalTime);
  energy=(totalTime/1000)*power;
  if(energy  >= 5){
  digitalWrite(13, HIGH);
  }
  
  lcd.setCursor(0, 1);
  lcd.println("Cuurent ");
  lcd.println(I1);
  startMillis = millis();
  // Wait a second before the new measurement
  delay(1000);
//  char key = keypad.getKey();
//  Serial.println(key);
//  lcd.print(key);
  lcd.clear();

}
