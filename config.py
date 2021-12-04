def can_build(env, platform):
    return not env["disable_3d"]


def configure(env):
    pass


def get_doc_classes():
    return [
        "TileMap3D",
        "TileSet3D",
        "TileSet3DCollection",
        "TileData3D",
        "TileData3DMesh"
    ]


def get_doc_path():
    return "doc_classes"
