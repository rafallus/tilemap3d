/*************************************************************************/
/*  tile_3d_item_view.h                                                  */
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

#ifndef TILE_3D_ITEM_VIEW_H
#define TILE_3D_ITEM_VIEW_H

#include "editor/editor_scale.h"
#include "editor/editor_node.h"
#include "tile_set_3d.h"

class Tile3DItemView : public BaseButton {
    GDCLASS(Tile3DItemView, BaseButton);

public:
    enum DisplayMode {
        DISPLAY_MODE_LIST,
		DISPLAY_MODE_THUMBNAIL
	};

private:
    const float LIST_PREVIEW_FRACTION = 0.6;

    int id;
    Label *label;
    CheckBox *checkbox;
    Ref<TileData3D> data;

    int preview_size;
    DisplayMode display_mode = DISPLAY_MODE_LIST;
    bool pending_sort = false;
    Ref<StyleBox> style;

    void _queue_sort();
    void _sort_children();
    void _draw_item();
    void _data_changed();
    void _label_minsize_changed();

protected:
	void _notification(int p_what);

public:
    void set_data(const Ref<TileData3D> &p_data);
    void set_preview_size(int p_size);
    void set_id(int p_id);
    int get_id() const;
    void set_display_mode(DisplayMode p_mode);
    virtual Size2 get_minimum_size() const override;

    Tile3DItemView();
    ~Tile3DItemView();
};

#endif // TILE_3D_ITEM_VIEW_H