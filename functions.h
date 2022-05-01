#include "definitions.h"

int scan_result;

struct Character get_enemy(int floor){
  struct Character enemy;
  if(IMPOSSIBLE_MODE){
    enemy.health = 999;
    enemy.max_health = 999;
    enemy.defense = 999;
    enemy.attack = 999; 
  }else{
    enemy.health = 50;
    enemy.max_health = 50;
    enemy.defense = 0;
    enemy.attack = 5; 
  }
  enemy.experience = 40;
  enemy.max_experience = 10;
  enemy.gold = 50;
  return enemy;
}

char* get_random_place(){
  int random_num = rand() % 3;
  switch(random_num){
    case 0:
      return "on some barrels";
    case 1:
      return "underneath a rock";
    default:
      return "on the ground";
  }
}

void display_bar(str label, int current, int max, str letters){
  printf("%s: ", label);
  printf("[");
  for(int i = 0; i < current / 10; i++){
    printf("#");
  }
  for(int i = 0; i < (max - current) / 10; i++){
    printf(" ");
  }
  printf("] %d %s\n", current, letters);
}

void print_title(){
  printf("===\n");
  printf("Dungeon of Chaos\n");
  printf("[1] Start Game\n");
  printf("[2] About\n");
  printf("[3] Exit Game\n");
  printf("===\n");
}

void about(){
  printf("===\n");
  printf("This game was made by Joshua Manzano.\n");
}

void display_stats(struct Character main_character){
  printf("===\n");
  printf("%s, Level %d\n", main_character.name, main_character.level);
  display_bar("Health", main_character.health, main_character.max_health, "HP");
  display_bar("Exp", main_character.experience, main_character.max_experience, "EXP");
  printf("Attack: %d\n", main_character.attack);
  printf("Defense: %d\n", main_character.defense);
  printf("Gold: %d\n", main_character.gold);
}

int battle_menu(){
  int choice;
  printf("[1] Normal Attack\n");
  printf("[2] Wild Attack\n");
  printf("[3] Flee\n");
  do{
    printf("Choice: ");
    scan_result = scanf("%d", &choice);
    if(choice < 1 || choice > 3){
      printf("Invalid choice.\n");
    }
  }while(choice < 1 || choice > 3);
  return choice;
}

void heal_player(struct Character* player, int amount){
  player->health += amount;
  if(player->health > player->max_health){
    player->health = player->max_health;
  }
}

void non_fatal_player_damaged(struct Character* player, int amount){
  player->health -= amount;
  if(player->health <= 0){
    player->health = 1;
  }
}

int player_damaged(struct Character player, int attack, int range){
  int damage = (attack - range) + rand() % (range * 2);
  damage -= player.defense;
  if(damage < 0){
    damage = 0;
  }
  if(attack == range){
    if(damage == 0){
      printf("Enemy misses his wild attack!\n");
    }else{
      printf("Enemy does a wild attack for %d damage!\n", damage);
    }
  }else{
    if(damage == 0){
      printf("Enemy does no damage to you!\n");
    }else{
      printf("Enemy attacks you for %d damage!\n", damage);
    }
  }
  player.health -= damage;
  return player.health;
}

int enemy_damaged(struct Character enemy, int attack, int range){
  int damage = (attack - range) + rand() % (range * 2);
  damage -= enemy.defense;
  if(damage < 0){
    damage = 0;
  }
  if(attack == range){
    if(damage == 0){
      printf("You miss your wild attack!\n");
    }else{
      printf("You wildy attack the enemy for %d damage!\n", damage);
    }
  }else{
    if(damage == 0){
      printf("You do no damage to the enemy!\n");
    }else{
      printf("You attack enemy for %d damage!\n", damage);
    }
  }
  enemy.health -= damage;
  return enemy.health;
}

