#include "produit.h"
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
void rechercheMultiCriteres(const Boutique *b)
{
    char categorie[LONG_CATEGORIE];
    float pmin, pmax;
    int enStock, i, n = 0;

    afficherTitre("RECHERCHE MULTI-CRITERES");
    if (!lireTexte("Categorie (vide = toutes) : ", categorie, sizeof categorie, 1)) return;
    for (;;) {
        if (!lireFlottant("Prix HT minimum : ", 0.0f, PRIX_MAX, &pmin)) return;
        if (!lireFlottant("Prix HT maximum : ", 0.0f, PRIX_MAX, &pmax)) return;
        if (pmin <= pmax) {
            break;
        }
        printf("  /!\\ Le minimum doit etre inferieur ou egal au maximum.\n");
    }
    enStock = lireOuiNon("Uniquement les produits en stock");

    afficherEnteteProduits();
    for (i = 0; i < b->nbProduits; i++) {
        const Produit *p = &b->produits[i];
        if (!p->actif) continue;
        if (categorie[0] != '\0' && comparerSansCasse(p->categorie, categorie) != 0) continue;
        if (p->prixUnitaireHT < pmin || p->prixUnitaireHT > pmax) continue;
        if (enStock && p->quantiteStock <= 0) continue;
        afficherProduit(p);
        n++;
    }
    printf("%d resultat(s).\n", n);
}