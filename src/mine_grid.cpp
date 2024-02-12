#include "mine_grid.h"

#include "field.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MineGrid::_bind_methods() {
  ClassDB::bind_method(D_METHOD("on_button_pressed", "index"),
                       &MineGrid::_on_button_pressed);

  ClassDB::bind_method(D_METHOD("set_mines_around_label", "node"),
                       &MineGrid::set_mines_around_label);
  ClassDB::bind_method(D_METHOD("get_mines_around_label"),
                       &MineGrid::get_mines_around_label);
  ClassDB::add_property("MineGrid",
                        PropertyInfo(Variant::NODE_PATH, "mines_around_label"),
                        "set_mines_around_label", "get_mines_around_label");

  ClassDB::bind_method(D_METHOD("set_time_label", "node"),
                       &MineGrid::set_time_label);
  ClassDB::bind_method(D_METHOD("get_time_label"), &MineGrid::get_time_label);
  ClassDB::add_property("MineGrid",
                        PropertyInfo(Variant::NODE_PATH, "time_label"),
                        "set_time_label", "get_time_label");
};

MineGrid::MineGrid() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
  }
}; // our initializer called by Godot

MineGrid::~MineGrid(){};

void MineGrid::_ready() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    return;
  }

  _mines_around_label = get_node<Label>(NodePath(_mines_around_label_path));
  _game_field = get_node<Field>("/root/MineField");
  grid = get_node<GridContainer>("../DraggableSpace/MineGrid");

  create_game();
}

void MineGrid::create_game() {
  _game_field->start_game(Vector2i(12, 12), 6);

  grid->set_columns(12);
  if (grid->get_child_count() < _game_field->get_cells_quantity()) {
    for (int i = grid->get_child_count() - 1; i < _game_field->get_cells_quantity(); i++) {
      Button *button = memnew(Button());
      grid->add_child(button);

      button->set_size(Vector2(40, 40));
      button->connect("pressed", Callable(this, "on_button_pressed").bind(i));
    }
  }

  update_grid();

  auto b = Variant(_game_field->get_mines_quantity());
  _mines_around_label->set_text(tr("MINESAROUND") + " " + b.stringify());
}
void MineGrid::_process(float delta){};

void MineGrid::_on_button_pressed(int index) {
  _game_field->reveal(index);
  update_grid();
}

void MineGrid::update_grid() {
  for (int i = 0; i < _game_field->get_cells_quantity(); i++) {
    char text[1];

    text[0] = _game_field->field[i].mines_around + '0';

    if (_game_field->field[i].hidden) {
      text[0] = ' ';
    } else if (_game_field->field[i].mine) {
      text[0] = 'x';
    }

    ((Button *)grid->get_child(i))->set_text(text);
  }

  int k = _game_field->see_gameover();
  if (k) {
    UtilityFunctions::print(k);
  }
  if (k == 2) {
    create_game();
  }
}

String MineGrid::get_mines_around_label() { return _mines_around_label_path; };
void MineGrid::set_mines_around_label(String path) {
  _mines_around_label_path = path;
};

String MineGrid::get_time_label() { return _time_label_path; };
void MineGrid::set_time_label(String path) { _time_label_path = path; };
