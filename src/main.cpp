/**
 * @file pet_firmware_sensor_de_temperatura.ino
 * @brief sernsor de temperatura e umidade para frezzers laboratorias 
 * @warning esse codigo foi feito para o projeto do PET
 * @todo instalar biclioteca DHT sensor library 
 * @bug nenhum bug detectado
 * @copyright leticia beatriz bezerra da silva
*/


#include <Arduino.h>
#include "DHT.h"

/****************************************
 DEFINES
*****************************************/

#define sensor 11     /* sensor DHT locado na porta 11*/
#define tipo_sensor DHT22 /* tipo de sensor utilizado: DHT22*/
#define buzina 12     /* buzzer locado na porta 12*/
#define interruptor 13 /* interruptor deslizante logado na porta 13*/

/*****************************************
 GLOBAL VARIABLE
*****************************************/

int temperatura=0;  /* variável que armazena os valores referente a temperatura do senosr DHT*/
int umidade=0;      /*variável que armazena os valores referente a umidade relativa do ar do senosr DHT*/

int dezena=0;   /*variável que reprenta a dezena do visor de 7 segmentos */
int unidade=0; /*variável que reprenta a unidade do visor de 7 segmentos */

 /*variável que recebe o valor que não deve ser ultrapassado pelo sensor*/
int valormaior=32;
int valormenor=0;

/*matriz do display de 7 segmentos*/
 int saidas[]={2,3,4,5,6,7,8,9,10,}; 
              // a b c d e f g
int pinos[10][7]= {
    //  letras    a,b,c,d,e,f,g  
                 {1,1,1,1,1,1,0}, //0
                 {0,1,1,0,0,0,0}, //1
                 {1,1,0,1,1,0,1}, //2
                 {1,1,1,1,0,0,1}, //3
                 {0,1,1,0,0,1,1}, //4
                 {1,0,1,1,0,1,1}, //5
                 {1,0,1,1,1,1,1}, //6
                 {1,1,1,0,0,0,0}, //7
                 {1,1,1,1,1,1,1}, //8
                 {1,1,1,1,0,1,1}  //9                           
};

DHT dht(sensor, tipo_sensor); /* inicializa o sensor DHT */


/**
 * @fn setup()
 * @param não há parametros de entrada
 * @return não a paramêtros de saida
 * @brief realiza o setup do sistema
 * @remarks função padrão arduino
 *
*/
void setup() {

  Serial.begin(9600);
  pinMode(buzina, OUTPUT);
  pinMode(interruptor, INPUT);
  pinMode(sensor, INPUT);


  for (int i=0;i<11;i++){
  pinMode(saidas[i],OUTPUT);
}

  dht.begin();
}

/**
 * @fn loop()
 * @brief realiza o loop principal do sistema
 * @return retorna uma menssagem caso haja falha na leitura do sensor
*/
void loop() {

  temperatura = dht.readTemperature(); /*O método readTemperture() por padrão devolve a temperatura em celsius*/
  umidade = dht.readHumidity(); 


/*verifica de ouve falhas*/
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println(F("Falha na cominicação do sensor DHT!"));
    return;
  }

/**aciona as buzinas se a temperatura for maior ou menor do que o esperado.
 * se o alarme for acionado ele pode ser deligado pelo interruptor*/
  if(((temperatura>valormaior)|(temperatura<valormenor))&(digitalRead(interruptor)==LOW)){
  digitalWrite(buzina,HIGH);
    tone(buzina,70,1000);
  }else{
  digitalWrite(buzina, LOW);
  noTone(buzina);
}
  Serial.print(F("umidade: "));
  Serial.print(umidade);
  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura);
  Serial.println(F(" ºC "));
  delay(100);  

 dezena= temperatura/10; /*vai mostrar a dezena do valor da temperatura no visor*/
 Serial.print("dezena: ");
 Serial.println(dezena);

 unidade = temperatura-(dezena*10); /*vai mostrar a unidade do valor da temperatura no visor*/
Serial.print("unidade: ");
 Serial.println(unidade);
 
   /************DISPLAY************/
   /************DEZENA*************/

  digitalWrite(2,pinos[dezena][0]);
  digitalWrite(3,pinos[dezena][1]);
  digitalWrite(4,pinos[dezena][2]);
  digitalWrite(5,pinos[dezena][3]);
  digitalWrite(6,pinos[dezena][4]);
  digitalWrite(7,pinos[dezena][5]);
  digitalWrite(8,pinos[dezena][6]);
  digitalWrite(9,0);
  digitalWrite(10,1);
  delayMicroseconds(1);
  
  /***********UNIDADE*************/
  digitalWrite(10,1);
  digitalWrite(9,1);
  delayMicroseconds(1000);
  
  digitalWrite(2,pinos[unidade][0]);
  digitalWrite(3,pinos[unidade][1]);
  digitalWrite(4,pinos[unidade][2]);
  digitalWrite(5,pinos[unidade][3]);
  digitalWrite(6,pinos[unidade][4]);
  digitalWrite(7,pinos[unidade][5]);
  digitalWrite(8,pinos[unidade][6]);
  digitalWrite(10,0);
  digitalWrite(9,1);
  delayMicroseconds(1000);
  
  digitalWrite(10,1);
  digitalWrite(9,1);
  delayMicroseconds(1);
  
}
