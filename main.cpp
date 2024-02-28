#include <iostream>
#include <winbgim.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>

using namespace std;

#define MAX 101
#define LMAX 11
#define TOPMAX 26



char operatori[6]={'+', '-', '*', '/', '^', '\0'};
char listaFct[][7]={"sqrt", "ln", "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg"};

char mesajErori[LMAX][MAX];
int nrErori;

char tastePermise[]={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                     '.', '+', '-', '*', '/', '^', ')', '('};

char expresie[MAX];
int k=-1;

char exprFisier[MAX];
bool avemInputFisier=0;

char textLimba[2][40][101];  ///text rom & eng din fisiere
bool tipLimba=0;

int comanda=0;




/// ------------------------------------------------------------

///              FUNCTII CREARE & UPDATE MENIU

/// ------------------------------------------------------------



struct punct
{
    int x, y;
};

struct dreptunghi
{
    punct SS, DJ;
};



/// DARK / LIGHT MODE

int tipMod=1; /// 1=DARK, mod prestabilit

struct appMod
{
    int meniubkg;  ///background meniu
    int afisbkg;   ///background zona afisare/introducere
    int culoarelin;   ///culoare bordura butoane/casete
    int culoaretxt;   ///culoare text butoane/casete
    int afistxt;   ///culoare text zona afisare/introducere
    int culoaregraf;  ///culoare linii graf

}temaMod;

void schimbaMod(int tip, appMod &temaMod)  ///schimbare dark/light mode -
{
    if(tip==1)   ///dark mode - paleta culori
    {
        temaMod.meniubkg=COLOR(57, 62, 70);
        temaMod.afisbkg=COLOR(34, 40, 49);
        temaMod.culoarelin=COLOR(0, 173, 181);
        temaMod.culoaretxt=COLOR(238, 238, 238);
        temaMod.afistxt=COLOR(238, 238, 238);
        temaMod.culoaregraf=COLOR(1, 173, 181);
    }
    else   ///light mode - paleta culori
    {
        temaMod.meniubkg=COLOR(245, 237, 206);
        temaMod.afisbkg=COLOR(137, 196, 225);
        temaMod.culoarelin=COLOR(88, 40, 127);
        temaMod.culoaretxt=COLOR(26, 0, 0);
        temaMod.afistxt=COLOR(26, 0, 0);
        temaMod.culoaregraf=COLOR(89, 40, 127);
    }
}



/// LIMBI

void limbi()
{
    char rand[MAX];
    int k;

    ///ROMANA

    k=0;
    FILE* fis1;
    if (!(fis1 = fopen("./Lb_rom.txt", "r")))
    {
        perror("Eroare la deschidere, cu mesajul de eroare:");
        exit(1);
    }
    while(!feof(fis1))
    {
        if (!fgets(rand,100,fis1))
        {
            perror("Eroare la citire.\n");
            exit(1);
        }
        k++;
        strcat(textLimba[0][k], rand);
    }
    fclose(fis1);


    ///ENGLEZA

    k=0;
    FILE* fis2;
    if (!(fis2 = fopen("./Lb_eng.txt", "r")))
    {
        perror("Eroare la deschidere, cu mesajul de eroare:");
        exit(1);
    }
    while(!feof(fis2))
    {
        if (!fgets(rand,100,fis2))
        {
            perror("Eroare la citire.\n");
            exit(1);
        }
        k++;
        strcat(textLimba[1][k], rand);
    }
    fclose(fis2);
}



/// REGULI

bool stareReguli=0;

void reguli()
{
    outtextxy(790, 80, textLimba[tipLimba][1]);
    outtextxy(640, 160, textLimba[tipLimba][2]);
	outtextxy(640, 190, textLimba[tipLimba][3]);
	outtextxy(640, 220, textLimba[tipLimba][4]);
    outtextxy(640, 290, textLimba[tipLimba][5]);
    outtextxy(640, 330, textLimba[tipLimba][6]);
    outtextxy(640, 360, textLimba[tipLimba][7]);
    outtextxy(640, 390, textLimba[tipLimba][36]);
	outtextxy(640, 420, textLimba[tipLimba][8]);
    outtextxy(640, 450, textLimba[tipLimba][9]);
    outtextxy(950, 500, textLimba[tipLimba][10]);
}



/// BUTOANE - CREARE, VERIFICARE

bool apartine(punct P, dreptunghi D) ///pentru verificare click in locatie valida (ex: buton)
{
    return D.SS.x<=P.x && P.x<=D.DJ.x && D.SS.y<=P.y && P.y<=D.DJ.y;
}

struct buton
{
    dreptunghi D;
    char text[20];
};

void initializeazaButon(buton &nume, int stX, int stY, int drX, int drY,  char text[50]) ///coordonate + text
{
    nume.D.SS.x=stX;
    nume.D.SS.y=stY;
    nume.D.DJ.x=drX;
    nume.D.DJ.y=drY;
    strcpy(nume.text,text);
}

void deseneazaButon(punct st, punct dr, int textX, int textY, char text[50])  ///design, schema de culori -
                                                                              ///conform temei alese (dark/light)
{
    setcolor(temaMod.culoarelin);
    rectangle(st.x, st.y, dr.x, dr.y);
    setcolor(temaMod.culoaretxt);
    outtextxy(textX, textY, text);
}



/// COMPONENTE MENIU

buton mod[3], limba[3], optiuni[5], chenarFct, butonFis, butonReguli, butonIesire;  ///chenarFct = zona citire expresie
                                                           ///butonFis = buton atasare fisier (expresie input)


/// CREARE MENIU

void deseneazaMeniul()
{
    ///SETARE DARK MODE - IMPLICIT
    schimbaMod(tipMod, temaMod);
    setbkcolor(temaMod.meniubkg);
    clearviewport();
    setcolor(temaMod.culoarelin);
    line(600, 0, 600, 600);  ///creare zona meniu & zona afisare

    ///Vector mode
    for (int i=1; i<3; i++)
    {
        ///Creare butoane
        mod[i].D.SS.x=15+(i>1)*50;
        mod[i].D.SS.y=15;
        mod[i].D.DJ.x=65+(i>1)*50;
        mod[i].D.DJ.y=45;
        strcpy(mod[i].text, (i==1)?textLimba[tipLimba][11]:textLimba[tipLimba][12]);
        deseneazaButon(mod[i].D.SS, mod[i].D.DJ, mod[i].D.SS.x+5, mod[i].D.SS.y+8, mod[i].text);
    }

    ///Vector limbi
    for (int i=1; i<3; i++)
    {
        ///Creare butoane
        limba[i].D.SS.x=(i==1)?485:535;
        limba[i].D.SS.y=15;
        limba[i].D.DJ.x=(i==1)?535:585;
        limba[i].D.DJ.y=45;
        strcpy(limba[i].text, (i==1)?textLimba[tipLimba][13]:textLimba[tipLimba][14]);
        deseneazaButon(limba[i].D.SS, limba[i].D.DJ, limba[i].D.SS.x+12, limba[i].D.SS.y+8, limba[i].text);
    }

    ///Zona citire functie
    initializeazaButon(chenarFct, 100, 130, 500, 200, textLimba[tipLimba][15]);
    deseneazaButon(chenarFct.D.SS, chenarFct.D.DJ, chenarFct.D.SS.x, chenarFct.D.SS.y-20, chenarFct.text);

    ///Buton atasare fisier
    initializeazaButon(butonFis, 220, 240, 380, 270, textLimba[tipLimba][16]);
    deseneazaButon(butonFis.D.SS, butonFis.D.DJ, butonFis.D.SS.x + (160-textwidth(butonFis.text))/2, butonFis.D.SS.y+8, butonFis.text);

    ///Buton reguli
    initializeazaButon(butonReguli, 15, 555, 75, 585, textLimba[tipLimba][34]);
    deseneazaButon(butonReguli.D.SS, butonReguli.D.DJ, butonReguli.D.SS.x + (60-textwidth(butonReguli.text))/2, butonReguli.D.SS.y+8, butonReguli.text);

    ///Buton iesire
    initializeazaButon(butonIesire, 525, 555, 585, 585, textLimba[tipLimba][35]);
    deseneazaButon(butonIesire.D.SS, butonIesire.D.DJ, butonIesire.D.SS.x + (60-textwidth(butonIesire.text))/2, butonIesire.D.SS.y+8, butonIesire.text);

    ///Meniu

    setcolor(temaMod.culoarelin);
    rectangle(100, 350, 500, 530);  ///delimitare zona optiuni derivare
    for (int i=1; i<=4; i++)
    {
        optiuni[i].D.SS.x=130+(i%2)*200;
        optiuni[i].D.DJ.x=270+(i%2)*200;
        optiuni[i].D.SS.y=380+(i>2)*75;
        optiuni[i].D.DJ.y=425+(i>2)*75;
        switch(i)
        {
        case 1:
            strcpy(optiuni[i].text,textLimba[tipLimba][17]);
            break;
        case 2:
            strcpy(optiuni[i].text,textLimba[tipLimba][18]);
            break;
        case 3:
            strcpy(optiuni[i].text,textLimba[tipLimba][19]);
            break;
        case 4:
            strcpy(optiuni[i].text,textLimba[tipLimba][20]);
            break;
        }
        deseneazaButon(optiuni[i].D.SS, optiuni[i].D.DJ, optiuni[i].D.SS.x+21, optiuni[i].D.SS.y+14, optiuni[i].text);
    }
    setbkcolor(temaMod.culoarelin);  ///setare background zona optiuni
    setfillstyle(SOLID_FILL, temaMod.meniubkg);
    floodfill(120, 355, temaMod.culoarelin);

    ///Zona afisare
    setbkcolor(temaMod.afisbkg);
    setfillstyle(SOLID_FILL, temaMod.afisbkg);
    floodfill(610, 310, temaMod.culoarelin);

}




