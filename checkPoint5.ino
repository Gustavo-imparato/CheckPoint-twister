//Check Point 5 - twister 
//Cada level gera uma sequencia numerica diferente, o jogador deve decora-la e pressionar os botoes correspondestes ou digitar os numeros dos leds, isso depende do modo escolhido
// Caso o jogador passe os 4 leveis convencionais ele entrara no mata-mata onde a cada acerto mais uma piscada sera adicionada infinitamente
// a dificuldade do jogo pode ser variada modificando os parametros da function geraSequencia(x, y) onde x é o tempo e y o numero de repeticoes totais dos leds 
// no ambito de testes também adicionei a linha 302, ela imprime o valor da sequencia numerica facilitando testes, no momento ela esta comentada
// Variáveis Globais
String sequenciaNumerica = "";
String sequenciaJogador = "";

const int LED_PINS[] = {8, 9, 10, 11}; // Array de LEDs (aumentado para 8 LEDs)
const int BUTTON_PINS[] = {2, 3, 4, 5}; // Array de Botões
const long SERIAL_TIMEOUT = 10000; // Timeout de 10 segundos para leitura do Serial
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const int BUZZER_PIN = 6; //pino do buzzer

int sequencia;
int ledInfinito = 8;

// Definindo o enum para os estados do jogo
enum GameState {
  START_GAME,
  LEVEL_1,
  LEVEL_2,
  LEVEL_3,
  LEVEL_4,
  LEVEL_INFINITO,
  GAME_OVER,
  PLAY_AGAIN
};
GameState stateGame = START_GAME;

bool jogarComBotoes = false; // Variável para escolher o modo de jogo

void setup() {
  // Iniciar comunicacao serial
  Serial.begin(9600);

  // Definir LEDs como OUTPUTs
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Definir Botões como INPUTs
  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]); i++) { // Corrigido para BUTTON_PINS
    pinMode(BUTTON_PINS[i], INPUT_PULLUP); // INPUT_PULLUP para garantir que o botão fique em HIGH quando não pressionado
  }

  pinMode(BUZZER_PIN, OUTPUT);//define buzzer como output

  // Definir a seed do gerador de números aleatórios
  randomSeed(analogRead(0));
}



