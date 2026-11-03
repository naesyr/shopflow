//
// Created by marie on 28/10/2026.
//

#ifndef PETIT_PROJET_PRODUIT_H
#define PETIT_PROJET_PRODUIT_H

typedef struct {
    char  reference[12];     /* unique, non vide : référence commerciale , 12 car pour avoir de la marge */
    char  designation[60];   /* non vide : libellé du produit,60 car "Clavier mécanique"  (55 caractère)*/
    char  categorie[30];     /* non vide : famille de produit, 30 car "Informatique" (12 caractère  )*/
    float prixUnitaireHT;    /* > 0 : prix unitaire hors taxes,*/
    int   quantiteStock;     /* >= 0 : stock disponible,*/
    int   seuilAlerte;       /* >= 0 : seuil de réapprovisionnement,*/
    int   actif;             /* 0 ou 1 : statut du produit par 1 est toujours en vente ou 0 supprimer de la vente */
} Produit;

void ajouterProduit(Produit *p);
void modifierProduit(Produit *p);

#endif //PETIT_PROJET_PRODUIT_H
