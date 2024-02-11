#ifndef MINE_GRID_H
#define MINE_GRID_H
#include "field.h"
#include "godot_cpp/templates/cowdata.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "godot_cpp/templates/hash_set.hpp"
#include "godot_cpp/templates/hashfuncs.hpp"
#include "godot_cpp/templates/list.hpp"
#include "godot_cpp/templates/pair.hpp"
#include "godot_cpp/templates/rb_map.hpp"
#include "godot_cpp/templates/rb_set.hpp"
#include "godot_cpp/templates/rid_owner.hpp"
#include "godot_cpp/templates/safe_refcount.hpp"
#include "godot_cpp/templates/search_array.hpp"
#include "godot_cpp/templates/self_list.hpp"
#include "godot_cpp/templates/sort_array.hpp"
#include "godot_cpp/templates/spin_lock.hpp"
#include "godot_cpp/templates/thread_work_pool.hpp"
#include "godot_cpp/templates/vector.hpp"
#include "godot_cpp/templates/vmap.hpp"
#include "godot_cpp/templates/vset.hpp"
#include "godot_cpp/variant/signal.hpp"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
namespace godot {

class MineGrid : public Node {
  GDCLASS(MineGrid, Node)

private:
  float time_passed;
  Field *_game_field;
GridContainer* grid;
public:
  static void _bind_methods();

  MineGrid();
  ~MineGrid();

  void _init(); // our initializer called by Godot

  void _ready();
  void _process(float delta);
  void _on_button_pressed(int index);

  void update_grid();
};

} // namespace godot

#endif
