#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
#include <time.h>

void my_sleep(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * (CLOCKS_PER_SEC / 1000));
}

/* ========================== STRUCTURES ========================== */

typedef struct routes routes;
typedef routes *liste;
struct routes {
    int id, bus_id, disponible;
    char T[200];
    liste next, previous;
};

typedef struct DLIST {
    int length;
    liste beg, end;
} *DOUBLE_list;

typedef struct buses buses;
typedef buses *next;
struct buses {
    int bus_id, capacite, disponible, taille, nbre_etudiants;
    char Matricule[100];
    next suiv;
};

typedef struct students students;
typedef students *next_student;
struct students {
    int student_id, route_id, bus;
    next_student suiv;
};

typedef struct presence presence;
typedef presence *point;
struct presence {
    int student_id, present;
    char date[11];
    point suiv;
};

/* ===================== FONCTIONS BUSES ===================== */

next insererT(next l,int v,char T[],int capa){
    next p = malloc(sizeof(buses));
    p->bus_id=v;
    p->disponible=1;
    strcpy(p->Matricule,T);
    p->capacite=capa;
    p->nbre_etudiants=0;
    p->suiv=l;
    p->taille = (l==NULL ? 1 : l->taille+1);
    return p;
}

next insererF(next l,int v,char T[],int capa){
    if(l==NULL) return insererT(l,v,T,capa);

    next p = malloc(sizeof(buses)), q=l;
    p->bus_id=v;
    p->disponible=1;
    strcpy(p->Matricule,T);
    p->capacite=capa;
    p->nbre_etudiants=0;
    p->suiv=NULL;

    while(q->suiv) q=q->suiv;
    q->suiv=p;
    l->taille++;
    return l;
}

void afficherREC(next l){
    if(l){
        printf("Bus %d | capacite %d | matricule: ", l->bus_id, l->capacite);
        puts(l->Matricule);
        afficherREC(l->suiv);
    }
}

int buses_disponible(next p){
    while(p){
        if(p->disponible) return 1;
        p=p->suiv;
    }
    return 0;
}

int bus_existe(next p,int v){
    while(p){
        if(p->bus_id==v) return 1;
        p=p->suiv;
    }
    return 0;
}

int disponible(next p,int v){
    while(p){
        if(p->bus_id==v && p->disponible) return 1;
        p=p->suiv;
    }
    return 0;
}

void marquer_nondis(next p,int v){
    while(p){
        if(p->bus_id==v){ p->disponible=0; return; }
        p=p->suiv;
    }
}

/* ===================== ROUTES ===================== */

DOUBLE_list insererFin(DOUBLE_list L, int v,int bus,char T[]){
    if(!L){
        L = malloc(sizeof(*L));
        L->length=0;
        L->beg=L->end=NULL;
    }

    liste p = malloc(sizeof(routes));
    p->id=v;
    p->bus_id=bus;
    p->disponible=1;
    strcpy(p->T,T);
    p->next=NULL;
    p->previous=L->end;

    if(L->end) L->end->next=p;
    else L->beg=p;

    L->end=p;
    L->length++;
    return L;
}

void afficher(DOUBLE_list L){
    if(!L){ printf("LISTE VIDE\n"); return; }

    int i=1;
    for(liste q=L->beg; q; q=q->next)
        if(q->disponible){
            printf("%d- Route %d | Bus %d | itineraire: ", i++, q->id, q->bus_id);
            puts(q->T);
        }
}

int route_existe(DOUBLE_list L,int v){
    if(!L) return 0;
    for(liste p=L->beg; p; p=p->next)
        if(p->id==v) return 1;
    return 0;
}

int routes_disponible(DOUBLE_list L){
    if(!L) return 0;
    for(liste p=L->beg; p; p=p->next)
        if(p->disponible) return 1;
    return 0;
}

int Rdisponible(DOUBLE_list L,int v){
    if(!L) return 0;
    for(liste p=L->beg; p; p=p->next)
        if(p->id==v && p->disponible) return 1;
    return 0;
}

void Rmarquer_nondis(DOUBLE_list L,int v){
    if(!L) return;
    for(liste p=L->beg;p;p=p->next)
        if(p->id==v){ p->disponible=0; return; }
}

/* ===================== ETUDIANTS ===================== */

