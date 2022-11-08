#include <Arduino.h>
#include <WS2812FX.h>
#include <OneButton.h>
#include <Fsm.h>
#include <TM1637TinyDisplay.h>


#include "ledControl.h"
#include "buttonsControl.h"
#include "settings.h"
#include "timeComando.h"

#define TRIGGER_PARADO 1
#define TRIGGER_INICIA_CONTAGEM 2
#define TRIGGER_DIMINUICOUNTDOWN 3
#define TRIGGER_PERDEU 4
#define TRIGGER_COUNTDOWN_ZERA 5
#define TRIGGER_PERDEU_ZERA 6
#define TRIGGER_CONFIG 7
#define TRIGGER_CONFIG_ADDTEMPO 8
#define TRIGGER_CONFIG_DECTEMPO 9

TimeComando timeComando;
TM1637TinyDisplay display(CLOCK_CLK, CLOCK_DIO);

int tempo;
bool notify10s = false;
bool notify3s = false;
long lastShow = 0;

OneButton button1(BUTTON1_PIN, true);
OneButton button2(BUTTON2_PIN, true);

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int _STATE = 0;

void click1();
void longpress1();
void click2();
void longpress2();
void duringLongPress1();
void duringLongPress2();
void on_zera_on_exit();
void on_parado_exit();
void on_countdown_exit();
void on_perdeu_exit();
void on_config_exit();
void on_zera_enter();
void on_parado_enter();
void on_countdown_enter();
void on_perdeu_enter();
void on_config_enter();
void on_state_countdown();

State state_parado(&on_parado_enter, NULL, &on_parado_exit);
State state_countdown(&on_countdown_enter, &on_state_countdown, &on_countdown_exit);
State state_perdeu(&on_perdeu_enter, NULL, &on_perdeu_exit);
State state_config(&on_config_enter, NULL, &on_config_exit);

Fsm fsm(&state_parado);

void setup() {
  display.flipDisplay(true);
  display.setBrightness(BRIGHT_HIGH);
  display.showString("WELCOME TO TIME COMANDO");
  delay(1000);
  display.clear();
  setupLeds(&ws2812fx);
  setupButtonsControl(&button1, &button2,
    click1, longpress1, click2, longpress2, duringLongPress1, duringLongPress2);
  Serial.begin(115200);

  Serial.println(timeComando.getTempoJogo());

  fsm.add_transition(&state_parado, &state_countdown, TRIGGER_INICIA_CONTAGEM, NULL);
  fsm.add_transition(&state_countdown, &state_parado, TRIGGER_PARADO, NULL);

  fsm.add_transition(&state_countdown, &state_perdeu, TRIGGER_PERDEU, NULL);

  fsm.add_transition(&state_parado, &state_config, TRIGGER_CONFIG, NULL);
  fsm.add_transition(&state_config, &state_config, TRIGGER_CONFIG_ADDTEMPO, NULL);
  fsm.add_transition(&state_config, &state_config, TRIGGER_CONFIG_DECTEMPO, NULL);
  fsm.add_transition(&state_config, &state_parado, TRIGGER_PARADO, NULL);
  fsm.add_transition(&state_perdeu, &state_parado, TRIGGER_PARADO, NULL);
}

void loop() {
  loopLeds(&ws2812fx);
  loopButtonsControl(&button1, &button2);
  fsm.run_machine();
}

void click1() {
  Serial.println("Click1");
  Serial.println(_STATE);
  switch (_STATE) {
    case TRIGGER_CONFIG:
      fsm.trigger(TRIGGER_CONFIG_ADDTEMPO);
      Serial.println("Adiciona Tempo");
      timeComando.addTempo();
      Serial.println(timeComando.getTempoJogo());
      display.showNumber(timeComando.getTempoJogo(), true);
    break;

    case TRIGGER_PARADO:
      fsm.trigger(TRIGGER_INICIA_CONTAGEM);
    break;

    case TRIGGER_COUNTDOWN_ZERA:
    case TRIGGER_PERDEU:
      fsm.trigger(TRIGGER_PARADO);
    break;

    default:
      break;
  }

}

void longpress1() {
  Serial.println("Click1 - longpress");

  switch (_STATE) {
  case TRIGGER_CONFIG:
    fsm.trigger(TRIGGER_PARADO);
    break;

  case TRIGGER_PARADO:
    fsm.trigger(TRIGGER_CONFIG);
    break;

  default:
    break;
  }
}

