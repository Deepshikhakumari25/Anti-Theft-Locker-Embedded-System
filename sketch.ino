#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- PIN CONFIGURATION ----------

const int SERVO_PIN = 10;
const int BUZZER_PIN = 11;
const int RED_LED_PIN = 12;
const int GREEN_LED_PIN = 13;

// ---------- SERVO ----------

const int LOCK_ANGLE = 0;
const int UNLOCK_ANGLE = 90;

// ---------- SECURITY ----------

const int MAX_ATTEMPTS = 3;
const unsigned long UNLOCK_TIME = 5000;
const unsigned long ALARM_TIME = 5000;

// ---------- PASSWORD ----------

const char STORED_PASSWORD[] = "1234";

// ---------- KEYPAD ----------

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// ---------- COMPONENTS ----------

Servo lockerServo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- VARIABLES ----------

String enteredPassword = "";

int failedAttempts = 0;

bool lockerUnlocked = false;

unsigned long unlockStartTime = 0;


// ---------- SETUP ----------

void setup()
{
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  lockerServo.attach(SERVO_PIN);

  lcd.init();
  lcd.backlight();

  lockLocker();

  showLockedScreen();

  Serial.println("================================");
  Serial.println(" ANTI-THEFT LOCKER SYSTEM");
  Serial.println("================================");
  Serial.println("System Started");
  Serial.println("Locker Status: LOCKED");
}


// ---------- MAIN LOOP ----------

void loop()
{
  // Automatic locking
  if (lockerUnlocked)
  {
    if (millis() - unlockStartTime >= UNLOCK_TIME)
    {
      lockLocker();

      Serial.println("Auto-lock activated.");
      Serial.println("Locker Status: LOCKED");

      showLockedScreen();
    }

    return;
  }

  char key = keypad.getKey();

  if (key)
  {
    handleKey(key);
  }
}


// ---------- KEYPAD HANDLER ----------

void handleKey(char key)
{
  Serial.print("Key Pressed: ");
  Serial.println(key);

  // Clear password
  if (key == '*')
  {
    enteredPassword = "";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    lcd.setCursor(0, 1);

    return;
  }

  // Submit password
  if (key == '#')
  {
    verifyPassword();
    return;
  }

  // Add numeric key
  if (key >= '0' && key <= '9')
  {
    if (enteredPassword.length() < 15)
    {
      enteredPassword += key;

      lcd.setCursor(0, 1);

      for (unsigned int i = 0; i < enteredPassword.length(); i++)
      {
        lcd.print('*');
      }
    }
  }
}


// ---------- PASSWORD VERIFICATION ----------

void verifyPassword()
{
  if (enteredPassword.length() == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");

    delay(1000);

    showLockedScreen();

    return;
  }

  if (enteredPassword.equals(STORED_PASSWORD))
  {
    failedAttempts = 0;

    Serial.println("ACCESS GRANTED");

    grantAccess();
  }
  else
  {
    failedAttempts++;

    Serial.print("ACCESS DENIED - Attempt ");
    Serial.println(failedAttempts);

    denyAccess();

    if (failedAttempts >= MAX_ATTEMPTS)
    {
      activateAlarm();

      failedAttempts = 0;

      showLockedScreen();
    }
    else
    {
      delay(1200);
      showLockedScreen();
    }
  }

  enteredPassword = "";
}


// ---------- ACCESS GRANTED ----------

void grantAccess()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Access Granted");

  lcd.setCursor(0, 1);
  lcd.print("Locker Open");

  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);

  tone(BUZZER_PIN, 2000, 150);

  unlockLocker();

  Serial.println("Locker UNLOCKED");

  unlockStartTime = millis();
}


// ---------- ACCESS DENIED ----------

void denyAccess()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Access Denied");

  lcd.setCursor(0, 1);
  lcd.print("Try Again");

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  tone(BUZZER_PIN, 1000, 400);
}


// ---------- ALARM ----------

void activateAlarm()
{
  Serial.println("================================");
  Serial.println("!!! SECURITY ALARM !!!");
  Serial.println("Too Many Wrong Attempts");
  Serial.println("================================");

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("TOO MANY");

  lcd.setCursor(0, 1);
  lcd.print("ATTEMPTS!");

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  unsigned long alarmStart = millis();

  while (millis() - alarmStart < ALARM_TIME)
  {
    tone(BUZZER_PIN, 1500);

    delay(250);

    noTone(BUZZER_PIN);

    delay(100);
  }

  noTone(BUZZER_PIN);
}


// ---------- LOCK ----------

void lockLocker()
{
  lockerServo.write(LOCK_ANGLE);

  lockerUnlocked = false;

  digitalWrite(GREEN_LED_PIN, LOW);
}


// ---------- UNLOCK ----------

void unlockLocker()
{
  lockerServo.write(UNLOCK_ANGLE);

  lockerUnlocked = true;
}


// ---------- LCD LOCKED SCREEN ----------

void showLockedScreen()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LOCKER LOCKED");

  lcd.setCursor(0, 1);
  lcd.print("Enter Password:");

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  enteredPassword = "";
}