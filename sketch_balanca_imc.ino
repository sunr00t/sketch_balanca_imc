/************************************************
* Projeto: Balança para Calculo de IMC (v1.0)   *
* Curso: Ciência da Computação / CCO6NA         *
* Disciplina: Microcontroladores                *
* Universidade dos Guararapes                   *
* Licença: MIT                                  *
*************************************************/

#include <Wire.h> //lib para dispositivos I2C
#include <PushButton.h>
#include <HX711.h>             //lib da balança
#include <LiquidCrystal_I2C.h> //lib do display
#include <Ultrasonic.h> //lib do hc-sr04

/*Configuração dos Pinos*/
#define pin_sck 2 // SCK (HX711)
#define pin_dt 3  //  DT (HX711)
#define pin_btn 4 // Botao

/* Presets da Balança */
#define tempo_espera 100 //constante de espera
#define fator_calibracao -24830.0f //fator de calibracao (real)
//#define fator_calibracao -100.0f //fator de calibracao (ambiente de testes)

/* INSTANCIANDO OS OBJETOS */
HX711 scale;                        //escala
LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd (display 16(lines)x 2(cols)) porta de comunicação 0x27
PushButton botao(pin_btn);
Ultrasonic sonic(12,13);             //Configura os pinos sensor ultrassônico (Trigger,Echo)


/* DECLARAÇÃO DE VÁRIAVEIS */
int ALTURA;
float PESO;
float IMC;
int contador = 0;
int distancia;
/* HEX dos Chars Especiais */
uint8_t relogio[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t check[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t weight[9] = {0x0E, 0x0A, 0x04, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t height[9] = {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x1F, 0x0E, 0x04};

/* Inicio das Configurações */
void setup()
{
  //Serial.begin(9600);

  lcd.begin();
  lcd.backlight();
  /* Construindo Chars Especiais */
  lcd.createChar(0, relogio);
  lcd.createChar(1, check);
  lcd.createChar(2, weight);
  lcd.createChar(3, height);

  /* Balanca Carregando (Display) */
  lcd.setCursor(1, 0);
  lcd.print(" Balanca IMC ");
  lcd.setCursor(0, 1);
  lcd.print("Carregando... ");
  lcd.write(0); //simbolo relogio

  /* Config da Balanca*/
  scale.begin(pin_dt, pin_sck);      ////inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  scale.tare();                      //zera a escala (balanca vazia)
  scale.set_scale(fator_calibracao); //ajusta a escala para o fator de calibracao

  /* Setup Finalizado (Display) */
  lcd.clear();
  lcd.print("Setup ");
  lcd.setCursor(0, 1);
  lcd.print("   Finalizado ");
  lcd.write(1); //simbolo relogio
  delay(3000);
  lcd.clear();
}

void loop()
{
  botao.button_loop(); // escutando evento do botao
  if (scale.is_ready()) //verifica se o modulo hx711 esta pronto para realizar leitura
  {
    PESO = scale.get_units();
    distancia = (sonic.read(CM));
    //Imprimindo peso e altura no display
    lcd.setCursor(0, 0);
    lcd.print("Alt.: ");
    lcd.print(distancia);    //Exibe no display as medidas
    lcd.print("");
    lcd.print(" cm");
    lcd.setCursor(13, 0);
    lcd.write(3); //simbolo de altura
    lcd.setCursor(0, 1);   
    lcd.print("Peso: ");
    lcd.print(scale.get_units(), 1); //retorna a leitura da variavel escala com a unidade quilogramas
    lcd.print(" KG");
    lcd.setCursor(13, 1);
    lcd.write(2); //simbolo de peso
//    lcd.setCursor(0, 1);
//    lcd.print("Alt.: ");
//    lcd.print(distancia);
//    lcd.print(" M");

    if (botao.pressed())
    {
      contador += 1;
      if (contador > 1)
      {
        lcd.clear();
        IMC = (PESO / (ALTURA * ALTURA));
        lcd.setCursor(0, 0);
        lcd.print("Calculo do IMC: ");
        lcd.setCursor(5, 1);
        lcd.print(IMC);
        lcd.write(1); //simbolo relogio
        delay(6000);
        
        lcd.clear();
        lcd.print("CLASSIFICACAO:");
        lcd.setCursor(0, 1);
        if (IMC < 18.5)
        {
          lcd.print("Magreza");
        }
        if (IMC >= 18.5 && IMC <= 24.9)
        {
          lcd.print("Adequado");
        }
        if (IMC >= 25 && IMC <= 29.9)
        {
          lcd.print("Sobrepeso");
        }
        if (IMC >= 30 && IMC <= 39.9)
        {
          lcd.print("Obesidade");
        }
        if (IMC >= 40)
        {
          lcd.print("Obesidade Grave");
        }
        delay(10000);
        lcd.clear();
        lcd.print("Resetando...");
        delay(5000);
      }
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Calibrando..");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde...  ");
    lcd.write(2);
    lcd.clear();
  }
  delay(tempo_espera); //intervalo de espera para leitura
  lcd.clear();
}
