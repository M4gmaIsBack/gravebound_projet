#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../logs/logging.h"

typedef struct {
    float sante;
    float puissance_attaque;
    float vitesse;
    float vitesse_max;
    int x;
    int y;
    char* type;
    int frameWidth;            
    int frameHeight;           
    int currentFrame;          
    int totalFrames;           
    int direction;         
    int moving;  
    SDL_Texture* texture;              
} Zombie;

typedef struct {
    char name[50];
    int level;
    int range;
    int maxExperience;
    int cooldown;
    int lastUsed;
    int damage;
} Skill;

typedef struct {
    SDL_Texture* texture;     
    int frameWidth;            
    int frameHeight;           
    int currentFrame;         
    int totalFrames;           
    int direction;             
    int moving;                
    int x;                    
    int y;                     
    int vie_max;
    int vie_actuelle;
    int force_attaque;
    int invincibilite;         
    float vitesse;               
    float vitesse_max;
    float defense;
    Skill skills[10];
} Personnage;

typedef struct {
    unsigned int seed;
    float scale;
    float amplitude;
    int taille;
} GenerationParams;

typedef struct {
    int max_zombies[100]; // Adjust size as needed
    int spawn_rate[100];  // Adjust size as needed
    int spawn_radius[100]; // Adjust size as needed
    int spawn_delay[100]; // Adjust size as needed
    float max_speed[100]; // Adjust size as needed
    float max_health[100]; // Adjust size as needed
    float max_damage[100]; // Adjust size as needed
    float max_defense[100]; // Adjust size as needed
} Config_values_zombies;

typedef struct {
    int map_size;
    char *region;
    float scale;
    float amplitude;
    int start_countdown_hour;
    int start_countdown_minute;
    int start_countdown_second;
    float start_hour;
} Config_values_map;

typedef struct {
    char name[100][50]; // Adjust size as needed
    int level[100]; // Adjust size as needed
    int range[100]; // Adjust size as needed
    int maxExperience[100]; // Adjust size as needed
    int cooldown[100]; // Adjust size as needed
    int lastUsed[100]; // Adjust size as needed
    int damage[100]; // Adjust size as needed
} Config_values_skills;

typedef struct {
    float vie_max[100]; // Adjust size as needed
    float force_attaque_max[100]; // Adjust size as needed
    float vitesse_max[100]; // Adjust size as needed
    float defense_max[100]; // Adjust size as needed
    int max_skills[100]; // Adjust size as needed
    Skill skills[100]; // Adjust size as needed
} Config_values_personnage;

typedef struct {
    Config_values_zombies zombies;
    Config_values_map map;
    Config_values_personnage personnage;
    Config_values_skills skills;
} Config;


int charger_config_zombies(Config_values_zombies *zombies, char *save);
int charger_config_map(Config_values_map *map, char *save);
int charger_config_personnage(Config_values_personnage *personnage, char *save);
int charger_config_skills(Config_values_skills *skills, char *save);
int charger_config(char *save);
int enregistrer_config(char *save);

#endif