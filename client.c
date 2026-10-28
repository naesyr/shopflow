
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "client.h"
#include "commande.h"
#include "utils.h"
/* Agrandit dynamiquement la capacité du tableau de clients de la boutique. */
static int agrandirRepertoire(Boutique *b)
{
    int nouvelle = (b->capaciteClients == 0) ? CAPACITE_INITIALE : b->capaciteClients * 2;
    Client *tmp = (Client *)realloc(b->clients, (size_t)nouvelle * sizeof(Client));

    if (tmp == NULL) {
        return RES_ERR_MEMOIRE;
    }
    b->clients = tmp;
    b->capaciteClients = nouvelle;
    return RES_OK;
}

/* Cherche un client par  son identifiant ( qu'il soit actif ou non). NULL si absent. */
Client *trouverClient(const Boutique *b, int idClient)
{
    int i;

    for (i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClient) {
            return &b->clients[i];
        }
    }
    return NULL;
}

/* Vérifie si l'adresse est complète (rue, ville, pays non vides et code postal à 5 chiffres */
int adresseEstValide(const Adresse *a)
{
    return a->rue[0] != '\0' && a->ville[0] != '\0' && a->pays[0] != '\0'
        && estChaineDeChiffres(a->codePostal, 5);
}

/* Controle les contraintes d'un client (hors du l'email). */
int clientEstValide(const Client *c)
{
    return c->nom[0] != '\0' && c->prenom[0] != '\0'
        && estEmailValide(c->email)
        && estChaineDeChiffres(c->telephone, 10)
        && adresseEstValide(&c->adresse);
}

