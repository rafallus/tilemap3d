/*************************************************************************/
/*  tile_set_3d.cpp                                                      */
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
#include "tile_set_3d.h"

/******* TileData3D *******/
/**************************/
void TileData3D::_queue_changed() {
	if (_changed_request) {
		return;
	}

	MessageQueue::get_singleton()->push_callable(callable_mp(this, &TileData3D::_update_changed));
	_changed_request = true;
}

void TileData3D::_update_changed() {
	emit_changed();
	_changed_request = false;
}

void TileData3D::set_span(const Vector3i &p_span) {
	span = p_span;
	_queue_changed();
}

Vector3i TileData3D::get_span() const {
	return span;
}

void TileData3D::set_preview(const Ref<Texture2D> &p_preview) {
	preview = p_preview;
	_queue_changed();
}

Ref<Texture2D> TileData3D::get_preview() const {
	return preview;
}

void TileData3D::set_probability(float p_probability) {
	probability = p_probability;
	_queue_changed();
}

float TileData3D::get_probability() const {
	return probability;
}

void TileData3D::set_source(const String &p_source) {
	source = p_source;
}

String TileData3D::get_source() const {
	return source;
}

void TileData3D::_set_alternative_id(int p_alt_id) {
	alternative_id = p_alt_id;
}

int TileData3D::_get_alternative_id() const{
	return alternative_id;
}

void TileData3D::_set_internal_probability(float p_internal_prob) {
	internal_probability = p_internal_prob;
}

float TileData3D::_get_internal_probability() const {
	return internal_probability;
}

bool TileData3D::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "source") {
		set_source(p_value);
		return true;
	} else if (p_name == "alternative_id") {
		_set_alternative_id(p_value);
		return true;
	} else if (p_name == "internal_probability") {
		_set_internal_probability(p_value);
		return true;
	}
	return false;
}

bool TileData3D::_get(const StringName &p_name, Variant &r_ret) const {
	if (p_name == "source") {
		r_ret = get_source();
		return true;
	} else if (p_name == "alternative_id") {
		r_ret = _get_alternative_id();
		return true;
	} else if (p_name == "internal_probability") {
		r_ret = _get_internal_probability();
		return true;
	}
	return false;
}

void TileData3D::_get_property_list(List<PropertyInfo> *p_list) const {
	PropertyInfo pi = PropertyInfo(Variant::STRING, "source");
	pi.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(pi);
	pi = PropertyInfo(Variant::INT, "alternative_id");
	pi.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(pi);
	pi = PropertyInfo(Variant::FLOAT, "internal_probability");
	pi.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(pi);
}

void TileData3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_span", "span"), &TileData3D::set_span);
	ClassDB::bind_method(D_METHOD("get_span"), &TileData3D::get_span);
	ClassDB::bind_method(D_METHOD("set_preview", "preview"), &TileData3D::set_preview);
	ClassDB::bind_method(D_METHOD("get_preview"), &TileData3D::get_preview);
	ClassDB::bind_method(D_METHOD("set_probability", "probabilty"), &TileData3D::set_probability);
	ClassDB::bind_method(D_METHOD("get_probability"), &TileData3D::get_probability);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3I, "span"), "set_span", "get_span");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "preview", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_preview", "get_preview");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_probability", "get_probability");
}

/******* TileData3DMesh *******/
/******************************/

void TileData3DMesh::set_mesh(const Ref<Mesh> &p_mesh) {
	mesh = p_mesh;
	_queue_changed();
}

Ref<Mesh> TileData3DMesh::get_mesh() const {
	return mesh;
}

void TileData3DMesh::set_mesh_transform(const Transform3D &p_transform) {
	mesh_transform = p_transform;
	_queue_changed();
}

Transform3D TileData3DMesh::get_mesh_transform() const {
	return mesh_transform;
}

void TileData3DMesh::set_material_override(const Ref<Material> &p_material) {
	material_override = p_material;
	_queue_changed();
}

Ref<Material> TileData3DMesh::get_material_override() const {
	return material_override;
}

void TileData3DMesh::set_transparency(float p_transparency) {
	transparency = p_transparency;
	_queue_changed();
}

float TileData3DMesh::get_transparency() const {
	return transparency;
}

void TileData3DMesh::set_cast_shadow_mode(GeometryInstance3D::ShadowCastingSetting p_mode) {
	cast_shadow = p_mode;
	_queue_changed();
}

GeometryInstance3D::ShadowCastingSetting TileData3DMesh::get_cast_shadow_mode() const {
	return cast_shadow;
}

void TileData3DMesh::set_lod_bias(float p_value) {
	lod_bias = p_value;
	_queue_changed();
}

float TileData3DMesh::get_lod_bias() const {
	return lod_bias;
}

void TileData3DMesh::set_ignore_occlusion_culling(bool p_value) {
	ignore_occlusion_culling = p_value;
	_queue_changed();
}

bool TileData3DMesh::is_ignoring_occlusion_culling() const {
	return ignore_occlusion_culling;
}

void TileData3DMesh::set_gi_mode(GeometryInstance3D::GIMode p_mode) {
	gi_mode = p_mode;
	_queue_changed();
}

GeometryInstance3D::GIMode TileData3DMesh::get_gi_mode() const {
	return gi_mode;
}

void TileData3DMesh::set_gi_lightmap_scale(GeometryInstance3D::LightmapScale p_scale) {
	gi_lightmap_scale = p_scale;
	_queue_changed();
}

GeometryInstance3D::LightmapScale TileData3DMesh::get_gi_lightmap_scale() const {
	return gi_lightmap_scale;
}

void TileData3DMesh::set_visibility_range_begin(float p_value) {
	visibility_range_begin = p_value;
	_queue_changed();
}

float TileData3DMesh::get_visibility_range_begin() const {
	return visibility_range_begin;
}

void TileData3DMesh::set_visibility_range_begin_margin(float p_value) {
	visibility_range_begin_margin = p_value;
	_queue_changed();
}

float TileData3DMesh::get_visibility_range_begin_margin() const {
	return visibility_range_begin_margin;
}

void TileData3DMesh::set_visibility_range_end(float p_value) {
	visibility_range_end = p_value;
	_queue_changed();
}

float TileData3DMesh::get_visibility_range_end() const {
	return visibility_range_end;
}

void TileData3DMesh::set_visibility_range_end_margin(float p_value) {
	visibility_range_end_margin = p_value;
	_queue_changed();
}

float TileData3DMesh::get_visibility_range_end_margin() const {
	return visibility_range_end_margin;
}

void TileData3DMesh::set_visibility_range_fade_mode(GeometryInstance3D::VisibilityRangeFadeMode p_mode) {
	visibility_range_fade_mode = p_mode;
	_queue_changed();
}

GeometryInstance3D::VisibilityRangeFadeMode TileData3DMesh::get_visibility_range_fade_mode() const {
	return visibility_range_fade_mode;
}

void TileData3DMesh::set_physics_data_count(int p_count) {
	physics_data.resize(p_count);
	_queue_changed();
	notify_property_list_changed();
}

int TileData3DMesh::get_physics_data_count() const {
	return physics_data.size();
}

void TileData3DMesh::add_physics_data(int p_index) {
	if (p_index < 0) {
		p_index = physics_data.size();
	}
	ERR_FAIL_INDEX(p_index, physics_data.size() + 1);
	physics_data.insert(p_index, PhysicsLayerTileData3D());

	notify_property_list_changed();
	_queue_changed();
}

void TileData3DMesh::move_physics_data(int p_from_index, int p_to_pos) {
	ERR_FAIL_INDEX(p_from_index, physics_data.size());
	ERR_FAIL_INDEX(p_to_pos, physics_data.size() + 1);
	physics_data.insert(p_to_pos, physics_data[p_from_index]);
	physics_data.remove_at(p_to_pos < p_from_index ? p_from_index + 1 : p_from_index);
	notify_property_list_changed();
	_queue_changed();
}

