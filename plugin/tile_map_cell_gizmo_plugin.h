/*************************************************************************/
/*  tile_map_cell_gizmo_plugin.h                                         */
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

#ifndef TILE_MAP_CELL_GIZMO_PLUGIN_H
#define TILE_MAP_CELL_GIZMO_PLUGIN_H

#include "editor/plugins/node_3d_editor_plugin.h"
#include "tile_map_3d.h"

class TileMapCellGizmoPlugin : public EditorNode3DGizmoPlugin {
	GDCLASS(TileMapCellGizmoPlugin, EditorNode3DGizmoPlugin);

private:

// protected:
// 	void _notification(int p_what);

public:
    bool has_gizmo(Node3D *p_spatial) override;
	bool can_be_hidden() const override;
	String get_gizmo_name() const override;
	// int get_priority() const override;

	// String get_handle_name(const EditorNode3DGizmo *p_gizmo, int p_id) const override;
	// Variant get_handle_value(const EditorNode3DGizmo *p_gizmo, int p_id) const override;
	// void set_handle(const EditorNode3DGizmo *p_gizmo, int p_id, Camera3D *p_camera, const Point2 &p_point) override;
	// void commit_handle(const EditorNode3DGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false) override;
	void redraw(EditorNode3DGizmo *p_gizmo) override;

    TileMapCellGizmoPlugin(Control *p_control);
    ~TileMapCellGizmoPlugin();
};

#endif // TILE_MAP_CELL_GIZMO_PLUGIN_H