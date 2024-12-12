typedef struct {
    int taille;
} region;

typedef struct {
    int taille;
    int **cases;
    region **regions;
} carte;

carte creerCarte(int taille) {
    carte map;
    map.taille = taille;

    map.cases = malloc(taille * sizeof(int *));
    for (int i = 0; i < taille; i++) {
        map.cases[i] = malloc(taille * sizeof(int));
    }

    map.regions = malloc(taille * sizeof(region));
    for (int i = 0; i < taille; i++) {
        map.regions[i] = malloc(taille * sizeof(region));
    }

    return map;
}

void detruireCarte(carte map) {
    for (int i = 0; i < map.taille; i++) {
        free(map.cases[i]);
    }
    free(map.cases);
}

void genererCarte(carte map) {
    for (int i = 0; i < map.taille; i++) {
        for (int j = 0; j < map.taille; j++) {
            map.cases[i][j] = rand() % 100;

            if (map.cases[i][j] > 75) {

            }
        }
    }
}
