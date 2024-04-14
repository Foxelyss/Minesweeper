#ifndef MINE_GRID_H
#define MINE_GRID_H

#include "field.h"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/texture2d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/vector2i.hpp"

#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/string.hpp"
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
  Field *_game_field;

  Control *_menu;
  GridContainer *_grid;

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

  Timer *_timer;

  AnimationPlayer *_ui_tweener;
  AnimationPlayer *_sfx;
  AnimatedSprite2D *_pop_animator;

  Array _cells_textures;

  int _first_cell = -1;

  bool _dragging = false;

  const int _time_before_timeout = 4 * 60;

  const StringName RECORDS_FILENAME = "user://records.json";

public:
  static void _bind_methods();

  FieldGrid();
  ~FieldGrid();

  void _ready() override;
  void _process(float delta);
  bool is_grid_fully_on_screen();
  void _input(Ref<InputEvent> event);
  void _on_button_pressed(InputEvent *event, int index);

  void start_game();
  void update_grid();
  void update_game_status();
  void create_records_file();
  void retry_game();
  void go_to_menu();

  void move_grid(float x, float y);

  int get_game_category();
  String format_time(int time);
  void show_records();
  void save_record(int time);
  void show_best_record();

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
