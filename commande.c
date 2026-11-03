#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "types.h"
#include "commande.h"
#include "utils.h"
#include "client.h"
#include "produit.h"

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

    // Frais de port offerts a partir de 60 HT brut
    c->fraisPort = (c->montantBrutHT >= 60.0) ? 0.0 : 5.90;

    c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}

void creerCommande(Boutique *b) {
    int idClient;
    if (!lireEntier("ID Client : ", 1, INT_MAX, &idClient)) return;

    Client *client = trouverClient(b, idClient);
    if (client == NULL) {
        printf("Client introuvable.\n");
        return;
    }

    // Reallocation si le tableau de commandes du client est plein
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

    // Date fictive
    c->dateCommande.jour = 28;
    c->dateCommande.mois = 10;
    c->dateCommande.annee = 2026;

    int nb;
    if (!lireEntier("Nombre de lignes a saisir : ", 1, 100, &nb)) return;

    for (int i = 0; i < nb; i++) {
        if (c->nbLignes >= c->capaciteLignes) {
            c->capaciteLignes *= 2;
            c->lignes = realloc(c->lignes, c->capaciteLignes * sizeof(LigneCommande));
        }

        char ref[12];
        char promptRef[50];
        sprintf(promptRef, "Reference produit %d : ", i+1);
        if (!lireTexte(promptRef, ref, sizeof(ref), 0)) continue;

        // Note: Assurez-vous d'avoir declarer trouverIndexProduit dans produit.h
        int idxP = -1;
        for (int k = 0; k < b->nbProduits; k++) {
            if (b->produits[k].actif && strcmp(b->produits[k].reference, ref) == 0) {
                idxP = k;
                break;
            }
        }

        if (idxP == -1) { printf("Produit inconnu.\n"); continue; }

        Produit *p = &b->produits[idxP];

        int qte;
        char promptQte[50];
        sprintf(promptQte, "Quantite (max %d) : ", p->quantiteStock);
        if (!lireEntier(promptQte, 1, p->quantiteStock, &qte)) continue;

        LigneCommande lc;
        strcpy(lc.refProduit, p->reference);
        strcpy(lc.designation, p->designation);
        lc.quantite = qte;
        lc.prixUnitaireHT = p->prixUnitaireHT;
        lc.sousTotalHT = qte * p->prixUnitaireHT;

        c->lignes[c->nbLignes++] = lc;
        p->quantiteStock -= qte; // Decrementation du stock
    }

    if (c->nbLignes == 0) {
        printf("Commande vide. Annulation.\n");
        free(c->lignes);
        return;
    }

    calculerMontantsCommande(c);

    // Points de fidelite : 1 point par tranche de 10 euros TTC
    int pointsGagnes = (int)(c->montantTTC / 10);
    client->pointsFidelite += pointsGagnes;
    client->nbCommandes++;

    printf("Commande %d creee avec succes. Montant TTC: %.2f EUR\n", c->numero, c->montantTTC);
}

void annulerCommande(Boutique *b) {
    int num;
    if (!lireEntier("Numero de commande a annuler : ", 1, INT_MAX, &num)) return;

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (c->numero == num) {
                if (strcmp(c->statut, "EN_COURS") != 0) {
                    printf("Seules les commandes EN_COURS peuvent etre annulees.\n");
                    return;
                }

                strcpy(c->statut, "ANNULEE");

                // Reintegration des stocks
                for (int k = 0; k < c->nbLignes; k++) {
                    for (int p = 0; p < b->nbProduits; p++) {
                        if (strcmp(b->produits[p].reference, c->lignes[k].refProduit) == 0) {
                            b->produits[p].quantiteStock += c->lignes[k].quantite;
                            break;
                        }
                    }
                }

                // Retrait des points de fidelite
                int pointsPerdus = (int)(c->montantTTC / 10);
                b->clients[i].pointsFidelite -= pointsPerdus;
                if (b->clients[i].pointsFidelite < 0) {
                    b->clients[i].pointsFidelite = 0;
                }

                printf("Commande annulee avec succes. Stocks et points fidelite mis a jour.\n");
                return;
            }
        }
    }
    printf("Commande introuvable.\n");
}

void changerStatutCommande(Boutique *b) {
    int num;
    if (!lireEntier("Numero de commande : ", 1, INT_MAX, &num)) return;

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
    printf("Logique algorithmique complexe impliquant la reintegration ou decrementation des stocks,\n");
    printf("la modification de nbLignes et le recalcul total des montants.\n");
    // STUB a implementer
}

void afficherDetailCommande(const Boutique *b) {
    int num;
    if (!lireEntier("Numero de commande : ", 1, INT_MAX, &num)) return;

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