#include "menu.h"
#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/v_box_container.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MainMenu::_bind_methods() {
  ClassDB::bind_method(D_METHOD("handle_button_press", "index"),
                       &MainMenu::handle_button_press);
}

MainMenu::MainMenu() {
  // Initialize any variables here.
  time_passed = 0.0;
}

MainMenu::~MainMenu() {
  // Add your cleanup here.
}

void MainMenu::_ready() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    return;
  }
  print_tree();
  auto p = get_node<VBoxContainer>("../Buttons");

  for (int i = 0; i < p->get_child_count(); i++) {
    auto v = Callable(this, "handle_button_press").bind(i);

    p->get_child(i)->get_node<Button>(".")->connect("pressed", v);
  }

  _game_field = get_node<Field>("/root/MineField");
}

void MainMenu::handle_button_press(int index) {
  switch (index) {
  case 0:
    _game_field->set(Vector2i(8, 8), 6);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 1:
    _game_field->set(Vector2i(30, 16), 26);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 2:
    _game_field->set(Vector2i(80, 50), 60);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 3:
    get_tree()->quit();
    break;
  }
}

void MainMenu::_process(double delta) {}