next_student insererFE(next_student l,int v,int rID,int bus){
    next_student p = malloc(sizeof(students)), q=l;
    p->student_id=v;
    p->route_id=rID;
    p->bus=bus;
    p->suiv=NULL;

    if(!l) return p;
    while(q->suiv) q=q->suiv;
    q->suiv=p;
    return l;
}

void afficheretudiant(next_student l){
    for(;l;l=l->suiv)
        printf("etudiant %d -> route %d\n", l->student_id, l->route_id);
}

void afficheretudiantbus(next_student l,int v){
    for(;l;l=l->suiv)
        if(l->bus==v)
            printf("etudiant %d -> route %d bus %d\n",l->student_id,l->route_id,l->bus);
}

/* ===== V rification d'unicit  pour  tudiants ===== */
int student_existe(next_student l, int id){
    while(l){
        if(l->student_id==id) return 1;
        l=l->suiv;
    }
    return 0;
}

/* ===================== PRESENCE ===================== */

point insererFEP(point l,int v,char T[],int pr){
    point p = malloc(sizeof(presence)), q=l;
    strcpy(p->date,T);
    p->present=pr;
    p->student_id=v;
    p->suiv=NULL;

    if(!l) return p;
    while(q->suiv) q=q->suiv;
    q->suiv=p;
    return l;
}

void afficheerpresence(point L){
    if(!L) return;
    printf("DATE : ");
    puts(L->date);

    for(point p=L;p;p=p->suiv)
        printf("etudiant %d : %s\n",p->student_id,p->present?"PRESENT":"ABSENT");
}

void clear_screen(){ system("cls"); }

/* ===================== SAISIES (raccourcies) avec unicit  ===================== */
/* ======= Les versions simplifi es (moins de r p titions) ======= */

next saisie_buses(){ /* version nettoy e et v rifie unicit  des bus_id */
    int n;
    printf("combien de bus ? : ");
    scanf("%d",&n);

    next L=NULL;
    char MA[100];
    int bus_id, capa;

    for(int i=0;i<n;i++){
        do{
            printf("bus id : "); scanf("%d",&bus_id);
            if(bus_existe(L,bus_id)) printf("Erreur: bus_id %d deja existe. Choisir un autre.\n", bus_id);
            else break;
        } while(1);

        printf("capacite : "); scanf("%d",&capa);
        getchar();
        printf("matricule : "); gets(MA);
        /***********************************************/
         MYSQL *con = mysql_init(NULL);
    con = mysql_real_connect(con, "localhost", "root", "Uchihasasuke332#",
                            "first_try", 0, NULL, 0);

    if (!con) {
        printf("Connexion echouee: %s\n", mysql_error(con));
        getchar();
      //  return 1;
    }
     char query[500];
    sprintf(query, "INSERT INTO bus VALUES (%d, '%s', %d)",
            bus_id, MA, capa);

    if (mysql_query(con, query) == 0) {
        printf(" Succes! Employe insere.\n");
         // AJOUTEZ CES 2 LIGNES POUR LE COMMIT
          mysql_query(con, "COMMIT");    // Force le commit immédiat
        // FIN DE L'AJOUT

        printf(" Donnees sauvegardees definitivement!\n");
    } else {
        printf(" Erreur: %s\n", mysql_error(con));
    }
    /***********************************************/
        L = insererF(L,bus_id,MA,capa);
    }
    return L;
}

DOUBLE_list saisie_routes(next L,int c){
    DOUBLE_list R=NULL;
    char T[100];
    int id, bus;

    for(int i=0;i<c;i++){
        /* id de route unique dans R */
        do{
            printf("Route id : ");
            scanf("%d",&id);
            if(route_existe(R,id)) printf("Erreur: route id %d deja existe. Choisir un autre.\n", id);
            else break;
        } while(1);

        bus=0;
        if(buses_disponible(L)){
            do{
                printf("bus id : ");
                scanf("%d",&bus);
                if(bus_existe(L,bus) && disponible(L,bus)){
                    marquer_nondis(L,bus);
                    break;
                }
                printf("bus invalide ou occupe\n");
            }while(1);
        } else {
            printf("Aucun bus disponible pour lier   la route %d\n", id);
        }

        getchar();
        printf("itineraire : ");
        gets(T);
        /***********************************************/
          MYSQL *con = mysql_init(NULL);
    con = mysql_real_connect(con, "localhost", "root", "Uchihasasuke332#",
                            "first_try", 0, NULL, 0);

    if (!con) {
        printf("Connexion echouee: %s\n", mysql_error(con));
        getchar();
      //  return 1;
    }
        char query[500];
    sprintf(query, "INSERT INTO route VALUES (%d, %d, '%s')",
            id, bus, T);
    if (mysql_query(con, query) == 0) {
        printf(" Succes! Employe insere.\n");
         // AJOUTEZ CES 2 LIGNES POUR LE COMMIT
          mysql_query(con, "COMMIT");    // Force le commit immédiat
        // FIN DE L'AJOUT

        printf(" Donnees sauvegardees definitivement!\n");
    } else {
        printf(" Erreur: %s\n", mysql_error(con));
    }
    /***********************************************/
        R = insererFin(R,id,bus,T);
    }
    return R;
}

