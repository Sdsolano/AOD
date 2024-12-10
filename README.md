# TP Alignement de séquences génétiques

## Description
Ce programme implémente trois versions de l'algorithme Needleman-Wunsch pour l'alignement de séquences génétiques :
- Algorithme itératif avec espace O(N+M)
- Algorithme cache-aware utilisant les paramètres du cache
- Algorithme cache-oblivious optimisant l'utilisation du cache sans paramètres spécifiques

## Compilation
```bash
make
```

## Exécution
```bash
./bin/distanceEdition file1 b1 L1 file2 b2 L2
```
Où :
- file1, file2 : fichiers contenant les séquences à comparer
- b1, b2 : positions de départ dans chaque fichier
- L1, L2 : longueurs des séquences à comparer

## Tests
Pour exécuter les tests de base :
```bash
make test
```

Pour exécuter les tests avec valgrind :
```bash
make valgrind
```

## Structure du projet
- src/ : Code source
  - distanceEdition.c : Programme principal
  - algorithms.c : Implémentation des algorithmes
  - algorithms.h : Déclarations des algorithmes
  - characters_to_base.h : Utilitaires pour la gestion des bases génétiques
- tests/ : Tests et fichiers de test
- bin/ : Exécutables compilés
- obj/ : Fichiers objets intermédiaires

## Paramètres du cache
Le programme utilise par défaut :
- Taille de ligne de cache (L) : 64 octets
- Taille du cache (Z) : 4096 octets
- Associativité (A) : 4 voies

Ces valeurs peuvent être modifiées dans les tests en utilisant valgrind.

## Auteur
[Samuel Solano]

## Date
28 Octobre 2024