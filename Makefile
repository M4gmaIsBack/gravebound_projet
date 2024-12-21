CC = gcc
CFLAGS = -g -Wall -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/SDL2
LDFLAGS = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Repertoires du projet
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Liste des fichiers sources
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/UI/graphique.c \
      $(SRC_DIR)/logs/logging.c \
      $(SRC_DIR)/controller/controller.c \
      $(SRC_DIR)/game/game.c \
	  $(SRC_DIR)/map/procedural.c

# Conversion des fichiers sources en fichiers objets
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nom de l'executable final
EXEC = $(BIN_DIR)/gravebound.exe

# Regle principale de compilation
all: $(EXEC)

# Creation automatique des repertoires necessaires
$(shell mkdir -p $(OBJ_DIR)/UI $(OBJ_DIR)/logs $(OBJ_DIR)/controller $(OBJ_DIR)/game $(OBJ_DIR)/map $(BIN_DIR))

# Regle de generation de l'executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Regle generique de compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers generes
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Execution du programme
run: $(EXEC)
	./$(EXEC)

# Declaration des cibles qui ne correspondent pas a des fichiers
.PHONY: all clean run