next_student saisie_ETUDIANTS(DOUBLE_list L,next B){
    next_student E=NULL;

    while(routes_disponible(L)){
        afficher(L);
        printf("choisir route pour inserer un etudiant: ");
        int d; scanf("%d",&d);

        if(!route_existe(L,d) || !Rdisponible(L,d)){
            printf("route invalide ou non disponible\n");
            continue;
        }

        liste P=L->beg;
        while(P && P->id!=d) P=P->next;

        next b=B;
        while(b && b->bus_id!=P->bus_id) b=b->suiv;

        if(!b){
            printf("erreur: aucun bus trouve pour la route\n");
            continue;
        }

        if(b->capacite<=0){
            printf("le bus %d est plein\n", b->bus_id);
            Rmarquer_nondis(L,d);
            continue;
        }

        int id;
        /* verifier unicite etudiant */
        do{
            printf("id etudiant : ");
            scanf("%d",&id);
            if(student_existe(E,id)) printf("Erreur: etudiant id %d deja existe. Choisir un autre.\n", id);
            else break;
        } while(1);

        /* assignation */
        b->capacite--;
        if(b->capacite==0) Rmarquer_nondis(L,d);
        /***********************************************/
 MYSQL *con = mysql_init(NULL);
    con = mysql_real_connect(con, "localhost", "root", "Uchihasasuke332#",
                            "first_try", 0, NULL, 0);

    if (!con) {
        printf("Connexion echouee: %s\n", mysql_error(con));
        getchar();
      //  return 1;
    }
     char query[500];
    sprintf(query, "INSERT INTO etudiant VALUES (%d,%d)",
            id,d);

    if (mysql_query(con, query) == 0) {
        printf(" Succes! Employe insere.\n");
         // AJOUTEZ CES 2 LIGNES POUR LE COMMIT
          mysql_query(con, "COMMIT");    // Force le commit immédiat
        // FIN DE L'AJOUT

        printf(" Donnees sauvegardees definitivement!\n");
    } else {
        printf(" Erreur: %s\n", mysql_error(con));
    }
    /***********************************************/
        E = insererFE(E,id,d,P->bus_id);

        printf("ajoute. places restantes %d\n", b->capacite);

        char r;
        printf("autre etudiant ? (o/n) ");
        scanf(" %c",&r);
        if(r!='o') break;
      //  clear_screen();
    }
    return E;
}

void marque_presence(next B,next_student E){
    clear_screen();
    afficherREC(B);

    int d;
    do{
        printf("bus id pour presence : ");
        scanf("%d",&d);
    }while(!bus_existe(B,d));

    afficheretudiantbus(E,d);

    char T[10];
    printf("date AAAA-MM-DD : ");
    getchar();
    gets(T);

    point A=NULL;
    for(next_student p=E;p;p=p->suiv){
        if(p->bus==d){
            int x;
            printf("etudiant %d present(1/0) : ",p->student_id);
            scanf("%d",&x);
            A = insererFEP(A,p->student_id,T,x);
        }
    }

    afficheerpresence(A);
}

/* ===================== MAIN ===================== */

int main(){
    next L = saisie_buses();
    clear_screen();
    afficherREC(L);

    DOUBLE_list R = saisie_routes(L, L->taille);
   // clear_screen();

    next_student E = saisie_ETUDIANTS(R,L);
    afficheretudiant(E);

    marque_presence(L,E);

    return 0;
}
