//
// Created by marie on 28/10/2026.
//

#ifndef COMMANDE_H
#define COMMANDE_H

#include "types.h"


void calculerMontantsCommande(Commande *c);
void creerCommande(Boutique *b);
void annulerCommande(Boutique *b);
void changerStatutCommande(Boutique *b);
void modifierLignesCommande(Boutique *b);
void afficherDetailCommande(const Boutique *b);




#endif //COMMANDE_H
