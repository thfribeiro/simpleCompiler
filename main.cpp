#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAIS 256
#define MENOS 257
#define VAR 258
#define NUM 259
#define IGUAL 260
#define NO_OP 261
#define ESP 262

using namespace std;

//Structs dos tokens e da lista de simbolos
struct token
{
    int tipo;
    int valor;
};

typedef struct
{
    string nome;
    int chave;
} REGISTRO;

typedef struct aux
{
    REGISTRO reg;
    struct aux *prox;
} ELEMENTO;

typedef ELEMENTO* PONT;

typedef struct
{
    PONT inicio;
} LISTA;

//--------------------------------------------------------------------

//Protótipos das funções

void inicializar(LISTA *l);
void exibir(LISTA *l);
void inserirFim(LISTA *l, REGISTRO elem);
int busca(LISTA *l, string nomeBusca);
void reinicializar(LISTA *l);
char nextChar();
token proxToken();
string token_name(int t);
void match(int t, int v);
void p();
void r();
void e();
void x();

//--------------------------------------------------------------------

//Lista Simplesmente encadeada -- Tabela de simbolos

void inicializar(LISTA *l)
{
    l->inicio=NULL;
}

void exibir(LISTA *l)
{
    PONT end = l->inicio;
    while(end!=NULL)
    {
        cout << "Nome [" << end->reg.nome << "] Chave ["<<end->reg.chave << "]" << endl;
        end = end->prox;
    }
}

void inserirFim(LISTA *l, REGISTRO elem)
{
    PONT atual = NULL;
    PONT novo = NULL;


    atual = l->inicio;

    while(atual!=NULL && atual->reg.nome!= elem.nome)
    {
        atual = atual->prox;
    }

    if(atual!= NULL && atual->reg.nome == elem.nome)
    {
        atual->reg.chave=elem.chave;
    }
    else
    {
        novo = (PONT) malloc(sizeof(ELEMENTO));
        novo->reg=elem;
        novo->prox=NULL;

        atual = l->inicio;

        if(l->inicio==NULL)
            l->inicio=novo;
        else
        {
            while(atual->prox != NULL)
                atual = atual->prox;

            atual->prox = novo;
        }
    }
}

int busca(LISTA *l, string nomeBusca)
{
    PONT atual = NULL;

    atual = l->inicio;

    while(atual!=NULL && atual->reg.nome!= nomeBusca)
    {
        atual = atual->prox;
    }

    if(atual!= NULL && atual->reg.nome== nomeBusca)
    {
        return atual->reg.chave;
    }
    else
    {
        return NULL;
    }

}

void reinicializar(LISTA *l)
{
    PONT aux=l->inicio;


    while(aux!=NULL)
    {
        l->inicio=l->inicio->prox;
        free(aux);
        aux=l->inicio;
    }
}

//--------------------------------------------------------------------

// Variáveis globais
string input;
char c = EOF;
token lookahead;
int pos=0;
LISTA lista;
REGISTRO simbolo;

int resultado=0;
int n1, n2, op;
string auxNOME;
string auxNOME2;


//--------------------------------------------------------------------

// Analise lexica
char nextChar()
{
    if (pos < input.length())
    {
        return input[pos++];
    }
    else
    {
        return EOF;
    }
}

token proxToken()
{
    token t;
    char auxC;
    int auxN = 0;

    if (c == EOF)
    {
        auxC = nextChar();
    }
    else
    {
        auxC = c;
        c = EOF;
    }

    if(isdigit(auxC))
    {
        do
        {
            auxN = auxN * 10 + atoi(&auxC);
            auxC = nextChar();
        }
        while (isdigit(auxC));
        t.tipo = NUM;
        t.valor = auxN;
        c = auxC;
    }
    else if(isalpha(auxC))
    {

        string nomeVAR = "";
        char auxVAR;
        do
        {
            auxVAR = auxC;
            nomeVAR += auxVAR;
            auxC = nextChar();
        }
        while (isalnum(auxC));

        simbolo.chave = busca(&lista, nomeVAR);
        simbolo.nome = nomeVAR;
        auxNOME = nomeVAR;
        inserirFim(&lista, simbolo);
        t.tipo = VAR;

        auxC = nextChar();

        if(auxC == '=')
        {
            auxNOME2 = nomeVAR;
        }
        if(auxC == EOF)
        {
            n2 = simbolo.chave;
        }
        c = auxC;

    }
    else if (auxC == '=')
    {
        t.tipo = IGUAL;

    }
    else if(auxC == '+')
    {
        t.tipo = MAIS;

    }
    else if (auxC == '-')
    {
        t.tipo = MENOS;

    }
    else if (auxC == ' ')
    {
        t.tipo = ESP;

    }
    else if (auxC == EOF)
    {
        t.tipo = EOF;

    }
    else
    {

    }
    return t;
}

