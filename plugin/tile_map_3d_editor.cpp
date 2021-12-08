/*************************************************************************/
/*  tile_map_3d_editor.cpp                                               */
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

#include "editor/plugins/node_3d_editor_plugin.h"
#include "tile_map_3d_editor.h"

#define CREATE_POINTS(i, j, b1, b2, a1, a2) \
    Vector2 p1 = i * b1 + j * b2;           \
    Vector2 p2 = (i + 1) * b1 + j * b2;     \
    Vector2 p3 = i * b1 + (j + 1) * b2;     \
    Vector3 v1 = Vector3();                 \
    v1[a1] = p1.x, v1[a2] = p1.y;           \
    Vector3 v2 = Vector3();                 \
    v2[a1] = p2.x, v2[a2] = p2.y;           \
    Vector3 v3 = Vector3();                 \
    v3[a1] = p3.x, v3[a2] = p3.y;

#define PUSH_POINTS(v1, v2, v3, a)       \
    points.push_back(v1);                \
    points.push_back(v2);                \
    points.push_back(v1);                \
    points.push_back(v3);                \
    colors.push_back(Color(1, 1, 1, a)); \
    colors.push_back(Color(1, 1, 1, a)); \
    colors.push_back(Color(1, 1, 1, a)); \
    colors.push_back(Color(1, 1, 1, a));

#define CREATE_POINTS2(i, j, ii, jj, b1, b2, a1, a2) \
    Vector2 p1 = i * b1 + j * b2;                    \
    Vector2 p2 = (i + ii) * b1 + (j + jj) * b2;      \
    Vector3 v1 = Vector3();                          \
    v1[a1] = p1.x, v1[a2] = p1.y;                    \
    Vector3 v2 = Vector3();                          \
    v2[a1] = p2.x, v2[a2] = p2.y;

#define PUSH_POINTS2(v1, v2, a)          \
    points.push_back(v1);                \
    points.push_back(v2);                \
    colors.push_back(Color(1, 1, 1, a)); \
    colors.push_back(Color(1, 1, 1, a));

