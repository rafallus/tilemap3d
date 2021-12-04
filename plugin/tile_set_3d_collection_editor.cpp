/*************************************************************************/
/*  tile_set_3d_collection_editor.cpp                                    */
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

#include "tile_set_3d_collection_editor.h"

/******* TileSet3DCollectionProxyObject *******/
/**********************************************/

void TileSet3DCollectionEditor::TileSet3DCollectionProxyObject::edit(Ref<TileSet3D> p_tile_set, int p_collection_id) {
    ERR_FAIL_NULL(p_tile_set);
    ERR_FAIL_COND(!p_tile_set->has_collection_id(p_collection_id));

    if (tile_set != p_tile_set) {
        tile_set = p_tile_set;
        cid = p_collection_id;
        cindex = tile_set->get_collection_index(cid);
        notify_property_list_changed();
    } else if (cid != p_collection_id) {
        cid = p_collection_id;
        cindex = tile_set->get_collection_index(cid);
        notify_property_list_changed();
    }
}

void TileSet3DCollectionEditor::TileSet3DCollectionProxyObject::update_collection_index(int p_index) {
    cindex = p_index;
}

bool TileSet3DCollectionEditor::TileSet3DCollectionProxyObject::_set(const StringName &p_name, const Variant &p_value) {
    if (p_name == "name") {
        tile_set->set_collection_name(cid, p_value);
        return true;
    } else if (p_name == "id") {
        tile_set->set_collection_id(cindex, p_value, false);
        cid = tile_set->get_collection_id(cindex);
        return true;
    }
    return false;
}

bool TileSet3DCollectionEditor::TileSet3DCollectionProxyObject::_get(const StringName &p_name, Variant &r_ret) const {
    if (p_name == "name") {
        r_ret = tile_set->get_collection_name(cid);
        return true;
    } else if (p_name == "id") {
        r_ret = cid;
        return true;
    }
    return false;
}

void TileSet3DCollectionEditor::TileSet3DCollectionProxyObject::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::STRING, "name", PROPERTY_HINT_NONE, ""));
    p_list->push_back(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_RANGE, "0,128,1,or_greater"));
}

/******* TileSet3DCollectionEditor *******/
/*****************************************/

void TileSet3DCollectionEditor::_add_source_requested(const Callable &p_process_method, const String &p_filters_str) {
    if (p_filters_str.is_empty()) {
		file_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_DIR);
    } else {
		file_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_FILES);
		Vector<String> f = p_filters_str.split(",");
		file_dialog->set_filters(f);
        if (!file_dialog->is_connected("files_selected", callable_mp(this, &TileSet3DCollectionEditor::_add_source_files_selected))) {
            file_dialog->connect("files_selected", callable_mp(this, &TileSet3DCollectionEditor::_add_source_files_selected), varray(p_process_method));
        }
    }
	file_dialog->popup_centered(Size2i(520,380) * EDSCALE);
}

void TileSet3DCollectionEditor::_add_source_files_selected(const PackedStringArray p_paths, const Callable &p_method) {
    if (file_dialog->is_connected("files_selected", callable_mp(this, &TileSet3DCollectionEditor::_add_source_files_selected))) {
        file_dialog->disconnect("files_selected", callable_mp(this, &TileSet3DCollectionEditor::_add_source_files_selected));
    }
    Ref<TileSet3DCollection> previous = tile_set->get_collection(cid)->duplicate();
    undo_redo->create_action(TTR("Add tiles to collection from files"));
    undo_redo->add_do_method(this, "_add_source_files_from_paths", p_paths, p_method);
    undo_redo->add_undo_method(*tile_set, "remove_collection", cid);
    int pos = tile_set->get_collection_index(cid);
    undo_redo->add_undo_method(*tile_set, "add_collection", previous, cid, pos);
    undo_redo->add_undo_method(this, "_update_tiles");
    undo_redo->commit_action();
}

void TileSet3DCollectionEditor::_add_source_files_from_paths(const PackedStringArray p_paths, const Callable &p_method) {
    Variant args[1];
	Variant *argsptr[1];
    argsptr[0] = &args[0];
    for (int i = 0; i < p_paths.size(); i++) {
        String path = p_paths[i];
        args[0] = path;
        Variant ret;
        Callable::CallError ce;
        p_method.call((const Variant **)argsptr, 1, ret, ce);
    }
}

