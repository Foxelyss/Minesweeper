#include "field.h"

using namespace godot;

void Field::_bind_methods(){};

Field::Field(){};
Field::~Field(){};

void Field::_init(){}; // our initializer called by Godot

void Field::_process(float delta){};
int Field::get_index_of_cell(int x, int y) { return x + y * width; }

Vector2 Field::get_coords_of_cell(int index) {
  return Vector2(index % width, index / width);
}

void Field::reveal(int x, int y) {

  if (field[get_index_of_cell(x, y)].mines_around) {
    field[get_index_of_cell(x, y)].hidden = false;
    return;
  }

  std::queue<int> cells_to_reveal;
  cells_to_reveal.push(get_index_of_cell(x, y));

  while (!cells_to_reveal.empty()) {
    int index = cells_to_reveal.front();
    cells_to_reveal.pop();

    if (!field[index].hidden)
      continue;

    Vector2 coordinates = get_coords_of_cell(index);
    int x = coordinates.x;
    int y = coordinates.y;

    field[index].hidden = false;

    if (field[index].mines_around > 0)
      continue;

    if (0 <= x < width - 1)
      cells_to_reveal.push(get_index_of_cell(x + 1, y));
    if (0 < x <= width - 1)
      cells_to_reveal.push(get_index_of_cell(x - 1, y));
    if (0 <= y < height - 1)
      cells_to_reveal.push(get_index_of_cell(x, y + 1));
    if (0 < y <= height - 1)
      cells_to_reveal.push(get_index_of_cell(x, y - 1));
  }
}

void Field::prepare_field() {
  for (int i = 0; i < width * height; i++) {

    field.push_back(Cell());
  }
}

void Field::plant_mines() {
  for (int i = 0; i < mines_quantity; i++) {
    int index = int(random()) % (width * height);

    while (field[index].mine)
      index = int(random()) % (width * height);

    field[index].mine = true;

    for (int j = 0; j < 3; j++) {
      int y = index / width - 1 + j;
      if (y < 0 || y >= 8)
        continue;

      for (int k = 0; k < 3; k++) {
        int x = index % width - 1 + k;
        if (x < 0 || x >= 8)
          continue;
        field[get_index_of_cell(x, y)].mines_around += 1;
      }
    }
  }
}

// def see_gameover():
//     flagged = 0
//     guessed = 0
//     hidden = 0

//     for x in field:
//         if x.mine and not x.hidden:
//             print("whoops")
//             return True
//         if x.hidden and x.mine:
//             guessed += 1
//         if x.flagged and x.mine:
//             flagged += 1
//         if x.hidden:
//             hidden += 1

//     if flagged == mines_quantity or guessed == mines_quantity and hidden ==
//     mines_quantity:
//         print("well, you win!")
//         return True