// explore scenarios
int battle_scenario(int floor, struct Character* player, int* exp_gained, int* gold_gained){
  printf("===\n");
  printf("Enemy encountered!\n");
  printf("*cue battle music*\n");
  struct Character enemy = get_enemy(floor);
  while(enemy.health > 0 && player->health > 0){
    printf("===\n");
    display_bar("Player", player->health, player->max_health, "HP");
    display_bar("Enemy", enemy.health, enemy.max_health, "HP");
    int choice = battle_menu();
    printf("===\n");
    switch(choice){
      case 1:
        enemy.health = enemy_damaged(enemy, player->attack, 3);
        break;
      case 2:
        enemy.health = enemy_damaged(enemy, player->attack, player->attack);
        break;
      default:
        printf("You fled the battle!\n");
        return 0;
        break;
    }
    // enemy turn
    if(enemy.health > 0){
      player->health = player_damaged(*player, enemy.attack, enemy.attack);
    }
  }

  if(enemy.health <= 0){
    *exp_gained = enemy.experience;
    *gold_gained = enemy.gold;
    return 1;
  }else if(player->health <= 0){
    return -1;
  }
}

void good_event(int* floor, struct Character* player){
  int random_num = rand() % 10;
  int healing, stats, gold_found;
  printf("===\n");
  if(random_num < 5){
    printf("You found a healing potion %s!\n", get_random_place());
    healing = (rand() % 3) + 2;
    heal_player(player, healing);
    printf("(You gained %d health)", healing);
  }else if(random_num == 5){
    printf("You found better equipment %s!\n", get_random_place());
    stats = 1;
    player->defense += stats;
    player->attack += stats;
    printf("(You gained %d attack and defense)", stats);
  }else{
    printf("You found some gold %s!\n", get_random_place());
    gold_found = (rand() % 3) + 2;
    player->gold += gold_found;
    printf("(You gained %d gold)", gold_found);
  }
  printf("\n");
}

void bad_event(int* floor, struct Character* player){
  int random_num = rand() % 10;
  int damage, stats, gold_lost;
  printf("===\n");
  if(random_num < 5){
    if(player->health >= 2){
      printf("You got hurt while exploring!\n");
      damage = (rand() % 3) + 2;
      non_fatal_player_damaged(player, damage);
      printf("(You lost around %d health)", damage);
    }else{
      printf("You wandered around till you got bored.");
    }
  }else if(random_num == 5){
    printf("You equipment got damaged while exploring!\n");
    stats = 1;
    if(player->defense >= 2 && player->attack >= 2){
      player->defense -= stats;
      player->attack -= stats;
      printf("(You lost around %d attack and defense)", stats);
    }else{
      printf("(Your attack and defense is at the minimum)");
    }
  }else{
    if(player->gold >= 5){
      printf("You dropped some gold while exploring!\n");
      gold_lost = (rand() % 3) + 2;
      player->gold -= gold_lost;
      printf("(You lost %d gold)", gold_lost);
    }else{
      printf("You wandered around till you got bored.");
    }
  }
  printf("\n");
}

void choice_event(int* floor, struct Character* player){
  // int random_num = rand() % 3;
  // printf("===\n");
  // switch(random_num){
  //   case 0:
  //     printf("You found some gold %s!", get_random_place());
  //     break;
  //   case 1:
  //     printf("You found some gold %s!", get_random_place());
  //     break;
  //   default:
  //     printf("You found some gold %s!", get_random_place());
  //     break;
  // }
  // printf("\n");
}

int decision_menu(int floor){
  int choice;

  printf("===\n");
  printf("You are at floor %d.\n", floor);
  printf("What would you like to do?\n");
  printf("[1] Explore\n");
  printf("[2] Sleep at Inn (%d gold)\n", 5);
  printf("[3] Sleep Outside (free)\n");
  printf("[4] Go to Shop\n");
  printf("[5] Display My Stats\n");
  do{
    printf("Choice: ");
    scan_result = scanf("%d", &choice);
    if(choice < 1 || choice > 5){
      printf("Invalid choice.\n");
    }
  }while(choice < 1 || choice > 5);
  return choice;
}

int title_menu(){
  int choice;
  do{
    print_title();
    printf("Choice: ");
    scan_result = scanf("%d", &choice);
    if(choice == 2){
      about();
    }else if(choice == 3){
      return 0;
    }
  }while(choice != 1);
  return 1;
}

