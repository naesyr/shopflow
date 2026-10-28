//
// Created by marie on 28/10/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "commande.h"
/* Calcule les sous-totaux, la remise par palier, la TVA, les frais de port et le TTC */
void calculerMontantsCommande(Commande *c) {
    c->montantBrutHT = 0.0;
    for (int i = 0; i < c->nbLignes; i++) {
        c->montantBrutHT += c->lignes[i].sousTotalHT;
    }

    // Paliers de remise sur montant brut
    if (c->montantBrutHT >= 600) c->tauxRemise = 15.0;
    else if (c->montantBrutHT >= 300) c->tauxRemise = 10.0;
    else if (c->montantBrutHT >= 100) c->tauxRemise = 5.0;
    else c->tauxRemise = 0.0;

    c->montantRemise = c->montantBrutHT * (c->tauxRemise / 100.0);
    c->montantHT = c->montantBrutHT - c->montantRemise;

    // TVA 20%
    c->montantTVA = c->montantHT * 0.20;

    // Frais de port offerts à partir de 60 HT brut
    c->fraisPort = (c->montantBrutHT >= 60.0) ? 0.0 : 5.90;

    c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}

void creerCommande(Boutique *b) {
    printf("ID Client : ");
    int idClient = lireEntier();
    int idx = trouverIndexClient(b, idClient);
    if (idx == -1) {
        printf("Client introuvable.\n");
        return;
    }
    Client *client = &b->clients[idx];

    // Réallocation si le tableau de commandes du client est plein
    if (client->nbCommandes >= client->capaciteCommandes) {
        client->capaciteCommandes = (client->capaciteCommandes == 0) ? 5 : client->capaciteCommandes * 2;
        client->commandes = realloc(client->commandes, client->capaciteCommandes * sizeof(Commande));
    }

    Commande *c = &client->commandes[client->nbCommandes];
    c->numero = b->numeroSuivant++;
    c->idClient = idClient;
    c->adresseLivraison = client->adresse;
    strcpy(c->statut, "EN_COURS");

    // Allocation initiale des lignes
    c->capaciteLignes = 5;
    c->nbLignes = 0;
    c->lignes = malloc(c->capaciteLignes * sizeof(LigneCommande));

    // (Pour simplifier, date fictive, à remplacer par une vraie saisie)
    c->dateCommande.jour = 28;
    c->dateCommande.mois = 10;
    c->dateCommande.annee = 2026;

    printf("Nombre de lignes a saisir : ");
    int nb = lireEntier();
    for (int i = 0; i < nb; i++) {
        if (c->nbLignes >= c->capaciteLignes) {
            c->capaciteLignes *= 2;
            c->lignes = realloc(c->lignes, c->capaciteLignes * sizeof(LigneCommande));
        }

        char ref[12];
        printf("Reference produit %d : ", i+1);
        lireChaine(ref, 12);
        int idxP = trouverIndexProduit(b, ref);
        if (idxP == -1) { printf("Produit inconnu.\n"); continue; }

        Produit *p = &b->produits[idxP];

        printf("Quantite (max %d) : ", p->quantiteStock);
        int qte = lireEntier();
        if (qte <= 0 || qte > p->quantiteStock) { printf("Quantite invalide.\n"); continue; }

        LigneCommande lc;
        strcpy(lc.refProduit, p->reference);
        strcpy(lc.designation, p->designation);
        lc.quantite = qte;
        lc.prixUnitaireHT = p->prixUnitaireHT;
        lc.sousTotalHT = qte * p->prixUnitaireHT;

        c->lignes[c->nbLignes++] = lc;
        p->quantiteStock -= qte; // Décrémentation du stock
    }

    if (c->nbLignes == 0) {
        printf("Commande vide. Annulation.\n");
        free(c->lignes);
        return;
    }

    calculerMontantsCommande(c);

    // Points de fidélité : 1 point par tranche de 10 euros TTC
    int pointsGagnes = (int)(c->montantTTC / 10);
    client->pointsFidelite += pointsGagnes;
    client->nbCommandes++;

    printf("Commande %d creee avec succes. Montant TTC: %.2f EUR\n", c->numero, c->montantTTC);
}

