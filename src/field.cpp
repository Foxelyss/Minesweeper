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

  if (_field[cell_index].mines_around) {
    _field[cell_index].hidden = false;
    return;
  }

  std::queue<int> cells_to_reveal;
  cells_to_reveal.push(cell_index);

  while (!cells_to_reveal.empty()) {
    int index = cells_to_reveal.front();
    cells_to_reveal.pop();

    if (!_field[index].hidden)
      continue;

    Vector2i coordinates = get_coords_of_cell(index);
    int x = coordinates.x;
    int y = coordinates.y;

    _field[index].hidden = false;

    if (_field[index].mines_around > 0)
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

void Field::reveal_all_hidden() {
  for (int i = 0; i < get_cells_quantity(); i++) {
    if (_field[i].hidden) {
      _field[i].hidden = false;
    }
  }
}
void Field::toggle_flag(int index) {
  if (_field[index].hidden) {
    _field[index].flagged = !_field[index].flagged;
  } else {
    _field[index].flagged = false;
  }
}

void Field::place_mines(int selected_cell = -1) {
  for (int i = 0; i < _mines_quantity; i++) {
    int index = 0;

    do {
      index = random.randi_range(0, _width * _height - 1);
    } while (_field[index].mine || index == selected_cell);

    _field[index].mine = true;

    for (int j = 0; j < 3; j++) {
      int y = index / _width - 1 + j;
      if (y < 0 || y >= _height)
        continue;

      for (int k = 0; k < 3; k++) {
        int x = index % _width - 1 + k;
        if (x < 0 || x >= _width)
          continue;
        _field[get_index_of_cell(x, y)].mines_around += 1;
      }
    }
  }
}
void Field::prepare_field() {
  for (int i = 0; i < _width * _height; i++) {
    _field.push_back(Cell());
  }
}

void Field::clear() { _field.clear(); }

void Field::start_game(int selected_cell) {
  clear();
  prepare_field();
  place_mines(selected_cell);
}

void Field::set(Vector2i resolution, int mines_quantity) {
  _width = resolution.x;
  _height = resolution.y;
  _mines_quantity = mines_quantity;
}

int Field::see_gameover() {
  int flagged = 0;
  int guessed = 0;
  int hidden = 0;

  for (int i = 0; i < get_cells_quantity(); i++) {
    auto x = _field[i];
    if (x.mine && !x.hidden)
      return 2;
    if (x.hidden && x.mine)
      guessed += 1;
    if (x.flagged && x.mine)
      flagged += 1;
    if (x.hidden)
      hidden += 1;
    if (x.flagged && !x.mine)
      flagged -= 1;
  }

  if (flagged == _mines_quantity ||
      guessed == _mines_quantity && hidden == _mines_quantity) {
    return 1;
  }

  return 0;
}
