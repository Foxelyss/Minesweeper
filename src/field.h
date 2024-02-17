#ifndef FIELD_H
#define FIELD_H

#include "godot_cpp/variant/vector2i.hpp"
#include <array>
#include <cstdint>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <queue>

struct Cell {
  uint8_t mines_around = 0;
  bool mine = false;
  bool hidden = true;
  bool flagged = false;
};

using namespace std;

namespace godot {

class Field : public Node {
  GDCLASS(Field, Node);

private:
  RandomNumberGenerator random;
  uint8_t _width, _height;
  uint8_t _mines_quantity = 6;

  int get_index_of_cell(int x, int y);

  Vector2i get_coords_of_cell(int index);

  void place_mines(int selected_cell);

  void prepare_field();

  void clear();

public:
  static void _bind_methods();

  Field();
  ~Field();

  // void _process(float delta);

  vector<Cell> field;

  void start_game(int selected_cell = -1);
  void set(Vector2i resolution, int mines_quantity);
  void reveal(int cell_index);
  void reveal_all_hidden();

  void toggle_flag(int index);

  int see_gameover();

  int get_mines_quantity() { return _mines_quantity; }
  Vector2i get_field_resolution() { return Vector2i(_width, _height); }
  int get_cells_quantity() { return _width * _height; }
};

} // namespace godot

#endif
