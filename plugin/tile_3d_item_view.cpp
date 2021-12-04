/*************************************************************************/
/*  tile_3d_item_view.cpp                                                */
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

#include "core/object/message_queue.h"
#include "scene/scene_string_names.h"
#include "tile_3d_item_view.h"

void Tile3DItemView::_queue_sort() {
	if (!is_inside_tree()) {
		return;
	}

	if (pending_sort) {
		return;
	}

	MessageQueue::get_singleton()->push_callable(callable_mp(this, &Tile3DItemView::_sort_children));
	pending_sort = true;
}

void Tile3DItemView::_sort_children() {
	Size2 size = get_size();
	Size2 ms = label->get_combined_minimum_size();
	Point2 ofs;

	if (!style.is_valid()) {
		style = get_theme_stylebox(SNAME("normal"));
	}

	if (display_mode == DISPLAY_MODE_THUMBNAIL) {
		ofs = Point2(0, preview_size);
		ms.width = size.width - style->get_minimum_size().width;
	} else if (display_mode == DISPLAY_MODE_LIST) {
		bool rtl = is_layout_rtl();
		int s = preview_size + get_theme_constant(SNAME("hseparation"));
		ms.width = size.width - s - style->get_minimum_size().width;
		if (rtl) {
			ofs = Point2(0, Math::floor((size.height - ms.height) * 0.5));
		} else {
			ofs = Point2(s, Math::floor((size.height - ms.height) * 0.5));
		}
	}

	ofs += style->get_offset();

	Rect2 rect = Rect2(ofs, ms);
	label->set_rect(rect);

	pending_sort = false;
}

void Tile3DItemView::_draw_item() {
	RID ci = get_canvas_item();
	Size2 size = get_size();
	Color color; // TODO: Change label font color
	bool rtl = is_layout_rtl();

	switch (get_draw_mode()) {
		case DRAW_NORMAL: {
			if (rtl && has_theme_stylebox(SNAME("normal_mirrored"), SNAME("Button"))) {
				style = get_theme_stylebox(SNAME("normal_mirrored"), SNAME("Button"));
			} else {
				style = get_theme_stylebox(SNAME("normal"), SNAME("Button"));
			}

			// Focus colors only take precedence over normal state.
			if (has_focus()) {
				color = get_theme_color(SNAME("font_focus_color"), SNAME("Button"));
			} else {
				color = get_theme_color(SNAME("font_color"), SNAME("Button"));
			}
		} break;
		case DRAW_HOVER_PRESSED: {
			if (has_theme_stylebox(SNAME("hover_pressed"), SNAME("Button")) && has_theme_stylebox_override("hover_pressed")) {
				if (rtl && has_theme_stylebox(SNAME("hover_pressed_mirrored"), SNAME("Button"))) {
					style = get_theme_stylebox(SNAME("hover_pressed_mirrored"), SNAME("Button"));
				} else {
					style = get_theme_stylebox(SNAME("hover_pressed"), SNAME("Button"));
				}

				if (has_theme_color(SNAME("font_hover_pressed_color"), SNAME("Button"))) {
					color = get_theme_color(SNAME("font_hover_pressed_color"), SNAME("Button"));
				} else {
					color = get_theme_color(SNAME("font_color"), SNAME("Button"));
				}
				break;
			}
			[[fallthrough]];
		}
		case DRAW_PRESSED: {
			if (rtl && has_theme_stylebox(SNAME("pressed_mirrored"), SNAME("Button"))) {
				style = get_theme_stylebox(SNAME("pressed_mirrored"), SNAME("Button"));
			} else {
				style = get_theme_stylebox(SNAME("pressed"), SNAME("Button"));
			}

			if (has_theme_color(SNAME("font_pressed_color"), SNAME("Button"))) {
				color = get_theme_color(SNAME("font_pressed_color")), SNAME("Button");
			} else {
				color = get_theme_color(SNAME("font_color"), SNAME("Button"));
			}
		} break;
		case DRAW_HOVER: {
			if (rtl && has_theme_stylebox(SNAME("hover_mirrored"), SNAME("Button"))) {
				style = get_theme_stylebox(SNAME("hover_mirrored"), SNAME("Button"));
			} else {
				style = get_theme_stylebox(SNAME("hover"), SNAME("Button"));
			}

			color = get_theme_color(SNAME("font_hover_color"), SNAME("Button"));
		} break;
		case DRAW_DISABLED: {
			if (rtl && has_theme_stylebox(SNAME("disabled_mirrored"), SNAME("Button"))) {
				style = get_theme_stylebox(SNAME("disabled_mirrored"), SNAME("Button"));
			} else {
				style = get_theme_stylebox(SNAME("disabled"), SNAME("Button"));
			}

			color = get_theme_color(SNAME("font_disabled_color"), SNAME("Button"));
		} break;
	}

	style->draw(ci, Rect2(Point2(0, 0), size));

	if (has_focus()) {
		Ref<StyleBox> style2 = get_theme_stylebox(SNAME("focus"), SNAME("Button"));
		style2->draw(ci, Rect2(Point2(), size));
	}

	if (data.is_valid()) {
		Rect2 texture_rect;
		Color color_preview(1, 1, 1, 1);
		if (is_disabled()) {
			color_preview.a = 0.4;
		}
		Point2 style_offset;
		if (rtl) {
			style_offset.x = -style->get_margin(SIDE_RIGHT);
		} else {
			style_offset.x = style->get_margin(SIDE_LEFT);
		}
		style_offset.y = style->get_margin(SIDE_TOP);

		if (display_mode == DISPLAY_MODE_LIST) {
			int valign = size.height - style->get_minimum_size().height;
			int _preview_size = int(round(preview_size * LIST_PREVIEW_FRACTION));
			Size2 texture_size = Size2(_preview_size, _preview_size);

			if (rtl) {
				texture_rect = Rect2(style_offset + Point2(size.x - texture_size.x, Math::floor((valign - texture_size.y) * 0.5)), texture_size);
			} else {
				texture_rect = Rect2(style_offset + Point2(0.0, Math::floor((valign - texture_size.y) * 0.5)), texture_size);
			}
		} else if (display_mode == DISPLAY_MODE_THUMBNAIL) {
			int halign = size.width - style->get_minimum_size().x;
			Size2 texture_size = Size2(preview_size, preview_size);
			texture_rect = Rect2(style_offset + Point2(Math::floor((halign - texture_size.x) * 0.5), 0), texture_size);
		}

		if (data->get_preview().is_valid() && texture_rect.size.width > 0) {
			Ref<Texture2D> preview = data->get_preview();
			draw_texture_rect_region(preview, texture_rect, Rect2(Point2(), preview->get_size()), color_preview);
		}
	}
}

