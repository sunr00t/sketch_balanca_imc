/************************************************
* Projeto: Balança para Calculo de IMC (v1.0)   *
* Curso: Ciência da Computação / CCO6NA         *
* Disciplina: Microcontroladores                *
* Universidade dos Guararapes                   *
* Licença: MIT                                  *
*************************************************/

#include <Wire.h> //lib para dispositivos I2C
#include <PushButton.h>
#include <HX711.h> //lib da balança
#include <LiquidCrystal_I2C.h> //lib do display
//#include <VL53L0X.h> //lib do laser

/*Configuração dos Pinos*/ 
#define pin_sck 2 // SCK (HX711)
#define pin_dt 3 //  DT (HX711)
#define pin_btn 4 // Botao

/* Presets da Balança */
#define tempo_espera 500 //constante de espera
#define fator_calibracao -24830.0f //constante do fator de calibracao

/* INSTANCIANDO OS OBJETOS */
HX711 scale; //escala
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd (display 16(lines)x 2(cols)) porta de comunicação 0x27
//VL53L0X laser; //laser
PushButton botao(pin_btn);

/* DECLARAÇÃO DE VÁRIAVEIS */
float ALTURA;
float PESO;
float IMC;

/* HEX dos Chars Especiais */
uint8_t relogio[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t weight[9] = {0x0E, 0x0A, 0x04, 0x0E, 0x1F, 0x1F, 0x1F,0x1F};
uint8_t height[9] = {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x1F, 0x0E, 0x04};


/* Inicio das Configurações */
void setup() {
  Serial.begin(9600);
 
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  /* Construindo Chars Especiais */
  lcd.createChar(0, relogio);
  lcd.createChar(1, check);
  lcd.createChar(2, weight);
  lcd.createChar(3, height);

  /* Balanca Carregando (Display) */
  lcd.setCursor(1,0);
  lcd.print(" Balanca IMC ");
  lcd.setCursor(0,1);
  lcd.print("Carregando... ");
  lcd.write(0); //simbolo relogio
  
   /* Config da Balanca*/
  scale.begin(pin_dt, pin_sck); ////inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  scale.tare(); //zera a escala (balanca vazia)
  scale.set_scale(fator_calibracao); //ajusta a escala para o fator de calibracao
  
  /* Setup Finalizado (Display) */
  lcd.clear();
  lcd.print("Setup ");
  lcd.setCursor(0,1);
  lcd.print("   Finalizado ");
  lcd.write(1); //simbolo relogio
  delay(3000);
  lcd.clear();

  /* Monitor Serial do Laser 
   * - Falta checar conectividade do modulo no modulo i2c
   * - Verificar como passar o address 0x29 
  */ 
}

void loop() {

  botao.button_loop();
  //verifica se o modulo hx711 esta pronto para realizar leitura
  if (scale.is_ready()){ 
    //Imprimindo peso e altura no display
    lcd.setCursor(1,0);
    lcd.print("Peso: ");
    lcd.print(scale.get_units(), 1); //retorna a leitura da variavel escala com a unidade quilogramas
    lcd.print(" KG ");
    lcd.write(2); //simbolo de peso
    lcd.setCursor(1,1);
    lcd.print("Alt.: ");
    lcd.print(ALTURA);
    lcd.print(" M ");
    lcd.write(3);//simbolo de altura
    
    if(botao.pressed()){
    Serial.println("Peso Capturado");
    Serial.println(scale.get_units(), 1);    
  }  
  
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