void TileData3DMesh::remove_physics_data(int p_index) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	physics_data.remove_at(p_index);
	notify_property_list_changed();
	_queue_changed();
}

int TileData3DMesh::get_physics_layer_index(int p_layer) const {
	for (int i = 0; i < physics_data.size(); i++) {
		if (physics_data[i].layer_id == p_layer) {
			return i;
		}
	}
	return -1;
}

void TileData3DMesh::set_physics_data_layer_id(int p_index, int p_layer) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	physics_data.write[p_index].layer_id = p_layer;
	_queue_changed();
}

int TileData3DMesh::get_physics_data_layer_id(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), -1);
	return physics_data[p_index].layer_id;
}

void TileData3DMesh::set_physics_data_shape(int p_index, int p_shapei, const Ref<Shape3D> &p_shape) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	ERR_FAIL_INDEX(p_shapei, physics_data[p_index].shapes.size());
	physics_data.write[p_index].shapes.write[p_shapei].shape = p_shape;
	_queue_changed();
}

Ref<Shape3D> TileData3DMesh::get_physics_data_shape(int p_index, int p_shapei) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), Ref<Shape3D>());
	ERR_FAIL_INDEX_V(p_shapei, physics_data[p_index].shapes.size(), Ref<Shape3D>());
	return physics_data[p_index].shapes[p_shapei].shape;
}

void TileData3DMesh::set_physics_data_shape_transform(int p_index, int p_shapei, const Transform3D &p_transform) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	ERR_FAIL_INDEX(p_shapei, physics_data[p_index].shapes.size());
	physics_data.write[p_index].shapes.write[p_shapei].local_transform = p_transform;
	_queue_changed();
}

Transform3D TileData3DMesh::get_physics_data_shape_transform(int p_index, int p_shapei) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), Transform3D());
	ERR_FAIL_INDEX_V(p_shapei, physics_data[p_index].shapes.size(), Transform3D());
	return physics_data[p_index].shapes[p_shapei].local_transform;
}

void TileData3DMesh::set_physics_data_linear_velocity(int p_index, const Vector3 &p_linear_velocity) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	physics_data.write[p_index].linear_velocity = p_linear_velocity;
	_queue_changed();
}

Vector3 TileData3DMesh::get_physics_data_linear_velocity(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), Vector3());
	return physics_data[p_index].linear_velocity;
}

void TileData3DMesh::set_physics_data_angular_velocity(int p_index, const Vector3 &p_angular_velocity) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	physics_data.write[p_index].angular_velocity = p_angular_velocity;
	_queue_changed();
}

Vector3 TileData3DMesh::get_physics_data_angular_velocity(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), Vector3());
	return physics_data[p_index].angular_velocity;
}

void TileData3DMesh::set_physics_data_shapes(int p_index, const Vector<ShapeData> &p_shapes) {
	ERR_FAIL_INDEX(p_index, physics_data.size());
	physics_data.write[p_index].shapes = p_shapes;
	_queue_changed();
	notify_property_list_changed();
}

Vector<TileData3DMesh::ShapeData> TileData3DMesh::get_physics_data_shapes(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, physics_data.size(), Vector<ShapeData>());
	return physics_data[p_index].shapes;
}

void TileData3DMesh::_set_physics_data_shapes(int p_index, const Array &p_shapes) {
	ERR_FAIL_COND(p_shapes.size() & 1);
	Vector<ShapeData> shapes;
	for (int i = 0; i < p_shapes.size(); i += 2) {
		ShapeData sd;
		Ref<Shape3D> shape = p_shapes[i];
		if (!shape.is_valid()) {
			shape = Ref<BoxShape3D>();
		}
		sd.shape = shape;

		if (p_shapes[i + 1]) {
			sd.local_transform = p_shapes[i + 1];
		}

		shapes.push_back(sd);
	}

	set_physics_data_shapes(p_index, shapes);
}

Array TileData3DMesh::_get_physics_data_shapes(int p_index) const {
	Vector<ShapeData> shapes = get_physics_data_shapes(p_index);
	Array ret;
	for (int i = 0; i < shapes.size(); i++) {
		ret.push_back(shapes[i].shape);
		ret.push_back(shapes[i].local_transform);
	}

	return ret;
}

void TileData3DMesh::set_navigation_data_count(int p_count) {
	navigation_data.resize(p_count);
	_queue_changed();
	notify_property_list_changed();
}

int TileData3DMesh::get_navigation_data_count() const {
	return navigation_data.size();
}

int TileData3DMesh::get_navigation_layer_index(int p_layer) const {
	for (int i = 0; i < navigation_data.size(); i++) {
		if (navigation_data[i].layer_id == p_layer) {
			return i;
		}
	}
	return -1;
}

void TileData3DMesh::set_navigation_data_layer_id(int p_index, int p_layer) {
	ERR_FAIL_INDEX(p_index, navigation_data.size());
	navigation_data.write[p_index].layer_id = p_layer;
	_queue_changed();
}

int TileData3DMesh::get_navigation_data_layer_id(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, navigation_data.size(), -1);
	return navigation_data[p_index].layer_id;
}

void TileData3DMesh::set_navigation_data_navmesh(int p_index, const Ref<NavigationMesh> &p_navmesh) {
	ERR_FAIL_INDEX(p_index, navigation_data.size());
	navigation_data.write[p_index].navmesh = p_navmesh;
	_queue_changed();
}

Ref<NavigationMesh> TileData3DMesh::get_navigation_data_navmesh(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, navigation_data.size(), Ref<NavigationMesh>());
	return navigation_data[p_index].navmesh;
}

void TileData3DMesh::set_navigation_data_navmesh_transform(int p_index, const Transform3D &p_transform) {
	ERR_FAIL_INDEX(p_index, navigation_data.size());
	navigation_data.write[p_index].local_transform = p_transform;
	_queue_changed();
}

Transform3D TileData3DMesh::get_navigation_data_navmesh_transform(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, navigation_data.size(), Transform3D());
	return navigation_data[p_index].local_transform;
}

void TileData3DMesh::set_light_transform(int p_index, const Transform3D &p_transform) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].local_transform = p_transform;
	_queue_changed();
}

Transform3D TileData3DMesh::get_light_transform(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), Transform3D());
	return lights[p_index].local_transform;
}

void TileData3DMesh::set_light_type(int p_index, TileData3DMesh::LightType p_type) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].type = p_type;
	_queue_changed();
}

TileData3DMesh::LightType TileData3DMesh::get_light_type(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), LIGHT_TYPE_OMNI);
	return lights[p_index].type;
}

void TileData3DMesh::set_light_range(int p_index, float p_range) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].light_range = p_range;
	_queue_changed();
}

float TileData3DMesh::get_light_range(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].light_range;
}

void TileData3DMesh::set_light_attenuation(int p_index, float p_attenuation) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].attenuation = p_attenuation;
	_queue_changed();
}

float TileData3DMesh::get_light_attenuation(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].attenuation;
}

void TileData3DMesh::set_light_spot_angle(int p_index, float p_angle) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].spot_angle = p_angle;
	_queue_changed();
}

float TileData3DMesh::get_light_spot_angle(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].spot_angle;
}

void TileData3DMesh::set_light_spot_angle_attenuation(int p_index, float p_angle_attenuation) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].spot_angle_attenuation = p_angle_attenuation;
	_queue_changed();
}

float TileData3DMesh::get_light_spot_angle_attenuation(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].spot_angle_attenuation;
}

