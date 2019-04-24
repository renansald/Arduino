#include "ListaAlunos.h"

ListaAlunos::ListaAlunos(){
  lista.frente = (apontador)malloc(sizeof(tAluno));
  lista.tras = lista.frente;
}

void ListaAlunos:: Addi(tAluno aluno){
  lista.tras->id = aluno.id;
  lista.tras->longitude = aluno.longitude;
  lista.tras->latitude = aluno.latitude;
  lista.tras->timer = aluno.timer;
  lista.tras->proximo = (apontador)malloc(sizeof(tAluno));
  lista.tras = lista.tras->proximo;
}

void ListaAlunos:: Remov(tAluno* aluno){
  if(IsEmpty()){
    return;
  }
  aluno->id = lista.frente->id;
  aluno->longitude = lista.frente->longitude;
  aluno->latitude = lista.frente->latitude;
  aluno->timer = lista.frente->timer;
  apontador x = lista.frente;
  lista.frente = lista.frente->proximo;
  free(x);
}

bool ListaAlunos:: IsEmpty(){
  return lista.frente == lista.tras;
}

void ListaAlunos::Imprime(){
  apontador aux = lista.frente;
  while(aux != lista.tras){
    Serial.println(aux->id);
    Serial.println(aux->latitude);
    Serial.println(aux->longitude);
    Serial.println(aux->timer);
    Serial.println();
    aux = aux->proximo;
  }
  free(aux);
}
