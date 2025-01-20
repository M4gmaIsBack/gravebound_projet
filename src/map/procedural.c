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

carte genererCarte(carte map, GenerationParams params) {
    srand(params.seed);

    // Génération des gradients pour le bruit
    float gradients[GRID_SIZE][GRID_SIZE][2];
    generateGradients(gradients);

    for (int i = 0; i < map.taille; i++) {
        for (int j = 0; j < map.taille; j++) {
            // Applique le bruit simplifié à chaque case
            float x = i * params.scale;
            float y = j * params.scale;
            float noise = simpleNoise(x, y, gradients);

            // Convertit le bruit en une région
            int region = region_plaines(noise);
            float brightness = noiseToBrightness(noise);
            map.cases[i][j].brightness_R = brightness;
            map.cases[i][j].brightness_G = brightness;
            map.cases[i][j].brightness_B = brightness;
            map.cases[i][j].region = region;
            map.cases[i][j].texture_path = map.regions[region];

            if (i == map.taille / 2 && j == map.taille / 2) {
                map.cases[i][j].structure.texture_path = "./assets/map/artefacts/house1.png";
                map.cases[i][j].structure.height = 700;
                map.cases[i][j].structure.width = 700;
            } else if (map.cases[i][j].region == 4 && rand() / (float)RAND_MAX < 0.0005) {
                map.cases[i][j].structure.texture_path = "./assets/map/artefacts/tree.png";
                map.cases[i][j].structure.height = 100;
                map.cases[i][j].structure.width = 100;
            } else {
                map.cases[i][j].structure.texture_path = NULL;
            }

            if (map.cases[i][j].region == 4 && map.cases[i][j].structure.texture_path == NULL && rand() / (float)RAND_MAX < 0.001) {

                char *stones[] = {
                    "./assets/map/artefacts/rock7_1.png",
                    "./assets/map/artefacts/rock7_2.png",
                    "./assets/map/artefacts/rock7_3.png",
                    "./assets/map/artefacts/rock7_4.png",
                    "./assets/map/artefacts/rock7_5.png"
                };

                int stones_size[] = {
                    64,
                    64,
                    32,
                    32,
                    16
                };

                int index = rand() % 5;

                map.cases[i][j].structure.texture_path = stones[index];
                map.cases[i][j].structure.height = stones_size[index];
                map.cases[i][j].structure.width = stones_size[index];
            }
        }
    }

    return map;
}

GenerationParams initGenerationParams() {
    GenerationParams params;
    params.seed = time(NULL);
    params.scale = config.map.scale;//0.004f;
    params.amplitude = config.map.amplitude;//1.75;
    params.taille = config.map.map_size;//1000;
    return params;
}

int sauvegarderParams(GenerationParams params, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/seed.txt", save);
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    fprintf(file, "%u\n%f\n%f\n%d", params.seed, params.scale, params.amplitude, params.taille);
    fclose(file);
    return 1;
}

GenerationParams chargerParams(char *save) {

    GenerationParams params;

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/seed.txt", save);
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Pas de seed trouvé, nouvelle génération");
        params = initGenerationParams();

        if (!sauvegarderParams(params, save)) {
            printf("Erreur lors de la sauvegarde des paramètres");
        }

        printf("seed: %u, scale: %f, amplitude: %f, taille: %d\n", params.seed, params.scale, params.amplitude, params.taille);
        return params;
    }

    if (fscanf(file, "%u\n%f\n%f\n%d", &params.seed, &params.scale, &params.amplitude, &params.taille) != 4) {
        printf("Erreur lors de la lecture des paramètres");
        fclose(file);
        return params;
    }

    fclose(file);

    printf("seed: %u, scale: %f, amplitude: %f, taille: %d\n", params.seed, params.scale, params.amplitude, params.taille);
    return params;
}