#include "menu.h"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/v_box_container.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MainMenu::_bind_methods() { ClassDB::bind_method(D_METHOD("handle_button_press", "index"), &MainMenu::handle_button_press); }

MainMenu::MainMenu() {}

MainMenu::~MainMenu() {}

void MainMenu::_ready() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    return;
  }
  auto p = get_node<VBoxContainer>("../Buttons");

  for (int i = 0; i < p->get_child_count(); i++) {
    auto v = Callable(this, "handle_button_press").bind(i);

    p->get_child(i)->get_node<Button>(".")->connect("pressed", v);
  }

  _game_field = get_node<Field>("/root/FieldRepresenter");
  get_node<AnimationPlayer>("../../../AnimationPlayer")->play("in");
}

void MainMenu::handle_button_press(int index) {
  Vector2i resolution;
  int mines_quantity;

  switch (index) {
  case 0:
    resolution = Vector2i(8, 8);
    mines_quantity = 6;
    break;
  case 1:
    resolution = Vector2i(30, 16);
    mines_quantity = 26;
    break;
  case 2:
    resolution = Vector2i(80, 50);
    mines_quantity = 60;
    break;
  case 3:
    get_tree()->quit();
    break;
  }

  _game_field->set_properties(resolution, mines_quantity);
  get_tree()->change_scene_to_file("res://game.tscn");
}

void MainMenu::_process(double delta) {}
