#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FLOOR 5
#define GOD_MODE 0
#define IMPOSSIBLE_MODE 0
#define DEBUG_MODE 0

typedef char str[];

struct Character{
  int level;
  int health;
  int max_health;
  int attack;
  int defense;
  int gold;
  int experience;
  int max_experience;
  char* name;
};

struct Character init_char(struct Character main_character){
  if(GOD_MODE){
    main_character.attack = 9999;
    main_character.defense = 9999;
    main_character.health = 9999;
    main_character.max_health = 9999;
    main_character.gold = 9999;
  }else{
    main_character.gold = 5;
    main_character.health = 100;
    main_character.max_health = 100;
    main_character.attack = 10;
    main_character.defense = 1;
  }
  main_character.experience = 0;
  main_character.max_experience = 100;
  main_character.level = 1;
  return main_character;
  }