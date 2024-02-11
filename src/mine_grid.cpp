#include "mine_grid.h"

#include "field.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/grid_container.hpp"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MineGrid::_bind_methods() {
  ClassDB::bind_method(D_METHOD("cringe", "index"), &MineGrid::cringe);
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

  p = get_node<Field>("/root/MineField"); //("/MineField");

  grid = get_node<GridContainer>("../MineGrid"); //("/MineField");
  p->height = 8;
  p->width = 12;
  p->prepare_field();
  p->plant_mines();
  grid->set_columns(p->width);
  for (int i = 0; i < p->field.size(); i++) {
    auto but = memnew(Button());
    grid->add_child(but);

    auto calp = Callable(this, "cringe").bind(i);
    but->connect("pressed", calp);
  }
  update_grid();
}
void MineGrid::_process(float delta){};
void MineGrid::cringe(int index) {
  p->reveal(index);
  update_grid();
  UtilityFunctions::print("%d", index);
}

void MineGrid::update_grid() {

  for (int i = 0; i < p->width * p->height; i++) {
    char text[1];

    text[0] = p->field[i].mines_around + '0';

    if (p->field[i].hidden) {
      text[0] = ' ';
    } else if (p->field[i].mine) {
      text[0] = 'x';
    }

    ((Button *)grid->get_child(i))->set_text(text);
  }
}