void TileSet3DCollectionEditor::_add_scene_atlas_source(const String &p_path) {
    ERR_FAIL_COND(tile_set->get_collection_type(cid) != TileSet3DCollection::COLLECTION_TYPE_ATLAS);
    Ref<PackedScene> scene = ResourceLoader::load(p_path);
    ERR_FAIL_NULL(scene);
    Node3D *root = Object::cast_to<Node3D>(scene->instantiate(PackedScene::GEN_EDIT_STATE_INSTANCE));
    if (root) {
        Vector<Ref<Mesh>> meshes;
        Vector<Transform3D> transforms;
        Vector<Ref<TileData3DMesh>> data_arr;
        int max_depth = EditorSettings::get_singleton()->get_setting("editors/tile_map_3d/tile_sets/source_node_max_depth");
        for (int i = 0; i < root->get_child_count(); i++) {
            Ref<TileData3DMesh> data = memnew(TileData3DMesh);
            Node *n = root->get_child(i);
            bool used = _handle_tile_source_node(n, data, 0, max_depth);

            if (used) {
                data->set_name(n->get_name());
                data->set_source(vformat("%s:%s", p_path, n->get_name()));
                tile_set->add_collection_tile(cid, data);
                Pair<Ref<Mesh>, Transform3D> pair = _get_tile_data_mesh(data);
                if (pair.first.is_valid()) {
                    meshes.push_back(pair.first);
                    transforms.push_back(pair.second);
                    data_arr.push_back(data);
                }
            }
        }

        if (meshes.size()) {
            Vector<Ref<Texture2D>> textures = EditorInterface::get_singleton()->make_mesh_previews(meshes, &transforms, EditorSettings::get_singleton()->get("editors/tile_map_3d/tile_sets/preview_size"));

            for (int i = 0; i < meshes.size(); i++) {
                Ref<Texture2D> texture = textures[i];
                Ref<TileData3DMesh> data = data_arr[i];
                data->set_preview(texture);
            }
        }
    }
}

bool TileSet3DCollectionEditor::_handle_tile_source_node(const Node *p_node, Ref<TileData3DMesh> &r_data, int p_depth, int p_max_depth) {
    if (!p_node->is_class("Node3D")) {
        return false;
    }

	bool used = false;
    if (p_node->is_class("MeshInstance3D")) {
        const MeshInstance3D *mi = Object::cast_to<MeshInstance3D>(p_node);
        used |= _handle_tile_source_meshinstance(mi, r_data, p_depth != 0);
    } else if (p_node->is_class("StaticBody3D")) {
        const StaticBody3D *sb = Object::cast_to<StaticBody3D>(p_node);
        used |= _handle_tile_source_staticbody(sb, r_data, p_depth != 0);
    }

	if (p_depth < p_max_depth) {
        for (int i = 0; i < p_node->get_child_count(); i++) {
            Node *child = p_node->get_child(i);
            used |= _handle_tile_source_node(child, r_data, p_depth + 1, p_max_depth);
        }
    }

    return used;
}

bool TileSet3DCollectionEditor::_handle_tile_source_meshinstance(const MeshInstance3D *p_mi, Ref<TileData3DMesh> &r_data, int p_depth) {
    Ref<Mesh> mesh = p_mi->get_mesh();
    if (!p_mi) {
        return false;
    }

    r_data->set_mesh(mesh);
    r_data->set_material_override(p_mi->get_material_override());
	r_data->set_transparency(p_mi->get_transparency());
	r_data->set_cast_shadow_mode(p_mi->get_cast_shadows_setting());
	r_data->set_lod_bias(p_mi->get_lod_bias());
	// r_data->set_ignore_occlusion_culling(p_mi->is_ignoring_occlusion_culling());
	r_data->set_gi_mode(p_mi->get_gi_mode());
	r_data->set_gi_lightmap_scale(p_mi->get_lightmap_scale());
	r_data->set_visibility_range_begin(p_mi->get_visibility_range_begin());
	r_data->set_visibility_range_begin_margin(p_mi->get_visibility_range_begin_margin());
	r_data->set_visibility_range_end(p_mi->get_visibility_range_end());
	r_data->set_visibility_range_end_margin(p_mi->get_visibility_range_end_margin());
	r_data->set_visibility_range_fade_mode(p_mi->get_visibility_range_fade_mode());
	if (p_depth > 0) {
        r_data->set_mesh_transform(_get_descendant_transform(p_mi, p_depth));
    }
    return true;
}