void loop() {
  switch (stateGame) {
    case START_GAME:
      Serial.println("* INICIO *");
      Serial.println("Pressione 'B' para jogar com botoes ou 'C' para jogar pelo console.");
      
      // Esperar que o jogador escolha o modo de jogo
      while (true) {
        if (Serial.available() > 0) {
          char opcao = Serial.read();
          if (opcao == 'B' || opcao == 'b') {
            jogarComBotoes = true;
            break;
          } else if (opcao == 'C' || opcao == 'c') {
            jogarComBotoes = false;
            break;
          }
        }
      }

      Serial.println("Jogo comecando...");
      piscaled(1000, 3);
      stateGame = LEVEL_1;
      break;

    case LEVEL_1:
      Serial.println("* Nivel 1 *");
      geraSequencia(1000, 3); // Gera sequência com 3 LEDs e tempo de 1000ms


      if (jogarComBotoes) {
        recebeInputDoJogador(3);
      } else {
        recebeInputDoJogadorConsole(3);
      }

      if (sequenciaJogador.equals(sequenciaNumerica)) {
        stateGame = LEVEL_2;
        Serial.println("Parabens! proximo nivel.");
        tocarSomAcerto(); // Toca som de acerto
        piscaled(1000, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!");
        tocarSomErro(); // Toca som de erro
        piscaled(300, 5);     
      }
      break;

    case LEVEL_2:
      Serial.println("* Nivel 2 *");
      geraSequencia(1000, 4); // Gera sequência com 4 LEDs e tempo de 800ms


      if (jogarComBotoes) {
        recebeInputDoJogador(4);
      } else {
        recebeInputDoJogadorConsole(4);
      }

      if (sequenciaJogador.equals(sequenciaNumerica)) {
        stateGame = LEVEL_3;
        Serial.println("Parabens! proximo nivel.");
        tocarSomAcerto(); // Toca som de acerto
        piscaled(1000, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!"); 
        tocarSomErro(); // Toca som de erro
        piscaled(300, 5);     
      }
      break;

    case LEVEL_3:
      Serial.println("* Nivel 3 *");
      geraSequencia(800, 6); // Gera sequência com 6 LEDs

      if (jogarComBotoes) {
        recebeInputDoJogador(6); // O jogador deve inserir 6 entradas
      } else {
        recebeInputDoJogadorConsole(6); // O jogador deve inserir 6 entradas
      }

      if (sequenciaJogador.equals(sequenciaNumerica)) {
        stateGame = LEVEL_4;
        Serial.println("Parabens! proximo nivel.");
        tocarSomAcerto(); // Toca som de acerto
        piscaled(1000, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!"); 
        tocarSomErro(); // Toca som de erro
        piscaled(300, 5);     
      }
      break;

    case LEVEL_4:
      Serial.println("* Nivel 4 *");
      geraSequencia(600, 8); // Gera sequência com 8 LEDs e tempo de 400ms

      if (jogarComBotoes) {
        recebeInputDoJogador(8);
      } else {
        recebeInputDoJogadorConsole(8);
      }

      if (sequenciaJogador.equals(sequenciaNumerica)) {
        stateGame = LEVEL_INFINITO;
        Serial.println("Parabens! Voce venceu!");
        tocarSomAcerto(); // Toca som de acerto
        piscaled(1000, 3);
      } else {
        stateGame = GAME_OVER;
        Serial.println("Errooooo!!!"); 
        tocarSomErro(); // Toca som de erro
        piscaled(300, 5);     
      }
      break;
    
    case LEVEL_INFINITO:
      Serial.println("*MATA MATA*");

      if (ledInfinito == 8) {
        ledInfinito = 8;
      }

      // Gera a sequência com ledInfinito LEDs
      geraSequencia(600, ledInfinito); 

      if (jogarComBotoes) {
          recebeInputDoJogador(ledInfinito); 
      } else {
          recebeInputDoJogadorConsole(ledInfinito); 
      }

      if (sequenciaJogador.equals(sequenciaNumerica)) {
          ledInfinito++; // Aumenta a dificuldade
          Serial.println("Parabéns! Continue jogando.");
          tocarSomAcerto(); 
          piscaled(1000, 3);
          Serial.println("Level: " + String(ledInfinito));
      } else {
          stateGame = GAME_OVER;
          Serial.println("Errooooo!!!"); 
          tocarSomErro(); 
          piscaled(300, 5);     
      }
      break;

    case GAME_OVER:
      Serial.println("Game Over");
      piscaled(100, 5);
      stateGame = PLAY_AGAIN;
      break;

    case PLAY_AGAIN:
      Serial.println("Pressione qualquer botao para jogar novamente.");
      
      // Esperar que qualquer botão seja pressionado para recomeçar o jogo
      while (verificaBotaoPressionado() == -1) {
        // Aguarda uma ação
      }

      stateGame = LEVEL_1;
      Serial.println("Jogo recomeçando...");
      piscaled(1000, 3);
      break;
  }
}


// Função para verificar se algum botão foi pressionado
int verificaBotaoPressionado() {
  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]); i++) { // Corrigido para BUTTON_PINS
    if (digitalRead(BUTTON_PINS[i]) == LOW) {
      delay(300); // Debouncing simples
      return i; // Retorna o índice do botão pressionado
    }
  }
  return -1; // Nenhum botão foi pressionado
}

// Função para receber a sequência de botões pressionados pelo jogador
void recebeInputDoJogador(int tamanho) {
  sequenciaJogador = ""; // Limpar a sequência do jogador

  while (sequenciaJogador.length() < tamanho) {
    int botao = verificaBotaoPressionado();
    if (botao != -1) {
      sequenciaJogador += String(botao + 1); // Adiciona o número correspondente ao LED
      Serial.println("Botao pressionado: " + String(botao + 1));
      tocaSom(botao);
    }
  }

  Serial.println("Sequencia do jogador: " + sequenciaJogador);
}

