//
// Created by marie on 28/10/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "statistique.h"


void calculerChiffreAffaires(const Boutique *b) {
    float totalHT = 0.0, totalTTC = 0.0;

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            // Le chiffre d'affaires exclut les commandes au statut ANNULEE
            if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                totalHT += b->clients[i].commandes[j].montantHT;
                totalTTC += b->clients[i].commandes[j].montantTTC;
            }
        }
    }
    printf("\nCHIFFRE D'AFFAIRES GLOBAL\n");
    printf("Total HT  : %10.2f EUR\n", totalHT);
    printf("Total TTC : %10.2f EUR\n", totalTTC);
}

void afficherAlertesStock(const Boutique *b) {
    printf("\nALERTES DE STOCK\n");
    int alertes = 0;
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif && b->produits[i].quantiteStock <= b->produits[i].seuilAlerte) {
            printf(" - %s : %s | Stock dispo : %d (Seuil alerte : %d)\n",
                   b->produits[i].reference, b->produits[i].designation,
                   b->produits[i].quantiteStock, b->produits[i].seuilAlerte);
            alertes++;
        }
    }
    if (alertes == 0) {
        printf("Aucun produit n'est en alerte de stock.\n");
    }
}

// Structure temporaire locale pour trier les produits
typedef struct {
    char reference[12];
    char designation[60];
    int quantiteVendue;
} StatProduit;

void afficherPalmaresProduits(const Boutique *b) {
    // Allocation dynamique d'un tableau temporaire
    StatProduit *stats = malloc(b->nbProduits * sizeof(StatProduit));
    if (stats == NULL) {
        printf("Erreur d'allocation pour les statistiques.\n");
        return;
    }
    int nbStats = 0;

    // 1. Initialisation des compteurs pour tous les produits actifs
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif) {
            strcpy(stats[nbStats].reference, b->produits[i].reference);
            strcpy(stats[nbStats].designation, b->produits[i].designation);
            stats[nbStats].quantiteVendue = 0;
            nbStats++;
        }
    }

    // 2. Cumul des quantites vendues (exclusion des commandes ANNULEES)
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                Commande *cmd = &b->clients[i].commandes[j];
                for (int k = 0; k < cmd->nbLignes; k++) {
                    // Recherche du produit dans le tableau temporaire
                    for (int s = 0; s < nbStats; s++) {
                        if (strcmp(stats[s].reference, cmd->lignes[k].refProduit) == 0) {
                            stats[s].quantiteVendue += cmd->lignes[k].quantite;
                            break;
                        }
                    }
                }
            }
        }
    }

    // 3. Tri a bulles par ordre decroissant des quantites
    for (int i = 0; i < nbStats - 1; i++) {
        for (int j = 0; j < nbStats - i - 1; j++) {
            if (stats[j].quantiteVendue < stats[j+1].quantiteVendue) {
                StatProduit temp = stats[j];
                stats[j] = stats[j+1];
                stats[j+1] = temp;
            }
        }
    }

    // 4. Affichage du Top 5
    printf("\nTOP 5 DES PRODUITS\n");
    int maxAffiche = (nbStats < 5) ? nbStats : 5;
    int placesOccupees = 0;

    for (int i = 0; i < maxAffiche; i++) {
        if (stats[i].quantiteVendue > 0) {
            printf("%d. %s (%s) - %d vendus\n", i+1, stats[i].designation, stats[i].reference, stats[i].quantiteVendue);
            placesOccupees++;
        }
    }

    if (placesOccupees == 0) {
        printf("Aucun produit vendu pour le moment.\n");
    }

    // Liberation de la memoire
    free(stats);
}

// Structure temporaire locale pour trier les clients
typedef struct {
    int idClient;
    char nom[50];
    char prenom[50];
    float caTotal;
} StatClient;

