#include "menu.h"
#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/v_box_container.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include <godot_cpp/core/class_db.hpp>
using namespace godot;

void MainMenu::_bind_methods() {
  ClassDB::bind_method(D_METHOD("handle_input", "index"),
                       &MainMenu::handle_input);
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
  auto p = get_node<VBoxContainer>("../Buttons");
  for (int i = 0; i < get_child_count(); i++) {
    ((Button *)p->get_child(i))
        ->connect("pressed", Callable(this, "handle_input").bind(i));
  }

  _game_field = get_node<Field>("/root/MineField");
}

void MainMenu::handle_input(int index) {
  print_tree();
  switch (index) {
  case 0:
    _game_field->start_game(Vector2i(8, 8), 6);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 1:
    _game_field->start_game(Vector2i(30, 16), 10);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 2:
    _game_field->start_game(Vector2i(80, 50), 16);
    get_tree()->change_scene_to_file("res://game.tscn");
    break;
  case 3:
    get_tree()->quit();
    break;
  }
}

void MainMenu::_process(double delta) {}