// Função para receber a sequência de números do jogador pelo console
void recebeInputDoJogadorConsole(int tamanho) {
  sequenciaJogador = ""; // Limpar a sequência do jogador

  while (sequenciaJogador.length() < tamanho) {
    if (Serial.available() > 0) {
      char entrada = Serial.read();
      if (entrada >= '1' && entrada <= '8') { // Verifica se a entrada está entre '1' e '8'
        sequenciaJogador += entrada; // Adiciona a entrada à sequência do jogador
        Serial.println("Numero pressionado: " + String(entrada));
      }
    }
  }

  Serial.println("Sequencia do jogador: " + sequenciaJogador);
}

void piscaled(int tempo, int vezes) {
  for (int i = 0; i < vezes; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      digitalWrite(LED_PINS[j], HIGH);
    }
    delay(tempo);
    for (int j = 0; j < NUM_LEDS; j++) {
      digitalWrite(LED_PINS[j], LOW);
    }
    delay(tempo);
  }
}

void geraSequencia(int tempo, int numLeds) {
    sequenciaNumerica = ""; // Limpa a sequência anterior

    // Pisca os LEDs na sequência gerada
    for (int j = 0; j < numLeds; j++) { // Aqui estamos usando o numLeds passado
        // Gera um número aleatório entre 0 e NUM_LEDS - 1
        int ledIndex = random(0, NUM_LEDS); // Gera um índice entre 0 e o número total de LEDs disponíveis
        digitalWrite(LED_PINS[ledIndex], HIGH);
        
        // Toca o som correspondente ao LED
        tocaSom(ledIndex);

        delay(tempo);
        digitalWrite(LED_PINS[ledIndex], LOW); // Apaga o LED após o atraso
        
        // Adiciona o número à sequência (de 1 a NUM_LEDS)
        sequenciaNumerica += String(ledIndex + 1); // +1 para mostrar LEDs de 1 a NUM_LEDS
    }

    // Essa linha deve ser retirada fora do ambiente de teste
    //Serial.println("Sequencia gerada: " + sequenciaNumerica);
}




// Função para tocar o som do buzzer de acordo com o LED
void tocaSom(int ledIndex) {
    // Defina os tons para cada LED (exemplo de frequências)
    int frequencias[] = {261, 294, 329, 349}; // Notas: Dó, Ré, Mi, Fá

    // Toca a frequência correspondente ao LED
    tone(BUZZER_PIN, frequencias[ledIndex], 300); // Toca por 300 ms
    delay(300); // Aguarda o tempo que o som toca
    noTone(BUZZER_PIN); // Para o som
}


void tocarSomLED(int ledIndex) {
  int frequencias[] = {262, 330, 392, 494}; // 4 frequências distintas (C4, E4, G4, B4)
  
  // Verifica se o ledIndex está dentro do intervalo válido (0 a 3)
  if (ledIndex >= 0 && ledIndex < 4) {
    tone(BUZZER_PIN, frequencias[ledIndex]); // Toca a frequência correspondente ao LED
    delay(200); // Duração do som
    noTone(BUZZER_PIN); // Para o som
  } else {
    Serial.println("Erro: Índice de LED fora do intervalo.");
  }
}


// Função para tocar o som de acerto
void tocarSomAcerto() {
   delay(500);// delay para não interferir com outro
  tone(BUZZER_PIN, 1000); // Frequência de acerto
  delay(500); // Duração do som
  noTone(BUZZER_PIN); // Para o som
}

// Função para tocar o som de erro
void tocarSomErro() {
  tone(BUZZER_PIN, 400); // Frequência de erro
  delay(1000); // Duração do som
  noTone(BUZZER_PIN);
}



