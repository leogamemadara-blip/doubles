#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 char T[N][20];
    int r=0;
    for(int i=0;i<N;i++){
            printf("arret %d",i);
            gets(T[i++]);
          printf("voulez inserer une autre arret");
          scanf("%d",&r);
          getchar();
while(r==0 && i<N){
printf("arret %d",i);
            gets(T[i++]);
}
if(r!=0)
    break;
}
//afiicher
 for(int i=0;i<N;i++){
            printf("arret %d",i);
            puts(T[i++]);

}
*/
typedef struct routes routes;
typedef routes *liste;
typedef struct routes{
int id;
int bus_id;
char T[200];
int disponible;
//int nbre_etudiants;
liste next;
liste previous;
}routes;
/*************************************************/
typedef struct DLIST{
 int length;
 liste beg;
 liste end;
}*DOUBLE;
/****************************************/
/*************************************************/
// je vais travailler avec LSL
typedef struct buses buses;
typedef buses *next;
typedef struct buses{
int bus_id;
char Matricule[100];
int capacite;
int disponible;
int taille;
int nbre_etudiants;                /*********/
next suiv;
}buses;
next insererT(next l,int v,char T[],int capa){
next p;
p=malloc(sizeof(buses));
p->bus_id =v;
p->disponible=1;
strcpy(p->Matricule,T);
p->capacite=capa;
p->suiv=l;
p->nbre_etudiants=0;                     /*********/
if(l==NULL){
p->taille=0;/***/
}
p->taille++;
return p;
}

void afficherREC(next l){
    if(l!=NULL){
 printf("le bus d'id %d a la capcaite de %d eleve  ",l->bus_id,l->capacite);
 printf("son matricule est : ");
 puts(l->Matricule);
 afficherREC(l->suiv);}
}

next insererF(next l,int v,char T[],int capa){
    if(l==NULL)
        return insererT(l,v,T,capa);
    else {
next p,q;
p=malloc(sizeof(buses));
p->bus_id =v;
p->disponible=1;/*********/
strcpy(p->Matricule,T);
p->capacite=capa;
p->nbre_etudiants=0;                    /*********/
p->suiv=NULL;
q=l;
while((q->suiv)!=NULL){
    q=q->suiv;
}
l->taille++;/***/
q->suiv=p;
return l;
    }
}
/********************Y-a-t-il de bus DISPONIBLE********************************/
int buses_disponible(next L){
next p=L;
while(p!=NULL){
    if(p->disponible==1)
        return 1;
    p=p->suiv;
}
return 0;
}

/********************BUS DISPONIBLE********************************/
int disponible(next L,int v){
next p=L;
while(p!=NULL){
        if(p->bus_id==v){
    if(p->disponible==1)
        return 1;
        }
    p=p->suiv;
}
return 0;
}
/********************MARQUE BUS NON DISPONIBLE********************************/
void marquer_nondis(next L,int v){
next p=L;
while(p!=NULL){
        if(p->bus_id==v){
    p->disponible=0;
        break;
        }
    p=p->suiv;
}
//return L;
}
/***********************BUS--EXISTE*******************************/
int bus_existe(next L,int v){
 next p=L;
while(p!=NULL){
    if(p->bus_id==v)
        return 1;
    p=p->suiv;
}
return 0;
}