///-----------------------------------------------------------------------------------

///                        VERIF EXPRESIE SCRISA CORECT

///-----------------------------------------------------------------------------------




void alertaErori(char mesajErori[][MAX], int nrErori)
{
    int lmax=0;
    for (int i=0; i<nrErori; i++)
    {
        if (textwidth(mesajErori[i])>lmax)
        {
            lmax=textwidth(mesajErori[i]);
        }
    }

    setcolor(WHITE);
    rectangle(600+(600-lmax-40)/2, 300-10*nrErori-20, 1200-(600-lmax-40)/2, 300+10*nrErori+20);
    setbkcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    floodfill(810, 310, WHITE);

    int k=0;
    for(int i=300-10*nrErori; i<=300+10*nrErori; i+=20)
    {
        outtextxy(600+(600-lmax)/2, i, mesajErori[k]);
        k++;
    }
}

bool verifCaracterePermise(char expresie[MAX])
{
    for (int i=0; i<strlen(expresie); i++)
    {
        char ch=expresie[i];
        if (!(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9' || ch=='.' || ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^' || ch=='(' || ch==')'))
        {
            return 0;
        }
    }
    return 1;
}

bool verifNrParanteze(char expresie[MAX])
{
    int cntParantezeDeschise=0, cntParantezeInchise=0;

    for(int i=0; i<strlen(expresie); i++)
    {
        if(expresie[i]=='(')
        {
            cntParantezeDeschise++;
        }
        if(expresie[i]==')')
        {
            cntParantezeInchise++;
        }
        if(cntParantezeDeschise<cntParantezeInchise)
        {
            return 0; ///a fost găsită o eroare
        }
    }

    if(cntParantezeDeschise!=cntParantezeInchise)
    {
        return 0; ///a fost găsită o eroare
    }
    return 1; ///eroarea nu provine de la numărul de paranteze
}

bool verifOperatori(char expresie[MAX], char operatori[6])
{
    for(int i=0; i<strlen(expresie)-1; i++)
    {
        if (i>0 && (strchr(operatori, expresie[i])!=NULL && expresie[i]!='-') && (isalnum(expresie[i-1])!=0 || expresie[i-1]=='e' || expresie[i-1]=='x'
            || expresie[i-1]==')') && (isalnum(expresie[i+1])!=0 || expresie[i+1]=='e' || expresie[i+1]=='x' || expresie[i+1]=='('))
        {
            return 1;  ///toti operatorii inafara de minus sunt au operand corect (numar, variabila sau paranteza corespunzatoare) in ambele capete
        }
        else if (expresie[i]=='-' && (isalnum(expresie[i+1])!=0 || expresie[i+1]=='e' || expresie[i+1]=='x' || expresie[i+1]=='('))
        {
            return 1;  ///pentru minus verificam doar operandul din stanga
        }

        if(strchr(operatori, expresie[i])!=NULL && (isalnum(expresie[i+1])!=0 || expresie[i+1]=='('))
        {
            continue;
        }
        else
        {
            if(strchr(operatori, expresie[i])!=NULL)
            ///caracterul curent este un operator, dar nu este urmat de o variabila/constantă/funcție/paranteză
            {
                return 0; ///a fost găsită o eroare
            }
        }

    }
    return 1; ///eroarea provine de la operatori
}

bool verifParantDeschisa(char expresie[101], char operatori[6])
{
    for(int i=0; i<strlen(expresie)-1; i++)
    {
        if(expresie[i]=='(' && (expresie[i+1]=='(' || isalnum(expresie[i+1])!=0 || ((expresie[i+1]=='+' || expresie[i+1]=='-') && isalnum(expresie[i+2])!=0)))
                                                                                    ///+ sau - este unar dacă următorul caracter este alfanumeric
        {
            continue;
        }
        else
        {
            if(expresie[i]=='(')
            ///caracterul curent este o paranteză deschisă, dar nu este urmată de o altă paranteză deschisă/
            ///un caracter alfanumeric (constantă/variabilă/funcție)/un operator unar (+ sau -)
            {
                return 0; ///a fost găsită o eroare
            }
        }
    }
    return 1; ///eroarea nu provine de la o paranteză deschisă
}

bool verifParantInchisa(char expresie[MAX], char operatori[6])
{
    for(int i=0; i<strlen(expresie)-1; i++)
    {
        if(expresie[i]==')' && (expresie[i+1]==')' || strchr(operatori, expresie[i+1])!=NULL))
        {
            continue;
        }
        else
        {
            if(expresie[i]==')')
            ///caracterul curent este o paranteză închisă,
            ///dar nu este urmată de o altă paranteză închisă/un operator
            {
                return 0; ///a fost găsită o eroare
            }
        }
    }
    return 1; ///eroarea nu provine de la o paranteză închisă
}

bool verifVariabila(char expresie[MAX], char operatori[6])
{
    for(int i=1; i<strlen(expresie)-1; i++)
    {
        if((expresie[i]=='x' || expresie[i]=='e') && (isalpha(expresie[i+1])==0 || isalpha(expresie[i-1])==0))
        ///caracterul curent este o variabilă, dar nu este urmată de un operator/o paranteză închisă
        {
            if((expresie[i+1]!=')' || strchr(operatori, expresie[i+1])==NULL) && verifParantDeschisa(expresie, operatori)==0)
            {
                return 0; ///a fost găsită o eroare
            }
        }
    }
    return 1; ///eroarea nu provine de la variabilă
}

bool verifConstanta(char expresie[MAX], char operatori[6])
{
    for(int i=1; i<strlen(expresie)-1; i++)
    {
        if(isdigit(expresie[i])!=0 && (isdigit(expresie[i+1]==0) || isdigit(expresie[i-1])==0))
        ///caracterul curent este o constantă, dar nu este urmată de un operator/o paranteză închisă
        {
            if((expresie[i+1]!=')' || strchr(operatori, expresie[i+1])==NULL) && verifParantDeschisa(expresie, operatori)==0)
            {
                return 0; ///a fost găsită o eroare
            }
        }
    }
    return 1; ///eroarea nu provine de la constantă
}

bool verifFunctie(char expresie[MAX], char operatori[6])
{
    int i=0;
    char f[11];
    f[0]='\0';
    while (expresie[i])
    {
        if (expresie[i]>='a' && expresie[i]<='z')
        {
            int k=strlen(f);
            f[k]=expresie[i];
            f[k+1]='\0';
        }
        else if (expresie[i]<'a' || expresie[i]>'z')
        {
            if (f[0]!='\0')
            {
                if (strcmp(f, "x")==0 || strcmp(f, "e")==0 || strcmp(f, "sqrt")==0 || strcmp(f, "ln")==0 || strcmp(f, "sin")==0 ||
                    strcmp(f, "cos")==0 || strcmp(f, "tg")==0 || strcmp(f, "ctg")==0 || strcmp(f, "arcsin")==0 || strcmp(f, "arccos")==0 ||
                    strcmp(f, "arctg")==0 || strcmp(f, "arcctg")==0)
                {
                }
                else
                {
                    return 0;
                }
                f[0]='\0';
            }
        }
        i++;
    }
    if (f[0]!='\0')
    {
        if (strcmp(f, "x")==0 || strcmp(f, "e")==0 || strcmp(f, "sqrt")==0 || strcmp(f, "ln")==0 || strcmp(f, "sin")==0 ||
            strcmp(f, "cos")==0 || strcmp(f, "tg")==0 || strcmp(f, "ctg")==0 || strcmp(f, "arcsin")==0 || strcmp(f, "arccos")==0 ||
            strcmp(f, "arctg")==0 || strcmp(f, "arcctg")==0)
        {
        }
        else
        {
            return 0;
        }
        f[0]='\0';
    }
    return 1;
}

bool verifNumar(char expresie[MAX])
{
    if(expresie[0]=='.')
    {
        return 0; ///a fost găsită o eroare
    }
    int nrPuncte=0;
    for(int i=1; i<strlen(expresie)-1; i++)
    {
        if(expresie[i]=='.')
        {
            nrPuncte++;
            if(isdigit(expresie[i-1])==0 || isdigit(expresie[i+1])==0)
            ///înainte și după punct trebuie să fie doar numere
            {
                return 0; ///a fost găsită o eroare
            }
            if(nrPuncte>1) ///trebuie să fie un singur punct într-un număr
            {
                return 0; ///a fost găsită o eroare
            }
        }
        else
        {
            if(isdigit(expresie[i])==0) ///resetează numărătoarea doar când nu mai este în număr
            {
                nrPuncte=0;
            }
        }
    }
    return 1; ///eroarea nu provine de la scrierea unui număr
}

