#include "field_grid.h"

#include "field.h"
#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
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
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void FieldGrid::_bind_methods() {
  ClassDB::bind_method(D_METHOD("on_button_pressed", "index"),
                       &FieldGrid::_on_button_pressed);
  ClassDB::bind_method(D_METHOD("retry_game"), &FieldGrid::retry_game);
  ClassDB::bind_method(D_METHOD("go_to_menu"), &FieldGrid::go_to_menu);

  ClassDB::bind_method(D_METHOD("set_mines_around_label", "mines_around_label"),
                       &FieldGrid::set_mines_around_label);
  ClassDB::bind_method(D_METHOD("get_mines_around_label"),
                       &FieldGrid::get_mines_around_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "mines_around_label"),
                        "set_mines_around_label", "get_mines_around_label");

  ClassDB::bind_method(D_METHOD("set_time_label", "time_label"),
                       &FieldGrid::set_time_label);
  ClassDB::bind_method(D_METHOD("get_time_label"), &FieldGrid::get_time_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "time_label"),
                        "set_time_label", "get_time_label");

  ClassDB::bind_method(D_METHOD("set_retry_button", "retry_button"),
                       &FieldGrid::set_retry_button);
  ClassDB::bind_method(D_METHOD("get_retry_button"),
                       &FieldGrid::get_retry_button);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "retry_button"),
                        "set_retry_button", "get_retry_button");

  ClassDB::bind_method(
      D_METHOD("set_flagging_radio_button", "flagging_radio-button"),
      &FieldGrid::set_flagging_radio_button);
  ClassDB::bind_method(D_METHOD("get_flagging_radio_button"),
                       &FieldGrid::get_flagging_radio_button);
  ClassDB::add_property(
      "FieldGrid", PropertyInfo(Variant::NODE_PATH, "flagging_radio-button"),
      "set_flagging_radio_button", "get_flagging_radio_button");

  ClassDB::bind_method(D_METHOD("set_game_status_label", "game_status_label"),
                       &FieldGrid::set_game_status_label);
  ClassDB::bind_method(D_METHOD("get_game_status_label"),
                       &FieldGrid::get_game_status_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "game_status_label"),
                        "set_game_status_label", "get_game_status_label");

  ClassDB::bind_method(
      D_METHOD("set_back_to_menu_button", "back_to_menu_button"),
      &FieldGrid::set_back_to_menu_button);
  ClassDB::bind_method(D_METHOD("get_back_to_menu_button"),
                       &FieldGrid::get_back_to_menu_button);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "back_to_menu_button"),
                        "set_back_to_menu_button", "get_back_to_menu_button");
};

FieldGrid::FieldGrid(){}; // our initializer called by Godot

FieldGrid::~FieldGrid(){};

void FieldGrid::_ready() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    return;
  }

  _time_label = get_node<Label>(NodePath(_time_label_path));
  _mines_around_label = get_node<Label>(NodePath(_mines_around_label_path));
  _game_status_label = get_node<Label>(NodePath(_game_status_label_path));
  _retry_button = get_node<Button>(NodePath(_retry_button_path));
  _back_to_menu_button = get_node<Button>(NodePath(_back_to_menu_button_path));
  _flagging_radio_button =
      get_node<Button>(NodePath(_flagging_radio_button_path));

  _back_to_menu_button->connect("pressed", Callable(this, "go_to_menu"));
  _retry_button->connect("pressed", Callable(this, "retry_game"));

  _game_field = get_node<Field>("/root/MineField");
  grid = get_node<GridContainer>("../DraggableSpace/MineGrid");

  start_game(_game_field->get_field_resolution(),
             _game_field->get_mines_quantity());
}

void FieldGrid::start_game(Vector2i resolution, int mines_quantity) {

  _game_field->start_game(resolution, mines_quantity);

  grid->set_columns(resolution.x);

  for (int i = 0; i < _game_field->field.size(); i++) {
    Button *button = memnew(Button());
    grid->add_child(button);

    button->set_custom_minimum_size(Vector2(40, 40));
    button->connect("pressed", Callable(this, "on_button_pressed").bind(i));
  }

  auto b = Variant(_game_field->get_mines_quantity());
  _mines_around_label->set_text(tr("MINESAROUND") + " " + b.stringify());

  update_grid();
}

void FieldGrid::retry_game() {
  _game_field->start_game(_game_field->get_field_resolution(),
                          _game_field->get_mines_quantity());

  grid->set_columns(_game_field->get_field_resolution().x);
  update_grid();
}
void FieldGrid::_process(float delta){};

void FieldGrid::_on_button_pressed(int index) {
  if (_flagging_radio_button->is_pressed()) {
    _game_field->toggle_flag(index);
  } else if (!_game_field->field[index].flagged) {
    _game_field->reveal(index);
  }

  update_grid();
}

void FieldGrid::update_grid() {
  int k = _game_field->see_gameover();
  if (k == 1) {
    UtilityFunctions::print("You won!");
  } else if (k == 2) {
    _game_field->reveal_all_hidden();
  }

  for (int i = 0; i < _game_field->get_cells_quantity(); i++) {
    char text[1];

    text[0] = _game_field->field[i].mines_around + '0';

    if (_game_field->field[i].flagged) {
      text[0] = 'F';
    } else if (_game_field->field[i].hidden) {
      text[0] = 'H';
    } else if (_game_field->field[i].mine) {
      text[0] = 'x';
    }

    if (!_game_field->field[i].hidden && !_game_field->field[i].flagged) {
      ((Button *)grid->get_child(i))->set_disabled(true);
    } else {
      ((Button *)grid->get_child(i))->set_disabled(false);
    }

    ((Button *)grid->get_child(i))->set_text(text);
  }
}

void FieldGrid::go_to_menu() {
  get_tree()->change_scene_to_file("res://menu.tscn");
}

String FieldGrid::get_mines_around_label() { return _mines_around_label_path; };
void FieldGrid::set_mines_around_label(String path) {
  _mines_around_label_path = path;
};

String FieldGrid::get_time_label() { return _time_label_path; };
void FieldGrid::set_time_label(String path) { _time_label_path = path; };

String FieldGrid::get_retry_button() { return _retry_button_path; };
void FieldGrid::set_retry_button(String path) { _retry_button_path = path; };

String FieldGrid::get_flagging_radio_button() {
  return _flagging_radio_button_path;
};
void FieldGrid::set_flagging_radio_button(String path) {
  _flagging_radio_button_path = path;
};

String FieldGrid::get_game_status_label() { return _game_status_label_path; };
void FieldGrid::set_game_status_label(String path) {
  _game_status_label_path = path;
};

String FieldGrid::get_back_to_menu_button() {
  return _back_to_menu_button_path;
};
void FieldGrid::set_back_to_menu_button(String path) {
  _back_to_menu_button_path = path;
};