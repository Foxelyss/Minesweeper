#ifndef FIELD_H
#define FIELD_H

#include <cstdint>
#include <godot_cpp/classes/control.hpp>
// #include <godot_cpp/variant/Vector2.hpp>
#include <queue>

struct Cell {
  int mines_around = 0;
  bool mine = false;
  bool hidden = true;
  bool flagged = false;
};
using namespace std;
namespace godot {

class Field : public Control {
  GDCLASS(Field, Control);

private:
  float time_passed;

public:
  static void _bind_methods();

  Field();
  ~Field();

  void _init(); // our initializer called by Godot

  void _process(float delta);
  vector<Cell> field;
  uint8_t width, height;
  uint8_t mines_quantity = 0;

  int get_index_of_cell(int x, int y);

  Vector2 get_coords_of_cell(int index);

  void reveal(int x, int y);

  void prepare_field();

  void plant_mines();
};

} // namespace godot

#endif