int verifExpresie(char expresie[MAX], char mesajErori[LMAX][MAX], int &nrErori)
{
    nrErori=0;
    if(verifCaracterePermise(expresie)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][21]);
        nrErori++;
    }
    if(verifNrParanteze(expresie)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][22]);
        nrErori++;
    }
    if(verifOperatori(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][23]);
        nrErori++;
    }
    if(verifVariabila(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][24]);
        nrErori++;
    }
    if(verifConstanta(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][25]);
        nrErori++;
    }
    if(verifFunctie(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][26]);
        nrErori++;
    }
    if(verifParantDeschisa(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][27]);
        nrErori++;
    }
    if(verifParantInchisa(expresie, operatori)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][28]);
        nrErori++;
    }
    if(verifNumar(expresie)==0)
    {
        strcpy(mesajErori[nrErori], textLimba[tipLimba][29]);
        nrErori++;
    }
    return nrErori;
}



///----------------------------------------------------------

///                  Prelucrare expresie

///----------------------------------------------------------



void textLitMici (char expresie[101])
{
    for (int i=0; expresie[i]; i++)
    {
        if (expresie[i]>='A' && expresie[i]<='Z')
        {
            expresie[i]=expresie[i]-'A'+'a';
        }
    }
}

void vectorizare(char expresie[MAX], char VECTOR[MAX][LMAX], int& k) ///k=nr de elem din VECTOR
{
    int i = 0,j;
    char s[MAX];
    s[0]='\0';
    while (i < strlen(expresie))
    {
        k++;
        if (strchr("sclta", expresie[i]))
        {
            j = 0;
            while (expresie[i] >= 'a' && expresie[i] <= 'z')
            {
                j++;i++;
            }
            strcpy(s, expresie + (i-j));
            s[j] = '\0';
            strcpy(VECTOR[k], s);
        }
        else
            if (strchr("()+-*/^xe", expresie[i]))
            {
                if(expresie[i]=='-' && (expresie[i-1]=='\0' || expresie[i-1]=='('))
                {
                    strcpy(VECTOR[k], "0");
                    k++;
                }
                strcpy(s, expresie + i);
                s[1] = '\0';
                strcpy(VECTOR[k], s);
                i++;
            }
            else
            {
                j = 0;
                while (expresie[i] >= '0' && expresie[i] <= '9' || expresie[i] == '.')
                {
                    j++;i++;
                }
                strcpy(s, expresie + (i-j));
                s[j] = '\0';
                strcpy(VECTOR[k], s);
            }
    }
}


typedef char tipDate;
struct nod
{
    char info[LMAX];
    nod *st, *dr;
};
typedef nod * arbore;

arbore arb,arb_derivat,arb_simplificat, arb_deriv2, arb_deriv2_simplif;

int esteArboreNul(arbore a)
{
    return (a==NULL);
}

int prioritate(char c)  /// prioritatea operatorilor
{
    if(c=='(' || c==')')
        return 0;
    if(c=='+' || c=='-')
        return 1;
    if(c=='*' || c=='/')
        return 2;
    if(c=='^')
        return 3;
    if(c=='c' || c=='s' || c=='l' || c=='t' || c=='a')
        return 4;
}

void parcurgereInPostordine(arbore a, char exprPostordine[MAX])
{
    if (!esteArboreNul(a))
    {
        parcurgereInPostordine(a->st,exprPostordine);
        parcurgereInPostordine(a->dr,exprPostordine);
        strcat(exprPostordine,a->info);
        strcat(exprPostordine,", ");
    }
}

void parcurgereInInordine(arbore a, char exprInordine[MAX], bool rad)
{
    if (!esteArboreNul(a))
    {
        if (a->st && a->dr)
        {

            if (rad)
            {
                if (a->info[0]=='-' && a->st->info[0]=='0')
                {
                    strcat(exprInordine,a->info);
                    parcurgereInInordine(a->dr,exprInordine, 0);
                }
                else
                {
                    parcurgereInInordine(a->st,exprInordine, 0);
                    strcat(exprInordine,a->info);
                    parcurgereInInordine(a->dr,exprInordine, 0);
                }
            }
            else
            {
                if (a->info[0]=='-' && a->st->info[0]=='0')
                {
                    strcat(exprInordine, " (");
                    strcat(exprInordine,a->info);
                    parcurgereInInordine(a->dr,exprInordine, 0);
                    strcat(exprInordine, ") ");
                }
                else
                {
                    strcat(exprInordine, " (");
                    parcurgereInInordine(a->st,exprInordine, 0);
                    strcat(exprInordine,a->info);
                    parcurgereInInordine(a->dr,exprInordine, 0);
                    strcat(exprInordine, ") ");
                }
            }

        }
        else
        {
            if ((a->st && (a->st->info[0]=='e' || a->st->info[0]=='x' || (a->st->info[0]>='0' && a->st->info[0]<='9')))
                && (a->info[0]>='a' && a->info[0]<='z'))
            {
                strcat(exprInordine, a->info);
                if (a->st)
                {
                    strcat(exprInordine, a->st->info);
                }
                else
                {
                    strcat(exprInordine, a->dr->info);
                }
            }
            else if (a->st && strchr("+-/*^sclta", a->st->info[0]) && strchr("sclta", a->info[0]))
            {
                strcat(exprInordine,a->info);
                strcat(exprInordine, " (");
                parcurgereInInordine(a->st,exprInordine, 0);
                strcat(exprInordine, ") ");
            }
            else
            {
                parcurgereInInordine(a->st,exprInordine, 0);
                strcat(exprInordine,a->info);
                parcurgereInInordine(a->dr,exprInordine, 0);
            }

        }
    }
}

void arborizare(char VECTOR[MAX][LMAX],int k, arbore& a)
{
    int i,top1,top2;
    char operatori[TOPMAX][MAX];
    arbore operanzi[TOPMAX];
    i=0;top1=0;top2=1;operatori[top2][0]='(';
    while(i<=k && top2>0)
    {
        i++;
        if(VECTOR[i][0]=='x' || VECTOR[i][0]=='e' || (VECTOR[i][0]>='0' && VECTOR[i][0]<='9'))
        {
            top1++;
            a=new nod;
            strcpy(a->info,VECTOR[i]);
            a->st=NULL;
            a->dr=NULL;
            operanzi[top1]=a;
        }
        else
        {
            if(VECTOR[i][0]=='(')
            {
                top2++;
                strcpy(operatori[top2],"(");
            }
            else
            {
                while(top2>0 && !strchr("()",operatori[top2][0]) && (prioritate(operatori[top2][0]) >= prioritate(VECTOR[i][0])))
                {
                    if(strchr("sclta",operatori[top2][0]))
                    {
                        a=new nod;
                        strcpy(a->info,operatori[top2]);
                        a->st=operanzi[top1];
                        a->dr=NULL;
                        operanzi[top1]=a;
                    }
                    else
                    {
                        a=new nod;
                        strcpy(a->info,operatori[top2]);
                        a->st=operanzi[top1-1];
                        a->dr=operanzi[top1];
                        operanzi[top1-1]=a;
                        top1--;
                    }
                    top2--;
                }
                if(top2>0)
                {
                    if(operatori[top2][0]!='(' || VECTOR[i][0]!=')')
                    {
                        top2++;
                        strcpy(operatori[top2],VECTOR[i]);
                    }
                    else
                    {
                        top2--;
                    }
                }
            }
        }
    }
}


int nrNiveluri(arbore a)
{
    if(a==NULL)return 0;
    else
    {
        int n1=nrNiveluri(a->st);
        int n2=nrNiveluri(a->dr);
        return 1+max(n1,n2);
    }
}

int nrColoane(arbore a)
{
    if(a==NULL)return 0;
    else
    {
        int n1=nrColoane(a->st);
        int n2=nrColoane(a->dr);
        return 1+n1+n2;
    }
}

void desenareNod(char elem[11], int xc, int yc, int latime, int inaltime)
{
    char arr[50];
    sprintf(arr,"%s",elem);

    setcolor(temaMod.culoaregraf);
    setfillstyle(SOLID_FILL,temaMod.meniubkg);
    fillellipse(xc,yc,textwidth(arr)+4, textheight("M")/2+8);

    setbkcolor(temaMod.meniubkg);
    setcolor(temaMod.culoaretxt);
    settextjustify(1,1);
    outtextxy(xc,yc+4,arr);
}

