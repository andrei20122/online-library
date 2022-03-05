#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#define EMPTY(trie) (trie == NULL)

typedef struct nod
{
    int marime; 
    char litera; 
    void* info; 

    struct nod* next[69]; 
}T_Nod, *Trie;

Trie init_Trie(int marime);
void eliberare_Trie(Trie *t, void (*Free)(void*));
int cautare_pozitie(char cauta);
void insert(Trie t, char *cuvant, void *info);
void* extract(Trie t, char *cuvant);
void stergere(Trie t, char *cuvant, void (*Free)(void*));
int Trie_goala(Trie t);
void cautare_prefix(Trie t, char *prefix, char **v, int *pos);
#endif // TRIE_H_INCLUDED
