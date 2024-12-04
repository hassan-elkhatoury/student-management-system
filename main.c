#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define black           0
#define blue            1
#define green           2
#define cyan            3  
#define red             4
#define purple          5
#define yellow          6
#define white           7
#define gray            8
#define light_blue      9
#define light_green     10
#define light_aqua      11
#define light_red       12
#define light_purple    13
#define light_yellow    14
#define bright_white    15
#define BOLD        "\033[1m"
#define ITALIC      "\033[3m"
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"


void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
typedef struct Options {
    char **ops;
    int len;
    char title[100];
} Options;

int visible_length(const char *text) {
    int len = 0;
    int in_escape = 0;
    
    while (*text) {
        if (*text == '\033') {  
            in_escape = 1;
        } else if (in_escape && *text == 'm') {  
            in_escape = 0;
        } else if (!in_escape) {
            len++;  
        }
        text++;
    }
    return len;
}

void print_centered(const char *text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    int console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int text_len = visible_length(text);

    int padding = (console_width - text_len) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

void draw(int op, Options options) {
    system("cls"); 
    char line[110];
    snprintf(line, sizeof(line), BOLD "%s\n", options.title);
    print_centered(line);
    for (int i = 0; i < options.len; i++) { 
        sprintf(line, ITALIC "%s%s%s" RESET, op == i ? BLUE "[" : WHITE " ", options.ops[i], op == i ? BLUE "]" : WHITE " ");
        print_centered(line);  
    }
}

int select_menu(Options options) {
    int current_option = 0;
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);  
    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    draw(current_option, options);

    INPUT_RECORD input;
    DWORD events;
    while (1) {
        ReadConsoleInput(hInput, &input, 1, &events);  // Read input from console
        if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown) {
            switch (input.Event.KeyEvent.wVirtualKeyCode) {
                case VK_UP:
                    if (current_option > 0) {
                        current_option--;
                        draw(current_option, options);
                    }
                    break;
                case VK_DOWN:
                    if (current_option < options.len-1) {
                        current_option++;
                        draw(current_option, options);
                    }
                    break;
                case VK_RETURN:  
                    return current_option;
            }
        }
    }
}
typedef struct 
{
    int code;
    char nom[50], prenom[100];
    float noteE, noteO,moy;  
} etu;

