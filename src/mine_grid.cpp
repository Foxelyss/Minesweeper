#include "mine_grid.h"

#include "field.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MineGrid::_bind_methods() {
  ClassDB::bind_method(D_METHOD("on_button_pressed", "index"),
                       &MineGrid::_on_button_pressed);
};

MineGrid::MineGrid(){};
MineGrid::~MineGrid(){};

void MineGrid::_init() {
  if (!Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
  }
}; // our initializer called by Godot

void MineGrid::_ready() {

  if (!Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
  }

  if (get_node_or_null("/root/MineField") == nullptr) {
    return;
  }

  _game_field = get_node<Field>("/root/MineField");
  grid = get_node<GridContainer>("../DraggableSpace/MineGrid");

  _game_field->height = 8;
  _game_field->width = 12;
  _game_field->mines_quantity = 6;

  _game_field->prepare_field();
  _game_field->plant_mines();

  grid->set_columns(_game_field->width);

  for (int i = 0; i < _game_field->field.size(); i++) {
    Button *button = memnew(Button());
    grid->add_child(button);

    button->connect("pressed", Callable(this, "on_button_pressed").bind(i));
  }
  update_grid();

  auto i = get_node<Label>("../MinesCounter");

  auto b = Variant(_game_field->mines_quantity);
  i->set_text(tr("MINESAROUND")+" " +b.stringify());
}

void MineGrid::_process(float delta){};

void MineGrid::_on_button_pressed(int index) {
  _game_field->reveal(index);
  update_grid();
  UtilityFunctions::print("%d", index);
}

void MineGrid::update_grid() {
  for (int i = 0; i < _game_field->width * _game_field->height; i++) {
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
}
