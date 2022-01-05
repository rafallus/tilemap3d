/*************************************************************************/
/*  tile_set_3d_editor.cpp                                               */
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

#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/tab_bar.h"
#include "scene/gui/separator.h"
#include "scene/gui/slider.h"
#include "tile_set_3d_editor.h"

void TileSet3DEditor::_tab_changed(int p_tab) {
    if (p_tab == 0) {
        tiles_tab->show();
        export_button->show();
        patterns_tab->hide();
        add_pattern_button->hide();
    } else if (p_tab == 1) {
        tiles_tab->hide();
        export_button->hide();
        patterns_tab->show();
        add_pattern_button->show();
    }
}

void TileSet3DEditor::_collection_selected(int p_index) {
    _selected_collection = p_index;
    collection_delete_button->set_disabled(false);
    no_collection_label->hide();
    collection_editor->show();
    int cid = collections_list->get_item_metadata(p_index);
    export_button->set_disabled(tileset->get_collection_tile_count(cid) == 0);
    collection_editor->edit(cid);
}

void TileSet3DEditor::_collection_delete() {
    ERR_FAIL_COND(!tileset.is_valid());

    int current_idx = collections_list->get_current();
    int cid = collections_list->get_item_metadata(current_idx);
    Ref<TileSet3DCollection> to_delete = tileset->get_collection(cid);
    undo_redo->create_action(TTR("Delete tile collection"));
    undo_redo->add_do_method(*tileset, "remove_collection", cid);
    undo_redo->add_undo_method(this, "_set_selected_collection", current_idx);
    undo_redo->add_undo_method(*tileset, "add_collection", to_delete, cid, current_idx);
    undo_redo->commit_action();
    _selected_collection = current_idx - 1;
}

void TileSet3DEditor::_collection_add_id_pressed(int p_id) {
    TileSet3DCollection::CollectionType ctype = p_id == 0 ? TileSet3DCollection::COLLECTION_TYPE_ATLAS : TileSet3DCollection::COLLECTION_TYPE_SCENES;
    int cid = tileset->get_next_collection_id();
    undo_redo->create_action(TTR("Add tile collection"));
    undo_redo->add_do_method(this, "_tile_set_add_collection", cid, ctype);
    undo_redo->add_undo_method(this, "_set_selected_collection", _selected_collection);
    undo_redo->add_undo_method(*tileset, "remove_collection", cid);
    undo_redo->commit_action();
}

void TileSet3DEditor::_collection_advanced_id_pressed(int p_id) {

}

void TileSet3DEditor::_tile_set_changed() {
    _update_collections_list();
}

void TileSet3DEditor::_collection_changed(int p_id) {
    int index = -1;
    for (int i = 0; i < collections_list->get_item_count(); i++) {
        if (p_id == int(collections_list->get_item_metadata(i))) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        _update_collection(index, p_id);
    }
}

void TileSet3DEditor::_update_collection(int p_index, int p_id) {
    String collection_name = tileset->get_collection_name(p_id);
    if (collection_name.is_empty()) {
        String nm = tileset->get_collection_type(p_id) == TileSet3DCollection::COLLECTION_TYPE_ATLAS ? TTR("Atlas Collection") : TTR("Scene Collection");
        collection_name = vformat("[%s]", nm);
    }
    String text = vformat("%s (ID: %d)", collection_name, p_id);
    collections_list->set_item_text(p_index, text);
    Ref<Texture2D> icon = tileset->get_collection_icon(p_id);
    collections_list->set_item_icon(p_index, icon);
}

void TileSet3DEditor::_update_collections_list() {
    collections_list->clear();
    int ncollections = tileset.is_null() ? 0 : tileset->get_collection_count();

	if (ncollections > 0) {
		for (int i = 0; i < ncollections; i++) {
			int cid = tileset->get_collection_id(i);
			int idx = collections_list->add_item("");
			collections_list->set_item_metadata(idx, cid);
            _update_collection(idx, cid);
        }


        if (_selected_collection < 0) {
			_selected_collection = 0;
        } else if (_selected_collection >= collections_list->get_item_count()) {
            _selected_collection = collections_list->get_item_count() - 1;
        }
		collections_list->select(_selected_collection);
		_collection_selected(_selected_collection);
    } else {
		collection_delete_button->set_disabled(true);
		no_collection_label->show();
		collection_editor->hide();
		_selected_collection = -1;
    }
}

void TileSet3DEditor::_tile_set_add_collection(int p_collection_id, TileSet3DCollection::CollectionType p_type) {
    _selected_collection = tileset->get_collection_count();
    tileset->create_collection(p_type, p_collection_id);
	String cname = vformat("Collection #%d", p_collection_id);
	String icon_name = p_type == TileSet3DCollection::COLLECTION_TYPE_ATLAS ? "TileSet" : "PackedScene";
	Ref<Texture2D> icon = get_theme_icon(icon_name, "EditorIcons");
	tileset->set_collection_icon(p_collection_id, icon);
	tileset->set_collection_name(p_collection_id, cname);
}

