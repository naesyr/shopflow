/* ==========================================================================
 * main.c - Programme principal du projet SHOPFLOW
 * ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "statistique.h"
#include "commande.h"
#include "utils.h"
#include "client.h"
#include "produit.h"
#include "initialisation.h"

static int lireChoix(void)
{
    int choix;
    printf("  Votre choix : ");
    if (scanf("%d", &choix) != 1)
    {
        exit(EXIT_FAILURE);
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return choix;
}

static void menuProduits(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES PRODUITS =====\n");
        printf("  1. Ajouter un produit\n");
        printf("  2. Modifier un produit\n");
        printf("  3. Supprimer un produit\n");
        printf("  4. Afficher le catalogue\n");
        printf("  5. Rechercher un produit\n");
        printf("  6. Recherche multi-criteres\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: saisirEtAjouterProduit(b);       break;
            case 2: modifierProduitInteractif(b);    break;
            case 3: supprimerProduitInteractif(b);   break;
            case 4: afficherCatalogue(b);             break;
            case 5: rechercherProduitInteractif(b);   break;
            case 6: rechercheMulticritere(b);        break;
            case 0:                                   break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuClients(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES CLIENTS =====\n");
        printf("  1. Ajouter un client\n");
        printf("  2. Modifier un client\n");
        printf("  3. Supprimer un client\n");
        printf("  4. Afficher la liste des clients\n");
        printf("  5. Afficher la fiche detaillee d'un client\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: saisirEtAjouterClient(b);      break;
            case 2: modifierClient(b);             break;
            case 3: supprimerClientInteractif(b);  break;
            case 4: afficherListeClients(b);       break;
            case 5: afficherClientDetaille(b);     break;
            case 0:                                break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuCommandes(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES COMMANDES =====\n");
        printf("  1. Creer une commande\n");
        printf("  2. Afficher une commande\n");
        printf("  3. Changer le statut d'une commande\n");
        printf("  4. Annuler une commande\n");
        printf("  5. Modifier les lignes d'une commande\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: creerCommande(b);          break;
            case 2: afficherDetailCommande(b); break;
            case 3: changerStatutCommande(b);  break;
            case 4: annulerCommande(b);        break;
            case 5: modifierLignesCommande(b); break;
            case 0:                            break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuStatistiques(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== STATISTIQUES =====\n");
        printf("  1. Chiffre d'affaires global\n");
        printf("  2. Palmares des produits\n");
        printf("  3. Palmares des clients\n");
        printf("  4. Alertes de stock\n");
        printf("  5. Statistiques par categorie\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: calculerChiffreAffaires(b);  break;
            case 2: afficherPalmaresProduits(b); break;
            case 3: afficherPalmaresClients(b);  break;
            case 4: afficherAlertesStock(b);     break;
            case 5: afficherStatCategorie(b);    break;
            case 0:                              break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

int main(void)
{
    int choix;
    Boutique maBoutique;

    memset(&maBoutique, 0, sizeof(Boutique));

    if (!initialiserDonneesTest(&maBoutique)) {
        printf("Erreur lors de l'initialisation des donnees de test.\n");
        return EXIT_FAILURE;
    }

    printf("==============================================\n");
    printf("  SHOPFLOW - Boutique en ligne\n");
    printf("==============================================\n");

    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("  1. Gestion des produits\n");
        printf("  2. Gestion des clients\n");
        printf("  3. Gestion des commandes\n");
        printf("  4. Statistiques\n");
        printf("  0. Quitter\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: menuProduits(&maBoutique);     break;
            case 2: menuClients(&maBoutique);      break;
            case 3: menuCommandes(&maBoutique);    break;
            case 4: menuStatistiques(&maBoutique); break;
            case 0:
                printf("\nLiberation de la memoire...\n");
                detruireBoutique(&maBoutique);
                printf("Au revoir !\n");
                break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);

    return EXIT_SUCCESS;
}