#include "field_grid.h"

#include "field.h"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/texture2d.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector2i.hpp"
using namespace godot;

void FieldGrid::_bind_methods() {
  ClassDB::bind_method(D_METHOD("on_button_pressed", "input", "index"),
                       &FieldGrid::_on_button_pressed);
  ClassDB::bind_method(D_METHOD("retry_game"), &FieldGrid::retry_game);
  ClassDB::bind_method(D_METHOD("go_to_menu"), &FieldGrid::go_to_menu);

  ClassDB::bind_method(D_METHOD("set_mines_around_label", "mines_around_label"),
                       &FieldGrid::set_mines_around_label);
  ClassDB::bind_method(D_METHOD("get_mines_around_label"),
                       &FieldGrid::get_mines_around_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "mines_around_label"),
                        "set_mines_around_label", "get_mines_around_label");

  ClassDB::bind_method(D_METHOD("set_time_label", "time_label"),
                       &FieldGrid::set_time_label);
  ClassDB::bind_method(D_METHOD("get_time_label"), &FieldGrid::get_time_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "time_label"),
                        "set_time_label", "get_time_label");

  ClassDB::bind_method(D_METHOD("set_retry_button", "retry_button"),
                       &FieldGrid::set_retry_button);
  ClassDB::bind_method(D_METHOD("get_retry_button"),
                       &FieldGrid::get_retry_button);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "retry_button"),
                        "set_retry_button", "get_retry_button");

  ClassDB::bind_method(
      D_METHOD("set_flagging_radio_button", "flagging_radio-button"),
      &FieldGrid::set_flagging_radio_button);
  ClassDB::bind_method(D_METHOD("get_flagging_radio_button"),
                       &FieldGrid::get_flagging_radio_button);
  ClassDB::add_property(
      "FieldGrid", PropertyInfo(Variant::NODE_PATH, "flagging_radio-button"),
      "set_flagging_radio_button", "get_flagging_radio_button");

  ClassDB::bind_method(D_METHOD("set_game_status_label", "game_status_label"),
                       &FieldGrid::set_game_status_label);
  ClassDB::bind_method(D_METHOD("get_game_status_label"),
                       &FieldGrid::get_game_status_label);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "game_status_label"),
                        "set_game_status_label", "get_game_status_label");

  ClassDB::bind_method(
      D_METHOD("set_back_to_menu_button", "back_to_menu_button"),
      &FieldGrid::set_back_to_menu_button);
  ClassDB::bind_method(D_METHOD("get_back_to_menu_button"),
                       &FieldGrid::get_back_to_menu_button);
  ClassDB::add_property("FieldGrid",
                        PropertyInfo(Variant::NODE_PATH, "back_to_menu_button"),
                        "set_back_to_menu_button", "get_back_to_menu_button");
};

FieldGrid::FieldGrid(){}; // our initializer called by Godot

FieldGrid::~FieldGrid(){};

void FieldGrid::_ready() {
  if (Engine::get_singleton()->is_editor_hint()) {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    return;
  } else {
    set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);
  }

  _time_label = get_node<Label>(NodePath(_time_label_path));
  _mines_around_label = get_node<Label>(NodePath(_mines_around_label_path));
  _game_status_label = get_node<Label>(NodePath(_game_status_label_path));
  _retry_button = get_node<Button>(NodePath(_retry_button_path));
  _back_to_menu_button = get_node<Button>(NodePath(_back_to_menu_button_path));
  _flagging_radio_button =
      get_node<Button>(NodePath(_flagging_radio_button_path));

  _back_to_menu_button->connect("pressed", Callable(this, "go_to_menu"));
  _retry_button->connect("pressed", Callable(this, "retry_game"));

  _game_field = get_node<Field>("/root/FieldRepresenter");
  grid = get_node<GridContainer>("../DraggableSpace/MineGrid");

  _ui_tweener = get_node<AnimationPlayer>("../AnimationPlayer");
  _ui_tweener->play("popin");

  _timer = memnew(Timer);
  add_child(_timer);
  _timer->set_one_shot(true);
  _timer->start(20 * 60);

  start_game(_game_field->get_field_resolution(),
             _game_field->get_mines_quantity());
}

void FieldGrid::start_game(Vector2i resolution, int mines_quantity) {
  grid->set_columns(resolution.x);

  for (int i = 0; i < _game_field->get_cells_quantity(); i++) {
    Button *button = memnew(Button());
    grid->add_child(button);

    button->set_custom_minimum_size(Vector2(28, 28));
    button->set_button_mask(MOUSE_BUTTON_LEFT | MOUSE_BUTTON_RIGHT);
    button->connect("gui_input", Callable(this, "on_button_pressed").bind(i));
  }
}

