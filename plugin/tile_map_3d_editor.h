/*************************************************************************/
/*  tile_map_3d_editor.h                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef TILE_MAP_3D_EDITOR_H
#define TILE_MAP_3D_EDITOR_H

// #include "editor/editor_plugin.h"
#include "editor/editor_node.h"
#include "scene/gui/box_container.h"
#include "tile_map_3d.h"

class TileMap3DEditor : public VBoxContainer {
	GDCLASS(TileMap3DEditor, VBoxContainer);

private:
    static const int GRID_MIN_SIZE = 18;
    static const int GRID_MARGIN = 8;

    TileMap3D *tilemap;
    Ref<TileSet3D> tileset;

    EditorNode *editor;
    UndoRedo *undo_redo = EditorNode::get_undo_redo();

    Vector3::Axis selected_axis;
    int floor_level = 0;

    RID grid;
    RID grid_instance;
    Ref<StandardMaterial3D> grid_mat;
    Transform3D grid_xform;

    void _draw_grid();
    void _update_tileset();
    void _tileset_changed();

protected:
	void _notification(int p_what);

public:
    void edit(TileMap3D *p_tilemap);
    void set_3d_controls_visibility(bool p_visible);
    EditorPlugin::AfterGUIInput forward_gui_input(Camera3D *p_camera, const Ref<InputEvent> &p_event);

    TileMap3DEditor(EditorNode *p_editor);
    ~TileMap3DEditor();
};

#endif // TILE_MAP_3D_EDITOR_H