#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produit.h"

void afficherProduit(Produit p) {
    if (p.actif == 1) {
        printf("Ref: %s | Nom: %s | Cat: %s | Prix HT: %.2f EUR | Stock: %d (Seuil: %d)\n",
               p.reference, p.designation, p.categorie, p.prixUnitaireHT, p.quantiteStock, p.seuilAlerte);
    }
}

void afficherCatalogue(Produit *catalogue, int nbProduits) {
    printf("\n=== CATALOGUE DES PRODUITS ===\n");
    for (int i = 0; i < nbProduits; i++) {
        if (catalogue[i].actif == 1) {
            afficherProduit(catalogue[i]);
        }
    }
    printf("==============================\n");
}

int rechercherProduitParRef(Produit *catalogue, int nbProduits, const char *ref) {
    for (int i = 0; i < nbProduits; i++) {
        if (strcmp(catalogue[i].reference, ref) == 0 && catalogue[i].actif == 1) {
            return i;
        }
    }
    return -1;
}

void saisirProduit(Produit *p) {
    printf("Donner la reference du produit : ");
    scanf("%s", p->reference);

    printf("Donner le nom du produit : ");
    scanf("%s", p->designation);

    printf("Donner la categorie du produit : ");
    scanf("%s", p->categorie);

    printf("Donner le prix HT : ");
    scanf("%f", &p->prixUnitaireHT);

    printf("Combien y a-t-il de produits en stock : ");
    scanf("%d", &p->quantiteStock);

    printf("Quel est le seuil d'alerte : ");
    scanf("%d", &p->seuilAlerte);

    printf("Le produit est-il actif : 0=non, 1=oui : ");
    scanf("%d", &p->actif);
}

void ajouterProduit(Produit **catalogue, int *nbProduits, int *capacite) {
    if (*nbProduits >= *capacite) {
        *capacite *= 2;
        Produit *temp = (Produit *)realloc(*catalogue, (*capacite) * sizeof(Produit));
        if (temp == NULL) {
            printf("Erreur lors de l'allocation mémoire pour le catalogue.\n");
            return;
        }
        *catalogue = temp;
    }

    Produit p;
    saisirProduit(&p);

    if (rechercherProduitParRef(*catalogue, *nbProduits, p.reference) != -1) {
        printf("Erreur : Un produit avec cette reference existe deja.\n");
        return;
    }

    (*catalogue)[*nbProduits] = p;
    (*nbProduits)++;
    printf("Produit ajoute avec succes !\n");
}

void mettreAJourProduit(Produit *catalogue, int nbProduits) {
    char ref[12];
    printf("Entrer la reference du produit a modifier : ");
    scanf("%s", ref);

    int index = rechercherProduitParRef(catalogue, nbProduits, ref);
    if (index == -1) {
        printf("Produit introuvable.\n");
        return;
    }

    printf("Modification pour le produit : %s\n", catalogue[index].designation);
    printf("Nouveau prix HT : ");
    scanf("%f", &catalogue[index].prixUnitaireHT);
    printf("Nouvelle quantite en stock : ");
    scanf("%d", &catalogue[index].quantiteStock);
    printf("Nouveau seuil d'alerte : ");
    scanf("%d", &catalogue[index].quantiteStock);
    scanf("%d", &catalogue[index].seuilAlerte);

    printf("Produit mis a jour avec succes !\n");
}

void supprimerProduit(Produit *catalogue, int nbProduits, const char *ref) {
    int index = rechercherProduitParRef(catalogue, nbProduits, ref);
    if (index == -1) {
        printf("Produit introuvable.\n");
        return;
    }

    catalogue[index].actif = 0;
    printf("Produit supprime (logiquement) avec succes.\n");
}