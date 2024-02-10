#include "mine_grid.h"
#include "field.h"

using namespace godot;

void MineGrid::_bind_methods(){
    // register_method("_ready", &MineGrid::_ready);
    // register_method("_process", &MineGrid::_process);
};

MineGrid::MineGrid(){};
MineGrid::~MineGrid(){};

void MineGrid::_init(){}; // our initializer called by Godot
void MineGrid::_ready() {
  auto p = (Field *)get_parent();
  p->height = 8;
  p->width = 8;
  p->prepare_field();
  p->plant_mines();
  set_columns(8);
  for (auto i : p->field) {
    auto but = new Button();
    char text[1];
    text[0] = (char)(i.mines_around + '0');
    but->set_text(text);
    add_child(but);
    print_tree();
  }
}
void MineGrid::_process(float delta){};
