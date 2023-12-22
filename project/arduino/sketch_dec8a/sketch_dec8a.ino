#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

#define SensorPin A0
#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define USONIC_DIV 0.034



#define RESTART_INTERVAL 301201 // 5 minutes in milliseconds  301201

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float calibration_value = 22.34 + 0.7;
unsigned long int avgValue;
int buf[10], temp;
unsigned long lastRestartTime = 0;
float phValue;
int percentage;
long distance;

void setup()
{
  pinMode(SensorPin, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);

  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.print("Initializing...");
  delay(1000);

  // pH Meter Code
  measurePH();

  // Ultrasonic Sensor Code
  measureDistance();

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.print("pH Value: ");
  tft.print(phValue);

  if (phValue < 7.0 || phValue >= 8.0)
  {
    Serial.println("reading:" + String(phValue) + " " + String(percentage)); //example    reading:4.8 68

  }
}

void loop()
{

  unsigned long currentMillis = millis();
  unsigned long elapsedRestartTime = currentMillis - lastRestartTime;

  measurePH();
  measureDistance();

  // Check if it's time to restart the Arduino
  if (elapsedRestartTime >= RESTART_INTERVAL && (phValue < 7.0 || phValue >= 8.0))
  {
    asm volatile("  jmp 0");
  }
  delay(1000);
}



void measurePH()
{
  // pH Meter Code
  for (int i = 0; i < 10; i++)
  {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }

  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for (int i = 2; i < 8; i++)
    avgValue += buf[i];

  phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = -5.70 * phValue + calibration_value;

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.print("pH Value: ");
  tft.print(phValue);

  tft.setCursor(0, 20);
  if (phValue < 4)
    tft.print("Very Acidic        ");
  else if (phValue >= 4 && phValue < 5)
    tft.print("Acidic        ");
  else if (phValue >= 5 && phValue < 7)
    tft.print("Acidic-ish    ");
  else if (phValue >= 7 && phValue < 8)
    tft.print("Neutral       ");
  else if (phValue >= 8 && phValue < 10)
    tft.print("Alkaline-ish  ");
  else if (phValue >= 10 && phValue < 11)
    tft.print("Alkaline      ");
  else if (phValue >= 11)
    tft.print("Very alkaline ");
}

void measureDistance()
{
  // Ultrasonic Sensor Code
  long duration;
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(11);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * USONIC_DIV / 2;
  percentage = map(distance, 10, 3, 0, 100);

  if (percentage < 0)
  {
    percentage = 0;
  }
  else if (percentage > 100)
  {
    percentage = 100;
  }

  tft.print("\nPercentage: ");
  tft.print(percentage);
  tft.print("% \nDistance: ");
  tft.print(distance);
  tft.print(" cm");
}
