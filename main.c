#include "functions.h"

int main() {

  srand(time(0));
  
  struct Character player;
  player = init_char(player);
  int current_floor = 1;
  int choice, scenario, result, cooldown = 2;
  int enemies_left = (rand() % 4) + 1; 
  int game_is_won = 0;
  int gold_gained = 0, exp_gained = 0;
  
  if(title_menu()){
    player = name_menu(player);
    while(player.health > 0 && !game_is_won){
      choice = decision_menu(current_floor);
      // explore
      if(choice == 1){
        scenario = rand() % 4;
        if(scenario == 0 || cooldown == 0){
          // battle event
          result = battle_scenario(current_floor, &player, &exp_gained, &gold_gained);
          if(result > 0){
            cooldown = 2;
            victory_message();
            player = gain_exp_gold(player, exp_gained, gold_gained);
            progress_floor(&game_is_won, &enemies_left, &current_floor);
          }else if(result < 0){
            defeat_message();
          }
        }else if(scenario == 1){
          good_event(&current_floor, &player);
          cooldown -= 1;
        }else if(scenario == 2){
          // bad_event(&current_floor, &player);
          good_event(&current_floor, &player);
          cooldown -= 1;
        }else if(scenario == 3){
          // choice_event(&current_floor, &player);
          bad_event(&current_floor, &player);
          cooldown -= 1;
        }
      }else if(choice == 2){
        sleep_inn(&player);
      }else if(choice == 3){
        sleep_outside(&player);
      }else if(choice == 4){
        shop_menu(&player);
      }else if(choice == 5){
        display_stats(player);
      }
    }
    if(game_is_won){
      printf("\n### VICTORY ### \n");
      printf("\nAfter a tough journey...\n");
      printf("\nYou have made it to the end!\n");
      printf("\nCongratulations %s!!\n", player.name);
    }else{
      printf("\n### GAME OVER ### \n");
      printf("\nYou cannot give up just yet...\n");
      printf("\n%s! Stay determined...\n", player.name);
    }
  }else{
    printf("Exiting game...\n");
  }
  return 0;
}