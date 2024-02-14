#ifndef MENU_H
#define MENU_H

#include "field.h"
#include "godot_cpp/classes/v_box_container.hpp"
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

class MainMenu : public Node {
  GDCLASS(MainMenu, Node)

private:
  double time_passed;
  Field *_game_field;

protected:
  static void _bind_methods();

public:
  MainMenu();
  ~MainMenu();

  void _ready() override;
  void handle_input(int index);
  void _process(double delta) override;
};

} // namespace godot

#endif