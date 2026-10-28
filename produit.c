#include "produit.h"

#include <produit.h>
#include <stdio.h>



void ajouterProduit(Produit *p) {
    printf("Donner la reference du produit :\n");
    scanf("%d", &p -> reference);
    printf("Donner le nom du produit\n");
    scanf("%s", &p -> designation);
    printf("Donner la categorie du produit :\n");
    scanf("%s", &p -> categorie);
    printf("Donner le prix HT\n");
    scanf("%f", &p -> prixUnitaireHT);
    printf("Combien y'a t il de produit en stock :\n");
    scanf("%d", &p -> quantiteStock);
    printf("Quelle est le seuil d'alerte :\n");
    scanf("%d", &p -> seuilAlerte);
    printf("Le produit est-il actif : 0=non, 1=oui :\n");
    scanf("%d", &p -> actif);
}

void mettre_a_jour_un_produit(Produit *p) {
    printf("Donner le prix HT\n");
    scanf("%f", &p -> prixUnitaireHT);
    printf("Combien y'a t il de produit en stock\n");
    scanf("%d", &p -> quantiteStock);
    printf("Quelle est le seuil d'alerte\n");
    scanf("%d", &p -> seuilAlerte);
}