void desenareArbore(arbore a, int niv, int stanga, int latime, int inaltime)
{
    if(a!=NULL)
    {
        int n1=nrColoane(a->st);
        int xc=stanga+latime*n1+latime/2;
        int yc=niv*inaltime-inaltime/2;

        if (a->st && a->dr && a->info[0]=='-' && strcmp(a->st->info,"0")==0)
        {
            desenareNod(a->info,xc,yc,latime, inaltime);
            int xcd=stanga+latime*(n1+1)+latime*nrColoane(a->dr->st)+latime/2;
            setcolor(temaMod.culoaregraf);
            line(xc,yc,xcd,yc+inaltime);
            desenareArbore(a->dr,niv+1,stanga+latime*(n1+1), latime, inaltime);
        }
        else
        {
            if (a->st!=NULL)
            {
                int xcs=stanga+latime*nrColoane(a->st->st)+latime/2;
                setcolor(temaMod.culoaregraf);
                line(xc,yc,xcs,yc+inaltime);
            }
            if (a->dr!=NULL)
            {
                int xcd=stanga+latime*(n1+1)+latime*nrColoane(a->dr->st)+latime/2;
                setcolor(temaMod.culoaregraf);
                line(xc,yc,xcd,yc+inaltime);
            }
            desenareNod(a->info,xc,yc,latime, inaltime);
            desenareArbore(a->st,niv+1,stanga, latime, inaltime);
            desenareArbore(a->dr,niv+1,stanga+latime*(n1+1), latime, inaltime);
        }
    }
}

void derivare(arbore arb_initial, arbore& arb_derivat)
{
    arbore arb1,arb2,arb3,arb4,arb5,arb6,arb7,arb8;
    arb_derivat=new nod;
    strcpy(arb_derivat->info,"new");
    arb_derivat->st=NULL;
    arb_derivat->dr=NULL;
    if(arb_initial->info[0]>='0' && arb_initial->info[0]<='9')    /// c'=0
        strcpy(arb_derivat->info,"0");
    else
        switch(arb_initial->info[0])
        {
        case 'x':   /// x'=1
                strcpy(arb_derivat->info,"1");
                break;
        case '+':   /// (f+g)'=f'+g'
            {
                derivare(arb_initial->st,arb3);
                arb_derivat->st=arb3;   /// f'
                derivare(arb_initial->dr,arb4);
                arb_derivat->dr=arb4;   /// g'
                strcpy(arb_derivat->info,"+");
                break;
            }
        case '-':   /// (f-g)'=f'-g'
            {
                derivare(arb_initial->st,arb3);
                arb_derivat->st=arb3;   /// f'
                derivare(arb_initial->dr,arb4);
                arb_derivat->dr=arb4;   /// g'
                strcpy(arb_derivat->info,"-");
                break;
            }
        case '*':   /// (f*g)'=f'*g+f*g'
            {
                arb1=new nod;
                arb2=new nod;
                derivare(arb_initial->st,arb3);
                derivare(arb_initial->dr,arb4);
                strcpy(arb_derivat->info,"+");
                arb1->st=arb3;  /// f'
                arb1->dr=arb_initial->dr;   /// g
                strcpy(arb1->info,"*");
                arb2->st=arb_initial->st;   /// f
                arb2->dr=arb4;  /// g'
                strcpy(arb2->info,"*");
                arb_derivat->st=arb1;   /// f'*g
                arb_derivat->dr=arb2;   /// f*g'
                break;
            }
        case '/':   /// (f/g)'=(f'*g-f*g')/(g^2)
            {
                arb1=new nod;
                arb2=new nod;
                arb5=new nod;
                derivare(arb_initial->st,arb3);
                derivare(arb_initial->dr,arb4);
                strcpy(arb1->info,"*");
                arb1->st=arb3;  /// f'
                arb1->dr=arb_initial->dr;   /// g
                strcpy(arb2->info,"*");
                arb2->st=arb_initial->st;   /// f
                arb2->dr=arb4;  /// g'
                strcpy(arb5->info,"-");
                arb5->st=arb1;  /// f'*g
                arb5->dr=arb2;  /// f*g'
                arb6=new nod;
                arb7=new nod;
                strcpy(arb6->info,"2");
                arb6->st=NULL;
                arb6->dr=NULL;
                strcpy(arb7->info,"^");
                arb7->st=arb_initial->dr;   /// g
                arb7->dr=arb6;  /// 2
                strcpy(arb_derivat->info,"/");
                arb_derivat->st=arb5;   /// f'*g-f*g'
                arb_derivat->dr=arb7;   /// g^2
                break;
            }
        case 's':   /// sin si sqrt
            {
                if(strcmp(arb_initial->info,"sin")==0)  /// (sin(u))'=cos(u)*u'
                {
                    arb1=new nod;
                    strcpy(arb1->info,"cos");
                    arb1->st=arb_initial->st;   /// u
                    arb1->dr=NULL;
                    derivare(arb_initial->st,arb2);
                    strcpy(arb_derivat->info,"*");
                    arb_derivat->st=arb1;   /// cos(u)
                    arb_derivat->dr=arb2;   /// u'
                }
                else    /// (sqrt(u))'=(u')/(2*sqrt(u))
                {
                    derivare(arb_initial->st,arb1);
                    arb2=new nod;
                    strcpy(arb2->info,arb_initial->info);
                    arb2->st=arb_initial->st;   /// u
                    arb2->dr=NULL;
                    arb3=new nod;
                    strcpy(arb3->info,"2");
                    arb3->st=NULL;
                    arb3->dr=NULL;
                    arb4=new nod;
                    strcpy(arb4->info,"*");
                    arb4->st=arb3;  /// 2
                    arb4->dr=arb2;  /// sqrt(u)
                    strcpy(arb_derivat->info,"/");
                    arb_derivat->st=arb1;   /// u'
                    arb_derivat->dr=arb4;   /// 2*sqrt(u)
                }
                break;
            }
        case 'c':   /// cos si ctg
            {
                if(strcmp(arb_initial->info,"cos")==0)  /// (cos(u))'= -sin(u)*(u')
                {
                    derivare(arb_initial->st,arb4);
                    arb1=new nod;
                    arb2=new nod;
                    arb3=new nod;
                    strcpy(arb1->info,"0");
                    arb1->st=NULL;
                    arb1->dr=NULL;
                    strcpy(arb2->info,"sin");
                    arb2->st=arb_initial->st;   /// u
                    arb2->dr=NULL;
                    strcpy(arb3->info,"*");
                    arb3->st=arb2;  /// sin(u)
                    arb3->dr=arb4;  /// u'
                    strcpy(arb_derivat->info,"-");
                    arb_derivat->st=arb1;   /// 0
                    arb_derivat->dr=arb3;   /// sin(u)*(u')
                }
                else    /// (ctg(u))'= -(u')/((sin(u))^2)
                {
                    derivare(arb_initial->st,arb1);
                    arb2=new nod;
                    strcpy(arb2->info,"sin");
                    arb2->st=arb_initial->st;   /// u
                    arb2->dr=NULL;
                    arb3=new nod;
                    strcpy(arb3->info,"2");
                    arb3->st=NULL;
                    arb3->dr=NULL;
                    arb4=new nod;
                    strcpy(arb4->info,"^");
                    arb4->st=arb2;  /// sin(u)
                    arb4->dr=arb3;  /// 2
                    arb5=new nod;
                    strcpy(arb5->info,"/");
                    arb5->st=arb1;  /// u'
                    arb5->dr=arb4;  /// (sin(u))^2
                    arb6=new nod;
                    strcpy(arb6->info,"0");
                    arb6->st=NULL;
                    arb6->dr=NULL;
                    strcpy(arb_derivat->info,"-");
                    arb_derivat->st=arb6;   /// 0
                    arb_derivat->dr=arb5;   /// (u')/((sin(u))^2)
                }
                break;
            }
        case 't':   /// (tg(u))'=(u')/((cos(u))^2)
            {
                derivare(arb_initial->st,arb1);
                arb2=new nod;
                strcpy(arb2->info,"cos");
                arb2->st=arb_initial->st;   /// u
                arb2->dr=NULL;
                arb3=new nod;
                strcpy(arb3->info,"2");
                arb3->st=NULL;
                arb3->dr=NULL;
                arb4=new nod;
                strcpy(arb4->info,"^");
                arb4->st=arb2;  /// cos(u)
                arb4->dr=arb3;  /// 2
                arb5=new nod;
                strcpy(arb_derivat->info,"/");
                arb_derivat->st=arb1;  /// u'
                arb_derivat->dr=arb4;  /// (cos(u))^2
                break;
            }
        case 'a':   /// arcsin, arccos, arctg si arcctg
            {
                if(strcmp(arb_initial->info,"arcsin")==0)   /// (arcsin(u))'=(u')/sqrt(1-u^2)
                {
                    derivare(arb_initial->st,arb1);
                    arb2=new nod;
                    strcpy(arb2->info,"2");
                    arb2->st=NULL;
                    arb2->dr=NULL;
                    arb3=new nod;
                    strcpy(arb3->info,"^");
                    arb3->st=arb_initial->st;   /// u
                    arb3->dr=arb2;  /// 2
                    arb4=new nod;
                    strcpy(arb4->info,"1");
                    arb4->st=NULL;
                    arb4->dr=NULL;
                    arb5=new nod;
                    strcpy(arb5->info,"-");
                    arb5->st=arb4;  /// 1
                    arb5->dr=arb3;  /// u^2
                    arb6=new nod;
                    strcpy(arb6->info,"sqrt");
                    arb6->st=arb5;  /// 1-u^2
                    arb6->dr=NULL;
                    strcpy(arb_derivat->info,"/");
                    arb_derivat->st=arb1;   /// u'
                    arb_derivat->dr=arb6;   /// sqrt(1-u^2)
                }
                else
                    if(strcmp(arb_initial->info,"arccos")==0)   /// (arccos(u))'= -(u')/sqrt(1-u^2)
                    {
                        derivare(arb_initial->st,arb1);
                        arb2=new nod;
                        strcpy(arb2->info,"2");
                        arb2->st=NULL;
                        arb2->dr=NULL;
                        arb3=new nod;
                        strcpy(arb3->info,"^");
                        arb3->st=arb_initial->st;   /// u
                        arb3->dr=arb2;  /// 2
                        arb4=new nod;
                        strcpy(arb4->info,"1");
                        arb4->st=NULL;
                        arb4->dr=NULL;
                        arb5=new nod;
                        strcpy(arb5->info,"-");
                        arb5->st=arb4;  /// 1
                        arb5->dr=arb3;  /// u^2
                        arb6=new nod;
                        strcpy(arb6->info,"sqrt");
                        arb6->st=arb5;  /// 1-u^2
                        arb6->dr=NULL;
                        arb7=new nod;
                        strcpy(arb7->info,"/");
                        arb7->st=arb1;   /// u'
                        arb7->dr=arb6;   /// sqrt(1-u^2)
                        arb8=new nod;
                        strcpy(arb8->info,"0");
                        arb8->st=NULL;
                        arb8->dr=NULL;
                        strcpy(arb_derivat->info,"-");
                        arb_derivat->st=arb8;   /// 0
                        arb_derivat->dr=arb7;   /// (u')/sqrt(1-u^2)
                    }
                    else
                        if(strcmp(arb_initial->info,"arctg")==0)    /// (arctg(u))'=u'/(1+u^2)
                        {
                            derivare(arb_initial->st,arb1);
                            arb2=new nod;
                            strcpy(arb2->info,"2");
                            arb2->st=NULL;
                            arb2->dr=NULL;
                            arb3=new nod;
                            strcpy(arb3->info,"^");
                            arb3->st=arb_initial->st;   /// u
                            arb3->dr=arb2;  /// 2
                            arb4=new nod;
                            strcpy(arb4->info,"1");
                            arb4->st=NULL;
                            arb4->dr=NULL;
                            arb5=new nod;
                            strcpy(arb5->info,"+");
                            arb5->st=arb4;  /// 1
                            arb5->dr=arb3;  /// u^2
                            strcpy(arb_derivat->info,"/");
                            arb_derivat->st=arb1;   /// u'
                            arb_derivat->dr=arb5;   /// 1+u^2
                        }
                        else
                            if(strcmp(arb_initial->info,"arcctg")==0)    /// (arcctg(u))'= -(u')/(1+u^2)
                            {
                                derivare(arb_initial->st,arb1);
                                arb2=new nod;
                                strcpy(arb2->info,"2");
                                arb2->st=NULL;
                                arb2->dr=NULL;
                                arb3=new nod;
                                strcpy(arb3->info,"^");
                                arb3->st=arb_initial->st;   /// u
                                arb3->dr=arb2;  /// 2
                                arb4=new nod;
                                strcpy(arb4->info,"1");
                                arb4->st=NULL;
                                arb4->dr=NULL;
                                arb5=new nod;
                                strcpy(arb5->info,"+");
                                arb5->st=arb4;  /// 1
                                arb5->dr=arb3;  /// u^2
                                arb6=new nod;
                                strcpy(arb6->info,"/");
                                arb6->st=arb1;  /// u'
                                arb6->dr=arb5;  /// 1+u^2
                                arb7=new nod;
                                strcpy(arb7->info,"0");
                                arb7->st=NULL;
                                arb7->dr=NULL;
                                strcpy(arb_derivat->info,"-");
                                arb_derivat->st=arb7;   /// 0
                                arb_derivat->dr=arb6;   /// u'/(1+u^2)
                            }
                break;
            }
        case 'l':   /// (ln(u))'= (u')/u
            {
                derivare(arb_initial->st,arb1);
                strcpy(arb_derivat->info,"/");
                arb_derivat->st=arb1;   /// u'
                arb_derivat->dr=arb_initial->st;    /// u
                break;
            }
        case '^':
            {
                if(arb_initial->st->info[0]=='e' || isdigit(arb_initial->st->info[0]))  /// (a^u)'=(a^u)*ln(a)*(u')
                {
                    derivare(arb_initial->dr,arb1);
                    arb2=new nod;
                    strcpy(arb2->info,arb_initial->st->info);
                    arb2->st=NULL;
                    arb2->dr=NULL;
                    arb3=new nod;
                    strcpy(arb3->info,"^");
                    arb3->st=arb2;  /// a
                    arb3->dr=arb_initial->dr;   /// u
                    arb4=new nod;
                    strcpy(arb4->info,"ln");
                    arb4->st=arb2;  /// a
                    arb4->dr=NULL;
                    arb5=new nod;
                    strcpy(arb5->info,"*");
                    arb5->st=arb3;  /// a^u
                    arb5->dr=arb4;  /// ln(a)
                    strcpy(arb_derivat->info,"*");
                    arb_derivat->st=arb5;   /// (a^u)*ln(a)
                    arb_derivat->dr=arb1;   /// u'
                }
                else    /// (u^n)'=n*(u^(n-1))*(u')
                {
                    derivare(arb_initial->st,arb1);
                    arb5=new nod;
                    arb2=new nod;
                    strcpy(arb5->info,"1");
                    arb5->st=NULL;
                    arb5->dr=NULL;
                    strcpy(arb2->info,"-");
                    arb2->st=arb_initial->dr;   /// n
                    arb2->dr=arb5;  /// 1
                    arb3=new nod;
                    strcpy(arb3->info,"^");
                    arb3->st=arb_initial->st;   /// u
                    arb3->dr=arb2;  /// n-1
                    arb4=new nod;
                    strcpy(arb4->info,"*");
                    arb4->st=arb_initial->dr;   /// n
                    arb4->dr=arb3;  /// u^(n-1)
                    strcpy(arb_derivat->info,"*");
                    arb_derivat->st=arb4;   /// n*(u^(n-1))
                    arb_derivat->dr=arb1;   /// u'
                }
                break;
            }
        }
}


