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

//#include "core/object/message_queue.h"
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

#define MIN_ALPHA 0.05

Pair<Vector<Vector3>, Vector<Color>> get_grid_lines_cuboid(const Vector2i &p0, const Vector2i &sz, const Vector2 &b1, const Vector2 &b2, int a1, int a2, int margin) {
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

// Pair<Vector<Vector3>, Vector<Color>> get_grid_lines_hexprism(const Vector2i &p0, const Vector2i &sz, const Vector2 &b1, const Vector2 &b2, int a1, int a2, int margin) {

// }

#undef MIN_ALPHA

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
    if (rect.size.y < minsz - 6) {
        rect.position.y -= (minsz - 6 - rect.size.y) * 0.5;
        rect.size.y = minsz - 6;
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
    Pair<Vector<Vector3>, Vector<Color>> r;
    if (tileset->get_tile_shape() == TileSet3D::TILE_SHAPE_CUBOID) {
        r = get_grid_lines_cuboid(rect.position, rect.size, Vector2(basis[axis1][axis1], basis[axis1][axis2]), Vector2(basis[axis2][axis1], basis[axis2][axis2]), axis1, axis2, GRID_MARGIN);
    }
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
        tileset->connect("changed", callable_mp(this, &TileMap3DEditor::_tileset_changed), Vector<Variant>(), CONNECT_DEFERRED);
    }

    if (tileset.is_null()) {
        set_3d_controls_visibility(false);
    }

    _tileset_changed();
}

void TileMap3DEditor::_tileset_changed() {
    // Wait to make sure tilemap is updated
    //MessageQueue::get_singleton()->push_callable(callable_mp(this, &TileMap3DEditor::_apply_tileset_changed));
    _draw_grid();
    _update_tileset_ui();
}

void TileMap3DEditor::_apply_tileset_changed() {
    // update_palette
    _draw_grid();
}