bool TileSet3DCollectionEditor::_handle_tile_source_staticbody(const StaticBody3D *p_sb, Ref<TileData3DMesh> &r_data, int p_depth) {
    return false;
}

void TileSet3DCollectionEditor::_tool_add_source_id_pressed(int p_id) {
    switch (p_id) {
    case 0: // Scene
        _add_source_requested(callable_mp(this, &TileSet3DCollectionEditor::_add_scene_atlas_source), ATLAS_SCENE_EXTENSIONS);
        break;
    case 1: // Single tile scene
		break;
	case 2: // Mesh
		break;
	case 3: // Empty tile
		break;
	case 5: // Scenes folder
		break;
	case 6: // Singel tiles folder
		break;
    case 7: // Meshes folder
		break;
    default:
        break;
    }
}

Pair<Ref<Mesh>, Transform3D> TileSet3DCollectionEditor::_get_tile_data_mesh(const Ref<TileData3DMesh> &p_data) const {
    Pair<Ref<Mesh>, Transform3D> mesh_pair;
    mesh_pair.first = p_data->get_mesh();
    mesh_pair.second = p_data->get_mesh_transform();
    return mesh_pair;
}

void TileSet3DCollectionEditor::_update_tiles() {
    int ntiles = tile_set->get_collection_tile_count(cid);
    int nitems = tiles_view->get_child_count(false);
    int preview_size = EDITOR_GET("editors/tile_map_3d/tile_sets/preview_size");
    bool needs_fit = tiles_view->get_child_count() == 0;
    for (int i = 0; i < ntiles; i++) {
        Tile3DItemView *item;
        if (i < nitems) {
            item  = Object::cast_to<Tile3DItemView>(tiles_view->get_child(i, false));
        } else {
            item = memnew(Tile3DItemView);
            item->set_preview_size(preview_size);
            item->set_display_mode(display_mode);
            item->set_button_group(tiles_group);
            tiles_view->add_child(item);
        }

        ERR_FAIL_NULL(item);

        int id = tile_set->get_collection_tile_id(cid, i);
        item->set_data(tile_set->get_collection_tile(cid, id));
        item->set_id(id);
    }

    if (needs_fit) {
        _fit_tiles_view();
    }

    for (int i = ntiles; i < nitems; i++) {
        Control *child = Object::cast_to<Control>(tiles_view->get_child(i, false));
        child->hide();
    }

    if (ntiles == 0) {
        no_tiles_label->show();
        tiles_view->hide();
    } else {
        no_tiles_label->hide();
        tiles_view->show();
    }
}

void TileSet3DCollectionEditor::_collection_changed(int p_idx) {
    if (p_idx == cid) {
        _update_tiles();
    }
}

void TileSet3DCollectionEditor::_fit_tiles_view() {
    if (display_mode == Tile3DItemView::DISPLAY_MODE_LIST) {
        tiles_view->set_columns(1);
    } else if (tiles_view->get_child_count() != 0) {
        Size2 size = tiles_scroll->get_size();
        Control *item = (Control *)tiles_view->get_child(0);
        Size2 item_size = item->get_combined_minimum_size();
        int hsep = tiles_view->get_theme_constant(SNAME("hseparation"));
        int ncolums = Math::floor((size.width + hsep) / (item_size.width + hsep));
        tiles_view->set_columns(ncolums);
    }
}

void TileSet3DCollectionEditor::_tile_selected(Object *p_button) {
    // Tile3DItemView *item = (Tile3DItemView *)p_button;
    // TODO: Show tile info
}

