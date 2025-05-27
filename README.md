# Minesweeper made in C++ with Godot!

![изображение](https://github.com/user-attachments/assets/b36a68aa-8398-42ad-b4ec-4ebbc6ee5116)

You can play it on itch.io https://foxelyss.itch.io/minesweeper!

It was pain to program because:
 - Godot 4 API doesn't have add_property, i.e. every property needs setter
and getter and code to link it with Godot
 - Godot docs have no docs how to make a release build of GDExtension
 - There is no tutorials online how to use event's. To understand that
`_input` has as its argument `Ref<InputEvent> event` which can be
casted to pointer is impossible unless you check Godot source code.
 - GDExtension API is just a labirinth of unused functionality
 - Debugging and printing info doesn't always work as expected!

_Building is done using default sconstruct, so you can check out official docs_
