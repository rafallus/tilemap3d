/*************************************************************************/
/*  tiles_3d_editor_plugin.cpp                                           */
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

#include "tiles_3d_editor_plugin.h"

void Tiles3DEditorPlugin::_node_removed(Node *p_node) {
    if (p_node == tilemap) {
		tilemap = nullptr;
        tilemap_editor->edit(tilemap);
	}
}

void Tiles3DEditorPlugin::edit(Object *p_object) {
    if (p_object) {
        if (p_object->is_class("TileMap3D")) {
            tilemap = Object::cast_to<TileMap3D>(p_object);
            tilemap_editor->edit(tilemap);
            tileset = tilemap->get_tileset();
            tileset_editor->edit(tileset);
        } else if (p_object->is_class("TileSet3D")) {
            tileset = Ref<TileSet3D>(p_object);
            tileset_editor->edit(tileset);
            if (tilemap && (tilemap->get_tileset() != tileset || !tilemap->is_inside_tree())) {
                tilemap = nullptr;
                tilemap_editor->edit(tilemap);
            }
        }
    }
}

bool Tiles3DEditorPlugin::handles(Object *p_object) const {
    return p_object->is_class("TileMap3D") || p_object->is_class("TileSet3D");
}

void Tiles3DEditorPlugin::make_visible(bool p_visible) {
    if (p_visible) {
        tilemap_editor->set_visible(tilemap);
        tileset_editor_button->set_visible(tileset.is_valid());
        if (!tilemap) {
            editor->make_bottom_panel_item_visible(tileset_editor);
        }
    } else {
        tilemap_editor->hide();
        tilemap_editor->set_3d_controls_visibility(false);
        if (tileset_editor->is_visible()) {
            tileset_editor->hide();
            editor->hide_bottom_panel();
        }
        tileset_editor_button->hide();
    }
}

EditorPlugin::AfterGUIInput Tiles3DEditorPlugin::forward_spatial_gui_input(Camera3D *p_camera, const Ref<InputEvent> &p_event) {
    return tilemap_editor->forward_gui_input(p_camera, p_event);
}

void Tiles3DEditorPlugin::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            get_tree()->connect("node_removed", callable_mp(this, &Tiles3DEditorPlugin::_node_removed));
        } break;
        case NOTIFICATION_EXIT_TREE: {
            get_tree()->disconnect("node_removed", callable_mp(this, &Tiles3DEditorPlugin::_node_removed));
        } break;
    }
}

Tiles3DEditorPlugin::Tiles3DEditorPlugin(EditorNode *p_node) {
    editor = p_node;

    EDITOR_DEF("editors/tile_map_3d/editor_side", 1);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/tile_map_3d/editor_side", PROPERTY_HINT_ENUM, "Left,Right"));

    tilemap_editor = memnew(TileMap3DEditor(editor));
	switch ((int)EditorSettings::get_singleton()->get("editors/tile_map_3d/editor_side")) {
		case 0: { // Left.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_LEFT, tilemap_editor);
		} break;
		case 1: { // Right.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT, tilemap_editor);
		} break;
	}
	tilemap_editor->hide();

    tileset_editor = memnew(TileSet3DEditor);
    tileset_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    tileset_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    tileset_editor->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
    tileset_editor->hide();
    tileset_editor_button = editor->add_bottom_panel_item(TTR("TileSet"), tileset_editor);
    tileset_editor_button->hide();
}

Tiles3DEditorPlugin::~Tiles3DEditorPlugin() {
}