#include <stdio.h>
#include <stdlib.h>
void clear_screen() {
    system("cls");
}
int main()
{
    printf("Hello world!\n");
    printf("Avant nettoyage...\n");
clear_screen();
printf("Après nettoyage !\n");
    return 0;
}
