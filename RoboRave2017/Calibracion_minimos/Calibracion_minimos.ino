
int PIN_S_DER = 11;
int PIN_S_IZQ = 13;
#define PIN_DER_EXT 9
#define PIN_IZQ_EXT 8

//estados
#define motores(a,b) motor_izq.setSpeed(a);motor_der.setSpeed(b);


int s_izq;
int s_der;
int s_izq_e;
int s_der_e;


unsigned long resta;


int min_s_izq = 1024,
    min_s_der = 1024,
    min_izq_e = 1024,
    min_der_e = 1024;

void setup() {
  Serial.begin(9600);
  resta = millis();
  pinMode(25, OUTPUT);
  digitalWrite(25, 1);
}

void loop() {
  s_izq = analogRead(PIN_S_IZQ);
  s_der = analogRead(PIN_S_DER);
  s_izq_e = analogRead(PIN_IZQ_EXT);
  s_der_e = analogRead(PIN_DER_EXT);
  if (s_izq < min_s_izq)
    min_s_izq = s_izq;
  if (s_der < min_s_der)
    min_s_der = s_der;
  if (s_izq_e < min_izq_e)
    min_izq_e = s_izq_e;
  if (s_der_e < min_der_e)
    min_der_e = s_der_e;
  if (millis() - resta > 1000) {
    Serial.print(min_izq_e);
    Serial.print("\t||\t");
    Serial.print(min_s_izq);
    Serial.print("\t||\t");
    Serial.print(min_s_der);
    Serial.print("\t||\t");
    Serial.println(min_der_e);
    resta = millis();
  }
}
