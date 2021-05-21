# Conventions à utiliser en C

Voici une liste des conventions à utiliser en C afin de simplifier la lisibilité et la lecture du code.

## Conventions de nommage

### Variables

Convention : Snake Case
Les mots sont en minuscules et sont liés par des underscores (tiret bas : `_`).
Exemple : `nom_de_variable`

### Fonctions

Convention : Camel Case
Les mots sont liés sans espace. Chaque mot commence par une majuscule à l’exception du premier.
Exemple : `nomDeFonction`

### Classes et Structures

Convention : Pascal Case
Les mots sont liés sans espace. Chaque mot commence par une Majuscule.
Exemple : `TypeDeStructure`

### Constantes

Convention : Screamng Snake Case
Les mots sont en majuscules et sont liés par des underscores (tiret bas : `_`).
Exemple : `NOM_DE_CONSTANTE`

## Conventions d'écritures

### Structures

#### Frame

```c
typedef struct Frame
{
    int x;  // x position  
    int y;  // y position
    char* texture[255]; // texture name for Tom
    bool can_hover;     // can be hover by the player
    bool can_kill;      // can kill the player
    bool can_use;       // can be use by the player
    bool can_open;      // can be open by the player
    Frame* usage[MAX_OBJECT];
} Frame;
```

#### Map

```c
typedef struct Map
{
    char* name[255]; // Name of the map
    char* author[255]; // Author of the map
    Frame* map[255];
}
```

## Variables et objet

### Types d'objet

- 1 : Levier
- 2 : Porte
- 3 : Mur
- 4 : Trou
- 5 : Torche

#### Affichage dans la fonction display

- Bouton : `u`
- Porte : `l`
- Mur : `&`
- Trou : `@`
- Torche : `*`
- Rien : `0`