void Nouvel_etudiant(){
    etu etu1;
    FILE *fetu;
    int code;
    system("cls");
    setColor(cyan);
    printf("Entrer le code de l'etudiant : ");
    if (scanf("%d", &etu1.code) != 1) {
        printf("Erreur de saisie du code.\n");
        return;
    }
    fetu=fopen("etudients.txt","rt");
    if(fetu==NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return;
        }
    while(fscanf(fetu,"%d\t%s\t%s\t%f\t%f\t%f\n", &code,etu1.nom, etu1.prenom,&etu1.noteE,&etu1.noteO,&etu1.moy)==6){
        if(code==etu1.code){
            printf(RED BOLD "L'etudiant existe deja.!!!\n" RESET);
            return;
        }
    }
    fclose(fetu);
    printf("Entrer le nom de l'etudiant : ");
    if (scanf("%49s", etu1.nom) != 1) {
        printf("Erreur de saisie du nom.\n");
        return;
    }
    printf("Entrer le prenom de l'etudiant : ");
    if (scanf("%99s", etu1.prenom) != 1) {
        printf("Erreur de saisie du prenom.\n");
        return;
    }
    printf("Entrer la note de l'etudiant en examen : ");
    if (scanf("%f", &etu1.noteE) != 1) {
        printf("Erreur de saisie de la note d'examen.\n");
        return;
    }
    printf("Entrer la note de l'etudiant en oral : ");
    if (scanf("%f", &etu1.noteO) != 1) {
        printf("Erreur de saisie de la note d'oral.\n");
        return;
    }
    fetu=fopen("etudients.txt","a+");
    if(fetu==NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return;
        }
    etu1.moy=(etu1.noteO+etu1.noteE)/2;
    fprintf(fetu, "%d\t%s\t%s\t%.2f\t%.2f\t%.2f\n", etu1.code, etu1.nom, etu1.prenom, etu1.noteE, etu1.noteO,etu1.moy);
    fclose(fetu);

    setColor(green);
    printf("Etudiant ajoute avec succes !!\n"RESET);
}
void Bulletin(){
    int ecode, trouve = 0;
    etu etu1;
    FILE *fetu ;
    fetu = fopen("etudients.txt", "rt");
    if(fetu == NULL){
        setColor(red);
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    setColor(cyan);
    printf("Entrer le code de l'etudiant : ");
    if (scanf("%d", &ecode) != 1) {
        setColor(red);
        printf("Erreur de saisie du code.\n");
        fclose(fetu);
        return;
    }
    rewind(fetu); 
    while(fscanf(fetu, "%d\t%s\t%s\t%f\t%f\t%f\n", &etu1.code, etu1.nom, etu1.prenom, &etu1.noteE,&etu1.noteO,&etu1.moy)==6){
        if(ecode == etu1.code){
            setColor(green);
            printf("Nom : %s\n", etu1.nom);
            printf("Prenom : %s\n", etu1.prenom);
            printf("note d'examen : %.2f\n", etu1.noteE);
            printf("note d'oral : %.2f\n", etu1.noteO);
            printf("moyenne : %.2f\n"RESET, etu1.moy);
            printf("\n");
            trouve = 1;
            break;
        }
    }
    if(trouve!=1){
        setColor(red);
        printf("etudient n'existe pas"RESET);
        printf("\n");
    }
    fclose(fetu);
}
void Moy_classe(){
FILE *fetu;
etu etu1;
float Sclasse=0,moyclasse=0,tr=0;
system("cls"); 
fetu = fopen("etudients.txt", "rt");
    if(fetu == NULL){
        printf("error");
        return;
    }
    while(fscanf(fetu, "%d\t%s\t%s\t%f\t%f\t%f\n",&etu1.code, etu1.nom, etu1.prenom, &etu1.noteE, &etu1.noteO,&etu1.moy)==6){
        Sclasse=Sclasse+etu1.moy;
        tr++;
    }
    if (tr==0)
        printf(RED"Aucun etudient dans le fichier\n");
    else{
    moyclasse=Sclasse/tr;
    printf( YELLOW BOLD"********************************\n");
    printf("* "GREEN ITALIC"Moyeene du classe est : %.2f"YELLOW BOLD" *\n", moyclasse);
    printf("********************************\n"RESET);
        }
        fclose(fetu);
}
void Liste_admis(){
    FILE *fetu;
    etu etu1;
    int tr=0,moy;
    system("cls"); 
    fetu = fopen("etudients.txt", "rt");
    if (fetu == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
        }
    
            rewind(fetu); 
            printf(YELLOW BOLD"*************************\n");
            printf("* "GREEN BOLD ITALIC "Etudiant admis sont : "YELLOW BOLD"*\n");
            printf("*************************\n"RESET);

    while(fscanf(fetu, "%d\t%s\t%s\t%f\t%f\t%f\n", &etu1.code, etu1.nom, etu1.prenom, &etu1.noteE, &etu1.noteO,&etu1.moy)==6){
        if(etu1.moy>=10){
            setColor(green);
            tr++;
            printf("code d'etudent %d : %d\n",tr,etu1.code);
            printf("Nom d'etudent %d : %s\n",tr,etu1.nom);
            printf("Prenom d'etudent %d : %s\n",tr,etu1.prenom);
            printf("note d'examen d'etudent %d : %.2f\n",tr,etu1.noteE);
            printf("note d'oral d'etudent %d : %.2f\n",tr,etu1.noteO);
            printf("moyenne d'etudent %d : %.2f\n",tr,etu1.moy);
            printf(YELLOW BOLD"********************************\n"RESET);
            }
    }
            if (tr==0)
        printf(RED"Aucun etudient dans le fichier\n");
        fclose(fetu);
}

void Supprimer_etu(){
 FILE *fetu,*ftemp;
 etu etu1;
 int tr=0,trr=0,code;
 fetu = fopen("etudients.txt", "rt");
 if (fetu == NULL) {
    printf("Erreur d'ouverture du fichier.\n");
    return;
    }
    ftemp = fopen("temp.txt", "wt");
    if (ftemp == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
        }
        printf(CYAN "Entrer le code d'etudient que vous voulez supprimer: ");
        scanf("%d",&code);
        rewind(fetu);
    while(fscanf(fetu, "%d\t%s\t%s\t%f\t%f\t%f\n", &etu1.code, etu1.nom, etu1.prenom, &etu1.noteE, &etu1.noteO,&etu1.moy)==6){
        trr++;
        if(code!=etu1.code){
            fprintf(ftemp, "%d\t%s\t%s\t%.2f\t%.2f\t%.2f\n",etu1.code,etu1.nom,etu1.prenom,etu1.noteE,etu1.noteO,etu1.moy);
            tr++;
        }
    }
    if(tr==0 && trr==0){
        printf(RED"Aucun etudient dans le fichier\n");
    }
    else{
    if(tr==trr){
        printf(RED"le code d'etudient n'existe pas\n");
    }
    else{
    printf(GREEN"Etudient supprimer avec succes\n"RESET);
    }
    }
    fclose(fetu);
    fclose(ftemp);
    remove("etudients.txt");
    rename("temp.txt", "etudients.txt");
}
void Modifier_etu(){
 FILE *fetu,*ftemp;
 etu etu1;
 int tr=0,trr=0,trrr=0,code;
 fetu = fopen("etudients.txt", "rt");
 if (fetu == NULL) {
    printf("Erreur d'ouverture du fichier.\n");
    return;
    }
    ftemp = fopen("temp.txt", "wt");
    if (ftemp == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
        }
        printf(CYAN "Entrer le code d'etudient que vous voulez modifier: ");
        scanf("%d",&code);
        rewind(fetu);
    while(fscanf(fetu, "%d\t%s\t%s\t%f\t%f\t%f\n", &etu1.code, etu1.nom, etu1.prenom, &etu1.noteE, &etu1.noteO,&etu1.moy)==6){
        trr++;
        if(code!=etu1.code){
            tr++;
        }
        else{
            printf(CYAN "Entrer le nouveau nom: ");
            scanf("%s",&etu1.nom);
            trrr++;
        }
    fprintf(ftemp, "%d\t%s\t%s\t%.2f\t%.2f\t%.2f\n",etu1.code,etu1.nom,etu1.prenom,etu1.noteE,etu1.noteO,etu1.moy);

    }
    if(tr==0 && trr==0){
        printf(RED"Aucun etudient dans le fichier\n"RESET);
    }
    else{
    if(tr==trr){
        printf(RED"le code d'etudient n'existe pas\n"RESET);
    }
    else if (trrr!=0){
    printf(GREEN"Etudient modifier avec succes\n"RESET);
    }
    }
    fclose(fetu);
    fclose(ftemp);
    remove("etudients.txt");
    rename("temp.txt", "etudients.txt");
}    

int main() {
    int choix;
    do {
    Options options;
    char *LOGIN_OPTIONS[] = {
        BOLD "Nouvel etudiant",
        BOLD "Afficher Bulletin",
        BOLD "Modifier le nom d'un etudient",
        BOLD "Supprimer un etudient",
        BOLD "Afficher la Moyenne de la classe",
        BOLD "Liste des Admis",
       RED BOLD "Quitter le programme"
    };
    strcpy(options.title, YELLOW "Menu Gestion Des Notes");
    options.ops = LOGIN_OPTIONS;
    options.len = sizeof(LOGIN_OPTIONS) / sizeof(LOGIN_OPTIONS[0]);

    int option = select_menu(options);
    switch (option) {
            case 0:
                Nouvel_etudiant();
                break;
            case 1:
                Bulletin();
                break;
            case 2:
                Modifier_etu();
                break;
            case 3:
                Supprimer_etu();
                break;
            case 4:
                Moy_classe();
                break;
            case 5:
                Liste_admis();
                break;
            case 6:
                setColor(green);
                printf("Fin du programme.\n"RESET);
                break;
            default:
                setColor(red);
                printf("Choix non valide.\n");
        }
        if (choix != 0) {
            system("pause");
        }
    } while (choix != 0);
    return 0;
}