void TileData3DMesh::set_light_omni_shadow_mode(int p_index, RenderingServer::LightOmniShadowMode p_mode) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].omni_shadow_mode = p_mode;
	_queue_changed();
}

RenderingServer::LightOmniShadowMode TileData3DMesh::get_light_omni_shadow_mode(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), RenderingServer::LIGHT_OMNI_SHADOW_CUBE);
	return lights[p_index].omni_shadow_mode;
}

void TileData3DMesh::set_light_color(int p_index, const Color &p_color) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].color = p_color;
	_queue_changed();
}

Color TileData3DMesh::get_light_color(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), Color());
	return lights[p_index].color;
}

void TileData3DMesh::set_light_energy(int p_index, float p_energy) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].energy = p_energy;
	_queue_changed();
}

float TileData3DMesh::get_light_energy(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].energy;
}

void TileData3DMesh::set_light_indirect_energy(int p_index, float p_energy) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].indirect_energy = p_energy;
	_queue_changed();
}

float TileData3DMesh::get_light_indirect_energy(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].indirect_energy;
}

void TileData3DMesh::set_light_projector(int p_index, const Ref<Texture2D> &p_projector) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].projector = p_projector;
	_queue_changed();
}

Ref<Texture2D> TileData3DMesh::get_light_projector(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), Ref<Texture2D>());
	return lights[p_index].projector;
}

void TileData3DMesh::set_light_size(int p_index, float p_size) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].size = p_size;
	_queue_changed();
}

float TileData3DMesh::get_light_size(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].size;
}

void TileData3DMesh::set_light_negative(int p_index, bool p_negative) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].negative = p_negative;
	_queue_changed();
}

bool TileData3DMesh::is_light_negative(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), false);
	return lights[p_index].negative;
}

void TileData3DMesh::set_light_specular(int p_index, float p_specular) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].specular = p_specular;
	_queue_changed();
}

float TileData3DMesh::get_light_specular(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].specular;
}

void TileData3DMesh::set_light_shadow_enabled(int p_index, bool p_enabled) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_enabled = p_enabled;
	_queue_changed();
}

bool TileData3DMesh::is_light_shadow_enabled(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), false);
	return lights[p_index].shadow_enabled;
}

void TileData3DMesh::set_light_shadow_color(int p_index, const Color &p_color) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_color = p_color;
	_queue_changed();
}

Color TileData3DMesh::get_light_shadow_color(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), Color());
	return lights[p_index].shadow_color;
}

void TileData3DMesh::set_light_shadow_bias(int p_index, float p_bias) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_bias = p_bias;
	_queue_changed();
}

float TileData3DMesh::get_light_shadow_bias(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].shadow_bias;
}

void TileData3DMesh::set_light_shadow_normal_bias(int p_index, float p_bias) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_normal_bias = p_bias;
	_queue_changed();
}

float TileData3DMesh::get_light_shadow_normal_bias(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].shadow_normal_bias;
}

void TileData3DMesh::set_light_shadow_reverse_cull_face(int p_index, bool p_reverse) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_reverse_cull_face = p_reverse;
	_queue_changed();
}

bool TileData3DMesh::is_light_shadow_reverse_cull_face(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), false);
	return lights[p_index].shadow_reverse_cull_face;
}

void TileData3DMesh::set_light_shadow_transmittance_bias(int p_index, float p_bias) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_transmittance_bias = p_bias;
	_queue_changed();
}

float TileData3DMesh::get_light_shadow_transmittance_bias(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].shadow_transmittance_bias;
}

void TileData3DMesh::set_light_shadow_fog_fade(int p_index, float p_fade) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_fog_fade = p_fade;
	_queue_changed();
}

float TileData3DMesh::get_light_shadow_fog_fade(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].shadow_fog_fade;
}

void TileData3DMesh::set_light_shadow_blur(int p_index, float p_blur) {
	ERR_FAIL_INDEX(p_index, lights.size());
	lights.write[p_index].shadow_blur = p_blur;
	_queue_changed();
}

float TileData3DMesh::get_light_shadow_blur(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, lights.size(), 0.0);
	return lights[p_index].shadow_blur;
}

bool TileData3DMesh::_set(const StringName &p_name, const Variant &p_value) {
	Vector<String> components = String(p_name).split("/", true, 2);
	if (components.size() == 2) {
		if (components[0].begins_with("physics_data_") && components[0].trim_prefix("physics_data_").is_valid_int()) {
			int index = components[0].trim_prefix("physics_data_").to_int();
			ERR_FAIL_INDEX_V(index, physics_data.size(), false);
			if (components[1] == "layer") {
				set_physics_data_layer_id(index, p_value);
				return true;
			} else if (components[1] == "linear_velocity") {
				set_physics_data_linear_velocity(index, p_value);
				return true;
			} else if (components[1] == "angular_velocity") {
				set_physics_data_angular_velocity(index, p_value);
				return true;
			} else if (components[1] == "shapes") {
				_set_physics_data_shapes(index, p_value);
				return true;
			} else if (components[1].begins_with("shape_")) {
				Vector<String> parts = components[1].split("/", true, 2);
				if (parts[0].trim_prefix("shape_").is_valid_int()) {
					int shapei = parts[0].trim_prefix("shape_").to_int();
					ERR_FAIL_INDEX_V(shapei, physics_data[index].shapes.size(), false);
					if (parts[1] == "shape") {
						set_physics_data_shape(index, shapei, p_value);
						return true;
					} else if (parts[1] == "transform") {
						set_physics_data_shape_transform(index, shapei, p_value);
						return true;
					}
				}
			}
		} else if (components[0].begins_with("navigation_data_") && components[0].trim_prefix("navigation_data_").is_valid_int()) {
			int index = components[0].trim_prefix("navigation_data_").to_int();
			ERR_FAIL_INDEX_V(index, physics_data.size(), false);
			if (components[1] == "layer") {
				set_navigation_data_layer_id(index, p_value);
				return true;
			} else if (components[1] == "navmesh") {
				set_navigation_data_navmesh(index, p_value);
				return true;
			} else if (components[1] == "transform") {
				set_navigation_data_navmesh_transform(index, p_value);
				return true;
			}
		}
	}
	return false;
}

bool TileData3DMesh::_get(const StringName &p_name, Variant &r_ret) const {
	Vector<String> components = String(p_name).split("/", true, 2);
	if (components.size() == 2) {
		if (components[0].begins_with("physics_data_") && components[0].trim_prefix("physics_data_").is_valid_int()) {
			int index = components[0].trim_prefix("physics_data_").to_int();
			ERR_FAIL_INDEX_V(index, physics_data.size(), false);
			if (components[1] == "layer") {
				r_ret = get_physics_data_layer_id(index);
				return true;
			} else if (components[1] == "linear_velocity") {
				r_ret = get_physics_data_linear_velocity(index);
				return true;
			} else if (components[1] == "angular_velocity") {
				r_ret = get_physics_data_angular_velocity(index);
				return true;
			} else if (components[1] == "shapes") {
				r_ret = _get_physics_data_shapes(index);
				return true;
			} else if (components[1].begins_with("shape_")) {
				Vector<String> parts = components[1].split("/", true, 2);
				if (parts[0].trim_prefix("shape_").is_valid_int()) {
					int shapei = parts[0].trim_prefix("shape_").to_int();
					ERR_FAIL_INDEX_V(shapei, physics_data[index].shapes.size(), false);
					if (parts[1] == "shape") {
						r_ret = get_physics_data_shape(index, shapei);
						return true;
					} else if (parts[1] == "transform") {
						r_ret = get_physics_data_shape_transform(index, shapei);
						return true;
					}
				}
			}
		} else if (components[0].begins_with("navigation_data_") && components[0].trim_prefix("navigation_data_").is_valid_int()) {
			int index = components[0].trim_prefix("navigation_data_").to_int();
			ERR_FAIL_INDEX_V(index, physics_data.size(), false);
			if (components[1] == "layer") {
				r_ret = get_navigation_data_layer_id(index);
				return true;
			} else if (components[1] == "navmesh") {
				r_ret = get_navigation_data_navmesh(index);
				return true;
			} else if (components[1] == "transform") {
				r_ret = get_navigation_data_navmesh_transform(index);
				return true;
			}
		}
	}
	return false;
}

