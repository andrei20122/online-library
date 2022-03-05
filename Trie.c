#include "Trie.h"

T_Nod* alloc_Nod(int marime, char litera, void* info)
{
    T_Nod* n = (T_Nod*)malloc(sizeof(T_Nod));
    n->marime = marime;
    n->litera = litera;
    n->info = info;
   int h=0;
   while(h<marime)
   {
        n->next[h] = NULL;
        h++;
    }
    
 return n;
}

Trie init_Trie(int marime)
{  
    return alloc_Nod(marime, '\0', NULL);
}

void eliberare_Trie(Trie *t, void (*Free)(void*))
{   int i;
   
   while(*t != NULL)
   {
    for(  i = 0; i < (*t)->marime; i++ )
        eliberare_Trie( &((*t)->next[i]), Free );

    if( (*t)->info != NULL )
        Free((*t)->info);
    
    free(*t);
    *t = NULL;
   }
    if( *t == NULL )
       {
           return;
       } 
}

int cautare_pozitie(char cauta)
{
    char alfabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";

    int y=0;

    while( y < 69)
    {
        if( alfabet[y] == cauta )
         return y;

     y++;
    }
    
    return 0;
}

void insert(Trie t, char *cuvant, void* info)
{
    T_Nod *n = t;
    int length = strlen(cuvant), i = 0;

    while( i < length  )
    {     
        int aux = cautare_pozitie(cuvant[i]);

        if(n -> next[aux] == NULL)

         n -> next[aux] = alloc_Nod(n -> marime, cuvant[i], NULL);

            n = n -> next[aux];

        i++;
    }

    
    n->info = info;
}

void* extract(Trie t, char *cuvant)
{
    T_Nod *n = t;
    int length = strlen(cuvant), i = 0;
    

    while( i < length  )
    {  
        int aux = cautare_pozitie(cuvant[i]);

        if(n -> next[aux] == NULL)

         n -> next[aux] = alloc_Nod(n -> marime, cuvant[i], NULL);

            n = n -> next[aux];

        i++;
    }

    if( i < length )
        return NULL;
    else
    {
        if( n->info != NULL )
            return n->info;
        else
            return NULL;
    }
}

void stergere(Trie t, char *cuvant, void (*Free)(void*))
{
    T_Nod *n = t;
    int length = strlen(cuvant), i ;
   
    for (i=0;i < length ;i++)
    {
        int j;
        char alfabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";
        int aux = -1; 
        for( j=0; j < 69; j++ )
            if( alfabet[j] == cuvant[i] )
                aux = j;

        if( n->next[aux] == NULL )
            break;
        else
            n = n->next[aux];

    }
    
     if( i == length )
    {
        Free(n->info);
        n->info = NULL;
    }
}

int Trie_goala(Trie t)
{
    if(t == NULL)
        return 1; 

    int ok = 1,i=0;

   while( i < t->marime)
   {
        ok = ok & Trie_goala(t->next[i]);
        i++;
   }
    if( t->info == NULL )
        return ok;
    else
        return 0; 
}

void cautare_prefix_Help(Trie t, char *cuvant, char **v, int *pos)
{   int i=0;
    if( t == NULL )
      {
           return;
      } 
     
    if( t->info != NULL )
    {
        strcpy(v[*pos], cuvant);
        (*pos)++;
    }

  
     while( i < t->marime)
       {
            if( t->next[i] != NULL )
        {
            strncat(cuvant, &(t->next[i])->litera, 1);
            cautare_prefix_Help(t->next[i], cuvant, v, pos);
            cuvant[strlen(cuvant) - 1] = '\0';
        }
        i++;
       }
    
   
}

void cautare_prefix(Trie t, char *prefix, char **v, int *pos)
{
    T_Nod *n = t;
    int i = 0, length = strlen(prefix);
    int j;
    while( i < length )
    {   
        char alfabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";
        int aux = -1; 
        for(j=0 ; j < 69; j++ )
            if( alfabet[j] == prefix[i] )
                aux = j;

        if( n->next[aux] == NULL )
            break;
        else
            n = n->next[aux];

        i++;
    }

    if( i != length )
        return;

    char cuvant[55]; cuvant[0] = '\0';
    cautare_prefix_Help(n, cuvant, v, pos);
}
