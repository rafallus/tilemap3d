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

void Tiles3DEditorPlugin::edit(Object *p_object) {
    tile_set = Ref<TileSet3D>(p_object);
    tileset_editor->edit(tile_set);
}

bool Tiles3DEditorPlugin::handles(Object *p_object) const {
    return p_object->is_class("TileSet3D");
}

void Tiles3DEditorPlugin::make_visible(bool p_visible) {
    if (p_visible) {
        tileset_editor_button->show();
        editor_node->make_bottom_panel_item_visible(tileset_editor);
    } else {
        tileset_editor_button->hide();
        editor_node->hide_bottom_panel();
    }
}

Tiles3DEditorPlugin::Tiles3DEditorPlugin(EditorNode *p_node) {
    editor_node = p_node;

    tileset_editor = memnew(TileSet3DEditor);
    tileset_editor->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    tileset_editor->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    tileset_editor->set_custom_minimum_size(Size2(0, 200) * EDSCALE);
    tileset_editor->hide();
    tileset_editor_button = editor_node->add_bottom_panel_item(TTR("TileSet"), tileset_editor);
    tileset_editor_button->hide();
}

Tiles3DEditorPlugin::~Tiles3DEditorPlugin() {
}