Pair<Vector<Vector3>, Vector<Color>> get_grid_lines_cf(const Vector2i &p0, const Vector2i &sz, const Vector2 &b1, const Vector2 &b2, int a1, int a2, int margin) {
    const float MIN_ALPHA = 0.05;
    float m = (1.0 - MIN_ALPHA) / margin;
    Vector<Vector3> points;
    Vector<Color> colors;
    for (int i = p0.x; i < p0.x + sz.x; i++) {
        for (int j = p0.y; j < p0.y + sz.y; j++) {
            CREATE_POINTS(i, j, b1, b2, a1, a2);
            PUSH_POINTS(v1, v2, v3, 1.0);
        }
    }
    int top = p0.y + sz.y + margin;
    for (int i = p0.x - margin; i < p0.x; i++) {
        for (int j = p0.y - margin; j < top; j++) {
            CREATE_POINTS(i, j, b1, b2, a1, a2);
            float a = (i - p0.x + margin) * m + MIN_ALPHA;
            if (j < p0.y) {
                a *= (j - p0.y + margin) * m + MIN_ALPHA;
            } else if (j > p0.y + sz.y) {
                a *= (p0.y + sz.y + margin - j - 1) * m + MIN_ALPHA;
            }
            PUSH_POINTS(v1, v2, v3, a);
        }
        CREATE_POINTS2(i, top, 1, 0, b1, b2, a1, a2);
        float a = MIN_ALPHA * ((i - p0.x + margin) * m + MIN_ALPHA);
        PUSH_POINTS2(v1, v2, a);
    }
    int right = p0.x + sz.x + margin;
    for (int i = p0.x + sz.x; i < right; i++) {
        for (int j = p0.y - margin; j < top; j++) {
            CREATE_POINTS(i, j, b1, b2, a1, a2);
            float a = (right - i - 1) * m + MIN_ALPHA;
            if (j < p0.y) {
                a *= (j - p0.y + margin) * m + MIN_ALPHA;
            } else if (j > p0.y + sz.y) {
                a *= (p0.y + sz.y + margin - j - 1) * m + MIN_ALPHA;
            }
            PUSH_POINTS(v1, v2, v3, a);
        }
        CREATE_POINTS2(i, top, 1, 0, b1, b2, a1, a2);
        float a = MIN_ALPHA * ((right - i - 1) * m + MIN_ALPHA);
        PUSH_POINTS2(v1, v2, a);
    }
    for (int j = p0.y - margin; j < top; j++) {
        CREATE_POINTS2(right, j, 0, 1, b1, b2, a1, a2);
        float a = MIN_ALPHA;
        if (j < p0.y) {
            a *= (j - p0.y + margin) * m + MIN_ALPHA;
        } else if (j > p0.y + sz.y) {
            a *= (p0.y + sz.y + margin - j - 1) * m + MIN_ALPHA;
        }
        PUSH_POINTS2(v1, v2, a);
    }
    for (int j = p0.y - margin; j < p0.y; j++) {
        for (int i = p0.x; i < p0.x + sz.x; i++) {
            CREATE_POINTS(i, j, b1, b2, a1, a2);
            float a = (j - p0.y + margin) * m + MIN_ALPHA;
            PUSH_POINTS(v1, v2, v3, a);
        }
    }
    for (int j = p0.y + sz.y; j < top; j++) {
        for (int i = p0.x; i < p0.x + sz.x; i++) {
            CREATE_POINTS(i, j, b1, b2, a1, a2);
            float a = (top - j - 1) * m + MIN_ALPHA;
            PUSH_POINTS(v1, v2, v3, a);
        }
    }
    for (int i = p0.x; i < p0.x + sz.x; i++) {
        CREATE_POINTS2(i, top, 1, 0, b1, b2, a1, a2);
        PUSH_POINTS2(v1, v2, MIN_ALPHA);
    }
    return Pair<Vector<Vector3>, Vector<Color>>(points, colors);
}
#undef CREATE_POINTS
#undef PUSH_POINTS
#undef CREATE_POINTS2
#undef PUSH_POINTS2

void TileMap3DEditor::_draw_grid() {
    RS::get_singleton()->mesh_clear(grid);

    if (tileset.is_null()) {
        return;
    }

    Rect2i rect = tilemap->get_used_cells_rect_proj(selected_axis);
    int minsz = 2 * GRID_MIN_SIZE + 1;
    if (rect.size.x < minsz) {
        rect.position.x -= (minsz - rect.size.x) * 0.5;
        rect.size.x = minsz;
    }
    if (rect.size.y < minsz) {
        rect.position.y -= (minsz - rect.size.y) * 0.5;
        rect.size.y = minsz;
    }

    bool inv[3] = {tileset->is_axis_inverted_x(), tileset->is_axis_inverted_y(), tileset->is_axis_inverted_z()};
    int axis1 = (selected_axis + 1) % 3;
    int axis2 = (selected_axis + 2) % 3;
    Basis basis = tilemap->get_cell_basis_vectors();
    if (inv[axis1]) {
        rect.position.x += rect.size.x;
        basis[axis1] = -basis[axis1];
    }
    if (inv[axis2]) {
        rect.position.y += rect.size.y;
        basis[axis2] = -basis[axis2];
    }
    Pair<Vector<Vector3>, Vector<Color>> r = get_grid_lines_cf(rect.position, rect.size, Vector2(basis[axis1][axis1], basis[axis1][axis2]), Vector2(basis[axis2][axis1], basis[axis2][axis2]), axis1, axis2, GRID_MARGIN);
    Array d;
    d.resize(RS::ARRAY_MAX);
    d[RS::ARRAY_VERTEX] = r.first;
    d[RS::ARRAY_COLOR] = r.second;
    RS::get_singleton()->mesh_add_surface_from_arrays(grid, RenderingServer::PRIMITIVE_LINES, d);
	RS::get_singleton()->mesh_surface_set_material(grid, 0, grid_mat->get_rid());
}

