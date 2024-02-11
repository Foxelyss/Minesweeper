extends Button


# Called when the node enters the scene tree for the first time.
func _ready():
	connect("pressed", _j)
func _j():
	print(999)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
