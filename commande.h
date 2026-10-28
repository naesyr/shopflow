//
// Created by marie on 28/10/2026.
//

#ifndef PETIT_PROJET_COMMANDE_H
#define PETIT_PROJET_COMMANDE_H

#include "types.h"

void creerCommande(Boutique *b);
void modifierLignesCommande(Boutique *b);
void changerStatutCommande(Boutique *b);
void annulerCommande(Boutique *b);
void afficherDetailCommande(const Boutique *b);
void calculerMontantsCommande(Commande *c);



#endif //PETIT_PROJET_COMMANDE_H
