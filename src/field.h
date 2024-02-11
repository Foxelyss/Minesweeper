#ifndef FIELD_H
#define FIELD_H

#include <cstdint>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <queue>

struct Cell {
  int mines_around = 0;
  bool mine = false;
  bool hidden = true;
  bool flagged = false;
};

using namespace std;

namespace godot {

class Field : public Node {
  GDCLASS(Field, Node);

private:
  float time_passed;
  RandomNumberGenerator random;

public:
  static void _bind_methods();

  Field();
  ~Field();

  //void _init(); // our initializer called by Godot

  //void _process(float delta);
  vector<Cell> field;
  uint8_t width, height;
  uint8_t mines_quantity = 6;

  int get_index_of_cell(int x, int y);

  Vector2i get_coords_of_cell(int index);

  void reveal(int cell_index);

  void prepare_field();

  void plant_mines();
};

} // namespace godot

#endif
