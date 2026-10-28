#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

/* Retire les espaces en debut et fin de chaine (en place). */
void supprimerEspacesBords(char *s)
{
    size_t debut = 0;
    size_t fin = strlen(s);

    while (s[debut] != '\0' && isspace((unsigned char)s[debut])) {
        debut++;
    }
    while (fin > debut && isspace((unsigned char)s[fin - 1])) {
        fin--;
    }
    memmove(s, s + debut, fin - debut);
    s[fin - debut] = '\0';
}

/* Lit une ligne sur stdin (sans '\n'). Retourne 0 en cas de fin de fichier.
 * Une ligne trop longue est tronquee et le reste est vide du tampon. */
int lireLigne(char *dest, size_t taille)
{
    size_t len;

    if (fgets(dest, (int)taille, stdin) == NULL) {
        return 0;
    }
    len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') {
        dest[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* on jette la fin de la ligne */
        }
    }
    return 1;
}

/* Saisit un texte (longueur < taille). Boucle jusqu'a obtenir une saisie valide.
 * Retourne 0 si fin de fichier. */
int lireTexte(const char *invite, char *dest, size_t taille, int autoriserVide)
{
    char tampon[256];

    for (;;) {
        printf("%s", invite);
        fflush(stdout);
        if (!lireLigne(tampon, sizeof tampon)) {
            return 0;
        }
        supprimerEspacesBords(tampon);
        if (tampon[0] == '\0' && !autoriserVide) {
            printf("  /!\\ La saisie ne peut pas etre vide.\n");
            continue;
        }
        if (strlen(tampon) >= taille) {
            printf("  /!\\ Trop long (%d caracteres maximum).\n", (int)(taille - 1));
            continue;
        }
        strcpy(dest, tampon);
        return 1;
    }
}

/* Saisit un entier dans [min, max]. Retourne 0 si fin de fichier. */
int lireEntier(const char *invite, int min, int max, int *valeur)
{
    char tampon[64];

    for (;;) {
        char *fin;
        long v;

        printf("%s", invite);
        fflush(stdout);
        if (!lireLigne(tampon, sizeof tampon)) {
            return 0;
        }
        supprimerEspacesBords(tampon);
        v = strtol(tampon, &fin, 10);
        if (tampon[0] == '\0' || *fin != '\0') {
            printf("  /!\\ Veuillez entrer un nombre entier.\n");
            continue;
        }
        if (v < min || v > max) {
            printf("  /!\\ Valeur attendue entre %d et %d.\n", min, max);
            continue;
        }
        *valeur = (int)v;
        return 1;
    }
}

/* Saisit un reel dans [min, max] (virgule ou point accepte). */
int lireFlottant(const char *invite, float min, float max, float *valeur)
{
    char tampon[64];

    for (;;) {
        char *fin;
        double v;
        char *virgule;

        printf("%s", invite);
        fflush(stdout);
        if (!lireLigne(tampon, sizeof tampon)) {
            return 0;
        }
        supprimerEspacesBords(tampon);
        virgule = strchr(tampon, ',');
        if (virgule != NULL) {
            *virgule = '.';
        }
        v = strtod(tampon, &fin);
        if (tampon[0] == '\0' || *fin != '\0') {
            printf("  /!\\ Veuillez entrer un nombre (ex : 12.50).\n");
            continue;
        }
        if (!(v >= (double)min && v <= (double)max)) {
            printf("  /!\\ Valeur attendue entre %.2f et %.2f.\n", (double)min, (double)max);
            continue;
        }
        *valeur = (float)v;
        return 1;
    }
}

/* Pose une question oui/non. Retourne 1 pour oui, 0 pour non (ou fin de fichier). */
int lireOuiNon(const char *invite)
{
    char tampon[16];

    for (;;) {
        printf("%s (o/n) : ", invite);
        fflush(stdout);
        if (!lireLigne(tampon, sizeof tampon)) {
            return 0;
        }
        supprimerEspacesBords(tampon);
        if (comparerSansCasse(tampon, "o") == 0 || comparerSansCasse(tampon, "oui") == 0) {
            return 1;
        }
        if (comparerSansCasse(tampon, "n") == 0 || comparerSansCasse(tampon, "non") == 0) {
            return 0;
        }
        printf("  /!\\ Repondez par o ou n.\n");
    }
}

