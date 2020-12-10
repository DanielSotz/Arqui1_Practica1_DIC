#include <LedControl.h>
#include "mensaje_matriz.h"
#include <LiquidCrystal.h>

#include "LedControl.h"
//definicion de filas
#define ROW_1 A0
#define ROW_2 A1
#define ROW_3 A2
#define ROW_4 A3
#define ROW_5 A4
#define ROW_6 A5
#define ROW_7 A6
#define ROW_8 A7
//definicion de columnas
#define COL_1 A8
#define COL_2 A9
#define COL_3 A10
#define COL_4 A11
#define COL_5 A12
#define COL_6 A13
#define COL_7 A14
#define COL_8 13
#define btnDerecha 5  //derecha
#define btnIzquierda 4 //izquierda
// botones de la izquierda; estado de los botones de la izquierda
#define btnRotar 6    //rotar
// boton start; estado botón start
#define btnStart 3
// botones de control de mensaje
#define MsgIzq 9
#define MsgDer 8

const byte rows[] = {
  ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte col[] = {
  COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

LedControl lc = LedControl(12, 10, 11, 1); // PINES DRIVER

int letras[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int pot = 0;
int dir = -1;
int espera = 0, espera_ = 0, pause = 0;
int tiempo = 0;
int contador_start = 0;
bool juego = false;
int jugador = 0;

int rs = 23, en = 22, d4 = 16, d5 = 17, d6 = 18, d7 = 19;
LiquidCrystal lcd (rs, en, d4, d5, d6, d7);

byte heart[8] = {
B01010,
B11111,
B11111,
B01110,
B00100,
B00000,
B00000,
};

byte carita[8] = {
B11111,
B10101,
B11111,
B11111,
B10101,
B10001,
B11111,
};

void setup() {
  // INICIALIZACIÓN DEL DRIVER
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);

  // INICIA LA FORMA DE LOS PINES REQUERIDOS
  Serial.begin(9600);
  pinMode(btnDerecha, INPUT);
  pinMode(btnIzquierda, INPUT);
  pinMode(btnRotar, INPUT);
  pinMode(btnStart, INPUT);
  pinMode(MsgIzq, INPUT);
  pinMode(MsgDer, INPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  Serial.println("Prueba Arduino");

  lcd.createChar(1, heart);
  lcd.createChar(2, carita);
  lcd.begin(16,2);
  
}

void Dibujarletras() {
  byte nuevo[] = {frase[letras[0]], frase[letras[1]], frase[letras[2]], frase[letras[3]], frase[letras[4]], frase[letras[5]], frase[letras[6]], frase[letras[7]]};
  drawScreen(nuevo);
  DibujarconDriver();
  if ((tiempo % (pot + 3)) == 0) {
    for (int i = 0; i < 16; i++) {
      if (dir == 1) {
        if ((letras[i]) != 151) {
          letras[i]++;
        } else {
          letras[i] = 0;
        }
      } else {
        if ((letras[i]) != 0) {
          letras[i]--;
        } else {
          letras[i] = 151;
        }
      }
    }
  }
}

void DibujarconDriver() {
  lc.setColumn(0, 0, frase[letras[8]]);
  lc.setColumn(0, 1, frase[letras[9]]);
  lc.setColumn(0, 2, frase[letras[10]]);
  lc.setColumn(0, 3, frase[letras[11]]);
  lc.setColumn(0, 4, frase[letras[12]]);
  lc.setColumn(0, 5, frase[letras[13]]);
  lc.setColumn(0, 6, frase[letras[14]]);
  lc.setColumn(0, 7, frase[letras[15]]);
}

void  drawScreen(byte buffer2[]) {
  // Turn on each row in series
  for (byte i = 0; i < 8; i++)        // cuenta la fila siguiente
  {
    digitalWrite(rows[i], HIGH);    //inicializa todas las filas
    for (byte a = 0; a < 8; a++)    
    {     
      digitalWrite(col[a], !((buffer2[i] >> a) & 0x01)); //inicializa todas las columnas
    }
    delay(2);
    allOff();
    digitalWrite(rows[i], LOW);     
  }
}

void allOff() {
  for (byte a = 0; a < 8; a++)    // count next row
  {
    digitalWrite(col[a], 1);
  }
}

String reloj()
 {
 int n = millis() / 1000 ;  // Se convierte a segundos
 int segundos = n % 60 ;
 int minutos = n / 60 ;

String S = String(minutos) + ":" + String(segundos);
 return (S);
 }


void loop() {
  // put your main code here, to run repeatedly:
  
  pot = analogRead(A15) / 145;
  Dibujarletras();
    
  Serial.println(contador_start);
  while (digitalRead(btnStart) == HIGH) {
    contador_start++;
    if (contador_start >= 60) {
      contador_start = 0;
      juego = true;     
    }
    delay(50);
  }

  if (tiempo < 99) {
    tiempo++;
  } else {
    tiempo = 0;
  }

  if (digitalRead(MsgDer) == HIGH) {
    dir = -1;
  }
  if (digitalRead(MsgIzq) == HIGH) {
    dir = 1;
  }

  lcd.setCursor(3,1);
  lcd.write ("FIGURA-1");
  
  lcd.setCursor(11,1);
  lcd.write (byte(1));

  lcd.setCursor(12,1);
  lcd.write (byte(2));
  
  lcd.setCursor(0,0);
  String r = reloj ();
  lcd.print (r);  
}
