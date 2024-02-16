#ifndef MINE_GRID_H
#define MINE_GRID_H

#include "field.h"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/vector2i.hpp"

#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
namespace godot {

class FieldGrid : public Node {
  GDCLASS(FieldGrid, Node)

private:
  float time_passed;
  Field *_game_field;

  GridContainer *grid;

  Label *_mines_around_label;
  Label *_time_label;
  Label *_game_status_label;

  Button *_retry_button;
  Button *_flagging_radio_button;
  Button *_back_to_menu_button;

  StringName _mines_around_label_path;
  StringName _time_label_path;
  StringName _game_status_label_path;

  StringName _retry_button_path;
  StringName _flagging_radio_button_path;
  StringName _back_to_menu_button_path;
  int _first_cell = -1;

public:
  static void _bind_methods();

  FieldGrid();
  ~FieldGrid();

  void _ready() override;
  void _process(float delta);
  void _input(InputEvent *event);
  void _on_button_pressed(int index);

  void start_game(Vector2i resolution, int mines_quantity);
  void update_grid();
  void retry_game();

  void go_to_menu();

  String get_mines_around_label();
  void set_mines_around_label(String path);

  String get_time_label();
  void set_time_label(String path);

  String get_retry_button();
  void set_retry_button(String path);

  String get_flagging_radio_button();
  void set_flagging_radio_button(String path);

  String get_game_status_label();
  void set_game_status_label(String path);

  String get_back_to_menu_button();
  void set_back_to_menu_button(String path);
};

} // namespace godot

#endif
