#ifndef MINE_GRID_H
#define MINE_GRID_H
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {

class MineGrid : public GridContainer {
  GDCLASS(MineGrid, GridContainer)

private:
  float time_passed;

public:
  static void _bind_methods();

  MineGrid();
  ~MineGrid();

  void _init(); // our initializer called by Godot

  void _ready();
  void _process(float delta);
};

} // namespace godot

#endif