void TileData3DMesh::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::NIL, "Miscellaneous", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_GROUP));
	for (int i = 0; i < physics_data.size(); i++) {
		String prefix = vformat("physics_data_%d/", i);
		p_list->push_back(PropertyInfo(Variant::INT, prefix + "layer", PROPERTY_HINT_RANGE, "-1,128,1,or_greater"));
		PropertyInfo p = PropertyInfo(Variant::VECTOR3, prefix + "linear_velocity");
		p.usage &= ~(physics_data[i].linear_velocity == Vector3() ? PROPERTY_USAGE_STORAGE : 0);
		p_list->push_back(p);
		p = PropertyInfo(Variant::VECTOR3, prefix + "angular_velocity");
		p.usage &= ~(physics_data[i].angular_velocity == Vector3() ? PROPERTY_USAGE_STORAGE : 0);
		p_list->push_back(p);
		p = PropertyInfo(Variant::ARRAY, prefix + "shapes");
		p.usage &= ~PROPERTY_USAGE_STORAGE;
		p_list->push_back(p);
		for (int j = 0; j < physics_data[i].shapes.size(); j++) {
			String shape_prefix = prefix + vformat("shape_%d/", j);
			p = PropertyInfo(Variant::OBJECT, shape_prefix + "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D");
			p.usage = PROPERTY_USAGE_NO_EDITOR;
			p_list->push_back(p);
			p = PropertyInfo(Variant::TRANSFORM3D, shape_prefix + "transform");
			p.usage = PROPERTY_USAGE_NO_EDITOR;
			p_list->push_back(p);
		}
	}
	for (int i = 0; i < navigation_data.size(); i++) {
		String prefix = vformat("navigation_data_%d/", i);
		p_list->push_back(PropertyInfo(Variant::INT, prefix + "layer", PROPERTY_HINT_RANGE, "-1,128,1,or_greater"));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prefix + "navmesh"));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prefix + "transform"));
	}
}

void TileData3DMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &TileData3DMesh::set_mesh);
	ClassDB::bind_method(D_METHOD("get_mesh"), &TileData3DMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh_transform", "transform"), &TileData3DMesh::set_mesh_transform);
	ClassDB::bind_method(D_METHOD("get_mesh_transform"), &TileData3DMesh::get_mesh_transform);
	ClassDB::bind_method(D_METHOD("set_material_override", "material"), &TileData3DMesh::set_material_override);
	ClassDB::bind_method(D_METHOD("get_material_override"), &TileData3DMesh::get_material_override);
	ClassDB::bind_method(D_METHOD("set_transparency", "transparency"), &TileData3DMesh::set_transparency);
	ClassDB::bind_method(D_METHOD("get_transparency"), &TileData3DMesh::get_transparency);
	ClassDB::bind_method(D_METHOD("set_cast_shadow_mode", "mode"), &TileData3DMesh::set_cast_shadow_mode);
	ClassDB::bind_method(D_METHOD("get_cast_shadow_mode"), &TileData3DMesh::get_cast_shadow_mode);
	ClassDB::bind_method(D_METHOD("set_lod_bias", "bias"), &TileData3DMesh::set_lod_bias);
	ClassDB::bind_method(D_METHOD("get_lod_bias"), &TileData3DMesh::get_lod_bias);
	ClassDB::bind_method(D_METHOD("set_ignore_occlusion_culling", "ignore"), &TileData3DMesh::set_ignore_occlusion_culling);
	ClassDB::bind_method(D_METHOD("is_ignoring_occlusion_culling"), &TileData3DMesh::is_ignoring_occlusion_culling);

	ClassDB::bind_method(D_METHOD("set_gi_mode", "mode"), &TileData3DMesh::set_gi_mode);
	ClassDB::bind_method(D_METHOD("get_gi_mode"), &TileData3DMesh::get_gi_mode);
	ClassDB::bind_method(D_METHOD("set_gi_lightmap_scale", "scale"), &TileData3DMesh::set_gi_lightmap_scale);
	ClassDB::bind_method(D_METHOD("get_gi_lightmap_scale"), &TileData3DMesh::get_gi_lightmap_scale);

	ClassDB::bind_method(D_METHOD("set_visibility_range_begin", "value"), &TileData3DMesh::set_visibility_range_begin);
	ClassDB::bind_method(D_METHOD("get_visibility_range_begin"), &TileData3DMesh::get_visibility_range_begin);
	ClassDB::bind_method(D_METHOD("set_visibility_range_begin_margin", "value"), &TileData3DMesh::set_visibility_range_begin_margin);
	ClassDB::bind_method(D_METHOD("get_visibility_range_begin_margin"), &TileData3DMesh::get_visibility_range_begin_margin);
	ClassDB::bind_method(D_METHOD("set_visibility_range_end", "value"), &TileData3DMesh::set_visibility_range_end);
	ClassDB::bind_method(D_METHOD("get_visibility_range_end"), &TileData3DMesh::get_visibility_range_end);
	ClassDB::bind_method(D_METHOD("set_visibility_range_end_margin", "value"), &TileData3DMesh::set_visibility_range_end_margin);
	ClassDB::bind_method(D_METHOD("get_visibility_range_end_margin"), &TileData3DMesh::get_visibility_range_end_margin);
	ClassDB::bind_method(D_METHOD("set_visibility_range_fade_mode", "mode"), &TileData3DMesh::set_visibility_range_fade_mode);
	ClassDB::bind_method(D_METHOD("get_visibility_range_fade_mode"), &TileData3DMesh::get_visibility_range_fade_mode);

	ClassDB::bind_method(D_METHOD("set_physics_data_count", "count"), &TileData3DMesh::set_physics_data_count);
	ClassDB::bind_method(D_METHOD("get_physics_data_count"), &TileData3DMesh::get_physics_data_count);
	ClassDB::bind_method(D_METHOD("add_physics_data", "to_position"), &TileData3DMesh::add_physics_data, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("move_physics_data", "index", "to_position"), &TileData3DMesh::move_physics_data);
	ClassDB::bind_method(D_METHOD("remove_physics_data", "index"), &TileData3DMesh::remove_physics_data);

	ClassDB::bind_method(D_METHOD("set_navigation_data_count", "count"), &TileData3DMesh::set_navigation_data_count);
	ClassDB::bind_method(D_METHOD("get_navigation_data_count"), &TileData3DMesh::get_navigation_data_count);

	ADD_GROUP("Rendering", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "mesh_transform"), "set_mesh_transform", "get_mesh_transform");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material_override", PROPERTY_HINT_RESOURCE_TYPE, "BaseMaterial3D,ShaderMaterial", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_DEFERRED_SET_RESOURCE), "set_material_override", "get_material_override");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transparency", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_transparency", "get_transparency");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cast_shadow", PROPERTY_HINT_ENUM, "Off,On,Double-Sided,Shadows Only"), "set_cast_shadow_mode", "get_cast_shadow_mode");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lod_bias", PROPERTY_HINT_RANGE, "0.001,128,0.001"), "set_lod_bias", "get_lod_bias");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignore_occlusion_culling"), "set_ignore_occlusion_culling", "is_ignoring_occlusion_culling");

	ADD_GROUP("Global Illumination", "gi_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gi_mode", PROPERTY_HINT_ENUM, "Disabled,Baked,Dynamic"), "set_gi_mode", "get_gi_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gi_lightmap_scale", PROPERTY_HINT_ENUM, String::utf8("1×,2×,4×,8×")), "set_gi_lightmap_scale", "get_gi_lightmap_scale");

	ADD_GROUP("Visibility Range", "visibility_range_");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility_range_begin", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_visibility_range_begin", "get_visibility_range_begin");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility_range_begin_margin", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_visibility_range_begin_margin", "get_visibility_range_begin_margin");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility_range_end", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_visibility_range_end", "get_visibility_range_end");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "visibility_range_end_margin", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_visibility_range_end_margin", "get_visibility_range_end_margin");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "visibility_range_fade_mode", PROPERTY_HINT_ENUM, "Disabled,Self,Dependencies"), "set_visibility_range_fade_mode", "get_visibility_range_fade_mode");

	ADD_GROUP("Miscellaneous", "");
	ADD_ARRAY_COUNT("Physics", "physics_data_count", "set_physics_data_count", "get_physics_data_count", "physics_data_");
	ADD_ARRAY_COUNT("Navigation", "navigation_data_count", "set_navigation_data_count", "get_navigation_data_count", "navigation_data_");

	BIND_ENUM_CONSTANT(LIGHT_TYPE_OMNI);
	BIND_ENUM_CONSTANT(LIGHT_TYPE_SPOT);
}


