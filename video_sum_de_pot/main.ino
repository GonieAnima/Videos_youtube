//D4 D6 motor eyector

#include <Servo.h>

Servo servo1;
Servo servo2;

// Variables para los pines de los relés y botones
const int rele1Pin = 12;
const int rele2Pin = 3;
const int rele3Pin = 11;
const int rele4Pin = 2;

const int boton1Pin = 10;
const int boton2Pin = 9;
const int boton3Pin = 8;
//const int boton4Pin = 0;

const int motorPin2 = 6; // Pin para el motor DC 
const int motorPin1 = 4; // Pin para el motor DC

// Variables para los valores de los botones y relés
int valorBoton1 = 0;
int valorBoton2 = 0;
int valorBoton3 = 0;
int valorBoton4 = 0;

bool estadoRele1 = false;
bool estadoRele2 = false;
bool estadoRele3 = false;
bool estadoRele4 = false;

bool toggle = false;  // Variable para alternar entre los dos servos
bool toggle2 = false; // Variable para alternar entre sentidos de motor dc
bool toggle3 = false; // Tercer toggle
bool toggle4 = false; // Cuarto toggle
bool toggle5 = false; // Quinto toggle

void setup() {
    
    servo1.attach(7);  // Pin para el primer servo
    servo2.attach(5); // Pin para el segundo servo

    pinMode(rele1Pin, OUTPUT);
    pinMode(rele2Pin, OUTPUT);
    pinMode(rele3Pin, OUTPUT);
    pinMode(rele4Pin, OUTPUT);

    pinMode(boton1Pin, INPUT);
    pinMode(boton2Pin, INPUT);
    pinMode(boton3Pin, INPUT);
    //pinMode(boton4Pin, INPUT);
    
    // Inicializar los relés en estado LOW (apagados)
    digitalWrite(rele1Pin, HIGH);
    digitalWrite(rele2Pin, HIGH);
    digitalWrite(rele3Pin, HIGH);
    digitalWrite(rele4Pin, HIGH);

    Serial.begin(9600);
}

// Variables bool para cada caso
bool caso1 = false;
bool caso2 = false;
bool caso3 = false;
bool caso4 = false;
bool caso5 = false;
bool caso6 = false;
bool caso7 = false;
bool caso8 = false;