void transfCharInNumar(char elem[LMAX],double& nr,int& i)
{
    int gasit,p,negativ;
    i=gasit=negativ=0;
    nr=0;
    p=1;
    if(strchr(elem,'.'))
       {
           if(elem[0]=='-')
                {
                    i++;
                    negativ=1;
                }
           while(elem[i]!='\0')
            {
                if(elem[i]!='.')
                    nr=nr*10+(elem[i]-48);
                else
                    gasit=1;
                if(!strchr(elem+i,'.'))
                    p=p*10;
                i++;
            }
            nr=nr/p;
            if(negativ==1)
                nr=nr*(-1);
       }
    else
        {
            if(elem[0]=='-')
                {
                    i++;
                    negativ=1;
                }
            while(elem[i]!='\0')
            {
                nr=nr*10+(elem[i]-48);
                i++;
            }
            if(negativ==1)
                nr=nr*(-1);
        }
}

void transfNumarInChar(double nr,int nr_max_cifre,char elem[LMAX])
{
    snprintf(elem,nr_max_cifre+1,"%f",nr);
}

void simplificare(arbore& arb_simplificat)
{
    double nr1,nr2,rez;
    int i1,i2,nr_max_cifre;
    arbore arb,arb1,arb2;
    if (!esteArboreNul(arb_simplificat))
    {
        simplificare(arb_simplificat->st);
        simplificare(arb_simplificat->dr);
        switch(arb_simplificat->info[0])
        {
        case '+':
            {
                if(isdigit(arb_simplificat->st->info[0]) && isdigit(arb_simplificat->dr->info[0]))
                {
                    arb=new nod;
                    arb->st=NULL;
                    arb->dr=NULL;
                    transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                    transfCharInNumar(arb_simplificat->dr->info,nr2,i2);
                    rez=nr1+nr2;
                    transfNumarInChar(rez,i1+i2,arb->info);
                    arb_simplificat=arb;
                }
                else
                    if(strcmp(arb_simplificat->st->info,"0")==0)
                        arb_simplificat=arb_simplificat->dr;
                    else
                        if(strcmp(arb_simplificat->dr->info,"0")==0)
                            arb_simplificat=arb_simplificat->st;
                        else
                            if(isdigit(arb_simplificat->st->info[0]) && arb_simplificat->dr->info[0]=='+')
                            {
                                transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                                if(strchr("xscltae",arb_simplificat->dr->st->info[0]) && isdigit(arb_simplificat->dr->dr->info[0]))
                                {
                                    arb=new nod;
                                    arb->st=NULL;
                                    arb->dr=NULL;
                                    transfCharInNumar(arb_simplificat->dr->dr->info,nr2,i2);
                                    rez=nr1+nr2;
                                    transfNumarInChar(rez,i1+i2,arb->info);
                                    arb_simplificat->st=arb;
                                    arb_simplificat->dr=arb_simplificat->dr->st;
                                }
                                else
                                    if(isdigit(arb_simplificat->dr->st->info[0]) && strchr("xscltae",arb_simplificat->dr->dr->info[0]))
                                    {
                                        arb=new nod;
                                        arb->st=NULL;
                                        arb->dr=NULL;
                                        transfCharInNumar(arb_simplificat->dr->st->info,nr2,i2);
                                        rez=nr1+nr2;
                                        transfNumarInChar(rez,i1+i2,arb->info);
                                        arb_simplificat->st=arb;
                                        arb_simplificat->dr=arb_simplificat->dr->dr;
                                    }
                            }
                            else
                                if(arb_simplificat->st->info[0]=='+' && isdigit(arb_simplificat->dr->info[0]))
                                {
                                    transfCharInNumar(arb_simplificat->dr->info,nr1,i1);
                                    if(strchr("xscltae",arb_simplificat->st->st->info[0]) && isdigit(arb_simplificat->st->dr->info[0]))
                                    {
                                        arb=new nod;
                                        arb->st=NULL;
                                        arb->dr=NULL;
                                        transfCharInNumar(arb_simplificat->st->dr->info,nr2,i2);
                                        rez=nr1+nr2;
                                        transfNumarInChar(rez,i1+i2,arb->info);
                                        arb_simplificat->st=arb_simplificat->st->st;
                                        arb_simplificat->dr=arb;
                                    }
                                    else
                                        if(isdigit(arb_simplificat->st->st->info[0]) && strchr("xscltae",arb_simplificat->st->dr->info[0]))
                                        {
                                            arb=new nod;
                                            arb->st=NULL;
                                            arb->dr=NULL;
                                            transfCharInNumar(arb_simplificat->st->st->info,nr2,i2);
                                            rez=nr1+nr2;
                                            transfNumarInChar(rez,i1+i2,arb->info);
                                            arb_simplificat->st=arb_simplificat->st->dr;
                                            arb_simplificat->dr=arb;
                                        }
                                }
                break;
            }
        case '-':
            {
                if(strcmp(arb_simplificat->dr->info,"0")==0)
                    arb_simplificat=arb_simplificat->st;
                else if(isdigit(arb_simplificat->st->info[0]) && isdigit(arb_simplificat->dr->info[0]))
                {
                    nr_max_cifre=0;
                    arb=new nod;
                    arb->st=NULL;
                    arb->dr=NULL;
                    transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                    transfCharInNumar(arb_simplificat->dr->info,nr2,i2);
                    rez=nr1-nr2;
                    if(i1==i2)
                        if(nr1>=nr2)
                            nr_max_cifre=i1;
                        else
                            nr_max_cifre=i1+1;
                    else
                        if(i1>i2)
                            nr_max_cifre=max(i1,i2);
                        else
                            nr_max_cifre=i1+i2;
                    if(rez>=0)
                    {
                        transfNumarInChar(rez,nr_max_cifre,arb->info);
                    }
                    else
                    {
                        rez=rez*(-1);
                        arb1=new nod;
                        strcpy(arb1->info,"0");
                        arb1->st=NULL;
                        arb1->dr=NULL;
                        arb2=new nod;
                        transfNumarInChar(rez,nr_max_cifre-1,arb2->info);
                        arb2->st=NULL;
                        arb2->dr=NULL;
                        strcpy(arb->info,"-");
                        arb->st=arb1;
                        arb->dr=arb2;
                    }
                    arb_simplificat=arb;
                }
                else
                    if(arb_simplificat->st->info[0]=='x' && arb_simplificat->dr->info[0]=='x')
                        {
                            arb=new nod;
                            strcpy(arb->info,"0");
                            arb->st=NULL;
                            arb->dr=NULL;
                            arb_simplificat=arb;
                        }
                break;
            }
        case '*':
            {
                if(strcmp(arb_simplificat->st->info,"0")==0)
                    arb_simplificat=arb_simplificat->st;
                else
                    if(strcmp(arb_simplificat->dr->info,"0")==0)
                        arb_simplificat=arb_simplificat->dr;
                    else
                        if(strcmp(arb_simplificat->st->info,"1")==0)
                            arb_simplificat=arb_simplificat->dr;
                        else
                            if(strcmp(arb_simplificat->dr->info,"1")==0)
                                arb_simplificat=arb_simplificat->st;
                            else
                                if(isdigit(arb_simplificat->st->info[0]) && arb_simplificat->dr->info[0]=='*')
                                {
                                    transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                                    if(strchr("xscltae",arb_simplificat->dr->st->info[0]) && isdigit(arb_simplificat->dr->dr->info[0]))
                                    {
                                        arb=new nod;
                                        arb->st=NULL;
                                        arb->dr=NULL;
                                        transfCharInNumar(arb_simplificat->dr->dr->info,nr2,i2);
                                        rez=nr1*nr2;
                                        nr_max_cifre=i1+i2;
                                        if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                            nr_max_cifre=1;
                                        transfNumarInChar(rez,nr_max_cifre,arb->info);
                                        arb_simplificat->st=arb;
                                        arb_simplificat->dr=arb_simplificat->dr->st;
                                    }
                                    else
                                        if(isdigit(arb_simplificat->dr->st->info[0]) && strchr("xscltae",arb_simplificat->dr->dr->info[0]))
                                        {
                                            arb=new nod;
                                            arb->st=NULL;
                                            arb->dr=NULL;
                                            transfCharInNumar(arb_simplificat->dr->st->info,nr2,i2);
                                            rez=nr1*nr2;
                                            nr_max_cifre=i1+i2;
                                            if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                                nr_max_cifre=1;
                                            transfNumarInChar(rez,nr_max_cifre,arb->info);
                                            arb_simplificat->st=arb;
                                            arb_simplificat->dr=arb_simplificat->dr->dr;
                                        }
                                }
                                else
                                    if(arb_simplificat->st->info[0]=='*' && isdigit(arb_simplificat->dr->info[0]))
                                    {
                                        transfCharInNumar(arb_simplificat->dr->info,nr1,i1);
                                        if(strchr("xscltae",arb_simplificat->st->st->info[0]) && isdigit(arb_simplificat->st->dr->info[0]))
                                        {
                                            arb=new nod;
                                            arb->st=NULL;
                                            arb->dr=NULL;
                                            transfCharInNumar(arb_simplificat->st->dr->info,nr2,i2);
                                            rez=nr1*nr2;
                                            nr_max_cifre=i1+i2;
                                            if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                                nr_max_cifre=1;
                                            transfNumarInChar(rez,nr_max_cifre,arb->info);
                                            arb_simplificat->st=arb_simplificat->st->st;
                                            arb_simplificat->dr=arb;
                                        }
                                        else
                                            if(isdigit(arb_simplificat->st->st->info[0]) && strchr("xscltae",arb_simplificat->st->dr->info[0]))
                                            {
                                                arb=new nod;
                                                arb->st=NULL;
                                                arb->dr=NULL;
                                                transfCharInNumar(arb_simplificat->st->st->info,nr2,i2);
                                                rez=nr1*nr2;
                                                nr_max_cifre=i1+i2;
                                                if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                                        nr_max_cifre=1;
                                                transfNumarInChar(rez,nr_max_cifre,arb->info);
                                                arb_simplificat->st=arb_simplificat->st->dr;
                                                arb_simplificat->dr=arb;
                                            }
                                    }
                                    else
                                        if(isdigit(arb_simplificat->st->info[0]) && isdigit(arb_simplificat->dr->info[0]))
                                        {
                                            nr_max_cifre=0;
                                            arb=new nod;
                                            arb->st=NULL;
                                            arb->dr=NULL;
                                            transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                                            transfCharInNumar(arb_simplificat->dr->info,nr2,i2);
                                            rez=nr1*nr2;
                                            nr_max_cifre=i1+i2;
                                            if(rez>=0)
                                                {
                                                    if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                                        nr_max_cifre=1;
                                                    transfNumarInChar(rez,nr_max_cifre,arb->info);
                                                }
                                            else
                                            {
                                                rez=rez*(-1);
                                                arb1=new nod;
                                                strcpy(arb1->info,"0");
                                                arb1->st=NULL;
                                                arb1->dr=NULL;
                                                arb2=new nod;
                                                transfNumarInChar(rez,nr_max_cifre,arb2->info);
                                                arb2->st=NULL;
                                                arb2->dr=NULL;
                                                strcpy(arb->info,"-");
                                                arb->st=arb1;
                                                arb->dr=arb2;
                                            }
                                            arb_simplificat=arb;
                                        }
                                        else
                                            if(arb_simplificat->st->info[0]=='x')
                                                if(arb_simplificat->dr->info[0]=='/' && arb_simplificat->dr->dr->info[0]=='x')
                                                    arb_simplificat=arb_simplificat->dr->st;
                break;
            }
        case '/':
            {
                if(strcmp(arb_simplificat->st->info,"0")==0)
                    arb_simplificat=arb_simplificat->st;
                else
                    if(strcmp(arb_simplificat->dr->info,"1")==0)
                        arb_simplificat=arb_simplificat->st;
                    else
                        if(arb_simplificat->st->info[0]=='x' && arb_simplificat->dr->info[0]=='x')
                        {
                            arb=new nod;
                            strcpy(arb->info,"1");
                            arb->st=NULL;
                            arb->dr=NULL;
                            arb_simplificat=arb;
                        }
                        else
                            if(isdigit(arb_simplificat->st->info[0]) && isdigit(arb_simplificat->dr->info[0]))
                            {
                                nr_max_cifre=0;
                                arb1=new nod;
                                arb1->st=NULL;
                                arb1->dr=NULL;
                                transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                                transfCharInNumar(arb_simplificat->dr->info,nr2,i2);
                                rez=nr1/nr2;
                                nr_max_cifre=i1+i2;
                                transfNumarInChar(rez,nr_max_cifre,arb1->info);
                                arb_simplificat=arb1;
                            }
                break;
            }
        case '^':
            {
                arb1=new nod;
                strcpy(arb1->info,"1");
                arb1->st=NULL;
                arb1->dr=NULL;
                if(strcmp(arb_simplificat->st->info,"0")==0)
                    arb_simplificat=arb1;
                else
                    if(strcmp(arb_simplificat->dr->info,"0")==0)
                        arb_simplificat=arb1;
                    else
                        if(strcmp(arb_simplificat->st->info,"1")==0)
                            arb_simplificat=arb_simplificat->dr;
                        else
                            if(strcmp(arb_simplificat->dr->info,"1")==0)
                                arb_simplificat=arb_simplificat->st;
                        else
                            if(isdigit(arb_simplificat->st->info[0]) && isdigit(arb_simplificat->dr->info[0]))
                                {
                                    nr_max_cifre=0;
                                    arb=new nod;
                                    arb->st=NULL;
                                    arb->dr=NULL;
                                    transfCharInNumar(arb_simplificat->st->info,nr1,i1);
                                    transfCharInNumar(arb_simplificat->dr->info,nr2,i2);
                                    rez=pow(nr1,nr2);
                                    nr_max_cifre=2*(i1+i2);
                                    transfNumarInChar(rez,nr_max_cifre,arb->info);
                                    arb_simplificat=arb;
                                }
                            else
                                if(arb_simplificat->st->info[0]=='^' && isdigit(arb_simplificat->dr->info[0]))
                                {
                                    transfCharInNumar(arb_simplificat->dr->info,nr1,i1);
                                    if(strchr("xscltae",arb_simplificat->st->st->info[0]) && isdigit(arb_simplificat->st->dr->info[0]))
                                    {
                                        arb=new nod;
                                        arb->st=NULL;
                                        arb->dr=NULL;
                                        transfCharInNumar(arb_simplificat->st->dr->info,nr2,i2);
                                        rez=nr1*nr2;
                                        nr_max_cifre=i1+i2;
                                        if(rez>=0 && rez<=9 && i1==1 && i2==1)
                                                nr_max_cifre=1;
                                        transfNumarInChar(rez,nr_max_cifre,arb->info);
                                        arb_simplificat->st=arb_simplificat->st->st;
                                        arb_simplificat->dr=arb;
                                    }
                                }
                break;
            }
        case 'l':
            {
                arb1=new nod;
                strcpy(arb1->info,"0");
                arb1->st=NULL;
                arb1->dr=NULL;
                arb2=new nod;
                strcpy(arb2->info,"1");
                arb2->st=NULL;
                arb2->dr=NULL;
                if(strcmp(arb_simplificat->st->info,"1")==0)
                    arb_simplificat=arb1;
                else
                    if(arb_simplificat->st->info[0]=='e')
                        arb_simplificat=arb2;
                break;
            }
        }
    }
}




