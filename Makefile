# Compilateur et options
CC = gcc
CFLAGS = -g -Wall -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/cjson -IC:/msys64/mingw64/include/SDL2 -I./cJSON
LDFLAGS = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lcJSON

# Répertoires du projet
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LOG_FILE = src/logs/game.log
SAVES_DIR = saves

# Nom de l'exécutable final
EXEC = $(BIN_DIR)/gravebound.exe

# Liste des fichiers sources
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/UI/graphique.c \
      $(SRC_DIR)/logs/logging.c \
      $(SRC_DIR)/controller/controller.c \
      $(SRC_DIR)/game/game.c \
      $(SRC_DIR)/map/procedural.c \
      $(SRC_DIR)/entities/character.c \
      $(SRC_DIR)/entities/zombies.c \
      $(SRC_DIR)/audio/audio.c \
      $(SRC_DIR)/UI/menu.c \
      $(SRC_DIR)/UI/cache.c \
      $(SRC_DIR)/game/time.c \
      $(SRC_DIR)/UI/menu_personnage.c \
      $(SRC_DIR)/map/map.c \
      $(SRC_DIR)/entities/skills.c \
      $(SRC_DIR)/entities/attack.c \
      $(SRC_DIR)/UI/minimap.c \
      $(SRC_DIR)/game/config.c

# Conversion des fichiers sources en fichiers objets
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règle principale de compilation
all: $(EXEC)

# Création automatique des répertoires nécessaires
$(shell mkdir -p $(OBJ_DIR)/UI $(OBJ_DIR)/logs $(OBJ_DIR)/controller $(OBJ_DIR)/audio $(OBJ_DIR)/game $(OBJ_DIR)/map $(OBJ_DIR)/entities $(BIN_DIR) $(SAVES_DIR))

# Génération de l'exécutable final
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Règle générique pour compiler les fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) # Crée les sous-dossiers pour les fichiers objets si nécessaire
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(SAVES_DIR) $(LOG_FILE)

# Exécution du programme
run: $(EXEC)
	./$(EXEC)

# Déclaration des cibles qui ne correspondent pas à des fichiers
.PHONY: all clean run