/* L'email existe-t-il deja (clients actifs ET desactives) ? idIgnore = client a exclure. */
int emailExiste(const Boutique *b, const char *email, int idIgnore)
{
    int i;

    for (i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient != idIgnore
            && comparerSansCasse(b->clients[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Ajoute un client : l'identifiant vient du compteur de la boutique.
 * Retourne l'identifiant (> 0) ou un code d'erreur (< 0). */
int ajouterClient(Boutique *b, const Client *modele)
{
    Client c;
    int r;

    if (!clientEstValide(modele)) {
        return RES_ERR_INVALIDE;
    }
    if (emailExiste(b, modele->email, 0)) {
        return RES_ERR_DOUBLON;
    }
    if (b->nbClients >= b->capaciteClients) {
        r = agrandirRepertoire(b);
        if (r != RES_OK) {
            return r;
        }
    }
    c = *modele;
    c.idClient = b->idClientSuivant;
    c.pointsFidelite = 0;
    c.nbCommandes = 0;
    c.capaciteCommandes = 0;
    c.commandes = NULL;
    c.actif = 1;
    b->clients[b->nbClients] = c;
    b->nbClients++;
    b->idClientSuivant++;
    return c.idClient;
}

/* Le client a-t-il au moins une commande non annulee ? */
int clientACommandesActives(const Client *c)
{
    int i;

    for (i = 0; i < c->nbCommandes; i++) {
        if (commandeEstActive(&c->commandes[i])) {
            return 1;
        }
    }
    return 0;
}

/* Suppression logique (refusee si des commandes non annulees existent). */
int supprimerClient(Boutique *b, int idClient)
{
    Client *c = trouverClient(b, idClient);

    if (c == NULL || !c->actif) {
        return RES_ERR_INTROUVABLE;
    }
    if (clientACommandesActives(c)) {
        return RES_ERR_REFUSE;
    }
    c->actif = 0;
    return RES_OK;
}

/* Credite des points de fidelite. */
void ajouterPointsFidelite(Client *c, int points)
{
    if (points > 0) {
        c->pointsFidelite += points;
    }
}

/* Retire des points (le solde ne descend jamais sous 0). */
void retirerPointsFidelite(Client *c, int points)
{
    if (points > 0) {
        c->pointsFidelite -= points;
        if (c->pointsFidelite < 0) {
            c->pointsFidelite = 0;
        }
    }
}

/* Chiffre d'affaires du client (commandes non annulees uniquement). */
void chiffreAffairesClient(const Client *c, float *ht, float *ttc)
{
    double sommeHT = 0.0, sommeTTC = 0.0;
    int i;

    for (i = 0; i < c->nbCommandes; i++) {
        if (commandeEstActive(&c->commandes[i])) {
            sommeHT += (double)c->commandes[i].montantHT;
            sommeTTC += (double)c->commandes[i].montantTTC;
        }
    }
    *ht = (float)sommeHT;
    *ttc = (float)sommeTTC;
}

/* Saisie d'un telephone a 10 chiffres. */
static int saisirTelephone(char *dest, size_t taille)
{
    for (;;) {
        if (!lireTexte("Telephone (10 chiffres) : ", dest, taille, 0)) {
            return 0;
        }
        if (estChaineDeChiffres(dest, 10)) {
            return 1;
        }
        printf("  /!\\ Le telephone doit contenir exactement 10 chiffres.\n");
    }
}

/* Saisie d'un email valide et unique (idIgnore = client en cours de modification). */
static int saisirEmail(const Boutique *b, int idIgnore, char *dest, size_t taille)
{
    for (;;) {
        if (!lireTexte("Email : ", dest, taille, 0)) {
            return 0;
        }
        if (!estEmailValide(dest)) {
            printf("  /!\\ Email invalide (il doit contenir un '@').\n");
        } else if (emailExiste(b, dest, idIgnore)) {
            printf("  /!\\ Cet email est deja utilise (meme par un client desactive).\n");
        } else {
            return 1;
        }
    }
}

/* Saisie d'une adresse complete (code postal a 5 chiffres). */
void saisirAdresse(Adresse *a)
{
    memset(a, 0, sizeof *a);
    if (!lireTexte("Rue : ", a->rue, sizeof a->rue, 0)) return;
    for (;;) {
        if (!lireTexte("Code postal (5 chiffres) : ", a->codePostal, sizeof a->codePostal, 0)) return;
        if (estChaineDeChiffres(a->codePostal, 5)) {
            break;
        }
        printf("  /!\\ Le code postal doit contenir 5 chiffres.\n");
    }
    if (!lireTexte("Ville : ", a->ville, sizeof a->ville, 0)) return;
    if (!lireTexte("Pays : ", a->pays, sizeof a->pays, 0)) return;
}

/* Creation interactive d'un client. */
void saisirEtAjouterClient(Boutique *b)
{
    Client c;
    int r;

    memset(&c, 0, sizeof c);
    afficherTitre("AJOUT D'UN CLIENT");
    if (!lireTexte("Nom : ", c.nom, sizeof c.nom, 0)) return;
    if (!lireTexte("Prenom : ", c.prenom, sizeof c.prenom, 0)) return;
    if (!saisirEmail(b, 0, c.email, sizeof c.email)) return;
    if (!saisirTelephone(c.telephone, sizeof c.telephone)) return;
    saisirAdresse(&c.adresse);

    r = ajouterClient(b, &c);
    if (r > 0) {
        printf("Client cree avec l'identifiant %d.\n", r);
    } else {
        printf("Echec : %s\n", messageErreur(r));
    }
}

/* Modification des coordonnees et de l'adresse (les commandes gardent leur adresse figee). */
void modifierClient(Boutique *b)
{
    int id;
    Client *c;
    char email[LONG_EMAIL];
    char tel[LONG_TELEPHONE];
    Adresse adr;

    afficherTitre("MISE A JOUR D'UN CLIENT");
    if (!lireEntier("Identifiant du client : ", 1, INT_MAX, &id)) return;
    c = trouverClient(b, id);
    if (c == NULL || !c->actif) {
        printf("Client introuvable.\n");
        return;
    }
    printf("Client : %s %s (%s)\n", c->prenom, c->nom, c->email);
    if (!saisirEmail(b, id, email, sizeof email)) return;
    if (!saisirTelephone(tel, sizeof tel)) return;
    printf("Nouvelle adresse de livraison :\n");
    saisirAdresse(&adr);
    if (!adresseEstValide(&adr)) {
        printf("Adresse incomplete : modification annulee.\n");
        return;
    }
    strcpy(c->email, email);
    strcpy(c->telephone, tel);
    c->adresse = adr;
    printf("Client %d mis a jour.\n", id);
}

/* Suppression logique interactive. */
void supprimerClientInteractif(Boutique *b)
{
    int id, r;

    afficherTitre("SUPPRESSION D'UN CLIENT");
    if (!lireEntier("Identifiant du client : ", 1, INT_MAX, &id)) return;
    r = supprimerClient(b, id);
    if (r == RES_OK) {
        printf("Client %d desactive (suppression logique).\n", id);
    } else if (r == RES_ERR_REFUSE) {
        printf("Refuse : ce client a des commandes non annulees.\n");
    } else {
        printf("Echec : %s\n", messageErreur(r));
    }
}

/* Liste des clients actifs : nombre de commandes et chiffre d'affaires. */
void afficherListeClients(const Boutique *b)
{
    int i, n = 0;

    afficherTitre("LISTE DES CLIENTS");
    printf("%-4s %-14s %-12s %-26s %6s %5s %11s %11s\n",
           "ID", "Nom", "Prenom", "Email", "Points", "Cmd", "CA HT", "CA TTC");
    afficherSeparateur();
    for (i = 0; i < b->nbClients; i++) {
        const Client *c = &b->clients[i];
        float ht, ttc;
        if (!c->actif) {
            continue;
        }
        chiffreAffairesClient(c, &ht, &ttc);
        printf("%-4d %-14.14s %-12.12s %-26.26s %6d %5d %11.2f %11.2f\n",
               c->idClient, c->nom, c->prenom, c->email, c->pointsFidelite,
               c->nbCommandes, (double)ht, (double)ttc);
        n++;
    }
    printf("%d client(s) actif(s).\n", n);
}

/* Fiche detaillee d'un client avec l'historique de ses commandes. */
void afficherClientDetaille(const Boutique *b)
{
    int id, j;
    const Client *c;
    float ht, ttc;

    afficherTitre("FICHE CLIENT");
    if (!lireEntier("Identifiant du client : ", 1, INT_MAX, &id)) return;
    c = trouverClient(b, id);
    if (c == NULL || !c->actif) {
        printf("Client introuvable.\n");
        return;
    }
    chiffreAffairesClient(c, &ht, &ttc);
    printf("Client #%d : %s %s\n", c->idClient, c->prenom, c->nom);
    printf("Email : %s - Tel : %s\n", c->email, c->telephone);
    printf("Adresse : %s, %s %s (%s)\n", c->adresse.rue, c->adresse.codePostal,
           c->adresse.ville, c->adresse.pays);
    printf("Points de fidelite : %d\n", c->pointsFidelite);
    printf("Chiffre d'affaires (hors annulees) : %.2f EUR HT / %.2f EUR TTC\n",
           (double)ht, (double)ttc);
    printf("\nHistorique des commandes (%d) :\n", c->nbCommandes);
    if (c->nbCommandes == 0) {
        printf("  Aucune commande.\n");
        return;
    }
    for (j = 0; j < c->nbCommandes; j++) {
        afficherResumeCommande(&c->commandes[j]);
    }
}

//
// Created by marie on 28/10/2026.
//