void victory_message(){
  int num = rand() % 3;
  printf("===\n");
  switch(num){
    case 1:
      printf("You are victorious in your battle!");
      break;
    case 2:
      printf("You have won your bout!");
      break;
    case 3:
      printf("You have vanquished the enemy!");
      break;
    default:
      printf("You won!");
  }
  printf("\n");
}

void defeat_message(){
  int num = rand() % 3;
  printf("===\n");
  switch(num){
    case 1:
      printf("You lost the battle.");
      break;
    case 2:
      printf("You have lost your life.");
      break;
    case 3:
      printf("You were vanquished.");
      break;
    default:
      printf("You were defeated!");
  }
  printf("\n");
}

void sleep_inn(struct Character* player){
  printf("===\n");
  if(player->gold >= 5){
    player->gold -= 5;
    player->health = player->max_health;
    printf("You spent 5 gold to sleep at an inn...\n");
    printf("You feel well refreshed!\n");
    printf("(HP is now full)\n");
  }else{
    printf("Not enough gold!\n");
  }
}

void sleep_outside(struct Character* player){
  printf("===\n");
  printf("I don't think you should sleep outside...\n");
}

void shop_menu(struct Character* player){
  int choice;
  do{
    printf("===\n");
    printf("Welcome to the shop!\n");
    printf("What would you like?\n");
    printf("[1] 5 Gold: Buy Weapon (+1 attack)\n");
    printf("[2] 5 Gold: Buy Armor (+1 defense)\n");
    printf("[3] 5 Gold: Buy EXP Potion (+5 EXP)\n");
    printf("[4] Exit\n");
    printf("Choice: ");
    scan_result = scanf("%d", &choice);
    switch(choice){
      case 1:
        printf("===\n");
        if(player->gold >= 5){
          printf("You bought a weapon for 5 gold!\n");
          printf("(+1 attack)\n");
          player->gold -= 5;
          player->attack += 1;
        }else{
          printf("Not enough gold!\n");
        }
        break;
      case 2:
        printf("===\n");
        if(player->gold >= 5){
          printf("You bought armor for 5 gold!\n");
          printf("(+1 defense)\n");
          player->gold -= 5;
          player->defense += 1;
        }else{
          printf("Not enough gold!\n");
        }
        break;
      case 3:
        printf("===\n");
        if(player->gold >= 5){
          printf("You bought an EXP potion for 5 gold!\n");
          printf("(+5 EXP)\n");
          player->gold -= 5;
          player->experience += 5;
        }else{
          printf("Not enough gold!\n");
        }
        break;
      default:
        printf("===\n");
        printf("Invalid choice!\n");
    }
  }while(choice != 4);
}

void progress_floor(int* game_is_won, int* enemies_left, int* current_floor){
  if(DEBUG_MODE){
    printf("\n\n### DEBUG ###");
    printf("\nenemies_left: %d, current_floor: %d\n", *enemies_left, *current_floor);
    printf("### DEBUG ###\n\n");
  }
  
  *enemies_left -= 1;
  if(*enemies_left <= 0){
    printf("===\n");
    printf("You have found the entrance to the next floor!\n");
    *current_floor += 1;
    *enemies_left = (rand() % 4) + 1;
  }
  if(*current_floor >= MAX_FLOOR){
    printf("You have reached the last floor!!\n");
    *game_is_won = 1;
  }
}

struct Character gain_exp_gold(struct Character player, int experience, int gold){
  printf("You have gained %d gold!\n", gold);
  player.gold += gold;
  printf("You have gained %d experience!\n", experience);
  player.experience += experience;
  display_bar("Exp: ", player.experience, player.max_experience, "EXP");
  if(player.experience >= player.max_experience){
    printf("You have leveled up!\n");
    player.experience = 0;
    player.level += 1;
    player.attack += 2;
    player.defense += 2;
    player.health += 2;
    player.max_health += 2;
    player.max_experience += 5;
    printf("Attack +2, Defense +2\n");
    printf("Max Health +2\n");
  }
  return player;
}

struct Character name_menu(struct Character player){
  char buffer;
  char name[20];
  printf("===\n");
  printf("What is your name?\nName: ");
  scan_result = scanf("%c", &buffer);
  scan_result = scanf("%s", name);
  printf("Your name is %s.\n", name);
  player.name = name;
  return player;
}