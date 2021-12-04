/*************************************************************************/
/*  tile_set_3d_collection_editor.h                                      */
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

#ifndef TILE_SET_3D_COLLECTION_EDITOR_H
#define TILE_SET_3D_COLLECTION_EDITOR_H

#include "editor/editor_resource_preview.h"
#include "scene/3d/physics_body_3d.h"
#include "tile_3d_item_view.h"

class TileSet3DCollectionEditor : public HSplitContainer {
	GDCLASS(TileSet3DCollectionEditor, HSplitContainer);

private:
    const String ATLAS_SCENE_EXTENSIONS = "*.tscn ; TSCN,*.scn ; SCN,*.res ; RES,*.gltf ; GlTF,*.glb ; Bin GlTF,*.fbx ; FBX,*.dae ; DAE";

    // -- Proxy object for a collection, needed by the inspector --
    class TileSet3DCollectionProxyObject : public Object {
        GDCLASS(TileSet3DCollectionProxyObject, Object);

    private:
        Ref<TileSet3D> tile_set;
        int cid;
        int cindex;

    protected:
		bool _set(const StringName &p_name, const Variant &p_value);
		bool _get(const StringName &p_name, Variant &r_ret) const;
		void _get_property_list(List<PropertyInfo> *p_list) const;
    public:
        void edit(Ref<TileSet3D> p_tile_set, int p_collection_id);
        void update_collection_index(int p_index);
    };

    Ref<TileSet3D> tile_set;

    UndoRedo *undo_redo = EditorNode::get_undo_redo();

    FileDialog *file_dialog;

    Button *tool_setup_button;
    Button *tool_select_button;
    Button *tool_paint_button;
    MenuButton *tool_add_source_menu;
    Button *tool_delete_button;
    Button *tool_duplicate_button;
    Label *no_tiles_label;
    ScrollContainer *tiles_scroll;
    GridContainer *tiles_view;
    Ref<ButtonGroup> tiles_group;

    EditorInspector *inspector;

    int cid;
    TileSet3DCollectionProxyObject *collection_proxy;

    Tile3DItemView::DisplayMode display_mode;

    void _add_source_requested(const Callable &p_process_method, const String &p_filters_str);
    void _add_source_files_selected(const PackedStringArray p_paths, const Callable &p_method);
    void _add_source_files_from_paths(const PackedStringArray p_paths, const Callable &p_method);
    void _add_scene_atlas_source(const String &p_path);
    bool _handle_tile_source_node(const Node *p_node, Ref<TileData3DMesh> &r_data, int p_depth, int p_max_depth);
    bool _handle_tile_source_meshinstance(const MeshInstance3D *p_mi, Ref<TileData3DMesh> &r_data, int depth);
    bool _handle_tile_source_staticbody(const StaticBody3D *p_sb, Ref<TileData3DMesh> &r_data, int depth);
    void _tool_add_source_id_pressed(int p_id);
    Pair<Ref<Mesh>, Transform3D> _get_tile_data_mesh(const Ref<TileData3DMesh> &p_data) const;
    void _fit_tiles_view();
    void _update_tiles();
    void _collection_changed(int p_idx);
    void _tile_selected(Object *p_button);
    static Transform3D _get_descendant_transform(const Node3D *p_node, int p_depth);


protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void edit(int p_collection_id);
    void set_tile_set(const Ref<TileSet3D> &p_set);
    void set_file_dialog(FileDialog *p_file_dialog);
    void set_display_mode(Tile3DItemView::DisplayMode p_mode);

    TileSet3DCollectionEditor();
    ~TileSet3DCollectionEditor();
};

#endif // TILE_SET_3D_COLLECTION_EDITOR_H