/*************************************************************************/
/*  tile_set_3d_editor.h                                                 */
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

#ifndef TILE_SET_3D_EDITOR_H
#define TILE_SET_3D_EDITOR_H

#include "tile_set_3d_collection_editor.h"

class TileSet3DEditor : public VBoxContainer {
	GDCLASS(TileSet3DEditor, VBoxContainer);

private:
    Ref<TileSet3D> tileset;

    UndoRedo *undo_redo = EditorNode::get_undo_redo();

    Button *export_button;
    Button *add_pattern_button;
    Button *display_thumbnails_button;
    Button *display_list_button;
    Button *collection_delete_button;
    MenuButton *collection_add_menu;
    MenuButton *collection_advanced_menu;

    HSplitContainer *tiles_tab;
    HSplitContainer *patterns_tab;

    ItemList *collections_list;
    Label *no_collection_label;
    TileSet3DCollectionEditor *collection_editor;

    FileDialog *file_dialog;

    int _selected_collection = -1;

    void _tab_changed(int p_tab);
    void _collection_selected(int p_index);
    void _collection_delete();
    void _collection_add_id_pressed(int p_id);
    void _collection_advanced_id_pressed(int p_id);
    void _tile_set_changed();
    void _collection_changed(int p_d);
    void _update_collection(int p_index, int p_id);
    void _update_collections_list();
    void _tile_set_add_collection(int p_collection_id, TileSet3DCollection::CollectionType p_type);
    void _set_selected_collection(int p_selected);
    void _display_mode_button_pressed(Object *p_button);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void edit(const Ref<TileSet3D> &p_set);

    TileSet3DEditor();
    ~TileSet3DEditor();
};

#endif // TILE_SET_3D_EDITOR_H