//
// Created by byakko on 27/10/2026.
//

#ifndef PROJET_SHOPFLOW_STRUCTURES_H
#define PROJET_SHOPFLOW_STRUCTURES_H


typedef struct {
    char rue[100];
    char ville[50];
    char codePostal[9]; //pour prendre en compte tout ls pays (max = Royaume-unis, 8 chiffre), +1 pour le /0
    char pays[50];
} Adresse;


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