void TileMap3DEditor::_fit_tools_buttons() {
    float w = get_size().width;
    float bw = select_tool_button->get_size().width;
    int hsep = get_theme_constant(SNAME("hseparation"), SNAME("GridContainer"));
    List<BaseButton*> buttons;
    tools_group->get_buttons(&buttons);
    int nb = buttons.size();
    int ncolums = MIN(Math::floor((w + hsep) / (bw + hsep)), nb);
    if (ncolums == tools_columns) {
        return;
    }
    float bh = select_tool_button->get_size().height;
    int vsep = get_theme_constant(SNAME("vseparation"), SNAME("GridContainer"));
    int ic = 0, ir = 0;
    for (BaseButton *button : buttons) {
        button->set_position(Point2(ic * (bw + hsep), ir * (bh + vsep)));
        ic++;
        if (ic == ncolums) {
            ic = 0;
            ir++;
        }
    }
    int nrows = Math::ceil(nb / float(ncolums));
    tools_container->set_custom_minimum_size(Size2(bw, nrows * bh + (nrows - 1) * vsep));
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

void TileMap3DEditor::_update_tileset_ui() {
    int selected = collection_options->get_selected();
    int ncollections = collection_options->get_item_count();
    collection_options->clear();

    if (tileset.is_null() || tileset->get_collection_count() == 0) {
        collection_options->add_item(TTR("(No Collections Found)"), -2); // Id = -1 will make the Id to be 0
        return;
    }

    for (int i = 0; i < tileset->get_collection_count(); i++) {
        int id = tileset->get_collection_id(i);
        String collection_name = tileset->get_collection_name(id);
        if (collection_name.is_empty()) {
            String nm = tileset->get_collection_type(id) == TileSet3DCollection::COLLECTION_TYPE_ATLAS ? TTR("Atlas Collection") : TTR("Scene Collection");
            collection_name = vformat("[%s]", nm);
        }
        String text = vformat("%s (ID: %d)", collection_name, id);
        Ref<Texture2D> icon = tileset->get_collection_icon(id);
        collection_options->add_icon_item(icon, collection_name, id);
    }

    if (collection_options->get_item_count() == ncollections) {
        collection_options->select(selected);
    } else {
        collection_options->select(0);
    }
    _collection_selected(collection_options->get_selected());
}

void TileMap3DEditor::_collection_selected(int p_index) {
    int cid = collection_options->get_item_id(p_index);

    for (int i = 0; i < tileset->get_collection_tile_count(cid); i++) {
        int id = tileset->get_collection_tile_id(cid, i);
        Ref<TileData3D> data = tileset->get_collection_tile(cid, id);
        tile_list->add_item(data->get_name(), data->get_preview());
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

            [[fallthrough]];
        }
        case NOTIFICATION_THEME_CHANGED: {
            select_tool_button->set_icon(get_theme_icon("ToolSelect", "EditorIcons"));
            paint_tool_button->set_icon(get_theme_icon("Edit", "EditorIcons"));
            brush_tool_button->set_icon(get_theme_icon("CanvasItem", "EditorIcons"));
            bucket_tool_button->set_icon(get_theme_icon("Bucket", "EditorIcons"));
            wand_tool_button->set_icon(get_theme_icon("Wand", "EditorIcons"));
            picker_tool_button->set_icon(get_theme_icon("ColorPick", "EditorIcons"));
            eraser_tool_button->set_icon(get_theme_icon("Eraser", "EditorIcons"));
            display_thumbnails_button->set_icon(get_theme_icon("FileThumbnail", "EditorIcons"));
            display_list_button->set_icon(get_theme_icon("FileList", "EditorIcons"));
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
        case NOTIFICATION_RESIZED: {
            _fit_tools_buttons();
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

    set_custom_minimum_size(Size2(130, 0) * EDSCALE);

    tools_container = memnew(Control);
    tools_container->set_h_size_flags(SIZE_EXPAND_FILL);
    tools_container->connect("resized", callable_mp(this, &TileMap3DEditor::_fit_tools_buttons), Vector<Variant>(), CONNECT_DEFERRED);
    add_child(tools_container);

    tools_group.instantiate();
    //display_mode_group->connect("pressed", callable_mp(this, &TileSet3DEditor::_display_mode_button_pressed));

    select_tool_button = memnew(Button);
    select_tool_button->set_toggle_mode(true);
    select_tool_button->set_pressed(true);
    select_tool_button->set_flat(true);
    select_tool_button->set_button_group(tools_group);
    select_tool_button->set_tooltip(TTR("Select, move, rotate and scale tiles."));
    tools_container->add_child(select_tool_button);

    paint_tool_button = memnew(Button);
    paint_tool_button->set_toggle_mode(true);
    paint_tool_button->set_pressed(false);
    paint_tool_button->set_flat(true);
    paint_tool_button->set_button_group(tools_group);
    paint_tool_button->set_tooltip(TTR("Paint on tiles overriding existing ones."));
    tools_container->add_child(paint_tool_button);

    brush_tool_button = memnew(Button);
    brush_tool_button->set_toggle_mode(true);
    brush_tool_button->set_pressed(false);
    brush_tool_button->set_flat(true);
    brush_tool_button->set_button_group(tools_group);
    brush_tool_button->set_tooltip(TTR("Paint above existing tiles."));
    tools_container->add_child(brush_tool_button);

    bucket_tool_button = memnew(Button);
    bucket_tool_button->set_toggle_mode(true);
    bucket_tool_button->set_pressed(false);
    bucket_tool_button->set_flat(true);
    bucket_tool_button->set_button_group(tools_group);
    bucket_tool_button->set_tooltip(TTR("Fill available cells with a single tile within an octant."));
    tools_container->add_child(bucket_tool_button);

    wand_tool_button = memnew(Button);
    wand_tool_button->set_toggle_mode(true);
    wand_tool_button->set_pressed(false);
    wand_tool_button->set_flat(true);
    wand_tool_button->set_button_group(tools_group);
    wand_tool_button->set_tooltip(TTR("Tile customization tool."));
    tools_container->add_child(wand_tool_button);

    picker_tool_button = memnew(Button);
    picker_tool_button->set_toggle_mode(true);
    picker_tool_button->set_pressed(false);
    picker_tool_button->set_flat(true);
    picker_tool_button->set_button_group(tools_group);
    picker_tool_button->set_tooltip(TTR("Select a tile as the current brush."));
    tools_container->add_child(picker_tool_button);

    eraser_tool_button = memnew(Button);
    eraser_tool_button->set_toggle_mode(true);
    eraser_tool_button->set_pressed(false);
    eraser_tool_button->set_flat(true);
    eraser_tool_button->set_button_group(tools_group);
    eraser_tool_button->set_tooltip(TTR("Delete tiles."));
    tools_container->add_child(eraser_tool_button);

    HSeparator *separator1 = memnew(HSeparator);
    add_child(separator1);

    HBoxContainer *axis_edit_container = memnew(HBoxContainer);
    axis_edit_container->set_h_size_flags(SIZE_EXPAND_FILL);
    axis_edit_container->add_theme_constant_override(SNAME("hseparation"), 0);
    add_child(axis_edit_container);

    Ref<ButtonGroup> axis_edit_group = memnew(ButtonGroup);

    Button *axis_edit_x = memnew(Button);
    axis_edit_x->set_text("X");
    axis_edit_x->set_toggle_mode(true);
    axis_edit_x->set_pressed(false);
    axis_edit_x->set_h_size_flags(SIZE_EXPAND_FILL);
    axis_edit_x->set_button_group(axis_edit_group);
    axis_edit_x->add_theme_font_size_override("font_size", 10 * EDSCALE);
    axis_edit_container->add_child(axis_edit_x);

    Button *axis_edit_y = memnew(Button);
    axis_edit_y->set_text("Y");
    axis_edit_y->set_toggle_mode(true);
    axis_edit_y->set_pressed(false);
    axis_edit_y->set_h_size_flags(SIZE_EXPAND_FILL);
    axis_edit_y->set_button_group(axis_edit_group);
    axis_edit_y->add_theme_font_size_override("font_size", 10 * EDSCALE);
    axis_edit_container->add_child(axis_edit_y);

    Button *axis_edit_z = memnew(Button);
    axis_edit_z->set_text("Z");
    axis_edit_z->set_toggle_mode(true);
    axis_edit_z->set_pressed(false);
    axis_edit_z->set_h_size_flags(SIZE_EXPAND_FILL);
    axis_edit_z->set_button_group(axis_edit_group);
    axis_edit_z->add_theme_font_size_override("font_size", 10 * EDSCALE);
    axis_edit_container->add_child(axis_edit_z);

    Button *axis_edit_m = memnew(Button);
    axis_edit_m->set_text("M");
    axis_edit_m->set_toggle_mode(true);
    axis_edit_m->set_pressed(true);
    axis_edit_m->set_h_size_flags(SIZE_EXPAND_FILL);
    axis_edit_m->set_button_group(axis_edit_group);
    axis_edit_m->add_theme_font_size_override("font_size", 10 * EDSCALE);
    axis_edit_container->add_child(axis_edit_m);

    VSplitContainer *bottom_split = memnew(VSplitContainer);
    bottom_split->set_h_size_flags(SIZE_EXPAND_FILL);
    bottom_split->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(bottom_split);

    layer_list = memnew(ItemList);
    layer_list->set_stretch_ratio(0.35);
    layer_list->set_h_size_flags(SIZE_EXPAND_FILL);
    layer_list->set_v_size_flags(SIZE_EXPAND_FILL);
    bottom_split->add_child(layer_list);

    VBoxContainer *bottom_container = memnew(VBoxContainer);
    bottom_container->set_h_size_flags(SIZE_EXPAND_FILL);
    bottom_container->set_v_size_flags(SIZE_EXPAND_FILL);
    bottom_split->add_child(bottom_container);

    OptionButton *element_types_options = memnew(OptionButton);
    element_types_options->set_h_size_flags(SIZE_EXPAND_FILL);
    element_types_options->add_item(TTR("Items"), 0);
    element_types_options->add_item(TTR("Patterns"), 0);
    bottom_container->add_child(element_types_options);

    collection_options = memnew(OptionButton);
    collection_options->set_h_size_flags(SIZE_EXPAND_FILL);
    collection_options->set_auto_translate(false);
    bottom_container->add_child(collection_options);
    collection_options->connect("item_selected", callable_mp(this, &TileMap3DEditor::_collection_selected));

    HBoxContainer *cursors_container = memnew(HBoxContainer);
    cursors_container->set_h_size_flags(SIZE_EXPAND_FILL);
    bottom_container->add_child(cursors_container);

    Button *main_cursor_button = memnew(Button);
    main_cursor_button->set_expand_icon(true);
    main_cursor_button->set_custom_minimum_size(Size2(40, 40) * EDSCALE);
    main_cursor_button->set_disabled(true);
    cursors_container->add_child(main_cursor_button);

    Button *sec_cursor_button = memnew(Button);
    sec_cursor_button->set_expand_icon(true);
    sec_cursor_button->set_custom_minimum_size(Size2(40, 40) * EDSCALE);
    sec_cursor_button->set_disabled(true);
    cursors_container->add_child(sec_cursor_button);

    HBoxContainer *tiles_tools_container = memnew(HBoxContainer);
    tiles_tools_container->set_h_size_flags(SIZE_EXPAND_FILL);
    bottom_container->add_child(tiles_tools_container);

    TextEdit *search_edit = memnew(TextEdit);
    search_edit->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_tools_container->add_child(search_edit);

    Ref<ButtonGroup> display_group = memnew(ButtonGroup);

    display_thumbnails_button = memnew(Button);
    display_thumbnails_button->set_toggle_mode(true);
    display_thumbnails_button->set_pressed(true);
    display_thumbnails_button->set_button_group(display_group);
    display_thumbnails_button->set_flat(true);
    tiles_tools_container->add_child(display_thumbnails_button);

    display_list_button = memnew(Button);
    display_list_button->set_toggle_mode(true);
    display_list_button->set_pressed(false);
    display_list_button->set_button_group(display_group);
    display_list_button->set_flat(true);
    tiles_tools_container->add_child(display_list_button);

    HSlider *zoom_slider = memnew(HSlider);
    zoom_slider->set_h_size_flags(SIZE_EXPAND_FILL);
    zoom_slider->set_v_size_flags(SIZE_SHRINK_CENTER);
    zoom_slider->set_value(50.0);
    bottom_container->add_child(zoom_slider);

    tile_list = memnew(ItemList);
    tile_list->set_h_size_flags(SIZE_EXPAND_FILL);
    tile_list->set_v_size_flags(SIZE_EXPAND_FILL);
    bottom_container->add_child(tile_list);
}

TileMap3DEditor::~TileMap3DEditor() {
}