#pragma once

class TimeComando {
  private:
    long millisTrigger;
    int tempoJogo;
    bool muted;

  public:
    TimeComando();
    TimeComando(int tempoJogo, bool isMutted);
    int getTempoJogo();

    void addTempo();
    void decTempo();

    void toggleMute();
    bool isMutted();

    void setMillisTrigger(long tempo);
    long getMillisTrigger();
};