/// ---------------------------------------------------------

///                Functionalitate butoane

/// ---------------------------------------------------------



void citesteInput (char input[MAX], int &k)
{
    bool iesiCasetaInput=0;

    setcolor(COLOR(255, 128, 0));
    rectangle(100, 130, 500, 200);
    rectangle(102, 132, 498, 198);

    do
    {
        if (!kbhit( ))   ///daca apasam o tasta
        {
            if (avemInputFisier)
            {
                deseneazaMeniul();
                expresie[0]='\0';
                avemInputFisier=0;
            }

            setbkcolor(temaMod.meniubkg);
            setcolor(temaMod.culoaretxt);

            char c=(char)getch();   ///ii ia cosul ASCII si il transforma in caracter
            if (strchr(tastePermise, c))    ///daca este una din tastele permise (pt a scrie expresia - vezi vector de taste permise)
            {
                if (k<100)   ///putem continua sa scriem expresia atat timp cat nu depasim dimensiunea maxima
                {
                    k++;
                    input[k]=c;
                    input[k+1]='\0';
                    if (115+textwidth(input)<=485)
                    {
                        outtextxy(115, 157, input);
                    }
                    else
                    {
                        input[k]=' ';
                        input[k+1]=' ';
                        input[k+2]=' ';
                        input[k+3]='\0';
                        k--;
                    }
                }
            }
            if ((int)c==8)  ///apasam backspace
            {
                if (k>-1)   ///cat timp avem ce sterge
                {
                    input[k]=' ';
                    input[k+1]=' ';
                    input[k+2]=' ';
                    input[k+3]='\0';
                    k--;    ///actualizam lungimea inputului
                    outtextxy(115, 157, input);
                }
            }
            if ((int)c==13)
            {
                iesiCasetaInput=1;
            }
        }

    }while(iesiCasetaInput==0);

    setcolor(temaMod.culoarelin);
    rectangle(100, 130, 500, 200);
    setcolor(temaMod.meniubkg);
    rectangle(102, 132, 498, 198);
}

