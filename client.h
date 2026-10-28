//
// Created by marie on 28/10/2026.
//

#ifndef PETIT_PROJET_CLIENT_H
#define PETIT_PROJET_CLIENT_H
#include "types.h"

/* ---- Acces / donnees ---- */
Client *trouverClient(const Boutique *b, int idClient); //Recherche un client par son identifiant unique dans la boutique.
int  adresseEstValide(const Adresse *a); //Vérifie la validité d'une adresse (champs obligatoires, format, etc.).
int  clientEstValide(const Client *c); // Vérifie la validité des données d'un client.

int  emailExiste(const Boutique *b, const char *email, int idIgnore); //Vérifie si une adresse email existe déjà dans la boutique.
int  ajouterClient(Boutique *b, const Client *modele); //Ajoute un nouveau client à la boutique à partir d'un modèle.
int  clientACommandesActives(const Client *c); //Vérifie si un client possède des commandes en cours ou actives.
int  supprimerClient(Boutique *b, int idClient); //Supprime un client de la boutique en fonction de son identifiant.

/* ---- Fidelite et chiffre d'affaires ---- */
void ajouterPointsFidelite(Client *c, int points); //Ajoute des points de fidélité au solde d'un client.
void retirerPointsFidelite(Client *c, int points); //Retire des points de fidélité au solde d'un client.
void chiffreAffairesClient(const Client *c, float *ht, float *ttc);//Calcule le chiffre d'affaires généré par un client.

/* ---- Fonctions interactives (menus) ---- */
void saisirAdresse(Adresse *a);//Invite l'utilisateur à saisir les informations d'une adresse au clavier.
void saisirEtAjouterClient(Boutique *b);//Interface interactive pour saisir et ajouter un nouveau client dans la boutique.
void modifierClient(Boutique *b);//
void supprimerClientInteractif(Boutique *b);//Interface interactive pour modifier les informations d'un client existant.

void afficherListeClients(const Boutique *b);//
void afficherClientDetaille(const Boutique *b);//Affiche la liste de tous les clients enregistrés dans la boutique.
#endif //PETIT_PROJET_CLIENT_H