void Tile3DItemView::_data_changed() {
	label->set_text(data->get_name());
	update();
}

void Tile3DItemView::_label_minsize_changed() {
	minimum_size_changed();
	_queue_sort();
}

void Tile3DItemView::set_data(const Ref<TileData3D> &p_data) {
	if (p_data == data) {
		return;
	}

	if (data.is_valid()) {
		data->disconnect("changed", callable_mp(this, &Tile3DItemView::_data_changed));
	}

	data = p_data;

	if (data.is_valid()) {
		set_disabled(false);
		data->connect("changed", callable_mp(this, &Tile3DItemView::_data_changed));
		label->set_text(data->get_name());
	} else {
		set_disabled(true);
		label->set_text("");
	}

	update();
}

void Tile3DItemView::set_preview_size(int p_size) {
	preview_size = p_size;
	minimum_size_changed();
	update();
}

void Tile3DItemView::set_id(int p_id) {
	id = p_id;
}

int Tile3DItemView::get_id() const {
	return id;
}

void Tile3DItemView::set_display_mode(DisplayMode p_mode) {
	display_mode = p_mode;

	if (display_mode == DISPLAY_MODE_THUMBNAIL) {
		int font_size = EDITOR_GET("editors/tile_map_3d/tile_sets/thumbnail_view_font_size"); // TODO: What if changed??
		label->add_theme_font_size_override("font_size", font_size * EDSCALE);
		label->set_max_lines_visible(2);
		label->set_align(Label::ALIGN_CENTER);
		set_h_size_flags(SIZE_FILL);
	} else {
		label->add_theme_font_size_override("font_size", get_theme_font_size(SNAME("font_size")));
		label->set_max_lines_visible(1);
		label->set_align(Label::ALIGN_LEFT);
		set_h_size_flags(SIZE_EXPAND_FILL);
	}

	minimum_size_changed();
	_queue_sort();
	update();
}

Size2 Tile3DItemView::get_minimum_size() const {
    Size2 minsize = Size2(preview_size, preview_size);
	Size2 ms = label->get_combined_minimum_size();

	if (display_mode == DISPLAY_MODE_THUMBNAIL) {
		minsize.width = MAX(ms.width, minsize.width);
		minsize.height += ms.height;
	} else {
		minsize.width += ms.width + get_theme_constant(SNAME("hseparation"));
		minsize.height = MAX(ms.height, minsize.height);
	}

	return minsize + get_theme_stylebox(SNAME("normal"))->get_minimum_size();
}

void Tile3DItemView::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            pending_sort = false;
			_queue_sort();
        } break;
        case NOTIFICATION_THEME_CHANGED: {
			minimum_size_changed();
			update();
        } break;
        case NOTIFICATION_DRAW: {
            _draw_item();
        } break;
		case NOTIFICATION_RESIZED: {
			_queue_sort();
		} break;
    }
}

Tile3DItemView::Tile3DItemView() {
    EDITOR_DEF("editors/tile_map_3d/tile_sets/thumbnail_view_font_size", 12);
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/tile_map_3d/tile_sets/thumbnail_view_font_size", PROPERTY_HINT_RANGE, "8,18"));
	set_disabled(true);
	set_mouse_filter(MOUSE_FILTER_STOP);
	set_toggle_mode(true);

	label = memnew(Label);
	label->set_align(Label::ALIGN_LEFT);
	label->set_autowrap_mode(Label::AUTOWRAP_WORD_SMART);
	// label->set_v_size_flags(SIZE_EXPAND | SIZE_SHRINK_CENTER);
	label->set_auto_translate(false);
	label->set_custom_minimum_size(Size2(80 * EDSCALE, 0));
	label->connect(SNAME("minimum_size_changed"), callable_mp(this, &Tile3DItemView::_label_minsize_changed));
	add_child(label, false, INTERNAL_MODE_FRONT);
}

Tile3DItemView::~Tile3DItemView() {
}