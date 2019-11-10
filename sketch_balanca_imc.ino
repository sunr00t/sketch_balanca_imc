

/************************************************
* Projeto: Balança para Calculo de IMC (v1.0)   *
* Curso: Ciência da Computação / CCO6NA         *
* Disciplina: Microcontroladores                *
* Universidade dos Guararapes                   *
* Licença: MIT                                  *
*************************************************/
#include <VL53L0X.h>
#include <HX711.h> //lib da balança
#include <Wire.h> //lib para dispositivos I2C
#include <LiquidCrystal_I2C.h> //lib do display

/* Presets da Balança */
HX711 escala; //declaracao do objeto escala na classe HX711 da biblioteca

//configuracao dos pinos para o modulo HX711
const int pin_sck = 2;
const int pin_dt = 3;
const int tempo_espera = 2000; //variavel de espera
float fator_calibracao = -24830; //pre-definicao da variavel de calibracao

/* Presets do Display */
LiquidCrystal_I2C lcd(0x27, 16, 2); // Usando display 16(lines)x 2(cols) e utilizando a porta de comunicação 0x27

/* HEX dos Chars Especiais */
uint8_t relogio[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t weight[9] = {0x0E, 0x0A, 0x04, 0x0E, 0x1F, 0x1F, 0x1F,0x1F};
uint8_t height[9] = {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x1F, 0x0E, 0x04};


/* Presets do Laser (VL53L0X) */

VL53L0X laser; // Instanciando objeto do laser

/* Inicio das Configurações */
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  /* Construindo Chars Especiais */
  lcd.createChar(0, relogio);
  lcd.createChar(1, check);
  lcd.createChar(2, weight);
  lcd.createChar(3, height);

  /* Config da Balanca*/
  lcd.setCursor(1,0);
  lcd.print(" Balanca IMC ");
  lcd.setCursor(0,1);
  lcd.print("Carregando... ");
  lcd.write(0); //simbolo relogio
  
  escala.begin(pin_dt, pin_sck); ////inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.tare(); //zera a escala
  escala.set_scale(fator_calibracao); //ajusta a escala para o fator de calibracao
  delay(3000);
  lcd.clear();

  /* Monitor Serial do Laser 
   * - Falta checar conectividade do modulo no modulo i2c
   * - Verificar como passar o address 0x29 
  */ 
}

void loop() {
    double ALTURA;
    double PESO;
    double IMC;

  
  //verifica se o modulo hx711 esta pronto para realizar leitura    
  if (escala.is_ready()){

    /* Imprimindo peso no display*/ 
    lcd.setCursor(1,0);
    lcd.print("Peso: ");
    lcd.print(escala.get_units(), 1); //retorna a leitura da variavel escala com a unidade quilogramas
    lcd.print(" KG ");
    lcd.write(2); //simbolo de peso
    lcd.setCursor(1,1);
    lcd.print("Alt.: 1.75");
    lcd.print(" M ");
    lcd.write(3);//simbolo de altura
    
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
