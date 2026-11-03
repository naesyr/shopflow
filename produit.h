#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

/* --- Recherche et verification --- */
Produit *trouverProduit(const Boutique *b, const char *reference);
int referenceExiste(const Boutique *b, const char *reference);
int produitEstUtilise(const Boutique *b, const char *reference);

/* --- Operations CRUD --- */
int ajouterProduit(Boutique *b, const Produit *modele);
int modifierProduit(Boutique *b, const char *reference, float nouveauPrix, int nouveauStock, int nouveauSeuil);
int supprimerProduit(Boutique *b, const char *reference);

/* --- Fonctions interactives (Menus) --- */
void saisirEtAjouterProduit(Boutique *b);
void modifierProduitInteractif(Boutique *b);
void supprimerProduitInteractif(Boutique *b);
void afficherCatalogue(const Boutique *b);
void rechercherProduitInteractif(Boutique *b);
void rechercheMultiCriteres(const Boutique *b);

#endif // PRODUIT_H