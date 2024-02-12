#include "field.h"
#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/variant/vector2i.hpp"

using namespace godot;

void Field::_bind_methods(){};

Field::Field(){};
Field::~Field(){};

int Field::get_index_of_cell(int x, int y) { return x + y * _width; }

Vector2i Field::get_coords_of_cell(int index) {
  return Vector2i(index % _width, index / _width);
}

void Field::reveal(int cell_index) {

  if (field[cell_index].mines_around) {
    field[cell_index].hidden = false;
    return;
  }

  std::queue<int> cells_to_reveal;
  cells_to_reveal.push(cell_index);

  while (!cells_to_reveal.empty()) {
    int index = cells_to_reveal.front();
    cells_to_reveal.pop();

    if (!field[index].hidden)
      continue;

    Vector2i coordinates = get_coords_of_cell(index);
    int x = coordinates.x;
    int y = coordinates.y;

    field[index].hidden = false;

    if (field[index].mines_around > 0 || index >= _height * _width)
      continue;

    if (0 <= x && x < _width - 1)
      cells_to_reveal.push(get_index_of_cell(x + 1, y));
    if (0 < x && x <= _width - 1)
      cells_to_reveal.push(get_index_of_cell(x - 1, y));
    if (0 <= y && y < _height - 1)
      cells_to_reveal.push(get_index_of_cell(x, y + 1));
    if (0 < y && y <= _height - 1)
      cells_to_reveal.push(get_index_of_cell(x, y - 1));
  }
}

void Field::prepare_field() {
  for (int i = 0; i < _width * _height; i++) {
    field.push_back(Cell());
  }
}

void Field::plant_mines() {
  for (int i = 0; i < _mines_quantity; i++) {
    int index = 0;

    do {
      index = random.randi_range(0, _width * _height - 1);
    } while (field[index].mine);

    field[index].mine = true;

    for (int j = 0; j < 3; j++) {
      int y = index / _width - 1 + j;
      if (y < 0 || y >= _height)
        continue;

      for (int k = 0; k < 3; k++) {
        int x = index % _width - 1 + k;
        if (x < 0 || x >= _width)
          continue;
        field[get_index_of_cell(x, y)].mines_around += 1;
      }
    }
  }
}

void Field::clear(){
field.clear();
}

void Field::start_game(Vector2i resolution, int mines_quantity) {
  clear();
  _width = resolution.x;
  _height = resolution.y;
  _mines_quantity = mines_quantity;

  prepare_field();
  plant_mines();
}

int Field::see_gameover() {
  int flagged = 0;
  int guessed = 0;
  int hidden = 0;

  for (Cell x : field) {
    if (x.mine and not x.hidden)
      return 2;
    if (x.hidden and x.mine)
      guessed += 1;
    if (x.flagged and x.mine)
      flagged += 1;
    if (x.hidden)
      hidden += 1;
  }
  if (flagged == _mines_quantity ||
      guessed == _mines_quantity && hidden == _mines_quantity)
    return 1;

  return 0;
}