Transform3D TileSet3DCollectionEditor::_get_descendant_transform(const Node3D *p_node, int p_depth) {
    if (p_depth == 1) {
        return p_node->get_transform();
    } else {
        Node3D *ancestor = p_node->get_parent_node_3d();
        for (int i = 0; i < p_depth - 1; i++) {
            ancestor = ancestor->get_parent_node_3d();
        }
        return ancestor->get_global_transform().affine_inverse() * p_node->get_global_transform();
    }
}

void TileSet3DCollectionEditor::edit(int p_collection_id) {
    ERR_FAIL_COND(tile_set.is_null());
    cid = p_collection_id;
    collection_proxy->edit(tile_set, cid);

    if (inspector->get_edited_object() != collection_proxy) {
        inspector->edit(collection_proxy);
    }

    _update_tiles();
}

void TileSet3DCollectionEditor::set_tile_set(const Ref<TileSet3D> &p_set) {
    if (tile_set.is_valid()) {
        tile_set->disconnect(SNAME("collection_changed"), callable_mp(this, &TileSet3DCollectionEditor::_collection_changed));
    }

    tile_set = p_set;

    if (tile_set.is_valid()) {
        tile_set->connect(SNAME("collection_changed"), callable_mp(this, &TileSet3DCollectionEditor::_collection_changed));
    }
}

void TileSet3DCollectionEditor::set_file_dialog(FileDialog *p_file_dialog) {
    file_dialog = p_file_dialog;
}

void TileSet3DCollectionEditor::set_display_mode(Tile3DItemView::DisplayMode p_mode) {
    display_mode = p_mode;
    for (int i = 0; i < tiles_view->get_child_count(); i++) {
        Tile3DItemView *item = Object::cast_to<Tile3DItemView>(tiles_view->get_child(i, false));
        item->set_display_mode(display_mode);
    }

    _fit_tiles_view();
}

void TileSet3DCollectionEditor::_notification(int p_what) {
    switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED:
            tool_setup_button->set_icon(get_theme_icon("Tools", "EditorIcons"));
            tool_select_button->set_icon(get_theme_icon("ToolSelect", "EditorIcons"));
            tool_paint_button->set_icon(get_theme_icon("CanvasItem", "EditorIcons"));
            tool_add_source_menu->set_icon(get_theme_icon("AssetLib", "EditorIcons"));
            tool_delete_button->set_icon(get_theme_icon("Remove", "EditorIcons"));
            tool_duplicate_button->set_icon(get_theme_icon("ActionCopy", "EditorIcons"));
            break;
        default:
            break;
    }
}

void TileSet3DCollectionEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_add_source_files_from_paths", "paths", "method"), &TileSet3DCollectionEditor::_add_source_files_from_paths);
    ClassDB::bind_method(D_METHOD("_update_tiles"), &TileSet3DCollectionEditor::_update_tiles);
}