void FieldGrid::retry_game() {
  _first_cell = -1;

  _timer->set_paused(false);
  _timer->start();

  _ui_tweener->play("retry");
  for (int i = 0; i < _game_field->get_cells_quantity(); i++) {
    ((Button *)grid->get_child(i))->set_disabled(false);
    ((Button *)grid->get_child(i))->set_text("");
    ((Button *)grid->get_child(i))->set_button_icon(Texture2D().create_placeholder());
  }
}
void FieldGrid::_process(float delta) {
  String time_left =
      tr("TIMECOUNTER") +
      Variant(int(20 * 60 - _timer->get_time_left())).stringify();
  _time_label->set_text(time_left);
};

void FieldGrid::_on_button_pressed(InputEvent *event, int index) {
  if (event->get_class() == "InputEventMouseButton" && event->is_pressed() &&
      !((Button *)grid->get_child(index))->is_disabled()) {
    auto inp = (InputEventMouseButton *)event;

    if (inp->get_button_index() == MOUSE_BUTTON_RIGHT ||
        _flagging_radio_button->is_pressed()) {
      _game_field->toggle_flag(index);
    } else if (inp->get_button_index() == MOUSE_BUTTON_LEFT) {
      if (_first_cell == -1) {
        _game_field->start_game(index);
        auto string = tr("MINESAROUND") + " " +
                      Variant(_game_field->get_mines_quantity()).stringify();
        _mines_around_label->set_text(string);
        _first_cell = index;
      }

      if (!_game_field->field[index].flagged) {
        _game_field->reveal(index);
      }
    }
    update_grid();
  }
}

void FieldGrid::update_grid() {
  int k = _game_field->see_gameover();
  if (_first_cell == -1) {
    return;
  }
  if (k > 0) {
    _timer->set_paused(true);
    _game_field->reveal_all_hidden();
    _ui_tweener->play_backwards("retry");
  }

  switch (k) {
  case 1:
    _game_status_label->set_text(tr("WIN"));
    break;
  case 2:
    _game_status_label->set_text(tr("LOSE"));
    break;
  default:
    _game_status_label->set_text("");
    break;
  }

  for (int i = 0; i < _game_field->get_cells_quantity(); i++) {
    String text;
    text = Variant(_game_field->field[i].mines_around).stringify();

    Button *target = ((Button *)grid->get_child(i));
    target->set_button_icon(Texture2D().create_placeholder());
    if (_game_field->field[i].flagged) {
      text = "";
      auto _icon_b =
          ResourceLoader::get_singleton()->load("res://assets/Page 1.png");
      target->set_button_icon(_icon_b);
    } else if (_game_field->field[i].hidden) {
      text = " ";
    } else if (_game_field->field[i].mine) {
      text = "";
      auto _icon_a =
          ResourceLoader::get_singleton()->load("res://assets/Page 2.png");
      target->set_button_icon(_icon_a);
    } else if (_game_field->field[i].mines_around == 0 &&
               !_game_field->field[i].hidden) {
      text = " ";
    }

    if (!_game_field->field[i].hidden && !_game_field->field[i].flagged) {
      target->set_disabled(true);
    } else {
      target->set_disabled(false);
    }

    target->set_text(text);
  }
}

void FieldGrid::go_to_menu() {
  get_tree()->change_scene_to_file("res://menu.tscn");
}

void FieldGrid::_input(InputEvent *event) {
  auto input = Input::get_singleton();
  if (event->get_class() == "InputEventMouseMotion" &&
      input->is_action_pressed("move_mode")) {

    auto ev = (InputEventMouseMotion *)event;
    auto rot = grid->get_position();
    Vector2 sizp = grid->get_size();
    sizp.x /= 2;
    sizp.y /= 2;

    rot.y = Math::clamp<float>(rot.y + ev->get_relative().y, -sizp.y, sizp.y);
    rot.x = Math::clamp<float>(rot.x + ev->get_relative().x, -sizp.x, sizp.x);
    grid->set_position(rot);
  }
}

String FieldGrid::get_mines_around_label() { return _mines_around_label_path; };
void FieldGrid::set_mines_around_label(String path) {
  _mines_around_label_path = path;
};

String FieldGrid::get_time_label() { return _time_label_path; };
void FieldGrid::set_time_label(String path) { _time_label_path = path; };

String FieldGrid::get_retry_button() { return _retry_button_path; };
void FieldGrid::set_retry_button(String path) { _retry_button_path = path; };

String FieldGrid::get_flagging_radio_button() {
  return _flagging_radio_button_path;
};
void FieldGrid::set_flagging_radio_button(String path) {
  _flagging_radio_button_path = path;
};

String FieldGrid::get_game_status_label() { return _game_status_label_path; };
void FieldGrid::set_game_status_label(String path) {
  _game_status_label_path = path;
};

String FieldGrid::get_back_to_menu_button() {
  return _back_to_menu_button_path;
};
void FieldGrid::set_back_to_menu_button(String path) {
  _back_to_menu_button_path = path;
};
