#include <DCMotor.h>
#include <Servo.h>

DCMotor motor_izq(4, 17, 16);
DCMotor motor_der(5, 30, 41);


Servo puerta;

//estados
#define motores(a,b) motor_izq.setSpeed(a);motor_der.setSpeed(b);

#define seguidor 1
#define caja 2

#define izq 0
#define der 1

//velociades

#define vel_adelante 60
#define giro_menor -55
#define giro_mayor 50

//pines

int PIN_S_DER = 11;
int PIN_S_IZQ = 13;
#define PIN_DER_EXT 9
#define PIN_IZQ_EXT 8
#define PIN_PULL_DER 40
#define PIN_PULL_IZQ 27

//servo

#define p_cerrada 60
#define p_abierta 140
#define PIN_SERVO 2

//variables de calibracion

int IZQ_N = 415;
int DER_N = 525;
#define IZQ_E_N 500
#define DER_E_N 300

int estado = seguidor;

//prioridad
boolean prioridad_izq = 0;
unsigned long t_gracia_interseccion;

//sensores

int s_izq;
int s_der;
int s_del;
int pull_izq;
int pull_der;
int s_izq_e;
int s_der_e;

int a = 0;
////////////////////////////////////////////////////////////
//325  ||  278 ||  356 ||  189
#define MIN_IZQ 278
#define MIN_DER 356
void setup()
{
  Serial.begin(9600);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  digitalWrite(25, 1);
  pinMode(PIN_PULL_DER, INPUT);
  pinMode(PIN_PULL_IZQ, INPUT);
  t_gracia_interseccion = millis();
  puerta.attach(2);
  puerta.write(p_cerrada);
  delay(500);
  puerta.detach();

  digitalWrite(22, prioridad_izq);
  digitalWrite(23, !prioridad_izq);
}

void loop()
{
  //Agregar switch.
  //Modo 0: tiempo corto de descarga.
  //Modo 1: Tiempo largo de descarga.
  
  if (estado == seguidor)
  {
    s_izq = analogRead(PIN_S_IZQ);
    s_der = analogRead(PIN_S_DER);
    s_izq_e = analogRead(PIN_IZQ_EXT);
    s_der_e = analogRead(PIN_DER_EXT);

    pull_izq = digitalRead(PIN_PULL_IZQ);
    pull_der = digitalRead(PIN_PULL_DER);

    //Si ve intersección cambia prioridad y guarda el tiempo
    if (s_izq < IZQ_N && s_der < DER_N && (millis() - t_gracia_interseccion) > 1000)
    {
      interseccion();
      t_gracia_interseccion = millis();
    }
    if (s_der < DER_N)
    {
      motores(giro_mayor + 50, -20 - 25 * (s_der < MIN_DER + 40));
    }
    else if (s_izq < IZQ_N)
    {
      motores(-20 - 25 * (s_izq < MIN_IZQ + 50), giro_mayor + 40);
      //motores(-50*(1-(s_izq-MIN_IZQ)/(IZQ_N-MIN_IZQ)), giro_mayor+10);
    }
    else
    {
      motores(vel_adelante + 10, vel_adelante);
    }


    if (!pull_der || !pull_izq)
    {
      estado = caja;
      motores(0, 0);
    }
  }
}

void interseccion()
{
  if (prioridad_izq)
  {
    s_der = analogRead(PIN_S_DER);

    motores(35, 35);
    while (s_der < DER_N + 2)
      s_der = analogRead(PIN_S_DER);

    motores(-55, 90);
    while (s_der < 550)
      s_der = analogRead(PIN_S_DER);
    while (s_der > 510)
      s_izq = analogRead(PIN_S_DER);
  }
  else
  {
    s_izq = analogRead(PIN_S_IZQ);

    motores(35, 35);
    while (s_izq > IZQ_N + 2)
      s_izq = analogRead(PIN_S_IZQ);

    motores(90, -55);
    while (s_izq < 490)
      s_izq = analogRead(PIN_S_IZQ);
    while (s_izq > 420)
      s_izq = analogRead(PIN_S_IZQ);

  }
  
  prioridad_izq = !prioridad_izq;
  digitalWrite(22, prioridad_izq);
  digitalWrite(23, !prioridad_izq);
  Serial.println("cambiando prioridad");
  
  motores(0, 0);
}
