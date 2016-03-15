#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Retorna uma string com o horário corrente
char* horarioCorrente() {
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime(&rawtime);

  char* output = (char*) malloc(sizeof(char)*100);
  sprintf(output, "%02d/%02d/%4d %2d:%02d:%02d",
    timeinfo->tm_mday,
    timeinfo->tm_mon + 1,
    timeinfo->tm_year + 1900,
    timeinfo->tm_hour,
    timeinfo->tm_min,
    timeinfo->tm_sec);
  return output;
}

// Imprime um evento NA SAÍDA PADRÃO, juntamente com o horário corrente
void registraEventoNoTerminal(char* evento) {
    printf("[%s]: %s\n", horarioCorrente(), evento);
}

// Imprime um evento EM ARQUIVO, juntamente com o horário corrente
void registraEventoEmArquivo(char* evento) {
    FILE * pFile = fopen("arquivo-de-log.txt","a");
    fprintf(pFile, "[%s]: %s\n", horarioCorrente(), evento);
    fclose(pFile);
}

// Finge que registra um evento, mas...
void registraEventoFake(char* evento) {
  // ...não faz nada!
}

// Calcula o dobro de um número e registra o fato de
// que esse cálculo foi feito
void calculaDobro(int x, void (*funcaoParaLogar)(char*)) {
    int resultado = x * 2;

    printf("Dobro de %d = %d\n", x, resultado);

    char evento[100];
    sprintf(evento, "Foi calculado o dobro do numero %d, resultando em %d.", x, resultado);
    funcaoParaLogar(evento);
}

// Rotina principal
#define NUMEROS_A_COMPUTAR 5

int main(int argc, char **argv)
{
    int i;
    int numeros[NUMEROS_A_COMPUTAR] = { 4, 2, 1, 0, 6 };

    for (i = 0; i < NUMEROS_A_COMPUTAR; i++) {
      // Alterne os comentários das linhas de código a seguir
      calculaDobro(numeros[i], registraEventoNoTerminal);
      // calculaDobro(numeros[i], registraEventoFake);
      // calculaDobro(numeros[i], registraEventoEmArquivo);
    }

    return 0;
}