/**RETOURNER LISTE VIDE**/
DOUBLE newdlisrt(void){
 return NULL;
}
/****************************************/
/**LISTE VIDE OU PAS**/
int estvide(DOUBLE L){
 return L==NULL;
}
/****************************************/
/**RECUPERER LA LONGUEUR**/
int longueur(DOUBLE L){
return L->length;
}
/****************************************/
/**AFFICHAGE**/
void afficher(DOUBLE L){
 if(estvide(L))
    printf("LISTE VIDEEEEE aucune route");
 else{
        int i=1;
    liste q=L->beg;
    while(q!=NULL){
            if(q->disponible){
            if(q->bus_id)
        printf("%d- l'tineraire du bus %d de la route %d a un itinairere : ",i++,q->bus_id,q->id);
        else
         printf("%d- pas de bus pour la route %d qui a un itinairere : ",i++,q->id);
        puts(q->T);
        q=q->next;
    }
    else
    q=q->next;
 }
}
}
/****************************************/
/**AFFICHAGE INVERSE**/
void afficherinverse(DOUBLE L){
 if(estvide(L))
    printf("LISTE VIDEEEEE");
 else{
    liste q=L->end;
    while(q!=NULL){
        printf("la route %d du bus %d a un itinairere : ",q->id,q->bus_id);
        puts(q->T);
        q=q->previous;
    }
 }
}
/****************************************/
DOUBLE insererFin(DOUBLE L, int v,int bus,char T[]){
 if(estvide(L)){
       //  L=malloc(sizeof(struct DLIST));
       /**************************/
    L=malloc(sizeof(*L));
    L->end=NULL;
    L->beg=NULL;
    liste p=malloc(sizeof(routes));
    p->id=v;
    p->bus_id=bus;
    p->disponible=1;
     strcpy(p->T,T);
    p->next=NULL;
   p->previous=L->end;
    L->beg=p;
    L->end=p;
    L->length=0;

 }
else{
          liste p=malloc(sizeof(routes));
     p->id=v;
    p->bus_id=bus;
    p->disponible=1;
  strcpy(p->T,T);
    p->previous=L->end;
    p->next=NULL;
    L->end->next=p;
    L->end=p;

}
 L->length++;
return L;
}
/**********************************************************/
void clear_screen() {
    system("cls");
}
/************************SAISIE DE BUS***************************************/
next saisie_buses(){
  int n;
printf("combien de bus ? : ");
scanf("%d",&n);
    next L=NULL;
    char MA[100],r='o';
    int c=0;//pour ne depasse pas le nbre de bus
    //int t=1;//qaund on dit on veut inserer n bus mais on insere aucun on tombe b inf/****/
    int capacite,bus_id;
    while(c<n && r=='o'){
    printf("voulez inserer un bus (o/n) : ");
    scanf(" %c",&r);
    if(r=='o'){
             do{
    printf("le bus id : ");
   scanf("%d",&bus_id);
   if(bus_id==0)
    printf("bus id doit etre diff de 0\n");
             }while(bus_id==0);
    //BUSES_ID[i++]=bus_id;
   printf("donner la capacite du bus  : ");
   scanf("%d",&capacite);
   getchar();
    printf("la matricule : ");
   gets(MA);
         L=insererF(L,bus_id,MA,capacite);
     c++;
     if(c<n){/***/
    do{

    printf("voulez inserer un autre bus (o/n) : ");
    scanf(" %c",&r);
        if(r=='o'){
    printf("le bus id : ");
   scanf("%d",&bus_id);
  // t++;
   printf("donner la capacite du bus  : ");
   scanf("%d",&capacite);
   getchar();
    printf("la matricule : ");
   gets(MA);
        L=insererF(L,bus_id,MA,capacite);
     c++;
            }
    }while(r=='o' && c<n);
     }/***/
    }
    else {
        //t=0;
     break;
    }
   // }
    }
    return L;
}
/*******************SAISE ROUTES***************************/
DOUBLE saisie_routes(next L,int c){
    int i=0;
    while(i<c){
char r;
    printf("passons aux routes\n");
    DOUBLE R=NULL;
    char T[100];
    int id,bus_idr;
    printf("voulez inserer une route (o/n) : ");
    scanf(" %c",&r);
    if(r=='o'){
    printf("donner l'id de la route : ");
   scanf("%d",&id);
   bus_idr=0;
   int v=0;
if(buses_disponible(L) && i<c){
   do{                                            /****/
  printf("donner le bus id : ");
   scanf("%d",&bus_idr);
 if(bus_existe(L,bus_idr)){
        if(disponible(L,bus_idr)){
        marquer_nondis(L,bus_idr);
       v=1;
       i++;
        }
       else{
        printf("deja ocuupe\n");
       }
 }
else{
    printf("le buses id n'existe pas\n");
}
   }while(v==0 &&  i<c);
}
//else
  //  printf("pas de bus disponible\n");
   getchar();
    printf("l'itineraire : ");
   gets(T);
     R=insererFin(R,id,bus_idr,T);
/****************************************/

    do{
            if(i<c){
    printf("voulez inserer une autre route (o/n) : ");
    scanf(" %c",&r);
        if(r=='o' && i<c){
   printf("donner l'id de la route : ");
   scanf("%d",&id);
    bus_idr=0;
  int v=0;
   if(buses_disponible(L) && i<c){
   do{                                            /****/
   printf("donner le bus id : ");
   scanf("%d",&bus_idr);
 if(bus_existe(L,bus_idr)){
        if(disponible(L,bus_idr)){
        marquer_nondis(L,bus_idr);
       v=1;
       i++;
        }
       else{
        printf("deja ocuupe\n");
       }
 }
else{
    printf("le buses id n'existe pas\n");
}
   }while(v==0 && i<c);
   }
else
    printf("pas de bus disponible\n");
   getchar();
    printf("l'itineraire : ");
   gets(T);
        R=insererFin(R,id,bus_idr,T);
        }
    }
    }while(r=='o'  && i<c);
        }
      return R;
    }

}

/********************************************************************/
int main()
{
next L=saisie_buses();
    clear_screen();
    if(L){
    printf("les %d buses disponibles : \n",L->taille);
    afficherREC(L);
    }
    else
        printf("pas de buses disponible donc on peut pas assigne les etudinats\n");
DOUBLE Y=saisie_routes(L,L->taille);
clear_screen();

    afficher(Y);



    return 0;
}
