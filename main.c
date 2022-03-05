#include <stdio.h>
#include <stdlib.h>
#include "Trie.h"

typedef struct _carte
{
    char *title, *author;
    int rating, pagini;
}Carte;

void free_Carte(void *a)
{
    Carte *carte = (Carte*)a;
    free(carte->title);
    free(carte->author);
    free(carte);
}

void free_Carte_Not(void *a)
{
    return;
}

void free_Author(void *a)
{
    Trie t = (Trie)a;
    eliberare_Trie(&t, free_Carte_Not);
}

Carte* creare_Carte()
{
    Carte *carte = (Carte*)malloc(sizeof(Carte));
    carte->author = (char*)calloc(55, sizeof(char));
    carte->title = (char*)calloc(55, sizeof(char));
    return carte;
}

void list_author(Trie authors, char *author, FILE *out)
{   int i;
    Trie carti_autor = (Trie)extract(authors, author);
    if( carti_autor == NULL )
        fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", author);
    else
    {   
        char **v;
        v = (char**)malloc(100*sizeof(char*));
        for(  i = 0; i < 100; i++)
            v[i] = (char*)malloc(55*sizeof(char));
        int pos = 0;

        cautare_prefix(carti_autor, "", v, &pos);
        for(  i = 0; i < pos; i++ )
            fprintf(out, "%s\n", v[i]);

        for(  i = 0; i < 100; i++)
            free(v[i]);
        free(v);
    }
}

int main(int argc, char **argv)
{
    Trie carti = init_Trie(69);
    Trie authors = init_Trie(69);
    FILE *in = fopen(argv[1], "rt");
    FILE *out = fopen(argv[2], "wt");

    char command[20];
    while( fscanf(in, "%s ", command) == 1 )
    {
        if( strcmp(command, "add_book") == 0 )
        {
            char info[400]; fgets(info, 400, in); info[strlen(info) - 1] = '\0';
            Carte* carte = creare_Carte();
            char* p;

            p = strtok(info, ":");
            strcpy(carte->title, p);
            p = strtok(NULL, ":");
            strcpy(carte->author, p);
            p = strtok(NULL, ":");
            carte->rating = atoi(p);
            p = strtok(NULL, ":");
            carte->pagini = atoi(p);

            insert(carti, carte->title, (void*)carte);

            void* autor = extract(authors, carte->author);
            if( autor == NULL )
            {
                Trie t = init_Trie(69);
                insert(t, carte->title, (void*)carte);
                insert(authors, carte->author, (void*)t);
            }
            else
                insert( (Trie)autor, carte->title, (void*)carte );
        }
        else if( strcmp(command, "search_book") == 0 )
        {
            char title[55]; fgets(title, 55, in); title[strlen(title) - 1] = '\0';

            if( strchr(title, '~') == NULL )
            {
                void* carte_1 = extract(carti, title);
                if( carte_1 == NULL )
                    fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", title);
                else
                {
                    Carte* carte = (Carte*)carte_1;
                    fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", carte->title, carte->author, carte->rating, carte->pagini);
                }
            }
            else
            {   int i;
                char **v;
                v = (char**)malloc(500*sizeof(char*));
                for(  i = 0; i < 500; i++)
                    v[i] = (char*)malloc(55*sizeof(char));
                int pos = 0;

                title[strlen(title) - 1] = '\0';
                cautare_prefix(carti, title, v, &pos);

                if( pos == 0 )
                    fprintf(out, "Nicio carte gasita.\n");
                else
                    for(  i = 0;  i<3 && i<pos; i++ )
                        fprintf(out, "%s%s\n", title,v[i]);

                for(  i = 0; i < 500; i++)
                        free(v[i]);
                free(v);
            }
        }
        else if( strcmp(command, "list_author") == 0 )
        {
            char author[55]; fgets(author, 55, in); author[strlen(author) - 1] = '\0';
            int i;
            if( strchr(author, '~') == NULL )
                list_author(authors, author, out);
            else
            {
                char **v;
                v = (char**)malloc(500*sizeof(char*));
                for(  i = 0; i < 500; i++)
                    v[i] = (char*)malloc(55*sizeof(char));
                int pos = 0;

                author[strlen(author) - 1] = '\0';
                cautare_prefix(authors, author, v, &pos);

                if( pos == 0 )
                    fprintf(out, "Niciun autor gasit.\n");
                else
                    for(  i = 0; i<3 && i<pos; i++ )
                        fprintf(out, "%s%s\n", author,v[i]);

                for(  i = 0; i < 500; i++)
                        free(v[i]);
                    free(v);
            }
        }
        else if( strcmp(command, "search_by_author") == 0 )
        {   int i;
            char info[400]; fgets(info, 400, in); info[strlen(info) - 1] = '\0';
            char* p = strtok(info, ":"); 

            if( strchr(p, '~') != NULL )
            {
               char **v;
                v = (char**)malloc(500*sizeof(char*));
                for(  i = 0; i < 500; i++)
                    v[i] = (char*)malloc(55*sizeof(char));
                int pos = 0;

                p[strlen(p) - 1] = '\0';
                cautare_prefix(authors, p, v, &pos);

                if( pos == 0 )
                    fprintf(out, "Niciun autor gasit.\n");
                else
                    for(  i = 0;  i<3 && i<pos; i++ )
                        fprintf(out, "%s%s\n", p,v[i]);

                for(  i = 0; i < 500; i++)
                        free(v[i]);
                free(v);
            }
            else
            {
                Trie aux = (Trie)extract(authors, p);
                if( aux == NULL )
                {
                    fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", p);
                    continue;
                }
                int i;
                char *q = strtok(NULL, ":"); 
                if( strchr(q, '~') != NULL )
                {

                    char **v;
                    v = (char**)malloc(500*sizeof(char*));
                    for(  i = 0; i < 500; i++)
                        v[i] = (char*)malloc(55*sizeof(char));
                    int pos = 0;

                    q[strlen(q) - 1] = '\0';
                    cautare_prefix(aux, q, v, &pos);

                    if( pos == 0 )
                        fprintf(out, "Nicio carte gasita.\n");
                    else
                        for(  i = 0; i<3 && i<pos; i++ )
                            fprintf(out, "%s%s\n", q,v[i]);

                    for(  i = 0; i < 500; i++)
                            free(v[i]);
                    free(v);
                }
                else
                {
                    void* carte_1 = extract(aux, q);
                    if( carte_1 == NULL )
                        fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", q);
                    else
                    {
                        Carte* carte = (Carte*)carte_1;
                        fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", carte->title, carte->author, carte->rating, carte->pagini);
                    }
                }
            }

        }
        else if( strcmp(command, "delete_book") == 0 )
        {
            char title[55]; fgets(title, 55, in); title[strlen(title) - 1] = '\0';
            Carte* carte = extract(carti, title);
            if( carte == NULL )
            {
                fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", title);
                continue;
            }

            char autor[55]; strcpy(autor, carte->author);

            stergere(carti, title, free_Carte);
            Trie t = extract(authors, autor);

            stergere(t, title, free_Carte_Not);
            if( Trie_goala(t) )
                stergere(authors, autor, free_Author);
        }
    }

    eliberare_Trie(&carti, free_Carte);
    eliberare_Trie(&authors, free_Author);
    return 0;
}