/* Annee bissextile ? */
int estBissextile(int annee)
{
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

/* Nombre de jours d'un mois (gere les annees bissextiles). */
int joursDansMois(int mois, int annee)
{
    static const int jours[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mois < 1 || mois > 12) {
        return 0;
    }
    if (mois == 2 && estBissextile(annee)) {
        return 29;
    }
    return jours[mois - 1];
}

/* Valide une date : annee 2000-2100, mois 1-12, jour selon le mois. */
int estDateValide(const Date *d)
{
    if (d->annee < ANNEE_MIN || d->annee > ANNEE_MAX) {
        return 0;
    }
    if (d->mois < 1 || d->mois > 12) {
        return 0;
    }
    return d->jour >= 1 && d->jour <= joursDansMois(d->mois, d->annee);
}

/* Date du jour (repli sur le 01/01/2026 si l'horloge est hors limites). */
Date dateDuJour(void)
{
    Date d = {1, 1, 2026};
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    if (lt != NULL) {
        d.jour = lt->tm_mday;
        d.mois = lt->tm_mon + 1;
        d.annee = lt->tm_year + 1900;
    }
    if (!estDateValide(&d)) {
        d.jour = 1;
        d.mois = 1;
        d.annee = 2026;
    }
    return d;
}

/* La chaine contient-elle exactement 'longueur' chiffres ? */
int estChaineDeChiffres(const char *s, size_t longueur)
{
    size_t i;

    if (strlen(s) != longueur) {
        return 0;
    }
    for (i = 0; i < longueur; i++) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }
    return 1;
}

/* Adresse electronique : un seul '@', partie locale et domaine non vides, pas d'espace. */
int estEmailValide(const char *email)
{
    const char *arobase = strchr(email, '@');
    size_t i;

    if (arobase == NULL || arobase == email || arobase[1] == '\0') {
        return 0;
    }
    if (strchr(arobase + 1, '@') != NULL) {
        return 0;
    }
    for (i = 0; email[i] != '\0'; i++) {
        if (isspace((unsigned char)email[i])) {
            return 0;
        }
    }
    return 1;
}

/* Arrondi au centieme (valeurs positives). */
float arrondi2(double x)
{
    return (float)((double)((long long)(x * 100.0 + 0.5)) / 100.0);
}

/* Comparaison de chaines insensible a la casse. */
int comparerSansCasse(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/* Le texte contient-il le mot-cle (sans tenir compte de la casse) ? */
int contientMotCle(const char *texte, const char *motCle)
{
    char t[256];
    char m[256];
    size_t i;

    for (i = 0; i < sizeof t - 1 && texte[i] != '\0'; i++) {
        t[i] = (char)tolower((unsigned char)texte[i]);
    }
    t[i] = '\0';
    for (i = 0; i < sizeof m - 1 && motCle[i] != '\0'; i++) {
        m[i] = (char)tolower((unsigned char)motCle[i]);
    }
    m[i] = '\0';
    return strstr(t, m) != NULL;
}

/* Passe la chaine en majuscules (en place). */
void mettreEnMajuscules(char *s)
{
    for (; *s != '\0'; s++) {
        *s = (char)toupper((unsigned char)*s);
    }
}

/* Titre de section. */
void afficherTitre(const char *titre)
{
    printf("\n==== %s ====\n", titre);
}

/* Ligne de separation. */
void afficherSeparateur(void)
{
    printf("--------------------------------------------------------------------------------\n");
}

/* Affiche une date jj/mm/aaaa. */
void afficherDate(const Date *d)
{
    printf("%02d/%02d/%04d", d->jour, d->mois, d->annee);
}

/* Message lisible pour un code de retour. */
const char *messageErreur(int code)
{
    switch (code) {
    case RES_OK:              return "Operation reussie.";
    case RES_ERR_MEMOIRE:     return "Erreur d'allocation memoire.";
    case RES_ERR_INTROUVABLE: return "Element introuvable.";
    case RES_ERR_DOUBLON:     return "Element deja existant (unicite non respectee).";
    case RES_ERR_INVALIDE:    return "Donnees invalides.";
    case RES_ERR_REFUSE:      return "Operation refusee par les regles metier.";
    case RES_ERR_STOCK:       return "Stock insuffisant.";
    default:                  return "Erreur inconnue.";
    }
}

//
// Created by marie on 28/10/2026.
//
