#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "types.h"
#include "produit.h"
#include "utils.h"

#define CAPACITE_INITIALE 10

static int agrandirCatalogue(Boutique *b) {
    int nouvelle = ((*b).capaciteProduits == 0) ? CAPACITE_INITIALE : (*b).capaciteProduits * 2;
    Produit *tmp = (Produit *)realloc((*b).produits, (size_t)nouvelle * sizeof(Produit));
    if (tmp == NULL) {
        return RES_ERR_MEMOIRE;
    }
    (*b).produits = tmp;
    (*b).capaciteProduits = nouvelle;
    return RES_OK;
}

Produit *trouverProduit(const Boutique *b, const char *reference) {
    for (int i = 0; i < (*b).nbProduits; i++) {
        if (strcmp((*b).produits[i].reference, reference) == 0) {
            return &(*b).produits[i];
        }
    }
    return NULL;
}

int referenceExiste(const Boutique *b, const char *reference) {
    return trouverProduit(b, reference) != NULL;
}

int produitEstUtilise(const Boutique *b, const char *reference) {
    for (int i = 0; i < (*b).nbClients; i++) {
        for (int j = 0; j < (*b).clients[i].nbCommandes; j++) {
            const Commande *cmd = &(*b).clients[i].commandes[j];
            if (strcmp((*cmd).statut, "ANNULEE") != 0) {
                for (int k = 0; k < (*cmd).nbLignes; k++) {
                    if (strcmp((*cmd).lignes[k].refProduit, reference) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int ajouterProduit(Boutique *b, const Produit *modele) {
    if (referenceExiste(b, (*modele).reference)) {
        return RES_ERR_DOUBLON;
    }
    if ((*b).nbProduits >= (*b).capaciteProduits) {
        int r = agrandirCatalogue(b);
        if (r != RES_OK) return r;
    }
    (*b).produits[(*b).nbProduits] = *modele;
    (*b).produits[(*b).nbProduits].actif = 1;
    (*b).nbProduits++;
    return RES_OK;
}

void saisirEtAjouterProduit(Boutique *b) {
    Produit p;
    memset(&p, 0, sizeof(p));
    afficherTitre("AJOUT D'UN PRODUIT");

    if (!lireTexte("Reference (ex: PRD-0001) : ", p.reference, sizeof(p.reference), 0)) return;
    if (referenceExiste(b, p.reference)) {
        printf("Erreur : Cette reference existe deja.\n");
        return;
    }
    if (!lireTexte("Designation : ", p.designation, sizeof(p.designation), 0)) return;
    if (!lireTexte("Categorie : ", p.categorie, sizeof(p.categorie), 0)) return;

    if (!lireFlottant("Prix unitaire HT : ", 0.01f, 100000.0f, &p.prixUnitaireHT)) return;
    if (!lireEntier("Quantite en stock : ", 0, INT_MAX, &p.quantiteStock)) return;
    if (!lireEntier("Seuil d'alerte stock : ", 0, INT_MAX, &p.seuilAlerte)) return;

    int r = ajouterProduit(b, &p);
    if (r == RES_OK) {
        printf("Produit %s ajoute avec succes.\n", p.reference);
    } else {
        printf("Echec de l'ajout : %s\n", messageErreur(r));
    }
}

void modifierProduitInteractif(Boutique *b) {
    char ref[12];
    afficherTitre("MODIFICATION D'UN PRODUIT");
    if (!lireTexte("Reference du produit : ", ref, sizeof(ref), 0)) return;

    Produit *p = trouverProduit(b, ref);
    if (p == NULL || !(*p).actif) {
        printf("Produit introuvable.\n");
        return;
    }

    printf("Produit actuel : %s | Prix: %.2f EUR | Stock: %d | Seuil: %d\n",
           (*p).designation, (*p).prixUnitaireHT, (*p).quantiteStock, (*p).seuilAlerte);

    lireFlottant("Nouveau prix HT : ", 0.01f, 100000.0f, &(*p).prixUnitaireHT);
    lireEntier("Nouveau stock : ", 0, INT_MAX, &(*p).quantiteStock);
    lireEntier("Nouveau seuil d'alerte : ", 0, INT_MAX, &(*p).seuilAlerte);

    printf("Produit %s mis a jour.\n", ref);
}

void supprimerProduitInteractif(Boutique *b) {
    char ref[12];
    afficherTitre("SUPPRESSION LOGIQUE D'UN PRODUIT");
    if (!lireTexte("Reference du produit a supprimer : ", ref, sizeof(ref), 0)) return;

    Produit *p = trouverProduit(b, ref);
    if (p == NULL || !(*p).actif) {
        printf("Produit introuvable.\n");
        return;
    }

    if (produitEstUtilise(b, ref)) {
        printf("Refuse : ce produit est present dans au moins une commande active.\n");
        return;
    }

    (*p).actif = 0;
    printf("Produit %s desactive (suppression logique).\n", ref);
}

void afficherCatalogue(const Boutique *b) {
    afficherTitre("CATALOGUE DES PRODUITS");
    printf("%-12s %-30s %-15s %10s %7s %7s %s\n",
           "Reference", "Designation", "Categorie", "Prix HT", "Stock", "Seuil", "Alerte");
    afficherSeparateur();

    int n = 0;
    float valeurTotaleHT = 0.0;

    for (int i = 0; i < (*b).nbProduits; i++) {
        const Produit *p = &(*b).produits[i];
        if (!(*p).actif) continue;

        char alerte[5] = "OK";
        if ((*p).quantiteStock <= (*p).seuilAlerte) {
            strcpy(alerte, "/!\\");
        }

        printf("%-12s %-30.30s %-15.15s %10.2f %7d %7d   %s\n",
               (*p).reference, (*p).designation, (*p).categorie,
               (*p).prixUnitaireHT, (*p).quantiteStock, (*p).seuilAlerte, alerte);
        n++;
        valeurTotaleHT += ((*p).quantiteStock * (*p).prixUnitaireHT);
    }
    printf("%d produit(s) actif(s) en catalogue. Valeur totale du stock : %.2f EUR HT\n", n, valeurTotaleHT);
}

void rechercherProduitInteractif(Boutique *b) {
    char motCle[60];
    afficherTitre("RECHERCHE DE PRODUIT");
    if (!lireTexte("Entrez un mot-cle ou une reference : ", motCle, sizeof(motCle), 0)) return;

    printf("%-12s %-30s %-15s %10s %7s\n", "Reference", "Designation", "Categorie", "Prix HT", "Stock");
    afficherSeparateur();

    int trouves = 0;
    for (int i = 0; i < (*b).nbProduits; i++) {
        const Produit *p = &(*b).produits[i];
        if (!(*p).actif) continue;

        if (strstr((*p).reference, motCle) != NULL || strstr((*p).designation, motCle) != NULL) {
            printf("%-12s %-30.30s %-15.15s %10.2f %7d\n",
                   (*p).reference, (*p).designation, (*p).categorie, (*p).prixUnitaireHT, (*p).quantiteStock);
            trouves++;
        }
    }
    printf("%d produit(s) trouve(s).\n", trouves);
}
// Dans produit.c
void afficherProduit(Produit *p) {
    printf("----------------------------------------\n");
    printf("Nom : %s\n", p->reference);
    printf("Categorie : %s\n", p->categorie);
    printf("Prix HT : %.2f €\n", p->prixUnitaireHT);
    printf("Stock : %d\n", p->quantiteStock);
}
void rechercheMultiCriteres(const Boutique *b)
{
    char categorie[30];
    float pmin, pmax;
    int enStock, i, n = 0;

    printf("\n=== RECHERCHE MULTI-CRITERES ===\n");
    printf("Categorie (* = toutes) : ");
    scanf("%s", categorie);
    for (;;) {
        printf("Prix HT minimum : ");
        scanf("%f", &pmin);
        printf("Prix HT maximum : ");
        scanf("%f", &pmax);
        if (pmin <= pmax) {
            break;
        }
        printf("  Le minimum doit etre inferieur ou egal au maximum.\n");
    }
    printf("Uniquement les produits en stock (1=oui, 0=non) : ");
    scanf("%d", &enStock);

    for (i = 0; i < b->nbProduits; i++) {
        const Produit *p = &b->produits[i];
        if (!p->actif) continue;
        if (strcmp(categorie, "*") != 0 && strcmp(p->categorie, categorie) != 0) continue;
        if (p->prixUnitaireHT < pmin || p->prixUnitaireHT > pmax) continue;
        if (enStock && p->quantiteStock <= 0) continue;
        afficherProduit(p);
        n++;
    }
    printf("%d resultat(s).\n", n);
}
