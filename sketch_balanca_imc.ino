#include <Adafruit_VL53L0X.h>
#include <vl53l0x_api_core.h>
#include <vl53l0x_i2c_platform.h>
#include <vl53l0x_api.h>

#include <HX711.h> //lib da balança
#include <Wire.h> //lib para dispositivos I2C
#include <LiquidCrystal_I2C.h> //lib do display

/************************************************
* Projeto: Balança para Calculo de IMC (v1.0)   *
* Curso: Ciência da Computação / CCO6NA         *
* Disciplina: Microcontroladores                *
* Universidade dos Guararapes                   *
* Licença: MIT                                  *
*************************************************/

/* Presets da Balança */
HX711 escala; //declaracao do objeto escala na classe HX711 da biblioteca

//configuracao dos pinos para o modulo HX711
const int pin_sck = 2;
const int pin_dt = 3;
const int tempo_espera = 1000; //variavel de espera
float fator_calibracao = -24830; //pre-definicao da variavel de calibracao

/* Presets do Display */
LiquidCrystal_I2C lcd(0x27, 16, 2); // Usando display 16(lines)x 2(cols) e utilizando a porta de comunicação 0x27

/* HEX dos Chars Especiais */
uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};
uint8_t peso[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};


/* Presets do Laser (VL53L0X) */
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

/* Inicio das Configurações */
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  /* Construindo Chars Especiais */
  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);

  /* Config da Balanca*/
  lcd.setCursor(0,0);
  lcd.print("Iniciando..");
  lcd.setCursor(0,1);
  lcd.print("Aguarde... ");
  lcd.write(2);
  escala.begin(pin_dt, pin_sck); ////inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.tare(); //zera a escala
  escala.set_scale(fator_calibracao); //ajusta a escala para o fator de calibracao
  delay(2000);

  /* Monitor Serial do Laser 
   * - Falta checar conectividade do modulo no modulo i2c
   * - Verificar como passar o address 0x29 
  */ 
}

void loop() {
  //verifica se o modulo hx711 esta pronto para realizar leitura    
  if (escala.is_ready()){
    /* Imprimindo peso no display*/ 
    lcd.setCursor(0,0);
    lcd.print("Peso: ");
    lcd.print(escala.get_units(), 1); //retorna a leitura da variavel escala com a unidade quilogramas
    lcd.print(" KG ");
    lcd.write(5);
    lcd.setCursor(0,1);
    lcd.print("Alt.: 1.75");
    lcd.print(" M");
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Calibrando..");
    lcd.setCursor(0,1);
    lcd.print("Aguarde...  ");
    lcd.write(2);
  }
  delay(tempo_espera); //intervalo de espera para leitura
  lcd.clear();  

}
