/*
 * 
   AUTOR: MAYRON REIS LAVERDA RIBEIRO
   ESTUDANTE ENGENHARIA DE COMPUTAÇÃO IFMG - CAMPUS BAMBUÍ

   TRABALHO DE CONCLUSÃO DE CURSO - PARTE 3 DE XX
   ==================================================================================================
   LEITOR CARTÃO - CIRCUITO
   CS 4
   MOSI 51
   MISO 50
   SCK 52
   ==================================================================================================
   LIGAÇÃO SENSORES
   RODAS DIANTEIRAS PINOS 2 E 3
   RODAS TRASEIRAS PINOS 18 E 19
   ==================================================================================================
   LEGENDA
   VA = Velocidade Angular
   DT = Delta Tempo
   VL = Velocidade Linear
   T = tempo
   TA = Tempo Anterior
*/


//============================================BIBLIOTECAS============================================
#include <SPI.h>          // Biblioteca de comunicação SPI Nativa
#include <SD.h>           // Biblioteca de comunicação com cartão SD Nativa
//============================================VARIÁVEIS============================================
const byte interruptPin2 = 2;
const byte interruptPin3 = 3;
const byte interruptPin18 = 18;
const byte interruptPin19 = 19;

volatile byte state = LOW;

const float pi = 3.141592;
const int chipSelect = 4;

const float raio_traseiras = 1.48;
const float raio_dianteiras = 1.12;

float VA_dianteira_esquerda, VA_dianteira_direita, VA_traseira_esquerda, VA_traseira_direita;
float DT_dianteira_esquerda, DT_dianteira_direita, DT_traseira_esquerda, DT_traseira_direita;
float VL_dianteira_esquerda, VL_dianteira_direita, VL_traseira_esquerda, VL_traseira_direita;
//float T_dianteira_esquerda, T_dianteira_direita, T_traseira_esquerda, T_traseira_direita;
float TA_dianteira_esquerda = 0, TA_dianteira_direita = 0, TA_traseira_esquerda = 0, TA_traseira_direita = 0;
float tempo;

String gravacao = "";

long controle_tempo = 0;
long intervalo_gravacao = 1000;

bool cartaoOk = true;

File dataFile;// Objeto responsável por escrever/Ler do cartão SD

//===============================================//===============================================

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);
  pinMode(interruptPin18, INPUT_PULLUP);
  pinMode(interruptPin19, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin2), Velocidade_Dianteira_Esquerda, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), Velocidade_Dianteira_Direita, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin18), Velocidade_Traseira_Esquerda, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin19), Velocidade_Traseira_Direita, RISING);

  //VERIFICA CARTÃO
  if (!SD.begin(chipSelect)) {
    Serial.println("Erro na leitura do arquivo não existe um cartão SD ou o módulo está conectado corretamente ?");
    cartaoOk = false;
    return;
  }
}

void loop() {
  tempo = millis();//INICIA O MILLIS
  gravacao = String(VL_dianteira_esquerda) + ";" + String(VL_dianteira_direita) + ";" + String(VL_traseira_esquerda) + ";" + String(VL_traseira_direita);
  Gravacao();
}

void Velocidade_Dianteira_Esquerda() {
  state = !state;

  if (TA_dianteira_esquerda != 0) {
    DT_dianteira_esquerda = tempo - TA_dianteira_esquerda;
    TA_dianteira_esquerda = tempo;
    VA_dianteira_esquerda = pi / (4 * (DT_dianteira_esquerda / 1000)); //∆θ/∆T --> 2π/∆T
    VL_dianteira_esquerda = VA_dianteira_esquerda * raio_dianteiras * 3.6;
    //gravacao = String(velLinear);
  } else {
    TA_dianteira_esquerda = tempo;
  }
}

void Velocidade_Dianteira_Direita() {
  state = !state;

  if (TA_dianteira_direita != 0) {
    DT_dianteira_direita = tempo - TA_dianteira_direita;
    TA_dianteira_direita = tempo;
    VA_dianteira_direita = pi / (4 * (DT_dianteira_direita / 1000)); //∆θ/∆T --> 2π/∆T
    VL_dianteira_direita = VA_dianteira_direita * raio_dianteiras * 3.6;
    //gravacao = String(velLinear);
  } else {
    TA_dianteira_direita = tempo;
  }
}

void Velocidade_Traseira_Direita() {
  state = !state;

  if (TA_traseira_direita != 0) {
    DT_traseira_direita = tempo - TA_traseira_direita;
    TA_traseira_direita = tempo;
    VA_traseira_direita = pi / (4 * (DT_traseira_direita / 1000)); //∆θ/∆T --> 2π/∆T
    VL_traseira_direita = VA_traseira_direita * raio_traseiras * 3.6;
    //gravacao = String(velLinear);
  } else {
    TA_traseira_direita = tempo;
  }
}

void Velocidade_Traseira_Esquerda() {
  state = !state;

  if (TA_traseira_esquerda != 0) {
    DT_traseira_esquerda = tempo - TA_traseira_esquerda;
    TA_traseira_esquerda = tempo;
    VA_traseira_esquerda = pi / (4 * (DT_traseira_esquerda / 1000)); //∆θ/∆T --> 2π/∆T
    VL_traseira_esquerda = VA_traseira_esquerda * raio_traseiras * 3.6;
    //gravacao = String(velLinear);
  } else {
    TA_traseira_esquerda = tempo;
  }
}

void Gravacao() {
  if (tempo - controle_tempo > intervalo_gravacao) {
    controle_tempo = tempo;    // Salva o tempo atual

    if (cartaoOk) {
      dataFile = SD.open("teste5.csv", FILE_WRITE);
    }

    if (dataFile) {
      Serial.println(gravacao);
      dataFile.println(gravacao);
      dataFile.close();
    }
  }
}

