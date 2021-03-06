// https://www.tinkercad.com/things/8UiU9b6vFgs-exquisite-bruticus/editel?tenant=circuits
// https://github.com/microsoft/vscode-arduino/issues/438#:~:text=On%20macOS%2C%20I%20added%20%23include%20%3CArduino.h%3E%20to%20beginning%20of%20.ino%20file%20and%20I%20filled%20c_cpp_properties.json%20content%20with%3A

#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // library for the LCD display w/ I2C Back
#include <Wire.h>

/* initialize the library by associating each LCD interface pins with the arduino pin number it is connected to */
// https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
LiquidCrystal_I2C lcd(0x27, 16, 2); // Pins A5 & A4

const int RED = 2;
const int GREEN = 3;
const int BLUE = 4;
const int YELLOW = 5;
const int PIEZO = 7;
const int REDB = 8;
const int GREENB = 9;
const int BLUEB = 10;
const int YELLOWB = 11;

const unsigned long INPUTTIME = 5000;
int order[100];
int i = 0;
unsigned long TIME = 0; // MUST NOT BE int - goes negative
bool LOST = false;

void setup()
{
  delay(100);
  Wire.begin(); // begin communication protocol to use two-wire communication/ I2C
  Serial.begin(9600);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.clear();      // clears the LCD screen to get started
  lcd.backlight();  // turns on the LCD backlight

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(REDB, INPUT_PULLUP); // Makes it HIGH to start so we can test for LOW
  pinMode(GREENB, INPUT_PULLUP);
  pinMode(BLUEB, INPUT_PULLUP);
  pinMode(YELLOWB, INPUT_PULLUP);
  randomSeed(analogRead(0) * analogRead(1));
  Serial.println("STARTING");
}

void loop()
{
  Serial.println("Starting Loop");
  if (LOST)
  {
    delay(10000000000); // Press Arduino reset to restart
  }
  while (true)
  {
    lcd.print("Level: ");
    lcd.print(i + 1);
    lcd.setCursor(0, 0);
    playSequence();
    TIME = millis();
    for (int j = 0; j < i; j++)
    {
      delay(50);
      if (!userInput(order[j]))
      {
        Serial.println("LOST");
        LOST = true;
        sorry();
        return;
      }
    }
  }
}

void sorry()
{
  lcd.setCursor(0, 1);
  lcd.print("GAME OVER :(");
  for (int j = 600; j > 100; j -= 5)
  {
    tone(PIEZO, j);
    delay(10);
  }
  noTone(PIEZO);
}

void playTone(int pin)
{
  digitalWrite(pin, HIGH);
  tone(PIEZO, pin * 100);
  delay(400);
  digitalWrite(pin, LOW);
  noTone(PIEZO);
  delay(200);
}

void playSequence()
{
  delay(1000);
  order[i] = random(RED, YELLOW + 1);
  for (int j = 0; j <= i; j++)
  {
    playTone(order[j]);
  }
  i++;
}

bool userInput(int light)
{
  while (true)
  {
    if (millis() >= TIME + INPUTTIME + i * 1000)
    {
      Serial.println("TIME UP");
      return false;
    }
    if (digitalRead(REDB) == LOW)
    {
      return userPressed(RED, light);
    }
    if (digitalRead(GREENB) == LOW)
    {
      return userPressed(GREEN, light);
    }
    if (digitalRead(BLUEB) == LOW)
    {
      return userPressed(BLUE, light);
    }
    if (digitalRead(YELLOWB) == LOW)
    {
      return userPressed(YELLOW, light);
    }
  }
}

bool userPressed(int color, int light)
{
  playTone(color);
  if (light == color)
  {
    return true;
  }
  else
  {
    return false;
  }
}