void afficherPalmaresClients(const Boutique *b) {
    StatClient *stats = malloc(b->nbClients * sizeof(StatClient));
    if (stats == NULL) {
        printf("Erreur d'allocation.\n");
        return;
    }
    int nbStats = 0;

    // 1. Initialisation et calcul du CA par client
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].actif) {
            stats[nbStats].idClient = b->clients[i].idClient;
            strcpy(stats[nbStats].nom, b->clients[i].nom);
            strcpy(stats[nbStats].prenom, b->clients[i].prenom);
            stats[nbStats].caTotal = 0.0;

            for (int j = 0; j < b->clients[i].nbCommandes; j++) {
                if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") != 0) {
                    stats[nbStats].caTotal += b->clients[i].commandes[j].montantHT;
                }
            }
            nbStats++;
        }
    }

    // 2. Tri a bulles decroissant par CA
    for (int i = 0; i < nbStats - 1; i++) {
        for (int j = 0; j < nbStats - i - 1; j++) {
            if (stats[j].caTotal < stats[j+1].caTotal) {
                StatClient temp = stats[j];
                stats[j] = stats[j+1];
                stats[j+1] = temp;
            }
        }
    }

    // 3. Affichage du classement
    printf("\n PALMARES DES CLIENTS \n");
    int clientsActifs = 0;
    for (int i = 0; i < nbStats; i++) {
        if (stats[i].caTotal > 0) {
            printf("%d. [%d] %s %s - CA Généré : %.2f EUR HT\n",
                   i+1, stats[i].idClient, stats[i].nom, stats[i].prenom, stats[i].caTotal);
            clientsActifs++;
        }
    }

    if (clientsActifs == 0) {
        printf("Aucun chiffre d'affaires n'a ete généré.\n");
    }

    free(stats);
}
typedef struct {
    char  categorie[30];
    int   quantite;
    float caBrutHT;
} StatCategorie;

/* Quantites vendues et CA brut HT par categorie (commandes ANNULEE exclues). */
static StatCategorie *construireStatsCategories(const Boutique *b, int *nb)
{
    StatCategorie *tab = NULL;
    int n = 0, cap = 0;

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            const Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, "ANNULEE") == 0) {
                continue;
            }
            for (int k = 0; k < c->nbLignes; k++) {
                const LigneCommande *l = &c->lignes[k];
                const char *cat = "Inconnue";

                /* retrouver la categorie du produit vendu */
                for (int p = 0; p < b->nbProduits; p++) {
                    if (strcmp(b->produits[p].reference, l->refProduit) == 0) {
                        cat = b->produits[p].categorie;
                        break;
                    }
                }
                /* categorie deja presente dans le tableau ? */
                int idx;
                for (idx = 0; idx < n; idx++) {
                    if (strcmp(tab[idx].categorie, cat) == 0) {
                        break;
                    }
                }
                if (idx == n) {                       /* nouvelle categorie */
                    if (n >= cap) {
                        int nouvelle = (cap == 0) ? 4 : cap * 2;
                        StatCategorie *tmp = realloc(tab, (size_t)nouvelle * sizeof *tab);
                        if (tmp == NULL) {
                            free(tab);
                            *nb = -1;
                            return NULL;
                        }
                        tab = tmp;
                        cap = nouvelle;
                    }
                    strcpy(tab[n].categorie, cat);
                    tab[n].quantite = 0;
                    tab[n].caBrutHT = 0.0f;
                    n++;
                }
                tab[idx].quantite += l->quantite;
                tab[idx].caBrutHT += l->sousTotalHT;
            }
        }
    }
    *nb = n;
    return tab;
}

/* Tri par insertion : CA brut decroissant. */
static void trierStatsCategories(StatCategorie *t, int n)
{
    for (int i = 1; i < n; i++) {
        StatCategorie cle = t[i];
        int j = i - 1;
        while (j >= 0 && t[j].caBrutHT < cle.caBrutHT) {
            t[j + 1] = t[j];
            j--;
        }
        t[j + 1] = cle;
    }
}

void afficherStatsCategories(const Boutique *b)
{
    int n;
    StatCategorie *stats = construireStatsCategories(b, &n);

    printf("\n==== STATISTIQUES PAR CATEGORIE ====\n");
    if (n < 0) {
        printf("Erreur : allocation memoire impossible.\n");
        return;
    }
    if (n == 0) {
        printf("Aucune vente.\n");
        return;
    }
    trierStatsCategories(stats, n);
    printf("%-16s %12s %18s\n", "Categorie", "Qte vendue", "CA brut HT (EUR)");
    printf("----------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-16.16s %12d %18.2f\n", stats[i].categorie, stats[i].quantite,
               (double)stats[i].caBrutHT);
    }
    free(stats);
}