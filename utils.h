//
// Created by marie on 28/10/2026.
//

#ifndef PETIT_PROJET_UTILS_H
#define PETIT_PROJET_UTILS_H
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include "types.h" //
// Bornes pour la validation des dates
#define ANNEE_MIN 2000
#define ANNEE_MAX 2100

// Codes de retour (utilisés dans messageErreur)
#define RES_OK              1
#define RES_ERR_MEMOIRE     2
#define RES_ERR_INTROUVABLE 3
#define RES_ERR_DOUBLON     4
#define RES_ERR_INVALIDE    5
#define RES_ERR_REFUSE      6
#define RES_ERR_STOCK       7

// --- Prototypes des fonctions utilitaires ---
void supprimerEspacesBords(char *s);
int lireLigne(char *dest, size_t taille);
int lireTexte(const char *invite, char *dest, size_t taille, int autoriserVide);
int lireEntier(const char *invite, int min, int max, int *valeur);
int lireFlottant(const char *invite, float min, float max, float *valeur);
int lireOuiNon(const char *invite);
int estBissextile(int annee);
int joursDansMois(int mois, int annee);
int estDateValide(const Date *d);
Date dateDuJour(void);
int estChaineDeChiffres(const char *s, size_t longueur);
int estEmailValide(const char *email);
float arrondi2(double x);
int comparerSansCasse(const char *a, const char *b);
int contientMotCle(const char *texte, const char *motCle);
void mettreEnMajuscules(char *s);
void afficherTitre(const char *titre);
void afficherSeparateur(void);
void afficherDate(const Date *d);
const char *messageErreur(int code);

#endif // UTILS_H

#endif //PETIT_PROJET_UTILS_H
