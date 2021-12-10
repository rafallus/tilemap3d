/*************************************************************************/
/*  tile_map_cell_gizmo_plugin.cpp                                       */
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

#include "tile_map_cell_gizmo_plugin.h"

bool TileMapCellGizmoPlugin::has_gizmo(Node3D *p_spatial) {
    return Object::cast_to<TileMap3D>(p_spatial) != nullptr;
}

bool TileMapCellGizmoPlugin::can_be_hidden() const {
    return false;
}

String TileMapCellGizmoPlugin::get_gizmo_name() const {
    return "TileMap3D";
}

void TileMapCellGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
    p_gizmo->clear();

    // PackedVector3Array lines;
    // lines.push_back(Vector3(0.5, 1, 0));
    // lines.push_back(Vector3(0.5, 2, 0));

    // PackedVector3Array handles;
    // handles.push_back(Vector3(-0.5, 1, 0));
    // handles.push_back(Vector3(-0.5, 2, 0));

    // p_gizmo->add_lines(lines, get_material("main", p_gizmo), false);
    // p_gizmo->add_handles(handles, get_material("handles", p_gizmo));

}

TileMapCellGizmoPlugin::TileMapCellGizmoPlugin(Control *p_control) {
    create_material("main", p_control->get_theme_color(SNAME("axis_x_color"), SNAME("Editor")));
    create_handle_material("handles");
}

TileMapCellGizmoPlugin::~TileMapCellGizmoPlugin() {

}