void golesteAfisare()
{
    setcolor(temaMod.culoarelin);
    rectangle(600, 0, 1200, 600);
    setbkcolor(temaMod.afisbkg);
    setfillstyle(SOLID_FILL, temaMod.afisbkg);
    floodfill(810, 310, temaMod.culoarelin);
    setcolor(temaMod.culoaretxt);
}

void animatieInputLung()
{
    setbkcolor(temaMod.meniubkg);
    setcolor(temaMod.culoaretxt);
    outtextxy(115, 157, textLimba[tipLimba][32]);
    for (int i=1; i<3; i++)
    {
        outtextxy(115+textwidth(textLimba[tipLimba][32]), 157, ".  ");
        delay(400);
        outtextxy(115+textwidth(textLimba[tipLimba][32]), 157, ".. ");
        delay(400);
        outtextxy(115+textwidth(textLimba[tipLimba][32]), 157, "...");
        delay(400);
    }
    outtextxy(115, 157, textLimba[tipLimba][33]);
    delay(1500);
    return;
}

void butonAles()
{
    char VECTOR[MAX][LMAX],exprPostordine[MAX], exprInordine[MAX], expr_derivata[MAX];
    exprPostordine[0]='\0';
    exprInordine[0]='\0';
    expr_derivata[0]='\0';
    int i,nr;
    punct p;

    if (ismouseclick(WM_LBUTTONDOWN))  ///dam click
    {
        clearmouseclick(WM_LBUTTONDOWN);
        p.x=mousex();
        p.y=mousey();
        bool valid=0;

        if(apartine(p,chenarFct.D))   ///introducere de la tastatura
        {
            valid=1;

            citesteInput(expresie, k);

            golesteAfisare();

            textLitMici(expresie);
            nrErori=verifExpresie(expresie, mesajErori, nrErori);  ///verif inputul e ok

            if (nrErori)   ///erori - afisam alerta
            {
                golesteAfisare();
                alertaErori(mesajErori, nrErori);

                ///input invalid - animatie
                setcolor(COLOR(255, 0, 127));
                rectangle(100, 130, 500, 200);
                rectangle(102, 132, 498, 198);

                delay(1500);

                setcolor(temaMod.culoarelin);
                rectangle(100, 130, 500, 200);
                setcolor(temaMod.meniubkg);
                rectangle(102, 132, 498, 198);

                return;
            }
            else    ///arbore expresie
            {
                ///expresie valida - animatie
                setcolor(COLOR(128, 255, 0));
                rectangle(100, 130, 500, 200);
                rectangle(102, 132, 498, 198);

                delay(1500);

                setcolor(temaMod.culoarelin);
                rectangle(100, 130, 500, 200);
                setcolor(temaMod.meniubkg);
                rectangle(102, 132, 498, 198);

                ///vectorizare & arborizare
                nr = 0;
                strcat(expresie,")");
                vectorizare(expresie, VECTOR, nr);
                arborizare(VECTOR,nr,arb);
                int k=strlen(expresie);
                expresie[k-1]='\0';

                return;
            }
        }

        if(apartine(p,butonFis.D))   ///citire din fisier
        {
            valid=1;

            expresie[0]='\0';
            FILE *fptr;

            if ((fptr = fopen("./Input_fisier.txt", "r")) == NULL)
            {
                printf("Error! opening file");
                exit(1);
            }
            fscanf(fptr, "%s", &exprFisier);
            fclose(fptr);

            if (textwidth(exprFisier)<=370)   ///115+textwidth(input)<=485  -  daca inputul nu e prea lung
            {
                strcpy(expresie, exprFisier);

                avemInputFisier=1;

                setbkcolor(temaMod.meniubkg);
                setcolor(temaMod.culoaretxt);
                outtextxy(115, 157, expresie);   ///o scriem pe spatiul de input (sa o vada utilizatorul)

                textLitMici(expresie);

                nrErori=verifExpresie(expresie, mesajErori, nrErori);  ///verif daca are erori

                if (nrErori)   ///erori - afisam alerta
                {
                    golesteAfisare();
                    alertaErori(mesajErori, nrErori);
                    return;
                }
                else   ///arbore expresie
                {
                    nr = 0;
                    strcat(expresie,")");
                    vectorizare(expresie, VECTOR, nr);

                    arborizare(VECTOR,nr,arb);
                    int k=strlen(expresie);
                    expresie[k-1]='\0';
                }
            }
            else   ///daca e prea lung inputul, nu il acceptamm
            {
                deseneazaMeniul();
                animatieInputLung();
                deseneazaMeniul();
                if(stareReguli)
                {
                    reguli();
                }
            }
            return;
        }

        for (i=1; i<3; i++)
        {
            if(apartine(p,mod[i].D))  ///change mode
            {
                valid=1;

                tipMod=i;
                schimbaMod(tipMod, temaMod);
                deseneazaMeniul();
                if (stareReguli)
                {
                    reguli();
                }
                if (expresie)
                {
                    setbkcolor(temaMod.meniubkg);
                    outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                }
                return;
            }

            else if(apartine(p,limba[i].D))  ///schimba mode
            {
                valid=1;

                tipLimba=i-1;

                deseneazaMeniul();
                if (stareReguli)
                {
                    reguli();
                }
                if (expresie)
                {
                    setbkcolor(temaMod.meniubkg);
                    outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                }
                return;
            }
        }

        if (apartine(p, butonReguli.D))
        {
            if (!stareReguli)
            {
                stareReguli=1;
                deseneazaMeniul();
                reguli();
            }
            else
            {
                stareReguli=0;
                deseneazaMeniul();
            }
        }

        if (apartine(p, butonIesire.D))
        {
            comanda=-1;
            return;
        }

        for (i=1; i<=5; i++)
        {
            if(apartine(p,optiuni[i].D) && expresie[0]!='\0')  ///butoanele funtioneaza doar daca avem input
            {
                if (expresie[0]!='\0' && nrErori==0)
                {
                    valid=1;

                    stareReguli=0;

                    switch(i)
                    {
                        case 1:
                        {
                            deseneazaMeniul();
                            if (expresie)
                            {
                                setbkcolor(temaMod.meniubkg);
                                outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                            }
                            setbkcolor(temaMod.afisbkg);

                            derivare(arb,arb_derivat);
                            arb_simplificat=new nod;
                            arb_simplificat=arb_derivat;
                            simplificare(arb_simplificat);

                            derivare(arb_simplificat, arb_deriv2);
                            arb_deriv2_simplif=new nod;
                            arb_deriv2_simplif=arb_deriv2;
                            simplificare(arb_deriv2_simplif);

                            parcurgereInInordine(arb_deriv2_simplif, exprInordine, 1);

                            if (textwidth(exprInordine)<400)
                            {
                                outtextxy(600 + (600-textwidth(textLimba[tipLimba][17]))/2, 280, textLimba[tipLimba][17]);
                                outtextxy(600 + (600-textwidth(exprInordine))/2, 320, exprInordine);
                            }
                            else
                            {
                                char exprInordine1[500], exprInordine2[500], k1=0, k2=0;
                                exprInordine1[0]='\0';
                                exprInordine2[0]='\0';

                                for (int i=0; i<strlen(exprInordine)/2; i++)
                                {
                                    exprInordine1[k1]=exprInordine[i];
                                    exprInordine1[k1+1]='\0';
                                    k1++;
                                }
                                for (int i=strlen(exprInordine)/2; i<strlen(exprInordine); i++)
                                {
                                    exprInordine2[k2]=exprInordine[i];
                                    exprInordine2[k2+1]='\0';
                                    k2++;
                                }

                                outtextxy(600 + (600-textwidth(textLimba[tipLimba][17]))/2, 260, textLimba[tipLimba][17]);
                                outtextxy(600 + (600-textwidth(exprInordine1))/2, 315, exprInordine1);
                                outtextxy(600 + (600-textwidth(exprInordine2))/2, 340, exprInordine2);
                            }

                            settextjustify(LEFT_TEXT,TOP_TEXT);
                            break;
                        }
                        case 2:
                        {
                            deseneazaMeniul();
                            if (expresie)
                            {
                                setbkcolor(temaMod.meniubkg);
                                outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                            }
                            setbkcolor(temaMod.afisbkg);

                            derivare(arb,arb_derivat);
                            arb_simplificat=new nod;
                            arb_simplificat=arb_derivat;
                            simplificare(arb_simplificat);

                            parcurgereInInordine(arb_simplificat, exprInordine, 1);

                            if (textwidth(exprInordine)<400)
                            {
                                outtextxy(600 + (600-textwidth(textLimba[tipLimba][17]))/2, 280, textLimba[tipLimba][17]);
                                outtextxy(600 + (600-textwidth(exprInordine))/2, 320, exprInordine);
                            }
                            else
                            {
                                char exprInordine1[500], exprInordine2[500], k1=0, k2=0;
                                exprInordine1[0]='\0';
                                exprInordine2[0]='\0';

                                for (int i=0; i<strlen(exprInordine)/2; i++)
                                {
                                    exprInordine1[k1]=exprInordine[i];
                                    exprInordine1[k1+1]='\0';
                                    k1++;
                                }
                                for (int i=strlen(exprInordine)/2; i<strlen(exprInordine); i++)
                                {
                                    exprInordine2[k2]=exprInordine[i];
                                    exprInordine2[k2+1]='\0';
                                    k2++;
                                }

                                outtextxy(600 + (600-textwidth(textLimba[tipLimba][17]))/2, 260, textLimba[tipLimba][17]);
                                outtextxy(600 + (600-textwidth(exprInordine1))/2, 315, exprInordine1);
                                outtextxy(600 + (600-textwidth(exprInordine2))/2, 340, exprInordine2);
                            }

                            settextjustify(LEFT_TEXT,TOP_TEXT);
                            break;
                        }
                        case 3:
                        {
                            deseneazaMeniul();
                            if (expresie)
                            {
                                setbkcolor(temaMod.meniubkg);
                                outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                            }
                            setbkcolor(temaMod.afisbkg);

                            outtextxy(620,20,textLimba[tipLimba][30]);

                            derivare(arb,arb_derivat);
                            arb_simplificat=new nod;
                            arb_simplificat=arb_derivat;
                            simplificare(arb_simplificat);

                            int latime, inaltime;
                            latime=600/(1+nrColoane(arb_simplificat));
                            inaltime=600/nrNiveluri(arb_simplificat);
                            desenareArbore(arb_simplificat,1,600,latime,inaltime);

                            settextjustify(LEFT_TEXT,TOP_TEXT);

                            break;
                        }
                        case 4:
                        {
                            deseneazaMeniul();
                            if (expresie)
                            {
                                setbkcolor(temaMod.meniubkg);
                                outtextxy(115, 157, expresie);   ///daca era vreun input introdus, nu vrem sa il pierdem (gen sa il vada utilizatorul)
                            }
                            setbkcolor(temaMod.afisbkg);

                            parcurgereInPostordine(arb,exprPostordine);
                            exprPostordine[strlen(exprPostordine)-2]='\0';
                            outtextxy(620,20,textLimba[tipLimba][31]);
                            outtextxy(620,45,exprPostordine);

                            int latime, inaltime;
                            latime=600/(1+nrColoane(arb));
                            inaltime=600/nrNiveluri(arb);
                            desenareArbore(arb,1,600,latime,inaltime);

                            settextjustify(LEFT_TEXT,TOP_TEXT);
                            break;
                        }
                    }
                    return;
                }
                else
                {
                    return;
                }
            }
        }
        if (!valid)
        {
            return;
        }
    }

    setbkcolor(temaMod.afisbkg);
}



/// -------------------------------------

///                  Main

/// -------------------------------------


int main()
{
    initwindow(1200,600);

    limbi();
    deseneazaMeniul();
    golesteAfisare();

    ///Initializare input NULL
    k=-1;
    expresie[0]='\0';


    do
    {
        butonAles();
        if (comanda==-1)
        {
            closegraph();
            return 0;
        }
    }
    while (comanda!=-1);


    getch();
    closegraph();
    return 0;
}