TileSet3DCollectionEditor::TileSet3DCollectionEditor() {
    EDITOR_DEF("editors/tile_map_3d/tile_sets/source_node_max_depth", 1);
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/tile_map_3d/tile_sets/source_node_max_depth", PROPERTY_HINT_RANGE, "1,4"));

    EDITOR_DEF("editors/tile_map_3d/tile_sets/preview_size", 64);
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/tile_map_3d/tile_sets/preview_size", PROPERTY_HINT_RANGE, "8,256"));

    ScrollContainer *middle_panel = memnew(ScrollContainer);
    middle_panel->set_enable_h_scroll(false);
    add_child(middle_panel);

    VBoxContainer *middle_container = memnew(VBoxContainer);
    middle_container->set_h_size_flags(SIZE_EXPAND_FILL);
    middle_container->set_v_size_flags(SIZE_EXPAND_FILL);
    middle_panel->add_child(middle_container);

    Label *inspector_label = memnew(Label);
    inspector_label->set_text(TTR("Atlas Properties:"));
    middle_container->add_child(inspector_label);

    inspector = memnew(EditorInspector);
    inspector->set_undo_redo(undo_redo);
    inspector->set_enable_v_scroll(false);
    middle_container->add_child(inspector);

    VBoxContainer *right_panel = memnew(VBoxContainer);
    right_panel->set_h_size_flags(SIZE_EXPAND_FILL);
    right_panel->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(right_panel);

    HBoxContainer *tool_box = memnew(HBoxContainer);
    right_panel->add_child(tool_box);

    Ref<ButtonGroup> tools_group = memnew(ButtonGroup);

    tool_setup_button = memnew(Button);
    tool_setup_button->set_toggle_mode(true);
    tool_setup_button->set_flat(true);
    tool_setup_button->set_tooltip(TTR("Collection setup."));
    tool_setup_button->set_pressed(true);
    tool_setup_button->set_button_group(tools_group);
    tool_box->add_child(tool_setup_button);

    tool_select_button = memnew(Button);
    tool_select_button->set_toggle_mode(true);
    tool_select_button->set_flat(true);
    tool_select_button->set_tooltip(TTR("Selection mode."));
    tool_select_button->set_button_group(tools_group);
    tool_box->add_child(tool_select_button);

    tool_paint_button = memnew(Button);
    tool_paint_button->set_toggle_mode(true);
    tool_paint_button->set_flat(true);
    tool_paint_button->set_tooltip(TTR("Paint mode."));
    tool_paint_button->set_button_group(tools_group);
    tool_box->add_child(tool_paint_button);

    VSeparator *separator = memnew(VSeparator);
    tool_box->add_child(separator);

    tool_add_source_menu = memnew(MenuButton);
    tool_add_source_menu->set_tooltip(TTR("Add source to collection."));
    tool_add_source_menu->get_popup()->add_item(TTR("Scene..."), 0);
    tool_add_source_menu->get_popup()->add_item(TTR("Single tile from scene..."), 1);
    tool_add_source_menu->get_popup()->add_item(TTR("Mesh..."), 2);
    tool_add_source_menu->get_popup()->add_item(TTR("Empty tile"), 3);
    tool_add_source_menu->get_popup()->add_separator();
    tool_add_source_menu->get_popup()->add_item(TTR("Scenes from folder..."), 4);
    tool_add_source_menu->get_popup()->add_item(TTR("Single tiles from folder..."), 5);
    tool_add_source_menu->get_popup()->add_item(TTR("Meshes from folder..."), 6);
    tool_add_source_menu->get_popup()->connect("id_pressed", callable_mp(this, &TileSet3DCollectionEditor::_tool_add_source_id_pressed));
    tool_box->add_child(tool_add_source_menu);

    tool_delete_button = memnew(Button);
    tool_delete_button->set_flat(true);
    tool_delete_button->set_tooltip(TTR("Delete selected tiles."));
    tool_delete_button->set_disabled(true);
    tool_box->add_child(tool_delete_button);

    tool_duplicate_button = memnew(Button);
    tool_duplicate_button->set_flat(true);
    tool_duplicate_button->set_tooltip(TTR("Duplicate selected tiles."));
    tool_duplicate_button->set_disabled(true);
    tool_box->add_child(tool_duplicate_button);

    no_tiles_label = memnew(Label);
    no_tiles_label->set_h_size_flags(SIZE_EXPAND | SIZE_SHRINK_CENTER);
    no_tiles_label->set_v_size_flags(SIZE_EXPAND | SIZE_SHRINK_CENTER);
    no_tiles_label->set_text(TTR("This collection contains no tiles."));
    right_panel->add_child(no_tiles_label);

    tiles_scroll = memnew(ScrollContainer);
    tiles_scroll->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_scroll->set_v_size_flags(SIZE_EXPAND_FILL);
    tiles_scroll->connect("resized", callable_mp(this, &TileSet3DCollectionEditor::_fit_tiles_view));
    right_panel->add_child(tiles_scroll);

    tiles_view = memnew(GridContainer);
    tiles_view->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_view->set_v_size_flags(SIZE_EXPAND_FILL);
    tiles_view->set_visible(false);
    tiles_scroll->add_child(tiles_view);

    tiles_group.instantiate();
    tiles_group->connect("pressed", callable_mp(this, &TileSet3DCollectionEditor::_tile_selected));

    collection_proxy = memnew(TileSet3DCollectionProxyObject);
}

TileSet3DCollectionEditor::~TileSet3DCollectionEditor() {
    memdelete(collection_proxy);
}