void TileSet3DEditor::_set_selected_collection(int p_selected) {
    _selected_collection = p_selected;
}

void TileSet3DEditor::_display_mode_button_pressed(Object *p_button) {
    if (p_button == display_thumbnails_button) {
        collection_editor->set_display_mode(Tile3DItemView::DISPLAY_MODE_THUMBNAIL);
    } else if (p_button == display_list_button) {
        collection_editor->set_display_mode(Tile3DItemView::DISPLAY_MODE_LIST);
    }
}

void TileSet3DEditor::edit(const Ref<TileSet3D> &p_set) {
    if (tileset == p_set) {
        return;
    }

    if (tileset.is_valid()) {
        tileset->disconnect("changed", callable_mp(this, &TileSet3DEditor::_tile_set_changed));
        tileset->disconnect("collection_changed", callable_mp(this, &TileSet3DEditor::_collection_changed));
    }

    tileset = p_set;
    collection_editor->set_tile_set(tileset);

    if (tileset.is_valid()) {
        tileset->connect("changed", callable_mp(this, &TileSet3DEditor::_tile_set_changed));
        tileset->connect("collection_changed", callable_mp(this, &TileSet3DEditor::_collection_changed));
    }

    _update_collections_list();
}

void TileSet3DEditor::_notification(int p_what) {
    switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED:
            export_button->set_icon(get_theme_icon("MoveUp", "EditorIcons"));
            add_pattern_button->set_icon(get_theme_icon("Override", "EditorIcons"));
            display_thumbnails_button->set_icon(get_theme_icon("FileThumbnail", "EditorIcons"));
            display_list_button->set_icon(get_theme_icon("FileList", "EditorIcons"));
            collection_delete_button->set_icon(get_theme_icon("Remove", "EditorIcons"));
            collection_add_menu->set_icon(get_theme_icon("Add", "EditorIcons"));
            collection_advanced_menu->set_icon(get_theme_icon("GuiTabMenuHl", "EditorIcons"));
            break;
        default:
            break;
    }
}

void TileSet3DEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_tile_set_add_collection", "id", "type"), &TileSet3DEditor::_tile_set_add_collection);
    ClassDB::bind_method(D_METHOD("_set_selected_collection", "selected"), &TileSet3DEditor::_set_selected_collection);
}