void click2() {
  Serial.println("Click2");
  Serial.println(_STATE);
  switch (_STATE) {
    case TRIGGER_CONFIG:
      fsm.trigger(TRIGGER_CONFIG_DECTEMPO);
      Serial.println("Remove Tempo");
      timeComando.decTempo();
      display.showNumber(timeComando.getTempoJogo(), true);
    break;
    case TRIGGER_PARADO:
      fsm.trigger(TRIGGER_INICIA_CONTAGEM);
    break;

    case TRIGGER_COUNTDOWN_ZERA:
    case TRIGGER_PERDEU:
      fsm.trigger(TRIGGER_PARADO);
    break;
    default:
      break;
  }
}

void longpress2() {
  Serial.println("Click2 - longpress");
  if (_STATE == TRIGGER_PARADO) {
    timeComando.toggleMute();
    Serial.printf("Estado do toogle mute %s\n", timeComando.isMutted() ? "MUTTED" : "NOT MUTTED");
  }
}

void duringLongPress1() {
  switch (_STATE) {
    case TRIGGER_CONFIG:
      fsm.trigger(TRIGGER_CONFIG_DECTEMPO);
      Serial.println("Adiciona Tempo");
      timeComando.addTempo();
      display.showNumber(timeComando.getTempoJogo(), true);
      delay(50);
    break;
    default:
      break;
  }
}

void duringLongPress2() {
  switch (_STATE) {
    case TRIGGER_CONFIG:
      fsm.trigger(TRIGGER_CONFIG_DECTEMPO);
      Serial.println("Remove Tempo");
      timeComando.decTempo();
      delay(50);
      display.showNumber(timeComando.getTempoJogo(), true);
    break;
    default:
      break;
  }
}

void on_zera_on_exit() {
  Serial.println("on_zera_on_exit");
}

void on_parado_exit() {
  Serial.println("on_parado_exit");
}

void on_countdown_exit() {
  Serial.println("on_countdown_exit");
}

void on_perdeu_exit() {
  Serial.println("on_perdeu_exit");
  delay(2000);
}

void on_config_exit() {
  Serial.println("on_config_exit");
}

void on_zera_enter() {
  Serial.println("on_zera_enter");
}

void on_parado_enter() {
  Serial.println("on_parado_enter");
  Serial.println("Zerar variaveis / mostrar zero no display");
  display.clear();
  ledStop(&ws2812fx);
  _STATE = TRIGGER_PARADO;
  notify10s = false;
  notify3s = false;

  tempo = timeComando.getTempoJogo();
  Serial.printf("Tempo de jogo %i\n", tempo);
  display.showNumber(tempo, true);
}

void on_countdown_enter() {
  Serial.println("on_countdown_enter");
  Serial.println("Iniciar contagem");
  ledRespira(&ws2812fx);
  notify10s = true;
  notify3s = true;

  timeComando.setMillisTrigger(millis() + timeComando.getTempoJogo() *1000);
  _STATE = TRIGGER_COUNTDOWN_ZERA;
}

void on_perdeu_enter() {
  Serial.println("on_perdeu_enter");
  ledPerdeu(&ws2812fx);
  loopLeds(&ws2812fx);
  _STATE = TRIGGER_PERDEU;
  display.showString("Perdeu!!!");
}

void on_config_enter() {
  Serial.println("on_config_enter");
  ledConfigura(&ws2812fx);
  _STATE = TRIGGER_CONFIG;
  display.showString("CONF");
}

void on_state_countdown() {
  long now = millis();

  if((now >= (timeComando.getMillisTrigger() - 10000)) && notify10s) {
    Serial.println("$$$ 10 secs");
    notify10s = false;
    ledStrobe(&ws2812fx);
  }

  if((now >= (timeComando.getMillisTrigger() - 3000)) && notify3s) {
    Serial.println("$$$ 3 secs");
    notify3s = false;
    ledMultiStrobe(&ws2812fx);
  }


  if(now >= timeComando.getMillisTrigger()) {
    Serial.println("#### Perdeu");
    fsm.trigger(TRIGGER_PERDEU);
  }

  if(millis() >= lastShow) {
    lastShow = millis() +1000;
    long tempoPassado = (timeComando.getMillisTrigger() - now)/1000;
    Serial.printf("Tempo Passado %ld\n", tempoPassado);
    display.showNumber(tempoPassado, true);
  }
}