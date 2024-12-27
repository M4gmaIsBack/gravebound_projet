#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../map/procedural.h"

#define SCALE 0.004f
#define GRID_SIZE 32
#define AMPLITUDE 1.75


float dotGridGradient(int ix, int iy, float x, float y, float gradients[GRID_SIZE][GRID_SIZE][2]) {
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return (dx * gradients[ix % GRID_SIZE][iy % GRID_SIZE][0] + dy * gradients[ix % GRID_SIZE][iy % GRID_SIZE][1]);
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float simpleNoise(float x, float y, float gradients[GRID_SIZE][GRID_SIZE][2]) {
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    float sx = fade(x - x0);
    float sy = fade(y - y0);

    float n0, n1, ix0, ix1;
    n0 = dotGridGradient(x0, y0, x, y, gradients);
    n1 = dotGridGradient(x1, y0, x, y, gradients);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, gradients);
    n1 = dotGridGradient(x1, y1, x, y, gradients);
    ix1 = lerp(n0, n1, sx);

    return lerp(ix0, ix1, sy) * AMPLITUDE;
}

void generateGradients(float gradients[GRID_SIZE][GRID_SIZE][2]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            float angle = (float)(rand() % 360) * M_PI / 180.0f;
            gradients[i][j][0] = cos(angle);
            gradients[i][j][1] = sin(angle);
        }
    }
}

carte creerCarte(int taille) {
    carte map;
    map.taille = taille;

    map.cases = malloc(taille * sizeof(chunk *));
    for (int i = 0; i < taille; i++) {
        map.cases[i] = malloc(taille * sizeof(chunk));
        for (int j = 0; j < taille; j++) {
            map.cases[i][j].region = 0;
            map.cases[i][j].x = i;
            map.cases[i][j].y = j;
        }
    }

    int nb_regions = 6; // A mettre dans fichier config

    map.regions = malloc(nb_regions * sizeof(char *));
    map.regions[0] = "./assets/map/water.png";
    map.regions[1] = "./assets/map/sand.bmp";
    map.regions[2] = "./assets/map/stone.jpg";
    map.regions[3] = "./assets/map/snow.jpg";
    map.regions[4] = "./assets/map/grass.png";
    map.regions[5] = "./assets/map/lava.jpg";

    return map;
}

int region_iles(float value) {
    if (value > 0.3) return 4;
    else if (value > 0.2) return 1;
    else if (value > -0.25) return 0;
    else if (value > -0.3) return 1;
    else if (value > -0.65) return 4;
    else if (value > -0.8) return 2;
    else return 5;
}

int region_plaines(float value) {
    if (value > -0.2) return 4;
    else if (value > -0.22) return 1;
    else if (value > -0.38) return 0;
    else if (value > -0.4) return 1;
    else if (value > -0.87) return 4;
    else if (value > -0.9) return 2;
    else return 5;
}

float noiseToBrightness(float value) {
    if (value < -0.7) return 255;

    return 200 + value * 50;
}

carte genererCarte(carte map) {
    srand(time(NULL));

    // Génération des gradients pour le bruit
    float gradients[GRID_SIZE][GRID_SIZE][2];
    generateGradients(gradients);

    for (int i = 0; i < map.taille; i++) {
        for (int j = 0; j < map.taille; j++) {
            // Applique le bruit simplifié à chaque case
            float x = i * SCALE;
            float y = j * SCALE;
            float noise = simpleNoise(x, y, gradients);

            // Convertit le bruit en une région
            int region = region_plaines(noise);
            float brightness = noiseToBrightness(noise);
            map.cases[i][j].brightness_R = brightness;
            map.cases[i][j].brightness_G = brightness;
            map.cases[i][j].brightness_B = brightness;
            map.cases[i][j].region = region;
            map.cases[i][j].texture_path = map.regions[region];

            if ((i - map.taille / 2) * (i - map.taille / 2) + (j - map.taille / 2) * (j - map.taille / 2) <= 50 * 50) {
                map.cases[i][j].region = 2;
                map.cases[i][j].texture_path = map.regions[2];
            }

            if (map.cases[i][j].region == 4 && rand() / (float)RAND_MAX < 0.001) {
                map.cases[i][j].structure.texture_path = "./assets/map/artefacts/tree.png";
                map.cases[i][j].structure.height = 125;
                map.cases[i][j].structure.width = 125;
            } else {
                map.cases[i][j].structure.texture_path = NULL;
            }

        }
    }

    return map;
}

void afficherCarte(carte map) {
    for (int i = 0; i < map.taille; i++) {
        for (int j = 0; j < map.taille; j++) {
            printf("%d ", map.cases[i][j].region);
        }
        printf("\n");
    }
}

int enregistrerCarte(carte map) {
    FILE *fichier = fopen("./assets/map/seed.txt", "w");
    for (int i = 0; i < map.taille; i++) {
        for (int j = 0; j < map.taille; j++) {
            fprintf(fichier, "%d ", map.cases[i][j].region);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);

    return 1;
}