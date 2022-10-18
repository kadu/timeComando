#include <Arduino.h>
#include "timeComando.h"

TimeComando::TimeComando() {
  this->tempoJogo = 75;
};

TimeComando::TimeComando(int tempoJogo, bool isMutted) {
  this->tempoJogo = tempoJogo;
  this->muted = isMutted;
};

int TimeComando::getTempoJogo() {
  return this->tempoJogo;
};

void TimeComando::addTempo() {
  this->tempoJogo++;
}

void  TimeComando::decTempo() {
  this->tempoJogo--;
}

void TimeComando::toggleMute() {
  this->muted = !this->muted;
}

bool TimeComando::isMutted() {
  return this->muted;
}

void TimeComando::setMillisTrigger(long tempo) {
  this->millisTrigger = tempo;
}

long TimeComando::getMillisTrigger() {
  return this->millisTrigger;
}