TileSet3DEditor::TileSet3DEditor() {
    HBoxContainer *toolbar = memnew(HBoxContainer);
    toolbar->set_h_size_flags(SIZE_EXPAND_FILL);
    add_child(toolbar);

    TabBar *tabs_bar = memnew(TabBar);
    tabs_bar->set_h_size_flags(SIZE_EXPAND_FILL);
    tabs_bar->set_tab_align(TabBar::ALIGN_LEFT);
    tabs_bar->add_tab(TTR("Tiles"));
    tabs_bar->add_tab(TTR("Patterns"));
    toolbar->add_child(tabs_bar);
    tabs_bar->connect("tab_changed", callable_mp(this, &TileSet3DEditor::_tab_changed));

    Control *separator1 = memnew(Control);
    separator1->set_h_size_flags(SIZE_EXPAND_FILL);
    separator1->set_stretch_ratio(0.4);
    toolbar->add_child(separator1);

    export_button = memnew(Button);
    export_button->set_visible(false);
    export_button->set_disabled(true);
    export_button->set_flat(true);
    export_button->set_tooltip(TTR("Export tiles in Atlas to PackedScene file."));
    toolbar->add_child(export_button);

    add_pattern_button = memnew(Button);
    add_pattern_button->set_visible(false);
    add_pattern_button->set_disabled(true);
    add_pattern_button->set_flat(true);
    add_pattern_button->set_tooltip(TTR("Add current selection in 3D viewport as pattern."));
    toolbar->add_child(add_pattern_button);

    VSeparator *separator2 = memnew(VSeparator);
    toolbar->add_child(separator2);

    Ref<ButtonGroup> display_mode_group = memnew(ButtonGroup);
    display_mode_group->connect("pressed", callable_mp(this, &TileSet3DEditor::_display_mode_button_pressed));

    display_thumbnails_button = memnew(Button);
    display_thumbnails_button->set_toggle_mode(true);
    display_thumbnails_button->set_pressed(true);
    display_thumbnails_button->set_flat(true);
    display_thumbnails_button->set_button_group(display_mode_group);
    display_thumbnails_button->set_tooltip(TTR("view items as thumbnails."));
    toolbar->add_child(display_thumbnails_button);

    display_list_button = memnew(Button);
    display_list_button->set_toggle_mode(true);
    display_list_button->set_flat(true);
    display_list_button->set_button_group(display_mode_group);
    display_list_button->set_tooltip(TTR("view items as list."));
    toolbar->add_child(display_list_button);

    HSlider *zoom_slider = memnew(HSlider);
    zoom_slider->set_h_size_flags(SIZE_EXPAND_FILL);
    zoom_slider->set_v_size_flags(SIZE_SHRINK_CENTER);
    zoom_slider->set_stretch_ratio(0.5);
    zoom_slider->set_value(50.0);
    toolbar->add_child(zoom_slider);

    LineEdit *search = memnew(LineEdit);
    search->set_h_size_flags(SIZE_EXPAND_FILL);
    search->set_stretch_ratio(0.5);
    search->set_editable(false);
    toolbar->add_child(search);

    tiles_tab = memnew(HSplitContainer);
    tiles_tab->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_tab->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(tiles_tab);

    VBoxContainer *tiles_left_container = memnew(VBoxContainer);
    tiles_left_container->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_left_container->set_v_size_flags(SIZE_EXPAND_FILL);
    tiles_left_container->set_stretch_ratio(0.25);
    tiles_tab->add_child(tiles_left_container);

    int thumbnail_size = 60;
    collections_list = memnew(ItemList);
    collections_list->set_h_size_flags(SIZE_EXPAND_FILL);
    collections_list->set_v_size_flags(SIZE_EXPAND_FILL);
    collections_list->set_fixed_icon_size(Size2(thumbnail_size, thumbnail_size) * EDSCALE);
    tiles_left_container->add_child(collections_list);
    collections_list->connect("item_selected", callable_mp(this, &TileSet3DEditor::_collection_selected));

    HBoxContainer *collection_actions = memnew(HBoxContainer);
    collection_actions->set_alignment(BoxContainer::ALIGN_END);
    tiles_left_container->add_child(collection_actions);

    collection_delete_button = memnew(Button);
    collection_delete_button->set_disabled(true);
    collection_delete_button->set_flat(true);
    collection_actions->add_child(collection_delete_button);
    collection_delete_button->connect("pressed", callable_mp(this, &TileSet3DEditor::_collection_delete));

    collection_add_menu = memnew(MenuButton);
    collection_add_menu->get_popup()->add_item(TTR("Atlas"), 0);
    collection_add_menu->get_popup()->add_item(TTR("Scene Collection"), 1);
    collection_actions->add_child(collection_add_menu);
    collection_add_menu->get_popup()->connect("id_pressed", callable_mp(this, &TileSet3DEditor::_collection_add_id_pressed));

    collection_advanced_menu = memnew(MenuButton);
    collection_advanced_menu->get_popup()->add_item(TTR("Duplicate Collection"), 0);
    collection_advanced_menu->get_popup()->add_item(TTR("Merge..."), 1);
    collection_advanced_menu->get_popup()->add_item(TTR("Split..."), 2);
    collection_actions->add_child(collection_advanced_menu);
    collection_advanced_menu->get_popup()->connect("id_pressed", callable_mp(this, &TileSet3DEditor::_collection_advanced_id_pressed));

    VBoxContainer *tiles_right_container = memnew(VBoxContainer);
    tiles_right_container->set_h_size_flags(SIZE_EXPAND_FILL);
    tiles_right_container->set_v_size_flags(SIZE_EXPAND_FILL);
    tiles_tab->add_child(tiles_right_container);

    no_collection_label = memnew(Label);
    no_collection_label->set_h_size_flags(SIZE_SHRINK_CENTER);
    no_collection_label->set_v_size_flags(SIZE_SHRINK_CENTER | SIZE_EXPAND);
    no_collection_label->set_text(TTR("No TileSet collection selected. Select or create a TileSet collection."));
    tiles_right_container->add_child(no_collection_label);

    collection_editor = memnew(TileSet3DCollectionEditor);
    collection_editor->set_h_size_flags(SIZE_EXPAND_FILL);
    collection_editor->set_v_size_flags(SIZE_EXPAND_FILL);
    collection_editor->set_visible(false);
    collection_editor->set_display_mode(Tile3DItemView::DISPLAY_MODE_THUMBNAIL);
    tiles_right_container->add_child(collection_editor);

    patterns_tab = memnew(HSplitContainer);
    patterns_tab->set_visible(false);
    patterns_tab->set_h_size_flags(SIZE_EXPAND_FILL);
    patterns_tab->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(patterns_tab);

    file_dialog = memnew(FileDialog);
    add_child(file_dialog);
    collection_editor->set_file_dialog(file_dialog);
}

TileSet3DEditor::~TileSet3DEditor() {

}
