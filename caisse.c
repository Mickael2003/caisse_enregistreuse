#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Structure pour représenter le stock de billets
typedef struct {
    int valeur;
    int quantite;
} StockBillet;

bool stock_suffisant(double rendu_necessaire, StockBillet *stock_billets, int nb_types_billets) {
    double rendu_test = rendu_necessaire;
    for (int i = 0; i < nb_types_billets; i++) {
        int nb_billets_a_rendre = floor(rendu_test / stock_billets[i].valeur);
        if (nb_billets_a_rendre > 0 && stock_billets[i].quantite >= nb_billets_a_rendre) {
            rendu_test -= nb_billets_a_rendre * stock_billets[i].valeur;
        }
    }
    return rendu_test <= 0.01;
}

void calcule_monnaie(double rendu, StockBillet *stock_billets, int nb_types_billets) {
    int nb_billets_a_rendre;
    double rendu_initial = rendu;
    int i;

    printf("Monnaie à rendre : %.2f\n", rendu_initial);
    printf("Billet(s) à rendre :\n");

    for (i = 0; i < nb_types_billets; i++) {
        nb_billets_a_rendre = floor(rendu / stock_billets[i].valeur);
        if (nb_billets_a_rendre > 0 && stock_billets[i].quantite >= nb_billets_a_rendre) {
            printf("%d billet(s) de %d\n", nb_billets_a_rendre, stock_billets[i].valeur);
            rendu -= nb_billets_a_rendre * stock_billets[i].valeur;
            stock_billets[i].quantite -= nb_billets_a_rendre;
        } else if (nb_billets_a_rendre > 0) {
            printf("Attention : Stock insuffisant pour les billets de %d (nécessaire %d, disponible %d)\n",
                   stock_billets[i].valeur, nb_billets_a_rendre, stock_billets[i].quantite);
        }
    }

    printf("Pièce(s) à rendre (%.2f restant):\n", rendu);
    int rendu_en_centimes = round(rendu * 100);
    double pieces[] = {2, 1, 0.50, 0.20, 0.10, 0.05, 0.01};
    int nb_pieces;
    int valeur_piece_en_centimes;
    int nb_types_pieces = sizeof(pieces) / sizeof(pieces[0]);

    for (i = 0; i < nb_types_pieces; i++) {
        valeur_piece_en_centimes = round(pieces[i] * 100);
        nb_pieces = rendu_en_centimes / valeur_piece_en_centimes;
        if (nb_pieces > 0) {
            printf("%d pièce(s) de %.2f\n", nb_pieces, pieces[i]);
            rendu_en_centimes -= nb_pieces * valeur_piece_en_centimes;
        }
    }
    if (rendu_en_centimes > 0) {
        printf("Avertissement : Impossible de rendre la monnaie exacte avec le stock actuel de pièces.\n");
    }
}

int main() {
    double valeur_a_payer;
    double somme_recue;
    double rendu;
    bool continuer_transactions = true;
    int choix;

    StockBillet stock_billets[] = {
        {200, 2},
        {100, 2},
        {50, 2},
        {20, 2},
        {10, 2},
        {5, 2}
    };
    int nb_types_billets = sizeof(stock_billets) / sizeof(stock_billets[0]);

    while (continuer_transactions) {
        // Affichage du stock de billets avant la transaction
        printf("\n--- Stock de billets actuel ---\n");
        for (int i = 0; i < nb_types_billets; i++) {
            printf("%d billets de %d\n", stock_billets[i].quantite, stock_billets[i].valeur);
        }
        printf("-----------------------------\n");

        printf("Entrez la valeur à payer (0 pour quitter) : ");
        if (scanf("%lf", &valeur_a_payer) != 1 || valeur_a_payer == 0) {
            continuer_transactions = false;
            continue;
        }

        printf("Entrez la somme donnée par le client : ");
        if (scanf("%lf", &somme_recue) != 1) {
            printf("Erreur de saisie pour la somme reçue.\n");
            while (getchar() != '\n'); // Nettoyer le buffer d'entrée
            continue;
        }

        if (somme_recue < valeur_a_payer) {
            printf("La somme reçue est insuffisante.\n");
        } else {
            rendu = somme_recue - valeur_a_payer;
            if (stock_suffisant(rendu, stock_billets, nb_types_billets)) {
                calcule_monnaie(rendu, stock_billets, nb_types_billets);
            } else {
                printf("Avertissement : Stock de billets insuffisant pour rendre %.2f.\n", rendu);
                printf("Que souhaitez-vous faire ?\n");
                printf("1. Fermer la caisse et arrêter le programme\n");
                printf("2. Remplir la caisse et continuer le programme\n");
                printf("Entrez votre choix (1 ou 2) : ");
                if (scanf("%d", &choix) == 1) {
                    if (choix == 1) {
                        continuer_transactions = false;
                    } else if (choix == 2) {
                        // Ici, vous ajouteriez la logique pour remplir la caisse.
                        // Pour cet exemple simple, nous allons réinitialiser le stock.
                        printf("Caisse réapprovisionnée.\n");
                        for (int i = 0; i < nb_types_billets; i++) {
                            stock_billets[i].quantite = 5; // Réinitialisation du stock
                        }
                    } else {
                        printf("Choix invalide. La caisse va être fermée.\n");
                        continuer_transactions = false;
                    }
                } else {
                    printf("Erreur de saisie. La caisse va être fermée.\n");
                    continuer_transactions = false;
                    while (getchar() != '\n'); // Nettoyer le buffer d'entrée
                }
            }
        }
    }

    printf("Fin du programme.\n");

    return 0;
}
