//
// Created by marie on 28/10/2026.
//

#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include "types.h"

typedef struct {
    char reference[12];
    char designation[60];
    int  quantite;
} StatProduit;

typedef struct {
    int   idClient;
    char  nom[50];
    char  prenom[50];
    float caHT;
    float caTTC;
} StatClient;

typedef struct {
    char  categorie[60];
    int   quantite;
    float caBrutHT;
} StatCategorie;

/* ---- Chiffre d'affaires ---- */
void calculerChiffreAffaires(const Boutique *b, float *ht, float *ttc);
void calculerChiffreAffairesPeriode(const Boutique *b, int mois, int annee, float *ht, float *ttc);
void afficherChiffreAffaires(const Boutique *b);

/* ---- Palmares produits ---- */
StatProduit *construireStatsProduits(const Boutique *b, int *nb);
void trierStatsProduits(StatProduit *t, int n);
void afficherPalmaresProduits(const Boutique *b);

/* ---- Palmares clients ---- */
StatClient *construireStatsClients(const Boutique *b, int *nb);
void trierStatsClients(StatClient *t, int n);
void afficherPalmaresClients(const Boutique *b);

/* ---- Extension 2 : statistiques par categorie ---- */
StatCategorie *construireStatsCategories(const Boutique *b, int *nb);
void trierStatsCategories(StatCategorie *t, int n);
void afficherStatsCategories(const Boutique *b);
#endif //STATISTIQUE_H
