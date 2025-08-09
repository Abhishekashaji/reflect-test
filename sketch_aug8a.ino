#include <TM1637Display.h>

// ===============================
// TM1637 setup
// ===============================
// First TM1637 - for time
#define CLK1 7
#define DIO1 8
TM1637Display displayTime(CLK1, DIO1);

// Second TM1637 - for pressing count
#define CLK2 9
#define DIO2 10
TM1637Display displayPoints(CLK2, DIO2);

int sequence[15] = {4, 6, 3, 5, 3, 5, 6, 4, 5, 3, 4, 5, 6, 4, 3};
volatile short int i = 0, point = 0;

void setup() {
  Serial.begin(9600);

  // LED pins
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(13, OUTPUT);

  // Button input
  pinMode(2, INPUT);

  // Attach interrupt for counting
  attachInterrupt(digitalPinToInterrupt(2), point_count, RISING);

  // Initialize TM1637 displays
  displayTime.setBrightness(0x0f);   // Max brightness
  displayPoints.setBrightness(0x0f); // Max brightness

  // Clear displays
  displayTime.showNumberDec(0);
  displayPoints.showNumberDec(0);
}

void start_routine() {
  for (i = 0; i <= 2; i++) {
    digitalWrite(3, HIGH);
    delay(2000);
    digitalWrite(3, LOW);
    delay(1000);
  }
  Serial.println("GO");
}

void loop() {
  short int j;
  point = 0; // reset score at the start
  displayPoints.showNumberDec(point); // reset display

  start_routine();

  for (j = 0; j <= 1; j++) {
    for (i = 0; i <= 14; i++) {
      digitalWrite(sequence[i], HIGH);

      // Countdown from 2 seconds
      for (int t = 2; t >= 0; t--) {
        displayTime.showNumberDec(t);
        delay(500);
      }

      digitalWrite(sequence[i], LOW);

      // LED off wait time 2 sec with countdown
      for (int t = 2; t >= 0; t--) {
        displayTime.showNumberDec(t);
        delay(500);
      }
    }
  }

  Serial.print("Score: ");
  Serial.println(point);
  while (1); // stop program
}

void point_count() {
  digitalWrite(sequence[i], LOW); // optional: turn off LED
  point++;
  displayPoints.showNumberDec(point); // update points display
}
