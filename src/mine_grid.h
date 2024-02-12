#ifndef MINE_GRID_H
#define MINE_GRID_H

#include "field.h"
#include "godot_cpp/variant/string_name.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
namespace godot {

class MineGrid : public Node {
  GDCLASS(MineGrid, Node)

private:
  float time_passed;
  Field *_game_field;

  GridContainer *grid;

  Label *_mines_around_label;
  Label *_time_label;

  StringName _mines_around_label_path;
  StringName _time_label_path;
public:
  static void _bind_methods();

  MineGrid();
  ~MineGrid();

  void _ready();
  void _process(float delta);
  void _on_button_pressed(int index);

  void update_grid();
  void create_game();

  String get_mines_around_label();
  void set_mines_around_label(String path);

  String get_time_label();
  void set_time_label(String path);
};

} // namespace godot

#endif
