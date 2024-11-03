#include <Servo.h>

#define TRG 2    // Trigger pin untuk sensor jarak ultrasonik
#define ECH 4    // Echo pin untuk sensor jarak ultrasonik
#define PINA 9   // Pin kontrol motor kiri
#define PINB 10  // Pin kontrol motor kiri
#define PINC 6   // Pin kontrol motor kanan
#define PIND 5   // Pin kontrol motor kanan
#define SERVO1 3   // Pin kontrol motor kanan

int Jdepan, Jkiri, Jkanan;  // Variabel untuk menyimpan jarak sensor
int cek = 15;               // Jarak cek untuk tindakan menghindar
Servo myServo;    

void setup() {
  Serial.begin(9600);
  pinMode(TRG, OUTPUT);   // Mengatur pin Trigger sebagai OUTPUT
  pinMode(ECH, INPUT);    // Mengatur pin Echo sebagai INPUT
  pinMode(PINA, OUTPUT);  // Mengatur pin kontrol motor kiri sebagai OUTPUT
  pinMode(PINB, OUTPUT);  // Mengatur pin kontrol motor kiri sebagai OUTPUT
  pinMode(PINC, OUTPUT);  // Mengatur pin kontrol motor kanan sebagai OUTPUT
  pinMode(PIND, OUTPUT);  // Mengatur pin kontrol motor kanan sebagai OUTPUT
  myServo.attach(SERVO1);  // Menghubungkan objek servo ke pin servo motor
  myServo.write(90);      // Menggerakkan servo motor ke posisi tengah
  delay(1000);            // Delay selama 1 detik
}
void loop() {
  Jdepan = jarak();  // Mengecek jarak di depan

  // Jika jarak di depan robot berada dalam kisaran tertentu (antara 0 hingga cek),
  // maka robot akan melakukan manuver menghindar
  if (Jdepan > 0 && Jdepan < cek) {
    back();  // Menggerakkan robot mundur
    delay(100);
    stop();  // Menghentikan robot
    delay(100);

    myServo.write(180);  // Menggerakkan servo ke posisi 180 derajat
    delay(500);
    Jkiri = jarak();

    myServo.write(0);  // Menggerakkan servo ke posisi 0 derajat
    delay(500);
    Jkanan = jarak();

    myServo.write(90);  // Menggerakkan servo kembali ke posisi tengah
    delay(250);

    // Membandingkan jarak di sebelah kiri dan kanan robot
    if (Jkiri > Jkanan) {
      while (Jdepan <= 15) {
        left();  // Menggerakkan robot ke kiri
        Jdepan = jarak();
      }
    } else if (Jkanan > Jkiri) {
      while (Jdepan <= 15) {
        right();  // Menggerakkan robot ke kanan
        Jdepan = jarak();
      }
    } else {
      stop();  // Jika jarak di kiri dan kanan sama, menghentikan robot
      delay(200);
    }
    stop();  // Menghentikan robot setelah menghindar
    delay(200);
  } else {
    maju();  // Jika tidak ada hambatan di depan, robot akan bergerak maju
  }
}

int jarak() {
  long duration;
  int distance;
  digitalWrite(TRG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRG, LOW);
  duration = pulseIn(ECH, HIGH);       // Mengukur durasi echo
  distance = (duration * 0.0343) / 2;  // Menghitung jarak berdasarkan durasi
  Serial.print("Distance sensor : ");
  Serial.println(distance);
  delay(10);
  return distance;
}

void maju() {
  digitalWrite(PINA, HIGH);  // Menggerakkan robot maju (motor kiri dan kanan berputar ke depan)
  digitalWrite(PINB, LOW);
  digitalWrite(PINC, HIGH);
  digitalWrite(PIND, LOW);
}

void right() {
  Serial.println("Kanan ");
  digitalWrite(PINA, HIGH);  // Menggerakkan robot ke kanan (motor kiri berputar ke depan, motor kanan berputar ke belakang)
  digitalWrite(PINB, LOW);
  digitalWrite(PINC, LOW);
  digitalWrite(PIND, HIGH);
  delay(10);
}

void left() {
  Serial.println("Kiri ");
  digitalWrite(PINA, LOW);  // Menggerakkan robot ke kiri (motor kiri berputar ke belakang, motor kanan berputar ke depan)
  digitalWrite(PINB, HIGH);
  digitalWrite(PINC, HIGH);
  digitalWrite(PIND, LOW);
  delay(10);
}

void stop() {
  Serial.println("Berhenti ");
  digitalWrite(PINA, LOW);  // Menghentikan kedua motor
  digitalWrite(PINB, LOW);
  digitalWrite(PINC, LOW);
  digitalWrite(PIND, LOW);
}

void back() {
  Serial.println("Mundur ");
  digitalWrite(PINA, LOW);  // Menggerakkan robot mundur (motor kiri dan kanan berputar ke belakang)
  digitalWrite(PINB, HIGH);
  digitalWrite(PINC, LOW);
  digitalWrite(PIND, HIGH);
}
