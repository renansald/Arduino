#ifndef CLISTA_ALUNOS_H
#define LISTA_ALUNOS_H

#include <Arduino.h>

typedef struct Alunos* apontador;

typedef struct Alunos{
  String id;
  String longitude;
  String latitude;
  String timer;
  apontador proximo;
} tAluno;

typedef struct lista{
  apontador frente, tras;
} tLista;

class ListaAlunos{
public:
  ListaAlunos();
  void Add(tAluno aluno);
  void Remove(tAluno *aluno);
  bool IsEmpty();
  void Imprime();

private:
  tLista lista;
};

#endif
