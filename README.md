# gravebound_projet

Projet en C


# RESUMER DU PROJET

Dans le jeu, le temps sera la monnaie. 

Le joueur pourra utiliser du temps pour acheter des améliorations dans des forges, des bases, etc. 

Ce temps représente aussi la survie :  s'il n'en a plus, c'est un GAME OVER.

En tuant des zombies, le joueur gagne du temps, et en ramenant des survivants trouvés dans la carte jusqu'à la base, 
il gagne aussi du temps.


# Objectifs

- Jeu Manette
- Jeu en 2D
- Environnement de jeu de survie
- Ajout d’obstacle fixe / mobile
- Ajout de base / forge et autres...
- Création de zombie variée
- Création de plusieurs personnages Militaires
- Création de la physique pour les environnements (glace, feu, eau)
- Ajout des collisions
- Ajout d’un système de money : Le Temps
- Ajout d’un système de boutique
- Gérer l’économie du Temps
- Ajout de statistique
- Ajout d’un csv pour tout la gestion de zombie et amélioration (fichier configuration) 
- map infinie avec plusieurs biomes


# Bonus

- Jeu clavier souris + manette
- Création de sauvegarde
- Multijoueur locale
- On peut rentrer dans des bâtiment, tour et notre base


# Personne dans le projet

- Lucas VALLION
- Jeremy DAMAS
- Chhay-Tommy ROBLIN


# Explication code

main.c : Point d'entree principal de l'application
- Initialisation des systemes de logging
- Configuration et demarrage du jeu
- Gestion du cycle de vie de base de l'application
- Tres minimaliste
- Delegation des taches complexes aux autres modules
- Initialisation et nettoyage des ressources

graphique.c : Gestion des composants graphiques SDL
- Initialisation de SDL
- Creation de la fenetre de jeu
- Chargement et rendu de la carte
- Gestion des ressources graphiques
- Abstraction des details SDL
- Creation et destruction des ressources graphiques
- Rendu de base de la carte et du personnage

logging.c : Systeme de journalisation
- Enregistrement des messages dans un fichier log
- Affichage simultanee des logs en console
- Capture des erreurs et evenements importants
- Support des formats variables
- Enregistrement dans un fichier persistant
- Debogage et suivi des evenements

game.c : Gestion du cycle de vie global du jeu
- Initialisation des composants du jeu
- Boucle principale de jeu
- Gestion des evenements
- Nettoyage des ressources
- Boucle d'evenements SDL
- Gestion des entrees (clavier/manette)
- Controle du flux principal du jeu

controller.c : Gestion des peripheriques de controle
- Initialisation des manettes/controleurs
- Interpretation des mouvements du joystick
- Deplacement de la carte
- Abstraction des controles SDL
- Gestion des zones mortes du joystick
- Transformation des entrees en mouvements