void TileMap3DEditor::_update_tileset() {
    if (tileset.is_valid()) {
        tileset->disconnect("changed", callable_mp(this, &TileMap3DEditor::_tileset_changed));
    }

    tileset = tilemap->get_tileset();

    if (tileset.is_valid()) {
        tileset->connect("changed", callable_mp(this, &TileMap3DEditor::_tileset_changed));
    }

    if (tileset.is_null()) {
        set_3d_controls_visibility(false);
    }

    _tileset_changed();
}

void TileMap3DEditor::_tileset_changed() {
    // update_palette
    _draw_grid();
}

void TileMap3DEditor::edit(TileMap3D *p_tilemap) {
    if (tilemap == p_tilemap && tileset == p_tilemap->get_tileset()) {
        if (tilemap && tileset.is_valid()) {
            set_3d_controls_visibility(true);
        }
        return;
    }

    tilemap = p_tilemap;

    if (!tilemap) {
        set_process(false);
        if (tileset.is_valid()) {
            tileset.unref();
        }
        set_3d_controls_visibility(false);
    } else {
        tilemap->set_transform_gizmo_visible(false);
        set_process(true);
        _update_tileset();
    }
}

void TileMap3DEditor::set_3d_controls_visibility(bool p_visible) {
    set_process(p_visible);
    RS::get_singleton()->instance_set_visible(grid_instance, p_visible);
}

EditorPlugin::AfterGUIInput TileMap3DEditor::forward_gui_input(Camera3D *p_camera, const Ref<InputEvent> &p_event) {
    if (!tilemap) {
		return EditorPlugin::AFTER_GUI_INPUT_PASS;
	}
    return EditorPlugin::AFTER_GUI_INPUT_PASS;
}

void TileMap3DEditor::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            grid = RS::get_singleton()->mesh_create();
            grid_instance = RS::get_singleton()->instance_create2(grid, get_tree()->get_root()->get_world_3d()->get_scenario());
            RS::get_singleton()->instance_set_layer_mask(grid_instance, 1 << Node3DEditorViewport::MISC_TOOL_LAYER);
            cell_plugin = Ref<TileMapCellGizmoPlugin>(memnew(TileMapCellGizmoPlugin(this)));
            Node3DEditor::get_singleton()->add_gizmo_plugin(cell_plugin);
        } break;
        case NOTIFICATION_EXIT_TREE: {
            RS::get_singleton()->free(grid_instance);
            RS::get_singleton()->free(grid);
            grid_instance = RID();
            grid = RID();
            Node3DEditor::get_singleton()->remove_gizmo_plugin(cell_plugin);
            Ref<TileMapCellGizmoPlugin>(cell_plugin).unref();
        } break;
        case NOTIFICATION_PROCESS: {
			if (!tilemap) {
				return;
			}

			Transform3D xf = tilemap->get_global_transform();

			if (xf != grid_xform) {
				RS::get_singleton()->instance_set_transform(grid_instance, xf);
				grid_xform = xf;
			}
            if (tileset != tilemap->get_tileset()) {
                _update_tileset();
            }
		} break;
    }
}

TileMap3DEditor::TileMap3DEditor(EditorNode *p_editor) {
    // ToolMode Node3DEditor::get_singleton()->get_tool_mode()
    editor = p_editor;

    grid_mat.instantiate();
    grid_mat->set_shading_mode(StandardMaterial3D::SHADING_MODE_UNSHADED);
	grid_mat->set_transparency(StandardMaterial3D::TRANSPARENCY_ALPHA);
	grid_mat->set_flag(StandardMaterial3D::FLAG_SRGB_VERTEX_COLOR, true);
	grid_mat->set_flag(StandardMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	grid_mat->set_albedo(Color(0.8, 0.5, 0.1));

    selected_axis = Vector3::AXIS_Y;
}

TileMap3DEditor::~TileMap3DEditor() {

}