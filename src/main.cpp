#include <Arduino.h>
#include <ESP32Servo.h>

// PINES
#define RED_LED     12
#define GREEN_LED   13
#define BLUE_LED    14
#define SERVO1_PIN  21
#define SERVO2_PIN  22

// TIEMPOS
#define TICK_DELAY     1   
#define TICK_ROJO      1000
#define TICK_VERDE     5000
#define TICK_BLUE      10000
#define TICK_SERVO     15
#define TICK_LOOP      2000

// Servos
Servo servo1, servo2;

// Contadores 
uint32_t RED_COUNT    = TICK_ROJO;
uint32_t GREEN_COUNT  = TICK_VERDE;
uint32_t BLUE_COUNT   = TICK_BLUE;
uint32_t SERVO_COUNT  = TICK_SERVO;
uint32_t loop_count = 0;

int loops = 0;

// Estados de los leds
bool RED_STATE = false, GREEN_STATE = false, BLUE_STATE = false;

// Variables de posición y direccion de los servos
// Existen para guardar al momento de la ejecucion su posicion y no reescribirla
int pos1 = 0, dir1 = 1;
int pos2 = 0, dir2 = 1;

// Control y escritura de leds
void control_led(uint32_t led, bool &state, const char *name)
{
  state = !state;
  digitalWrite(led,state);
  Serial.print("LED ");
  Serial.print(name);
  Serial.println(state ? ": ON" : ": OFF");
  
}

// Control y escritura de servos
void control_servo(Servo Serv, int &pos, int &dir, const char *name)
{
  if (pos == 0)
  {
    Serial.print("Servo #");
    Serial.print(name);
    Serial.println(": 0 -> 180");
    dir = 1;
  }
  if (pos == 180)
  {
    Serial.print("Servo #");
    Serial.print(name);
    Serial.println(": 180 -> 0");
    dir = -1;
  }
  Serv.write(pos);

  pos += dir;
}

// Setup de pines de leds y servos
void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);

  servo1.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, 500, 2400);
  servo2.setPeriodHertz(50);
  servo2.attach(SERVO2_PIN, 500, 2400);

  delay(1000);
}

// Loop principal: Para realizar un funcionamiento simultáneo todos los procesos tienen su propio "COUNT" y "TICK"
// Esto sirve para realizar una cuenta cada ciclo de loop en base al delay y "TICK_DELAY" donde entra a la función
// en su tiempo especificado sin bloquear el resto de funciones
void loop()
{
  loops++;
  if (RED_COUNT >= TICK_ROJO)
  {
    control_led(RED_LED, RED_STATE, "ROJO");
    RED_COUNT = 0;
  }

  if (GREEN_COUNT >= TICK_VERDE)
  {
    control_led(GREEN_LED, GREEN_STATE, "VERDE");
    GREEN_COUNT = 0;
  }

  if (BLUE_COUNT >= TICK_BLUE)
  {
    control_led(BLUE_LED, BLUE_STATE, "BLUE");
    BLUE_COUNT = 0;
  }

  if (SERVO_COUNT >= TICK_SERVO)
  {
    control_servo(servo1, pos1, dir1, "1");
    control_servo(servo2, pos2, dir2, "2");
    SERVO_COUNT = 0;
  }
  if (loop_count >= TICK_LOOP)
  {
    Serial.print("Vueltas en 2 segundos:");
    Serial.println(loops);
    loops = 0;
    loop_count = 0;
  }
  delay(TICK_DELAY);
  RED_COUNT    += TICK_DELAY;
  GREEN_COUNT  += TICK_DELAY;
  BLUE_COUNT   += TICK_DELAY;
  SERVO_COUNT  += TICK_DELAY;
}