void annulerCommande(Boutique *b) {
    printf("Numero de commande a annuler : ");
    int num = lireEntier();

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (c->numero == num) {
                if (strcmp(c->statut, "EN_COURS") != 0) {
                    printf("Seules les commandes EN_COURS peuvent etre annulees.\n");
                    return;
                }

                strcpy(c->statut, "ANNULEE");

                // Réintégration des stocks
                for (int k = 0; k < c->nbLignes; k++) {
                    int idxP = trouverIndexProduit(b, c->lignes[k].refProduit);
                    if (idxP != -1) {
                        b->produits[idxP].quantiteStock += c->lignes[k].quantite;
                    }
                }

                // Retrait des points de fidélité
                int pointsPerdus = (int)(c->montantTTC / 10);
                b->clients[i].pointsFidelite -= pointsPerdus;
                if (b->clients[i].pointsFidelite < 0) {
                    b->clients[i].pointsFidelite = 0;
                }

                printf("Commande annulee avec succes. Stocks et points fidélité mis a jour.\n");
                return;
            }
        }
    }
    printf("Commande introuvable.\n");
}

void changerStatutCommande(Boutique *b) {
    printf("Numero de commande : ");
    int num = lireEntier();

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (c->numero == num) {
                if (strcmp(c->statut, "EN_COURS") == 0) {
                    strcpy(c->statut, "EXPEDIEE");
                    printf("Nouveau statut de la commande %d : EXPEDIEE.\n", c->numero);
                } else if (strcmp(c->statut, "EXPEDIEE") == 0) {
                    strcpy(c->statut, "LIVREE");
                    printf("Nouveau statut de la commande %d : LIVREE.\n", c->numero);
                } else {
                    printf("Progression impossible depuis le statut actuel (%s).\n", c->statut);
                }
                return;
            }
        }
    }
    printf("Commande introuvable.\n");
}

void modifierLignesCommande(Boutique *b) {
    printf("Pour ajouter ou retirer une ligne sur une commande EN_COURS.\n");
    printf("Logique algorithmique complexe impliquant la réintégration ou décrémentation des stocks,\n");
    printf("la modification de nbLignes et le recalcul total des montants.\n");
    printf("// À implémenter selon votre approche : Ajouter une fonction pour ajouter une ligne, et une autre pour supprimer.\n");
}

void afficherDetailCommande(const Boutique *b) {
    printf("Numero de commande : ");
    int num = lireEntier();

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (c->numero == num) {
                printf("\n--- DETAILS COMMANDE %d ---\n", c->numero);
                printf("Statut : %s | Client ID : %d\n", c->statut, c->idClient);
                printf("Date : %02d/%02d/%04d\n", c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee);
                printf("--- Lignes ---\n");
                for (int k = 0; k < c->nbLignes; k++) {
                    printf(" - %s (%s) | Qte: %d x %.2f = %.2f EUR HT\n",
                           c->lignes[k].designation, c->lignes[k].refProduit,
                           c->lignes[k].quantite, c->lignes[k].prixUnitaireHT, c->lignes[k].sousTotalHT);
                }
                printf("----------------\n");
                printf("Montant Brut HT : %8.2f EUR\n", c->montantBrutHT);
                printf("Remise (%.1f %%) : %8.2f EUR\n", c->tauxRemise, c->montantRemise);
                printf("Montant net HT  : %8.2f EUR\n", c->montantHT);
                printf("TVA (20 %%)      : %8.2f EUR\n", c->montantTVA);
                printf("Frais de port   : %8.2f EUR\n", c->fraisPort);
                printf("MONTANT TTC     : %8.2f EUR\n", c->montantTTC);
                return;
            }
        }
    }
    printf("Commande introuvable.\n");
}