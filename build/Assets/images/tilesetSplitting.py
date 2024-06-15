from PIL import Image

image_path = "/home/gizmo/Levi/Projects/GameProgramming/Mario/assets/images/tileset.png"
tileset = Image.open(image_path)

width, height = tileset.size

tile_size = 16

tree_image = tileset.crop((4 * tile_size, 5 * tile_size, 5 * tile_size, 6 * tile_size))
tree_image_path = f"rock2.png"
tree_image.save(tree_image_path)
