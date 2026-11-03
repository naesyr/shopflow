/* ==========================================================================
 * menu.c - Squelette de demarrage pour tester le projet (XTI101)
 * --------------------------------------------------------------------------
 * BUT : un programme qui COMPIL ET TOURNE en 1 commande, des le premier jour.
 *
 *
 * CONTENU : navigation complete (4 sous-menus) + bouchons [STUB] pour
 * chaque fonctionnalite. Vous devez changer les stubs,
 * un par un, par leurs vraies fonctions ecrites dans leurs modules :
 *
 * Le cycle de travail (à répéter pour chaque fonctionnalité)
 * Copy1. CHOISIR une fonctionnalité du menu           (ex. « ajouter un produit »)
 * 2. ÉCRIRE la fonction dans son module           (produit.c)
 * 3. DÉCLARER le prototype dans son header        (produit.h)
 * 4. INCLURE le header dans menu_rapide.c         (#include "produit.h")
 * 5. SUPPRIMER le stub correspondant              (la ligne [STUB] ajouterProduit)
 * 6. RECOMPILER et tester
 *
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

/* --------------------------------------------------------------------------
 * Lecture d'un choix au clavier (version simple, suffisant pour demarrer).
 * A remplacer plus tard par une saisie securisee (module utils.c).
 * -------------------------------------------------------------------------- */
static int lireChoix(void)
{
    int choix;
    printf("  Votre choix : ");
    if (scanf("%d", &choix) != 1)
    {
        exit(EXIT_FAILURE); /* entree fermee : on quitte proprement */
    }
    /* Nettoyage du buffer ajouté pour éviter les boucles infinies avec les vraies fonctions */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    return choix;
}

/* ==========================================================================
 * STUBS : affichent un message pour PROUVER que l'appel arrive.
 * A REMPLACER par les vraies fonctions des modules au fil des seances.
 * ========================================================================== */

/* --- Module PRODUITS ------------------------------------------------------ */
void ajouterProduitStub(void)      { printf("  [STUB] ajouterProduit - a implementer dans produit.c\n"); }
void modifierProduitStub(void)     { printf("  [STUB] modifierProduit - a implementer dans produit.c\n"); }
void supprimerProduitStub(void)    { printf("  [STUB] supprimerProduit - a implementer dans produit.c\n"); }
void afficherCatalogueStub(void)   { printf("  [STUB] afficherCatalogue - a implementer dans produit.c\n"); }

/* --- Module CLIENTS ------------------------------------------------------- */
/* STUBS SUPPRIMES (Etape 5 du cycle) car remplaces par les fonctions de client.c */

/* --- Module COMMANDES ----------------------------------------------------- */
/* STUBS SUPPRIMES (Etape 5 du cycle) car remplaces par les fonctions de commande.c */

/* --- Module STATISTIQUES -------------------------------------------------- */
void afficherPalmaresProduitsStub(void) { printf("  [STUB] afficherPalmaresProduits - a implementer dans statistique.c\n"); }
void afficherPalmaresClientsStub(void) { printf("  [STUB] afficherPalmaresClients - a implementer dans statistique.c\n"); }
void afficherAlertesStockStub(void) { printf("  [STUB] afficherAlertesStock - a implementer dans statistique.c\n"); }

/* ==========================================================================
 * SOUS-MENUS : un seul motif a retenir, applique 4 fois.
 * Pour ajouter une fonctionnalite : un printf + un case + la borne max.
 * ========================================================================== */

static void menuProduits(Boutique *b)
{
    (void)b; /* Pour eviter le warning du compilateur en attendant l'implementation */
    int choix;
    do
    {
        printf("\n===== GESTION DES PRODUITS =====\n");
        printf("  1. Ajouter un produit\n");
        printf("  2. Modifier un produit\n");
        printf("  3. Supprimer un produit\n");
        printf("  4. Afficher le catalogue\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: ajouterProduitStub();    break;
            case 2: modifierProduitStub();   break;
            case 3: supprimerProduitStub();  break;
            case 4: afficherCatalogueStub(); break;
            case 0:                      break; /* retour automatique */
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
        printf("  5. Afficher la fiche detaillee d'un client\n"); /* Ajout pour utiliser la fonction existante */
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
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: calculerChiffreAffaires(b);      break; /* Fonction réelle implémentée */
            case 2: afficherPalmaresProduitsStub();  break;
            case 3: afficherPalmaresClientsStub();   break;
            case 4: afficherAlertesStockStub();      break;
            case 0:                                  break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

/* ==========================================================================
 * PROGRAMME PRINCIPAL
 * ========================================================================== */
int main(void)
{
    int choix;
    Boutique maBoutique;

    /* Initialisation obligatoire pour pouvoir passer 'maBoutique' aux menus */
    memset(&maBoutique, 0, sizeof(Boutique));
    
    /* A DECOMMENTER QUAND LE MODULE INITIALISATION SERA PRET :
    initialiserDonneesTest(&maBoutique); 
    */

    printf("==============================================\n");
    printf("  SHOPFLOW - Boutique en ligne (squelette)\n");
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
                printf("\n  Au revoir !\n"); 
                /* A DECOMMENTER POUR LIBERER LA MEMOIRE :
                detruireBoutique(&maBoutique); 
                */
                break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);

    return EXIT_SUCCESS;
}