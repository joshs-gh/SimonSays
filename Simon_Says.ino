// https://www.tinkercad.com/things/8UiU9b6vFgs-exquisite-bruticus/editel?tenant=circuits

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
unsigned long TIME = 0;   // MUST NOT BE int - goes negative
bool LOST = false;

void setup()
{
  delay(100);
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(REDB, INPUT_PULLUP);      // Makes it HIGH to start so we can test for LOW
  pinMode(GREENB, INPUT_PULLUP);
  pinMode(BLUEB, INPUT_PULLUP);
  pinMode(YELLOWB, INPUT_PULLUP);
  randomSeed(analogRead(0));
  Serial.println("STARTING");
}

void loop()
{
  if (LOST) {
    delay(10000000000);  // Press Arduino reset to restart
  }
  while (true) {
    playSequence();
    TIME = millis();
    for (int j=0; j < i; j++) {
      delay(100);
      if (!userInput(order[j])) {
        Serial.println("LOST");
        LOST = true;
        sorry();
        return;
      }
    }
  }
}

void sorry() {
  for (int j=600; j>100; j-=5){
    tone(PIEZO, j);
    delay(10);
  }
  noTone(PIEZO);
}

void playTone(int pin) {
  digitalWrite(pin, HIGH);
      tone(PIEZO, pin*100);
      delay(400);
      digitalWrite(pin, LOW);
      noTone(PIEZO);
      delay(200);
}

void playSequence() {
    delay(1000);
    order[i] = random(RED, YELLOW+1);
    for (int j=0; j <= i; j++) {
      playTone(order[j]);
    }
    i++;
}


bool userInput(int light) {
  while (true) {
    if (millis() >= TIME + INPUTTIME + i*1000) {
      Serial.println("TIME UP");
      return false;
    }
    if (digitalRead(REDB) == LOW) {
      playTone(RED);
      if (light == RED) {
        return true;
      }
      else {
        return false;
      }
    }
    if (digitalRead(GREENB) == LOW) {
      playTone(GREEN);
      if (light == GREEN) {
        return true;
      }
      else {
        return false;
      }
    }
    if (digitalRead(BLUEB) == LOW) {
      playTone(BLUE);
      if (light == BLUE) {
        return true;
      }
      else {
        return false;
      }
    }
    if (digitalRead(YELLOWB) == LOW) {
      playTone(YELLOW);
      if (light == YELLOW) {
        return true;
      }
      else {
        return false;
      }
    }
  }
}