string token_name(int t)
{
    switch (t)
    {
    case NUM:
        return "NUM";
    case MAIS:
        return "MAIS";
    case MENOS:
        return "MENOS";
    case VAR:
        return "VAR";
    case IGUAL:
        return "IGUAL";
    case ESP:
        return "";
    }
    return "";
}

//--------------------------------------------------------------------

//Analise sintatica

void match(int t, int v)
{
    if (t == NUM)
    {
        if (op == IGUAL)
        {
            simbolo.chave = v;
            simbolo.nome = auxNOME;
            inserirFim(&lista, simbolo);
        }

        else if (op==MAIS)
        {
            if(n1!= NULL)
                resultado= resultado + v + n1;
            else
                resultado+=v;

        }
        else if (op == MENOS)
        {
            if(n1 != NULL)
                resultado = resultado - v - n1;
            else
                resultado-=v;
        }
        else
        {
            resultado+=v;
        }
    }

    else if(t == VAR)
    {

        if(n1 == NULL)
        {
            n1 = busca(&lista, auxNOME);
        }

        if(n1 == NULL)
            resultado+= v;
        else
        {

            if(op == MAIS)
            {
                n1 = 0;
                n1 = n1 + busca(&lista, auxNOME);

            }
            else if(op == MENOS)
            {
                n1 = 0;
                n1 = n1 - busca(&lista, auxNOME);

            }
            else if (op == IGUAL)
            {
                simbolo.chave = n1;
                simbolo.nome= auxNOME2;
                inserirFim(&lista, simbolo);

            }

            resultado = resultado + n1;
            n1 = 0;

        }
    }

    else if (t == IGUAL)
    {
        op = IGUAL;
    }

    else if (t== MAIS)
    {
        op = MAIS;
    }

    else if (t==MENOS)
    {
        op = MENOS;
    }

    else if (t == ESP)
    {

    }
    else
    {
        cout << endl << "Syntax error" << endl;
    }
    lookahead = proxToken();
}

//---------------------------------------------------

void p()
{
    if(lookahead.tipo == NUM)
    {
        match(NUM, lookahead.valor);
        r();
    }
    else if(lookahead.tipo == VAR)
    {
        match(VAR, 0);
        x();
    }
    else if (lookahead.tipo == ESP)
    {
        match(ESP, 0);
        p();
    }
    else
    {
        cout << endl << "Syntax error" << endl;
    }

}

//-----------------------------------------------

void x()
{
    if(lookahead.tipo == IGUAL)
    {
        match(IGUAL, 0);

        e();
    }
    else if (lookahead.tipo == MAIS)
    {
        //match(MAIS, 0);
        r();
    }
    else if (lookahead.tipo == MENOS)
    {
        //match(MENOS, 0);
        r();
    }
    else if(lookahead.tipo == ESP)
    {
        match(ESP, 0);
        x();
    }
    else if (lookahead.tipo == EOF)
    {
        cout << endl << n2 << endl;
    }
    else
    {
        cout << endl << "Syntax error" << endl;
    }
}

//-------------------------------------------------

void e()
{
    if(lookahead.tipo == NUM)
    {
        match(NUM, lookahead.valor);
        r();
    }
    else if(lookahead.tipo == VAR)
    {
        match(VAR, 0);
        r();
    }
    else if (lookahead.tipo == ESP)
    {
        match(ESP,0);
        e();
    }
    else
    {
        cout << endl << "Syntax error" << endl;
    }
}

//--------------------------------------------------

void r()
{

    if(lookahead.tipo == MAIS)
    {
        match(MAIS, 0);
        e();
    }
    else if(lookahead.tipo == MENOS)
    {
        match(MENOS, 0);
        e();
    }
    else if (lookahead.tipo == ESP)
    {
        match(ESP, 0);
        r();
    }
    else
    {
        if(op == MENOS || op == MAIS)
            cout << endl << "Resultado: "<< resultado << endl;

        resultado = 0;
        op = NO_OP;
    }
}

//--------------------------------------------------------------------


int main()
{
    inicializar(&lista);
    cout << "$ ";
    getline(cin, input);
    while (!input.empty())
    {
        lookahead = proxToken();
        p();
        cout << endl << "$ ";
        getline(cin, input);
        pos = 0;
    }

    exibir(&lista);
    reinicializar(&lista);

    return 0;
}
