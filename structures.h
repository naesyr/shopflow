//
// Created by byakko on 27/10/2026.
//

#ifndef PROJET_SHOPFLOW_STRUCTURES_H
#define PROJET_SHOPFLOW_STRUCTURES_H


typedef struct {
    char rue[100];
    char ville[51];
    char codePostal[9]; //pour prendre en compte tout ls pays (max = Royaume-unis, 8 chiffre), +1 pour le /0
    char pays[50];
} Adresse;

typedef struct {
    char  reference[12];     /* unique, non vide : référence commerciale , 12 car pour avoir de la marge */
    char  designation[60];   /* non vide : libellé du produit,60 car "Clavier mécanique"  (55 caractère)*/
    char  categorie[30];     /* non vide : famille de produit, 30 car "Informatique" (12 caractère  )*/
    float prixUnitaireHT;    /* > 0 : prix unitaire hors taxes,*/
    int   quantiteStock;     /* >= 0 : stock disponible,*/
    int   seuilAlerte;       /* >= 0 : seuil de réapprovisionnement,*/
    int   actif;             /* 0 ou 1 : statut du produit par 1 est toujours en vente ou 0 supprimer de la vente */
} Produit;


typedef struct {
    int numero;
    int numClient;
    //Date dateCommande;
    /* utilisation de la structure date définie avant*/
    char statut[20];
    int nbLignes;
    int capaciteLignes;
    //LigneCommande *lignes;
    /*
    pointeur vers la structure LigneCommande pour la création
    d'un tableau dynamique car on ne connait pas la taille de la commande en avance
     */
    float montantBrutHT;
    float tauxRemise;
    float montantRemise;
    float montantHT;
    float montantTVA;
    float montantTTC;
    float fraisPort;
    Adresse adresseLivraison;
} Commande;

#endif //PROJET_SHOPFLOW_STRUCTURES_H