/******* TileData3DScene *******/
/*******************************/

void TileData3DScene::set_scene(const Ref<PackedScene> &p_scene) {
	scene = p_scene;
	_queue_changed();
}

Ref<PackedScene> TileData3DScene::get_scene() const {
	return scene;
}

void TileData3DScene::set_display_placeholder(bool p_display) {
	display_placeholder = p_display;
	_queue_changed();
}

bool TileData3DScene::is_displaying_placeholder() const {
	return display_placeholder;
}

void TileData3DScene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_scene", "scene"), &TileData3DScene::set_scene);
	ClassDB::bind_method(D_METHOD("get_scene"), &TileData3DScene::get_scene);
	ClassDB::bind_method(D_METHOD("set_display_placeholder", "display"), &TileData3DScene::set_display_placeholder);
	ClassDB::bind_method(D_METHOD("get_display_placeholder"), &TileData3DScene::is_displaying_placeholder);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "display_placeholder"), "set_display_placeholder", "is_displaying_placeholder");
}

/******* TileSet3DCollection *******/
/***********************************/

bool TileSet3DCollection::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "type") {
		int i = p_value;
		type = CollectionType(i);
		return true;
	} else if (p_name == "icon") {
		icon = p_value;
		return true;
	} else if (p_name == "tiles_count") {
		ids.resize(p_value);
		return true;
	} else {
		Vector<String> components = String(p_name).split("/", true, 2);
		if (components.size() == 2) {
			if (components[0].begins_with("tile_") && components[0].trim_prefix("tile_").is_valid_int()) {
				int index = components[0].trim_prefix("tile_").to_int();
				ERR_FAIL_INDEX_V(index, ids.size(), false);
				if (components[1] == "id") {
					ids.write[index] = p_value;
					return true;
				} else if (components[1] == "data") {
					int id = ids[index];
					Ref<TileData3D> data = Object::cast_to<TileData3D>(p_value);
					tiles[id] = data;
					return true;
				}
			} else if (components[0].begins_with("alternative_") && components[0].trim_prefix("alternative_").is_valid_int()) {
				int id = components[0].trim_prefix("alternative_").to_int();
				ERR_FAIL_COND_V(!alternatives.has(id), false);
				if (components[1] == "alternatives_count") {
					alternatives[id]->ids.resize(p_value);
					return true;
				} else if (components[1].begins_with("alt_")) {
					Vector<String> parts = components[1].split("/", true, 2);
					if (parts.size() == 2 && parts[0].trim_prefix("alt_").is_valid_int()) {
						int index = parts[0].trim_prefix("alt_").to_int();
						TileSet3DTileAlternatives *alternative = alternatives[id];
						ERR_FAIL_INDEX_V(index, alternative->ids.size(), false);
						if (parts[1] == "id") {
							alternative->ids.write[index] = p_value;
							return true;
						} else if (parts[1] == "data") {
							int alt_id = alternative->ids[index];
							Ref<TileData3D> data = Object::cast_to<TileData3D>(p_value);
							alternative->tiles[alt_id] = data;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool TileSet3DCollection::_get(const StringName &p_name, Variant &r_ret) const {
	if (p_name == "type") {
		r_ret = type;
		return true;
	} else if (p_name == "icon") {
		r_ret = icon;
		return true;
	} else if (p_name == "tiles_count") {
		r_ret = ids.size();
		return true;
	} else {
		Vector<String> components = String(p_name).split("/", true, 2);
		if (components.size() == 2) {
			if (components[0].begins_with("tile_") && components[0].trim_prefix("tile_").is_valid_int()) {
				int index = components[0].trim_prefix("tile_").to_int();
				ERR_FAIL_INDEX_V(index, ids.size(), false);
				int id = ids[index];
				if (components[1] == "id") {
					r_ret = id;
					return true;
				} else if (components[1] == "data") {
					r_ret = tiles[id];
					return true;
				}
			} else if (components[0].begins_with("alternative_") && components[0].trim_prefix("alternative_").is_valid_int()) {
				int id = components[0].trim_prefix("alternative_").to_int();
				ERR_FAIL_COND_V(!alternatives.has(id), false);
				if (components[1] == "alternatives_count") {
					r_ret = alternatives[id]->ids.size();
					return true;
				} else if (components[1].begins_with("alt_")) {
					Vector<String> parts = components[1].split("/", true, 2);
					if (parts.size() == 2 && parts[0].trim_prefix("alt_").is_valid_int()) {
						int index = parts[0].trim_prefix("alt_").to_int();
						TileSet3DTileAlternatives *alternative = alternatives[id];
						ERR_FAIL_INDEX_V(index, alternative->ids.size(), false);
						int alt_id = alternative->ids[index];
						if (parts[1] == "id") {
							r_ret = alt_id;
							return true;
						} else if (parts[1] == "data") {
							r_ret = alternative->tiles[alt_id];
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void TileSet3DCollection::_get_property_list(List<PropertyInfo> *p_list) const {
	PropertyInfo p = PropertyInfo(Variant::INT, "type");
	p.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(p);
	p = PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D");
	p.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(p);
	p = PropertyInfo(Variant::INT, "tiles_count");
	p.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(p);
	for (int i = 0; i < ids.size(); i++) {
		String prefix = vformat("tile_%d/", i);
		p = PropertyInfo(Variant::INT, prefix + "id");
		p.usage = PROPERTY_USAGE_NO_EDITOR;
		p_list->push_back(p);
		p = PropertyInfo(Variant::OBJECT, prefix + "data", PROPERTY_HINT_RESOURCE_TYPE, "TileData3D");
		p.usage = PROPERTY_USAGE_NO_EDITOR;
		p_list->push_back(p);
	}
	for (Map<int, TileSet3DTileAlternatives*>::Element *E = alternatives.front(); E; E->next()) {
		int id = E->key();
		TileSet3DTileAlternatives *alternative = E->get();
		String prefix = vformat("alternative_%d/", id);
		p = PropertyInfo(Variant::INT, prefix + "alternatives_count");
		p.usage = PROPERTY_USAGE_NO_EDITOR;
		p_list->push_back(p);
		for (int i = 0; i < alternative->ids.size(); i++) {
			String alt_prefix = prefix + vformat("alt_%d/", i);
			p = PropertyInfo(Variant::INT, alt_prefix + "id");
			p.usage = PROPERTY_USAGE_NO_EDITOR;
			p_list->push_back(p);
			p = PropertyInfo(Variant::OBJECT, alt_prefix + "data", PROPERTY_HINT_RESOURCE_TYPE, "TileData3D");
			p.usage = PROPERTY_USAGE_NO_EDITOR;
			p_list->push_back(p);
		}
	}
}

TileSet3DCollection::TileSet3DCollection() {
}

TileSet3DCollection::~TileSet3DCollection() {
	for (Map<int, TileSet3DTileAlternatives*>::Element *E = alternatives.front(); E; E->next()) {
		memdelete(E->get());
	}
}

/******* TileSet3D *******/
/*************************/

#ifndef INT_POS_SIGN
#define INT_POS_SIGN(m_v) (((m_v) == 0) ? (1) : (((m_v) < 0) ? (-1) : (1)))
#endif

void TileSet3D::_set_cached_collection(int p_id) const {
	if (_cached_id != p_id) {
		const Map<int, Ref<TileSet3DCollection>>::Element *e = collections.find(p_id);
		if (e) {
			_cached_collection = e->get();
			_cached_id = p_id;
		} else {
			_cached_collection.unref();
			_cached_id = -1;
		}
	}
}

void TileSet3D::_set_collection_count(int p_count) {
	collections_ids.resize(p_count);
}

void TileSet3D::_queue_changed() {
	if (_changed_requested) {
		return;
	}

	MessageQueue::get_singleton()->push_callable(callable_mp(this, &TileSet3D::_update_changed));
	_changed_requested = true;
}

void TileSet3D::_queue_collection_changed(int p_idx) {
	if (_changed_collections.has(p_idx)) {
		return;
	}

	if (_changed_collections.size() == 0) {
		MessageQueue::get_singleton()->push_callable(callable_mp(this, &TileSet3D::_update_collection_changed));
	}

	_changed_collections.insert(p_idx);
}

void TileSet3D::_update_changed() {
	emit_changed();
	_changed_requested = false;
}

void TileSet3D::_update_collection_changed() {
	for (Set<int>::Element *E = _changed_collections.front(); E; E = E->next()) {
		emit_signal(SNAME("collection_changed"), E->get());
	}
	_changed_collections.clear();
}

void TileSet3D::set_tile_shape(TileShape p_shape) {
	if (p_shape != tile_shape) {
		tile_shape = p_shape;
		_queue_changed();
	}
}

TileSet3D::TileShape TileSet3D::get_tile_shape() const {
	return tile_shape;
}

void TileSet3D::set_cell_size(const Vector3 &p_size) {
	if (cell_size != p_size) {
		cell_size = p_size;
		_queue_changed();
	}
}

Vector3 TileSet3D::get_cell_size() const {
	return cell_size;
}

void TileSet3D::set_cell_center_x(bool p_center) {
	if (cell_center_x != p_center) {
		cell_center_x = p_center;
		_queue_changed();
	}
}

bool TileSet3D::is_cell_centered_x() const {
	return cell_center_x;
}

void TileSet3D::set_cell_center_y(bool p_center) {
	if (cell_center_y != p_center) {
		cell_center_y = p_center;
		_queue_changed();
	}
}

bool TileSet3D::is_cell_centered_y() const {
	return cell_center_y;
}

void TileSet3D::set_cell_center_z(bool p_center) {
	if (cell_center_z != p_center) {
		cell_center_z = p_center;
		_queue_changed();
	}
}

bool TileSet3D::is_cell_centered_z() const {
	return cell_center_z;
}

void TileSet3D::set_main_axis(MainAxis p_main) {
	if (axis_main != p_main) {
		axis_main = p_main;
		_queue_changed();
	}
}

TileSet3D::MainAxis TileSet3D::get_main_axis() const {
	return axis_main;
}

void TileSet3D::set_axis_invert_x(bool p_invert) {
	if (axis_invert_x != p_invert) {
		axis_invert_x = p_invert;
		_queue_changed();
	}
}

bool TileSet3D::is_axis_inverted_x() const {
	return axis_invert_x;
}

void TileSet3D::set_axis_invert_y(bool p_invert) {
	if (axis_invert_y != p_invert) {
		axis_invert_y = p_invert;
		_queue_changed();
	}
}

bool TileSet3D::is_axis_inverted_y() const {
	return axis_invert_y;
}

void TileSet3D::set_axis_invert_z(bool p_invert) {
	if (axis_invert_z != p_invert) {
		axis_invert_z = p_invert;
		_queue_changed();
	}
}

bool TileSet3D::is_axis_inverted_z() const {
	return axis_invert_z;
}

void TileSet3D::clear() {
	_cached_id = -1;
	_cached_collection.unref();
	collections.clear();
	collections_ids.resize(0);

	_queue_changed();
}

int TileSet3D::get_collection_count() const {
	return collections_ids.size();
}

void TileSet3D::add_collection(const Ref<TileSet3DCollection> &p_collection, int p_id, int p_pos) {
	int pos = p_pos < 0 || p_pos > collections_ids.size() ? collections_ids.size() : p_pos;
	if (collections.has(p_id)) {
		collections_ids.erase(p_id);
	}
	collections_ids.insert(pos, p_id);
	collections[p_id] = p_collection;
	_queue_changed();
}

int TileSet3D::create_collection(TileSet3DCollection::CollectionType p_type, int p_id_override) {
	int id = p_id_override >= 0 ? p_id_override : get_next_collection_id();
	Ref<TileSet3DCollection> new_collection = memnew(TileSet3DCollection);
	new_collection->type = p_type;
	collections[id] = new_collection;
	collections_ids.push_back(id);
	_cached_id = id;
	_cached_collection = new_collection;
	_queue_changed();
	return id;
}

int TileSet3D::get_next_collection_id(int p_initial, int p_inc) const {
	int id = p_initial >= 0 ? p_initial : collections.size();
	int inc = INT_POS_SIGN(p_inc);
	while (collections.has(id)) {
		id = (id + inc) % 1073741824; // 2 ** 30
		if (id < 0) {
			id = collections.size();
			inc = 1;
		}
	}
	return id;
}

void TileSet3D::remove_collection(int p_id) {
	int index = -1;
	for (int i = 0; i < collections_ids.size(); i++) {
		if (collections_ids[i] == p_id) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		if (_cached_id == p_id) {
			_cached_id = -1;
			_cached_collection.unref();
		}
		collections.erase(p_id);
		collections_ids.remove_at(index);
	}

	_queue_changed();
}

Ref<TileSet3DCollection> TileSet3D::get_collection(int p_id) const {
	ERR_FAIL_COND_V(!collections.has(p_id), Ref<TileSet3DCollection>());
	return collections[p_id];
}

int TileSet3D::get_collection_index(int p_id) const {
	return collections_ids.find(p_id);
}

void TileSet3D::set_collection_name(int p_id, const String &p_name) {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL(_cached_collection);
	_cached_collection->set_name(p_name);
	_queue_collection_changed(p_id);
}

String TileSet3D::get_collection_name(int p_id) const {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL_V(_cached_collection, "");
	return _cached_collection->get_name();
}

void TileSet3D::set_collection_id(int p_index, int p_new_id, bool p_force) {
	ERR_FAIL_INDEX(p_index, collections_ids.size());

	int id = collections_ids[p_index];
	if (id == p_new_id) {
		return;
	}

	_set_cached_collection(id);
	ERR_FAIL_NULL(_cached_collection);
	collections.erase(id);
	if (p_force) {
		id = p_new_id;
		collections[id] = _cached_collection;
		_cached_id = id;
	} else {
		int inc = p_new_id > id ? 1 : -1;
		id = get_next_collection_id(p_new_id, inc);
		collections[id] = _cached_collection;
		_cached_id = id;
	}
	collections_ids.write[p_index] = id;
	_queue_changed();
}

int TileSet3D::get_collection_id(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, collections_ids.size(), -1);
	return collections_ids[p_index];
}

bool TileSet3D::has_collection_id(int p_id) const {
	return collections.has(p_id);
}

TileSet3DCollection::CollectionType TileSet3D::get_collection_type(int p_id) const {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL_V(_cached_collection, TileSet3DCollection::COLLECTION_TYPE_ATLAS);
	return _cached_collection->type;
}

void TileSet3D::set_collection_icon(int p_id, const Ref<Texture2D> &p_icon) {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL(_cached_collection);
	_cached_collection->icon = p_icon;
	_cached_collection->emit_changed();
	_queue_collection_changed(p_id);
}

Ref<Texture2D> TileSet3D::get_collection_icon(int p_id) const {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL_V(_cached_collection, Ref<Texture2D>());
	return _cached_collection->icon;
}

int TileSet3D::get_collection_tile_count(int p_id, bool p_include_alternatives) const {
	_set_cached_collection(p_id);
	ERR_FAIL_NULL_V(_cached_collection, 0);

	if (p_include_alternatives) {
		int count = _cached_collection->ids.size();
		for (Map<int, TileSet3DCollection::TileSet3DTileAlternatives*>::Element *E=_cached_collection->alternatives.front(); E; E = E->next()) {
			count += E->get()->ids.size();
		}
		return count;
	} else {
		return _cached_collection->ids.size();
	}
}

int TileSet3D::add_collection_tile(int p_collection_id, Ref<TileData3D> p_tile, int p_id_override, int p_pos) {
	int id = p_id_override >= 0 ? p_id_override : get_next_tile_id(p_collection_id);
	int pos = p_pos < 0 || p_pos > _cached_collection->ids.size() ? _cached_collection->ids.size() : p_pos;

	ERR_FAIL_NULL_V(_cached_collection, -1);
	ERR_FAIL_COND_V_MSG((p_tile->is_class("TileData3DMesh") && _cached_collection->type == TileSet3DCollection::COLLECTION_TYPE_SCENES) || \
			(p_tile->is_class("TileData3DScene") && _cached_collection->type == TileSet3DCollection::COLLECTION_TYPE_ATLAS), -1, "Incorret tile class for specified collection.");

	if (_cached_collection->tiles.has(id)) {
		_cached_collection->ids.erase(id);
	}

	_cached_collection->ids.insert(pos, id);
	_cached_collection->tiles[id] = p_tile;
	_queue_collection_changed(p_collection_id);
	return id;
}

Ref<TileData3D> TileSet3D::get_collection_tile(int p_collection_id, int p_tile_id, int p_alt_id) const {
	_set_cached_collection(p_collection_id);
	ERR_FAIL_NULL_V(_cached_collection, nullptr);

	if (p_alt_id > 0) {
		ERR_FAIL_COND_V(!_cached_collection->alternatives.has(p_tile_id), nullptr);
		ERR_FAIL_COND_V(!_cached_collection->alternatives[p_tile_id]->tiles.has(p_alt_id), nullptr);
		return _cached_collection->alternatives[p_tile_id]->tiles[p_alt_id];
	} else {
		ERR_FAIL_COND_V(!_cached_collection->tiles.has(p_tile_id), nullptr);
		return _cached_collection->tiles[p_tile_id];
	}
}

int TileSet3D::get_collection_tile_id(int p_collection_id, int p_tile_index) {
	_set_cached_collection(p_collection_id);
	ERR_FAIL_NULL_V(_cached_collection, -1);

	return _cached_collection->ids[p_tile_index];
}

int TileSet3D::add_collection_alternative_tile(int p_collection_id, int p_base_id, Ref<TileData3D> p_tile, int p_id_override, int p_pos) {
	int id = p_id_override > 0 ? p_id_override : get_next_alternative_tile_id(p_collection_id, p_base_id);

	ERR_FAIL_NULL_V(_cached_collection, -1);

	if (!_cached_collection->alternatives.has(p_base_id)) {
		_cached_collection->alternatives[p_base_id] = memnew(TileSet3DCollection::TileSet3DTileAlternatives);
	}

	TileSet3DCollection::TileSet3DTileAlternatives *alternatives = _cached_collection->alternatives[p_base_id];
	int pos = p_pos < 0 || p_pos > alternatives->ids.size() ? alternatives->ids.size() : p_pos;

	if (alternatives->tiles.has(id)) {
		alternatives->ids.erase(id);
	}

	alternatives->ids.insert(pos, id);
	alternatives->tiles[id] = p_tile;
	_queue_collection_changed(p_collection_id);
	return id;
}

int TileSet3D::get_next_tile_id(int p_collection_id, int p_initial, int p_inc) const {
	_set_cached_collection(p_collection_id);
	ERR_FAIL_NULL_V(_cached_collection, -1);

	int id = p_initial >= 0 ? p_initial : _cached_collection->ids.size();
	int inc = INT_POS_SIGN(p_inc);
	while (_cached_collection->tiles.has(id)) {
		id = (id + inc) % 1073741824; // 2 ** 30
		if (id < 0) {
			id = _cached_collection->ids.size();
			inc = 1;
		}
	}
	return id;
}

int TileSet3D::get_next_alternative_tile_id(int p_collection_id, int p_base_id, int p_initial, int p_inc) const {
	_set_cached_collection(p_collection_id);
	ERR_FAIL_NULL_V(_cached_collection, -1);

	TileSet3DCollection::TileSet3DTileAlternatives *alternatives = nullptr;
	const Map<int, TileSet3DCollection::TileSet3DTileAlternatives*>::Element *e = _cached_collection->alternatives.find(p_base_id);
	if (e) {
		alternatives = e->get();
	} else {
		return 1;
	}

	int id = p_initial > 0 ? p_initial : alternatives->ids.size() + 1;
	int inc = INT_POS_SIGN(p_inc);

	while (alternatives->tiles.has(id)) {
		id = (id + inc) % 1073741824; // 2 ** 30
		if (id < 1) {
			id = alternatives->ids.size() + 1;
			inc = 1;
		}
	}
	return id;
}

int TileSet3D::get_collection_alternatives_count(int p_collection_id, int p_base_id) {
	_set_cached_collection(p_collection_id);
	ERR_FAIL_NULL_V(_cached_collection, 0);

	const Map<int, TileSet3DCollection::TileSet3DTileAlternatives*>::Element *e = _cached_collection->alternatives.find(p_base_id);
	if (e) {
		TileSet3DCollection::TileSet3DTileAlternatives *alternatives = e->get();
		return alternatives->ids.size();
	} else {
		return 0;
	}
}

bool TileSet3D::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "collections_count") {
		_set_collection_count(p_value);
		return true;
	} else {
		Vector<String> components = String(p_name).split("/", true, 2);
		if (components.size() == 2) {
			if (components[0].begins_with("collection_") && components[0].trim_prefix("collection_").is_valid_int()) {
				int index = components[0].trim_prefix("collection_").to_int();
				ERR_FAIL_INDEX_V(index, collections_ids.size(), false);
				if (components[1] == "id") {
					collections_ids.write[index] = p_value;
					return true;
				} else if (components[1] == "collection") {
					int id = collections_ids[index];
					Ref<TileSet3DCollection> collection = Object::cast_to<TileSet3DCollection>(p_value);
					collections[id] = collection;
					return true;
				}
			}
		}
	}
	return false;
}

bool TileSet3D::_get(const StringName &p_name, Variant &r_ret) const {
	if (p_name == "collections_count") {
		r_ret = get_collection_count();
		return true;
	} else {
		Vector<String> components = String(p_name).split("/", true, 2);
		if (components.size() == 2) {
			if (components[0].begins_with("collection_") && components[0].trim_prefix("collection_").is_valid_int()) {
				int index = components[0].trim_prefix("collection_").to_int();
				ERR_FAIL_INDEX_V(index, collections_ids.size(), false);
				int id = collections_ids[index];
				if (components[1] == "id") {
					r_ret = id;
					return true;
				} else if (components[1] == "collection") {
					r_ret = collections[id];
					return true;
				}
			}
		}
	}
	return false;
}

void TileSet3D::_get_property_list(List<PropertyInfo> *p_list) const {
	PropertyInfo p = PropertyInfo(Variant::INT, "collections_count");
	p.usage = PROPERTY_USAGE_NO_EDITOR;
	p_list->push_back(p);
	for (int i = 0; i < collections_ids.size(); i++) {
		p = PropertyInfo(Variant::INT, vformat("collection_%d/id", i));
		p.usage = PROPERTY_USAGE_NO_EDITOR;
		p_list->push_back(p);
		p = PropertyInfo(Variant::OBJECT, vformat("collection_%d/collection", i), PROPERTY_HINT_RESOURCE_TYPE, "TileSet3DCollection");
		p.usage = PROPERTY_USAGE_NO_EDITOR;
		p_list->push_back(p);
	}
}

void TileSet3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_tile_shape", "shape"), &TileSet3D::set_tile_shape);
	ClassDB::bind_method(D_METHOD("get_tile_shape"), &TileSet3D::get_tile_shape);
	ClassDB::bind_method(D_METHOD("set_cell_size", "size"), &TileSet3D::set_cell_size);
	ClassDB::bind_method(D_METHOD("get_cell_size"), &TileSet3D::get_cell_size);
	ClassDB::bind_method(D_METHOD("set_cell_center_x", "center"), &TileSet3D::set_cell_center_x);
	ClassDB::bind_method(D_METHOD("is_cell_centered_x"), &TileSet3D::is_cell_centered_x);
	ClassDB::bind_method(D_METHOD("set_cell_center_y", "center"), &TileSet3D::set_cell_center_x);
	ClassDB::bind_method(D_METHOD("is_cell_centered_y"), &TileSet3D::is_cell_centered_y);
	ClassDB::bind_method(D_METHOD("set_cell_center_z", "center"), &TileSet3D::set_cell_center_x);
	ClassDB::bind_method(D_METHOD("is_cell_centered_z"), &TileSet3D::is_cell_centered_z);

	ClassDB::bind_method(D_METHOD("set_main_axis", "main"), &TileSet3D::set_main_axis);
	ClassDB::bind_method(D_METHOD("get_main_axis"), &TileSet3D::get_main_axis);
	ClassDB::bind_method(D_METHOD("set_axis_invert_x", "invert"), &TileSet3D::set_axis_invert_x);
	ClassDB::bind_method(D_METHOD("is_axis_inverted_x"), &TileSet3D::is_axis_inverted_x);
	ClassDB::bind_method(D_METHOD("set_axis_invert_y", "invert"), &TileSet3D::set_axis_invert_y);
	ClassDB::bind_method(D_METHOD("is_axis_inverted_y"), &TileSet3D::is_axis_inverted_y);
	ClassDB::bind_method(D_METHOD("set_axis_invert_z", "invert"), &TileSet3D::set_axis_invert_z);
	ClassDB::bind_method(D_METHOD("is_axis_inverted_z"), &TileSet3D::is_axis_inverted_z);

	ClassDB::bind_method(D_METHOD("clear"), &TileSet3D::clear);
	ClassDB::bind_method(D_METHOD("get_collection_count"), &TileSet3D::get_collection_count);
	ClassDB::bind_method(D_METHOD("add_collection", "collection", "id", "pos"), &TileSet3D::add_collection, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("create_collection", "type", "id_override"), &TileSet3D::create_collection, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_next_collection_id"), &TileSet3D::get_next_collection_id, DEFVAL(-1), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("remove_collection", "id"), &TileSet3D::remove_collection);
	ClassDB::bind_method(D_METHOD("get_collection"), &TileSet3D::get_collection);
	ClassDB::bind_method(D_METHOD("set_collection_name", "id", "name"), &TileSet3D::set_collection_name);
	ClassDB::bind_method(D_METHOD("get_collection_name"), &TileSet3D::get_collection_name);
	ClassDB::bind_method(D_METHOD("set_collection_id", "index", "new_id", "force"), &TileSet3D::set_collection_id, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("get_collection_id", "index"), &TileSet3D::get_collection_id);
	ClassDB::bind_method(D_METHOD("get_collection_type", "id"), &TileSet3D::get_collection_type);
	ClassDB::bind_method(D_METHOD("set_collection_icon", "id", "icon"), &TileSet3D::set_collection_icon);
	ClassDB::bind_method(D_METHOD("get_collection_icon", "id"), &TileSet3D::get_collection_icon);
	ClassDB::bind_method(D_METHOD("get_collection_tile_count", "id", "include_alternatives"), &TileSet3D::get_collection_tile_count, DEFVAL(false));

	ClassDB::bind_method(D_METHOD("add_collection_tile", "collection_id", "tile", "id_override", "pos"), &TileSet3D::add_collection_tile, DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_collection_tile", "collection_id", "tile_id", "alt_id"), &TileSet3D::get_collection_tile, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("add_collection_alternative_tile", "collection_id", "base_id", "tile", "id_override", "pos"), &TileSet3D::add_collection_alternative_tile, DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_next_tile_id", "collection_id", "initial", "inc"), &TileSet3D::get_next_tile_id, DEFVAL(-1), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_next_alternative_tile_id", "collection_id", "base_id", "initial", "inc"), &TileSet3D::get_next_alternative_tile_id, DEFVAL(-1), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_collection_alternatives_count", "collection_id", "base_id"), &TileSet3D::get_collection_alternatives_count);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_shape", PROPERTY_HINT_ENUM, "Cuboid,Hexagonal Prism"), "set_tile_shape", "get_tile_shape");

	ADD_GROUP("Cell", "cell_");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "cell_size"), "set_cell_size", "get_cell_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cell_center_x"), "set_cell_center_x", "is_cell_centered_x");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cell_center_y"), "set_cell_center_y", "is_cell_centered_y");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cell_center_z"), "set_cell_center_z", "is_cell_centered_z");

	ADD_GROUP("Axes", "axis_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis_main", PROPERTY_HINT_ENUM, "X Axis,Y Axis,Z Axis"), "set_main_axis", "get_main_axis");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "axis_invert_x"), "set_axis_invert_x", "is_axis_inverted_x");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "axis_invert_y"), "set_axis_invert_y", "is_axis_inverted_y");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "axis_invert_z"), "set_axis_invert_z", "is_axis_inverted_z");

	ADD_SIGNAL(MethodInfo("collection_changed", PropertyInfo(Variant::INT, "collection_idx")));

	BIND_ENUM_CONSTANT(TILE_SHAPE_CUBOID);
	BIND_ENUM_CONSTANT(TILE_SHAPE_HEXAGONAL_PRISM);

	BIND_ENUM_CONSTANT(MAIN_AXIS_X);
	BIND_ENUM_CONSTANT(MAIN_AXIS_Y);
	BIND_ENUM_CONSTANT(MAIN_AXIS_Z);
}

TileSet3D::TileSet3D() {
}

TileSet3D::~TileSet3D() {
}

#undef INT_POS_SIGN

