#include <LedControl.h>

// DIN, CLK, CS, quantidade de displays
LedControl lc = LedControl(11, 13, 10, 1);


// ===== CONFIGURAÇÕES DO JOGO =====

const int TOTAL_FASES = 5;

const int SEQUENCIA_INICIAL = 3;

const int TEMPO_SIMBOLO = 700;
const int TEMPO_PAUSA = 250;

const int ROTACAO = 1;

// Pontuação
const int PONTOS_POR_FASE = 100;
const int PONTUACAO_VITORIA = 500;

int pontos = 0;


// ===== BOTÕES =====

const int pinos[3] = {2, 3, 4};

const char* nomes[3] = {
  "Quadrado",
  "X",
  "Circulo"
};


// ===== DESENHOS =====

byte quadrado[8] = {
  B11111111, B10000001, B10000001, B10000001,
  B10000001, B10000001, B10000001, B11111111
};

byte xis[8] = {
  B10000001, B01000010, B00100100, B00011000,
  B00011000, B00100100, B01000010, B10000001
};

byte circulo[8] = {
  B00111100, B01000010, B10000001, B10000001,
  B10000001, B10000001, B01000010, B00111100
};

byte feliz[8] = {
  B00111100, B01000010, B10100101, B10000001,
  B10100101, B10011001, B01000010, B00111100
};

byte triste[8] = {
  B00111100, B01000010, B10100101, B10000001,
  B10011001, B10100101, B01000010, B00111100
};


byte numeros[5][8] = {

  {
    B00011000, B00111000, B00011000, B00011000,
    B00011000, B00011000, B00111100, B00000000
  },

  {
    B00111100, B01100110, B00000110, B00001100,
    B00110000, B01100000, B01111110, B00000000
  },

  {
    B00111100, B01100110, B00000110, B00011100,
    B00000110, B01100110, B00111100, B00000000
  },

  {
    B00001100, B00011100, B00101100, B01001100,
    B01111110, B00001100, B00001100, B00000000
  },

  {
    B01111110, B01100000, B01111100, B00000110,
    B00000110, B01100110, B00111100, B00000000
  }

};


// Agora a sequência pode ter até 7 imagens
byte* simbolos[3] = {
  quadrado,
  xis,
  circulo
};

int sequencia[TOTAL_FASES + SEQUENCIA_INICIAL - 1];


// ===== DISPLAY =====

void girar90(byte entrada[], byte saida[]) {

  for (int linha = 0; linha < 8; linha++) {

    saida[linha] = 0;

    for (int col = 0; col < 8; col++) {

      byte bit = (entrada[7 - col] >> (7 - linha)) & 1;

      saida[linha] |= bit << (7 - col);

    }
  }
}


void mostrarDesenho(byte desenho[]) {

  byte atual[8];
  byte temp[8];

  for (int i = 0; i < 8; i++) {
    atual[i] = desenho[i];
  }

  for (int r = 0; r < ROTACAO; r++) {

    girar90(atual, temp);

    for (int i = 0; i < 8; i++) {
      atual[i] = temp[i];
    }

  }

  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, atual[i]);
  }

}


// ===== COMUNICAÇÃO COM O SOFTWARE =====

void enviarResultado() {

  Serial.print("RESULT:");
  Serial.println(pontos);

}


// ===== LÓGICA DO JOGO =====

// Gera uma sequência nova para cada fase

void gerarSequencia(int quantidade) {

  Serial.print("Nova sequencia: ");

  for (int i = 0; i < quantidade; i++) {

    sequencia[i] = random(0, 3);

    Serial.print(nomes[sequencia[i]]);
    Serial.print(" ");

  }

  Serial.println();

}


// Mostra a sequência da fase atual

void mostrarSequencia(int quantidade) {

  for (int i = 0; i < quantidade; i++) {

    mostrarDesenho(simbolos[sequencia[i]]);

    delay(TEMPO_SIMBOLO);

    lc.clearDisplay(0);

    delay(TEMPO_PAUSA);

  }

}


// Lê um clique com debounce

int lerBotao() {

  while (true) {

    for (int b = 0; b < 3; b++) {

      if (digitalRead(pinos[b]) == LOW) {

        delay(20);

        if (digitalRead(pinos[b]) != LOW) {
          continue;
        }

        mostrarDesenho(simbolos[b]);

        unsigned long inicio = millis();

        while (millis() - inicio < 80) {

          if (digitalRead(pinos[b]) == LOW) {
            inicio = millis();
          }

        }

        lc.clearDisplay(0);

        Serial.print("Apertou: ");
        Serial.println(nomes[b]);

        return b;

      }

    }

  }

}


// ===== DERROTA =====

void perdeu() {

  Serial.println(">>> ERROU!");

  enviarResultado();

  mostrarDesenho(triste);

  delay(1500);

  lerBotao();

  delay(500);

}


// ===== VITÓRIA =====

void ganhou() {

  pontos = PONTUACAO_VITORIA;

  Serial.println(">>> VENCEU!");

  enviarResultado();

  for (int i = 0; i < 4; i++) {

    mostrarDesenho(feliz);

    delay(400);

    lc.clearDisplay(0);

    delay(200);

  }

  mostrarDesenho(feliz);

  delay(1000);

  lerBotao();

  delay(500);

}


// ===== PROGRAMA PRINCIPAL =====

void setup() {

  Serial.begin(9600);

  for (int b = 0; b < 3; b++) {
    pinMode(pinos[b], INPUT_PULLUP);
  }

  lc.shutdown(0, false);

  lc.setIntensity(0, 8);

  lc.clearDisplay(0);

  randomSeed(analogRead(A0));

}


// ===== LOOP PRINCIPAL =====

void loop() {

  pontos = 0;

  Serial.println(">>> NOVA PARTIDA");

  for (int fase = 1; fase <= TOTAL_FASES; fase++) {

    // Cada fase adiciona uma imagem
    int quantidade = SEQUENCIA_INICIAL + fase - 1;

    Serial.print("--- FASE ");
    Serial.println(fase);

    Serial.print("Quantidade de imagens: ");
    Serial.println(quantidade);


    // Gera uma sequência aleatória nova
    gerarSequencia(quantidade);


    // Mostra o número da fase

    mostrarDesenho(numeros[fase - 1]);

    delay(1000);

    lc.clearDisplay(0);

    delay(500);


    // Apresenta a sequência

    mostrarSequencia(quantidade);


    // Verifica as respostas

    for (int i = 0; i < quantidade; i++) {

      int resposta = lerBotao();

      if (resposta != sequencia[i]) {

        Serial.print("Esperado: ");
        Serial.println(nomes[sequencia[i]]);

        perdeu();

        return;

      }

    }


    // Fase concluída

    pontos += PONTOS_POR_FASE;

    Serial.print("Pontuacao atual: ");
    Serial.println(pontos);

    delay(600);

  }


  // Vitória

  ganhou();

}
