//
// Created by marie on 28/10/2026.
//

#ifndef PRODUIT_H
#define PRODUIT_H
#include "types.h"


void afficherProduit(Produit p);
void afficherCatalogue(Produit *catalogue, int nbProduits);
int rechercherProduitParRef(Produit *catalogue, int nbProduits, const char *ref);
void ajouterProduit(Produit **catalogue, int *nbProduits, int *capacite);
void mettreAJourProduit(Produit *catalogue, int nbProduits);
void supprimerProduit(Produit *catalogue, int nbProduits, const char *ref);
void rechercheMultiCriteres(const Boutique *b);
int trouverIndexProduit(const Boutique *b, const char *reference);
#endif //PRODUIT_H
