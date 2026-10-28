//
// Created by byakko on 27/10/2026.
//

#ifndef PROJET_SHOPFLOW_STRUCTURES_H
#define PROJET_SHOPFLOW_STRUCTURES_H

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

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
    Date dateCommande;
    /* utilisation de la structure date définie avant*/
    char statut[20];
    int nbLignes;
    int capaciteLignes;
    LigneCommande *lignes;
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

// test branching
typedef struct {
    int idClient;                 // Identifiant du client : type 'int' car c'est un entier unique > 0
    char nom[50];             // Nom de famille : tableau de 50 caractères pour respecter la limite de 49 car. max (+1 pour le caractère nul '\0'), non vide
    char prenom[50];              // Prénom : tableau de 50 caractères pour stocker le prénom, non vide
    char email[80];               // Adresse électronique : tableau de 80 caractères pour contenir l'arobase '@' et garantir l'unicité
    char telephone[15];           // Téléphone de contact : tableau de 15 caractères pour stocker les 10 chiffres du numéro (+ le caractère nul '\0')
    Adresse adresse;              // Adresse de livraison : utilisation d'une structure imbriquée pour grouper les informations de localisation
    int pointsFidelite;           // Points de fidélité cumulés : entier positif ou nul (>= 0), mis à jour par calcul
    int nbCommandes;              // Nombre de commandes du client : entier positif ou nul (>= 0)
    capaciteCommandes;        // Taille du tableau alloué : entier supérieur ou égal au nombre de commandes pour gérer le redimensionnement
    Commande *commandes;        // Historique des commandes : pointeur alloué dynamiquement (via malloc) pour s'adapter au nombre variable de commandes
    int actif;                    // Statut du client : entier valant 0 ou 1 pour gérer la suppression logique sans effacer les données
} Client;

typedef struct {
    char nom[50];
    Produit *produits;
    int nbProduits;
    int capaciteProduits;
    Client *clients;
    int nbClients;
    int capaciteClients;
    int numeroSuivant;
    int idClientSuivant;
} Boutique;


#endif //PROJET_SHOPFLOW_STRUCTURES_H