
//
// Created by marie on 28/10/2026.
//

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "initialisation.h"
#include "commande.h"

/* Initialise une boutique entierement vide */
void initialiserBoutiqueVide(Boutique *b, const char *nom) {
    memset(b, 0, sizeof(Boutique));
    strncpy(b->nom, nom, sizeof(b->nom) - 1);
    b->numeroSuivant = 1000;
    b->idClientSuivant = 1;
}

/*
 * Chargement du jeu de donnees de demonstration
 * Retourne 1 si succes, 0 sinon
 */
int initialiserDonneesTest(Boutique *b) {
    // 1. Initialisation de base
    initialiserBoutiqueVide(b, "ShopFlow");
    b->numeroSuivant = 1007; // 6 commandes creees
    b->idClientSuivant = 6;  // 5 clients crees

    /* --- 1. CATALOGUE PRODUITS --- */
    b->capaciteProduits = 10;
    b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit));
    if (b->produits == NULL) return 0;
    b->nbProduits = 0;

    Produit modeleProduits[8] = {
        {"PRD-0001", "Clavier mecanique", "Informatique", 79.90, 42, 10, 1},
        {"PRD-0002", "Souris sans fil", "Informatique", 24.50, 8, 15, 1}, // Alerte stock
        {"PRD-0003", "Ecran 27 pouces", "Informatique", 199.00, 4, 5, 1},   // Alerte stock
        {"PRD-0004", "Casque audio", "Audio", 89.90, 25, 10, 1},
        {"PRD-0005", "Enceinte Bluetooth", "Audio", 59.90, 12, 8, 1},
        {"PRD-0006", "Cable HDMI 2 m", "Accessoires", 9.90, 120, 30, 1},
        {"PRD-0007", "Hub USB-C 7 ports", "Accessoires", 49.90, 14, 10, 1},
        {"PRD-0008", "Disque SSD 1 To", "Stockage", 109.00, 18, 6, 1}
    };

    for (int i = 0; i < 8; i++) {
        b->produits[b->nbProduits++] = modeleProduits[i];
    }

    /* --- 2. CLIENTS --- */
    b->capaciteClients = 8;
    b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client));
    if (b->clients == NULL) return 0;
    b->nbClients = 0;

    Client modeleClients[5] = {
        {1, "Bernard", "Julie", "j.bernard@mail.fr", "0612345678", {"12 rue de la Paix", "Paris", "75002", "France"}, 33, 0, 4, NULL, 1},
        {2, "Martin", "Paul", "pmartin@mail.com", "0698765432", {"4 avenue des Fleurs", "Lyon", "69003", "France"}, 0, 0, 4, NULL, 1},
        {3, "Dubois", "Alice", "alice.d@test.fr", "0711223344", {"8 place de la Gare", "Marseille", "13001", "France"}, 85, 0, 4, NULL, 1},
        {4, "Petit", "Luc", "luc.petit@fai.fr", "0644556677", {"15 impasse du Moulin", "Lille", "59000", "France"}, 12, 0, 4, NULL, 1},
        {5, "Lefevre", "Emma", "emma.l@domain.com", "0788990011", {"22 boulevard Carnot", "Bordeaux", "33000", "France"}, 0, 0, 4, NULL, 1}
    };

    for (int i = 0; i < 5; i++) {
        b->clients[i] = modeleClients[i];
        b->clients[i].commandes = (Commande *)malloc(b->clients[i].capaciteCommandes * sizeof(Commande));
        if (b->clients[i].commandes == NULL) return 0;
        b->clients[i].nbCommandes = 0;
        b->nbClients++;
    }

    /* --- 3. COMMANDES --- */

    // Commande 1 : Client 1, > 600 EUR HT, 3 lignes, LIVREE
    Commande *c1 = &b->clients[0].commandes[b->clients[0].nbCommandes++];
    c1->numero = 1001;
    c1->idClient = 1;
    c1->dateCommande = (Date){10, 10, 2026};
    strcpy(c1->statut, "LIVREE");
    c1->capaciteLignes = 4;
    c1->lignes = (LigneCommande *)malloc(c1->capaciteLignes * sizeof(LigneCommande));
    c1->nbLignes = 3;
    c1->lignes[0] = (LigneCommande){"PRD-0003", "Ecran 27 pouces", 3, 199.00, 597.00};
    c1->lignes[1] = (LigneCommande){"PRD-0001", "Clavier mecanique", 1, 79.90, 79.90};
    c1->lignes[2] = (LigneCommande){"PRD-0006", "Cable HDMI 2 m", 2, 9.90, 19.80};
    c1->adresseLivraison = b->clients[0].adresse;
    calculerMontantsCommande(c1);

    // Commande 2 : Client 3, 3 lignes, EXPEDIEE
    Commande *c2 = &b->clients[2].commandes[b->clients[2].nbCommandes++];
    c2->numero = 1002;
    c2->idClient = 3;
    c2->dateCommande = (Date){12, 10, 2026};
    strcpy(c2->statut, "EXPEDIEE");
    c2->capaciteLignes = 4;
    c2->lignes = (LigneCommande *)malloc(c2->capaciteLignes * sizeof(LigneCommande));
    c2->nbLignes = 3;
    c2->lignes[0] = (LigneCommande){"PRD-0008", "Disque SSD 1 To", 1, 109.00, 109.00};
    c2->lignes[1] = (LigneCommande){"PRD-0007", "Hub USB-C 7 ports", 1, 49.90, 49.90};
    c2->lignes[2] = (LigneCommande){"PRD-0002", "Souris sans fil", 1, 24.50, 24.50};
    c2->adresseLivraison = b->clients[2].adresse;
    calculerMontantsCommande(c2);

    // Commande 3 : Client 4, 1 ligne, EN_COURS
    Commande *c3 = &b->clients[3].commandes[b->clients[3].nbCommandes++];
    c3->numero = 1003;
    c3->idClient = 4;
    c3->dateCommande = (Date){15, 10, 2026};
    strcpy(c3->statut, "EN_COURS");
    c3->capaciteLignes = 2;
    c3->lignes = (LigneCommande *)malloc(c3->capaciteLignes * sizeof(LigneCommande));
    c3->nbLignes = 1;
    c3->lignes[0] = (LigneCommande){"PRD-0005", "Enceinte Bluetooth", 1, 59.90, 59.90};
    c3->adresseLivraison = b->clients[3].adresse;
    calculerMontantsCommande(c3);

    // Commande 4 : Client 2, 2 lignes, ANNULEE
    Commande *c4 = &b->clients[1].commandes[b->clients[1].nbCommandes++];
    c4->numero = 1004;
    c4->idClient = 2;
    c4->dateCommande = (Date){18, 10, 2026};
    strcpy(c4->statut, "ANNULEE");
    c4->capaciteLignes = 2;
    c4->lignes = (LigneCommande *)malloc(c4->capaciteLignes * sizeof(LigneCommande));
    c4->nbLignes = 2;
    c4->lignes[0] = (LigneCommande){"PRD-0004", "Casque audio", 1, 89.90, 89.90};
    c4->lignes[1] = (LigneCommande){"PRD-0006", "Cable HDMI 2 m", 1, 9.90, 9.90};
    c4->adresseLivraison = b->clients[1].adresse;
    calculerMontantsCommande(c4);

    // Commande 5 : Client 1, 1 ligne, LIVREE
    Commande *c5 = &b->clients[0].commandes[b->clients[0].nbCommandes++];
    c5->numero = 1005;
    c5->idClient = 1;
    c5->dateCommande = (Date){20, 10, 2026};
    strcpy(c5->statut, "LIVREE");
    c5->capaciteLignes = 2;
    c5->lignes = (LigneCommande *)malloc(c5->capaciteLignes * sizeof(LigneCommande));
    c5->nbLignes = 1;
    c5->lignes[0] = (LigneCommande){"PRD-0007", "Hub USB-C 7 ports", 1, 49.90, 49.90};
    c5->adresseLivraison = b->clients[0].adresse;
    calculerMontantsCommande(c5);

    // Commande 6 : Client 5, 2 lignes, EN_COURS
    Commande *c6 = &b->clients[4].commandes[b->clients[4].nbCommandes++];
    c6->numero = 1006;
    c6->idClient = 5;
    c6->dateCommande = (Date){25, 10, 2026};
    strcpy(c6->statut, "EN_COURS");
    c6->capaciteLignes = 2;
    c6->lignes = (LigneCommande *)malloc(c6->capaciteLignes * sizeof(LigneCommande));
    c6->nbLignes = 2;
    c6->lignes[0] = (LigneCommande){"PRD-0001", "Clavier mecanique", 2, 79.90, 159.80};
    c6->lignes[1] = (LigneCommande){"PRD-0002", "Souris sans fil", 1, 24.50, 24.50};
    c6->adresseLivraison = b->clients[4].adresse;
    calculerMontantsCommande(c6);

    return 1;
}

/* Liberation complete de toute la memoire dynamique */
void detruireBoutique(Boutique *b) {
    if (b == NULL) return;

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (b->clients[i].commandes[j].lignes != NULL) {
                free(b->clients[i].commandes[j].lignes);
            }
        }
        if (b->clients[i].commandes != NULL) {
            free(b->clients[i].commandes);
        }
    }

    if (b->clients != NULL) {
        free(b->clients);
    }

    if (b->produits != NULL) {
        free(b->produits);
    }

    // Remise a zero pour securite
    b->clients = NULL;
    b->produits = NULL;
    b->nbClients = 0;
    b->nbProduits = 0;
    b->capaciteClients = 0;
    b->capaciteProduits = 0;
}