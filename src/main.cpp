#include <cassert>
#include <cstdio>
#include <iostream>
#include "MapInfo.hpp"
#include "PairwiseDistances.hpp"
#include "bc.hpp"

using namespace std;
using namespace bc;

int main() {
  printf("Player C++ bot starting\n");

  // It's good to try and make matches deterministic. It's not required, but it
  // makes debugging wayyy easier.
  // Now if you use random() it will produce the same output each map.
  srand(0);

  printf("Connecting to manager...\n");

  GameController gc;

  printf("Connected!\n");

  MapInfo map_info(gc.get_starting_planet(gc.get_planet()));
  PairwiseDistances distances(map_info.passable_terrain);

  // loop through the whole game.
  while (true) {
    uint32_t round = gc.get_round();
    map_info.update_karbonite(gc);
    printf("Round: %d\n", round);

    // Note that all operations perform copies out of their data structures,
    // returning new objects.
    vector<Unit> units = gc.get_my_units();

    for (size_t i = 0; i < units.size(); i++) {
      Unit &unit = units[i];

      // Calls on the controller take unit IDs for ownership reasons.
      uint16_t id = unit.get_id();
      Direction random_direction = Direction(rand() % 8);
      if (gc.can_replicate(id, random_direction)) {
        gc.replicate(id, random_direction);
      }

      random_direction = Direction(rand() % 8);
      if (gc.can_move(id, random_direction) && gc.is_move_ready(id)) {
        gc.move_robot(id, random_direction);
      }
    }

    fflush(stdout);

    gc.next_turn();
  }
}
