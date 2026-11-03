//
// Created by marie on 28/10/2026.
//

#ifndef INITIALISATION_H
#define INITIALISATION_H

#include "types.h"

/* Initialise une boutique entierement vide avec un nom donne */
void initialiserBoutiqueVide(Boutique *b, const char *nom);

/* Charge le jeu de donnees de demonstration en memoire */
int initialiserDonneesTest(Boutique *b);

/* Libere toute la memoire dynamique allouee pour la boutique (produits, clients, commandes, lignes) */
void detruireBoutique(Boutique *b);

#endif // INITIALISATION_H