void loop() {
    // Junta los pines de los botones en un array
    const int botones[] = {boton1Pin, boton2Pin, boton3Pin};
    int valoresBotones[3];

    // Lee el estado de cada botón y guarda en el array
    for (int i = 0; i < 3; i++) {
        valoresBotones[i] = digitalRead(botones[i]);
    }

    // Crea un "código" binario a partir de los botones (ej: 101)
    int codigo = (valoresBotones[0] << 2) | (valoresBotones[1] << 1) | valoresBotones[2];

    // Reinicia todas las variables de caso a false
    caso1 = caso2 = caso3 = caso4 = caso5 = caso6 = caso7 = caso8 = false;

    // Maneja todas las combinaciones posibles (0-7)
    switch (codigo) {
        case 0b000:
            Serial.println("caso1 (boton 4 pulsado)");
            caso1 = true;
            break;
        case 0b001:
            Serial.println("caso2 (boton 3 pulsado)");
            caso2 = true;
            break;
        case 0b010:
            Serial.println("caso3 (eyectar pulsado)");
            caso3 = true;
            toggle = !toggle;
            break;
        case 0b011:
            Serial.println("caso4 (boton adelante pulsado)");
            caso4 = true;
            break;
        case 0b100:
            Serial.println("caso5 (boton Play pulsado)");
            caso5 = true;
            break;
        case 0b101:
            Serial.println("caso6 (boton 2 pulsado)");
            caso6 = true;
            break;
        case 0b110:
            Serial.println("caso7 (boton atras pulsado)");
            caso7 = true;
            break;
        case 0b111:
            Serial.println("caso8 (nada pulsado) ");
            caso8 = true;
            break;
    }
    Serial.print("Toggle: ");
    Serial.println(toggle ? "ON" : "OFF");
    Serial.print("Toggle2 en: ");
    Serial.println(toggle2 ? "ON" : "OFF");
    // Escenario según toggleVar
    static int pos1 = 90; // Posición inicial para servo1
    static int pos2 = 90; // Posición inicial para servo2
    // Variables estáticas para control de aceleración
    static unsigned long lastMoveTime = 0;
    static int moveStep = 2; // Paso inicial pequeño
    static unsigned long buttonHoldStart = 0;
    static bool lastAdelante = false;
    static bool lastAtras = false;

    // Detectar si se mantiene pulsado adelante o atrás
    bool adelante = caso4;
    bool atras = caso7;
    // Si se pulsa Play, activar o desactivar el toggle2
    if (caso5) {
        toggle2 = !toggle2;
        Serial.print("Toggle2 en: ");
        Serial.println(toggle2 ? "ON" : "OFF");
        
        // Control de dirección del motor según toggle2
        if (!toggle2) {
        digitalWrite(motorPin1, LOW);   // 
        digitalWrite(motorPin2, HIGH);  // 
        delay(2500); // Espera para cambiar dirección
        } 
        else {
        digitalWrite(motorPin1, HIGH);  // 
        digitalWrite(motorPin2, LOW);   // 
        delay(2500); // Espera para cambiar dirección
    }

    }
    if (caso1) {
        toggle3 = !toggle3;
        Serial.print("Toggle2 en: ");
        Serial.println(toggle2 ? "ON" : "OFF");
        
        // Control de dirección del motor según toggle2
        if (!toggle3) {
            digitalWrite(rele1Pin,HIGH);
            
        } 
        else {
            digitalWrite(rele1Pin,LOW);
        
    }
    delay(200);
    }
    if (caso2) {
        toggle4 = !toggle4;
        Serial.print("Toggle2 en: ");
        Serial.println(toggle2 ? "ON" : "OFF");
        
        // Control de dirección del motor según toggle2
        if (!toggle4) {
            digitalWrite(rele2Pin,HIGH);
        } 
        else {
            digitalWrite(rele2Pin,LOW);
    }
    delay(200);
    }
    if (caso6) {
        toggle5 = !toggle5;
        Serial.print("Toggle2 en: ");
        Serial.println(toggle2 ? "ON" : "OFF");
        
        // Control de dirección del motor según toggle2
        if (!toggle5) {
            digitalWrite(rele3Pin,HIGH);
        } 
        else {
            digitalWrite(rele3Pin,LOW);
    }
    delay(200);
    }
    
    //cuando  se pulse play los motores en 3 y 6 cambia dirección

    // Si se pulsa adelante o atrás, calcular aceleración
    if (adelante || atras) {
        // Si es la primera vez que se pulsa, reiniciar variables
        if ((adelante && !lastAdelante) || (atras && !lastAtras)) {
            buttonHoldStart = millis();
            moveStep = 10; // Primer movimiento más grande
            lastMoveTime = 0;
        } else {
            unsigned long holdTime = millis() - buttonHoldStart;
            // Aumentar el paso según el tiempo que se mantiene pulsado
            if (holdTime > 2000) moveStep = 20;
            else if (holdTime > 1000) moveStep = 10;
            else if (holdTime > 500) moveStep = 5;
            else moveStep = 2;
        }

        // Controlar la frecuencia de movimiento para suavizar
        if (millis() - lastMoveTime > 50) {
            if (toggle == false) {
                if (adelante) {
                    pos1 = min(pos1 + moveStep, 180);
                    servo1.write(pos1);
                    Serial.print("servo1 adelante: ");
                    Serial.println(pos1);
                }
                if (atras) {
                    pos1 = max(pos1 - moveStep, 0);
                    servo1.write(pos1);
                    Serial.print("servo1 atras: ");
                    Serial.println(pos1);
                }
            } else {
                if (adelante) {
                    pos2 = min(pos2 + moveStep, 180);
                    servo2.write(pos2);
                    Serial.print("servo2 adelante: ");
                    Serial.println(pos2);
                }
                if (atras) {
                    pos2 = max(pos2 - moveStep, 0);
                    servo2.write(pos2);
                    Serial.print("servo2 atras: ");
                    Serial.println(pos2);
                }
            }
            lastMoveTime = millis();
        }
    }
    lastAdelante = adelante;
    lastAtras = atras;
    delay(100); 
    pos1 = 90;
    pos2 = 90;
    servo1.write(pos1);
    servo2.write(pos2);
    }
