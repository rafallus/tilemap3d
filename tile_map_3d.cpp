/*************************************************************************/
/*  tile_map_3d.cpp                                                      */
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
#include "tile_map_3d.h"

void TileMap3D::_queue_octants_dirty() {
	if (awaiting_update) {
		return;
	}

	MessageQueue::get_singleton()->push_callable(callable_mp(this, &TileMap3D::_update_octants_callback));
	awaiting_update = true;
}

void TileMap3D::_recreate_octant_data() {
	_clear_octants();
	for (int i = 0; i < layers.size(); i++) {
		const Map<MapCell, MapTile> &tile_map = layers[i].tile_map;
		for (const KeyValue<MapCell, MapTile> &E : tile_map) {
			OctantKey ok = _cell_to_octant(E.key);
			_insert_octant_cell(ok, E.key);
		}
	}
	_queue_octants_dirty();
}

void TileMap3D::_update_octants_callback() {
	ERR_FAIL_NULL(tile_set);
	if (!awaiting_update) {
		return;
	}

	List<OctantKey> to_delete;
	for (const KeyValue<OctantKey, Octant *> &E : octant_map) {
		if (_octant_update(E.value)) {
			to_delete.push_back(E.key);
		}
	}

	while (to_delete.front()) {
		OctantKey ok = to_delete.front()->get();
		// Octant *oct = octant_map[ok];
		octant_map.erase(ok);
		to_delete.pop_front();
		// memdelete(oct); TODO: Check if needs to be deleted
	}

	_update_visibility();
	awaiting_update = false;
}

void TileMap3D::_update_visibility() {
	if (!is_inside_tree()) {
		return;
	}

	for (KeyValue<OctantKey, Octant *> &E : octant_map) {
		Octant *oct = E.value;
		for (int i = 0; i < oct->multimesh_instances.size(); i++) {
			const Octant::MultimeshInstance &mi = oct->multimesh_instances[i];
			RS::get_singleton()->instance_set_visible(mi.instance, is_visible_in_tree());
		}

	// 	if (octant->collision_debug_instance.is_valid()) {
	// 		RS::get_singleton()->instance_set_visible(octant->collision_debug_instance, is_visible_in_tree());
	// 	}

	// 	if (octant->area_debug_instance.is_valid()) {
	// 		RS::get_singleton()->instance_set_visible(octant->area_debug_instance, is_visible_in_tree());
	// 	}
	// }

	// for (int i = 0; i < baked_meshes.size(); i++) {
	// 	RS::get_singleton()->instance_set_visible(baked_meshes[i].instance, is_visible_in_tree());
	}
}

void TileMap3D::_clear_octants() {
	for (const KeyValue<OctantKey, Octant *> &E : octant_map) {
		if (is_inside_world()) {
			_octant_exit_world(E.value);
		}

		_octant_clean_up(E.value);
		memdelete(E.value);
	}

	octant_map.clear();
}

void TileMap3D::_octant_clean_up(Octant *p_oct) {
	// Erase multimeshes
	for (int i = 0; i < p_oct->multimesh_instances.size(); i++) {
		RS::get_singleton()->free(p_oct->multimesh_instances[i].instance);
		RS::get_singleton()->free(p_oct->multimesh_instances[i].multimesh);
	}
	p_oct->multimesh_instances.clear();

	// Erase body shapes

	// Erase body shapes debug

	// Erase navigation
}

void TileMap3D::_octant_enter_world(Octant *p_oct) {
	for (int i = 0; i < p_oct->multimesh_instances.size(); i++) {
		RID instance = p_oct->multimesh_instances[i].instance;
		RS::get_singleton()->instance_set_scenario(instance, get_world_3d()->get_scenario());
		RS::get_singleton()->instance_set_transform(instance, get_global_transform());
	}

	// PhysicsServer3D::get_singleton()->body_set_state(g.static_body, PhysicsServer3D::BODY_STATE_TRANSFORM, get_global_transform());
	// PhysicsServer3D::get_singleton()->body_set_space(g.static_body, get_world_3d()->get_space());

	// if (area_enabled && g.area.is_valid()) {
	// 	PhysicsServer3D::get_singleton()->area_set_transform(g.area->rid, get_global_transform());
	// 	PhysicsServer3D::get_singleton()->area_set_space(g.area->rid, get_world_3d()->get_space());
	// }

	// if (g.collision_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_scenario(g.collision_debug_instance, get_world_3d()->get_scenario());
	// 	RS::get_singleton()->instance_set_transform(g.collision_debug_instance, get_global_transform());
	// }

	// if (g.area_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_scenario(g.area_debug_instance, get_world_3d()->get_scenario());
	// 	RS::get_singleton()->instance_set_transform(g.area_debug_instance, get_global_transform());
	// }

	// if (bake_navigation && mesh_library.is_valid()) {
	// 	for (KeyValue<IndexKey, Octant::NavMesh> &F : g.navmesh_ids) {
	// 		if (cell_map.has(F.key) && F.value.region.is_valid() == false) {
	// 			Ref<NavigationMesh> nm = mesh_library->get_item_navmesh(cell_map[F.key].item);
	// 			if (nm.is_valid()) {
	// 				RID region = NavigationServer3D::get_singleton()->region_create();
	// 				NavigationServer3D::get_singleton()->region_set_layers(region, navigation_layers);
	// 				NavigationServer3D::get_singleton()->region_set_navmesh(region, nm);
	// 				NavigationServer3D::get_singleton()->region_set_transform(region, get_global_transform() * F.value.xform);
	// 				NavigationServer3D::get_singleton()->region_set_map(region, get_world_3d()->get_navigation_map());

	// 				F.value.region = region;
	// 			}
	// 		}
	// 	}
	// }
}

void TileMap3D::_octant_exit_world(Octant *p_oct) {
	for (int i = 0; i < p_oct->multimesh_instances.size(); i++) {
		RS::get_singleton()->instance_set_scenario(p_oct->multimesh_instances[i].instance, RID());
	}

	// PhysicsServer3D::get_singleton()->body_set_state(g.static_body, PhysicsServer3D::BODY_STATE_TRANSFORM, get_global_transform());
	// PhysicsServer3D::get_singleton()->body_set_space(g.static_body, RID());

	// if (area_enabled && g.area.is_valid()) {
	// 	PhysicsServer3D::get_singleton()->area_set_space(g.area->rid, RID());
	// }

	// if (g.collision_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_scenario(g.collision_debug_instance, RID());
	// }

	// if (g.area_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_scenario(g.area_debug_instance, RID());
	// }

	// for (KeyValue<IndexKey, Octant::NavMesh> &F : g.navmesh_ids) {
	// 	if (F.value.region.is_valid()) {
	// 		NavigationServer3D::get_singleton()->free(F.value.region);
	// 		F.value.region = RID();
	// 	}
	// }
}

void TileMap3D::_octant_transform(Octant *p_oct) {
	for (int i = 0; i < p_oct->multimesh_instances.size(); i++) {
		RS::get_singleton()->instance_set_transform(p_oct->multimesh_instances[i].instance, get_global_transform());
	}

	// PhysicsServer3D::get_singleton()->body_set_state(g.static_body, PhysicsServer3D::BODY_STATE_TRANSFORM, get_global_transform());

	// if (area_enabled && g.area.is_valid()) {
	// 	PhysicsServer3D::get_singleton()->area_set_transform(g.area->rid, get_global_transform());
	// }

	// if (g.collision_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_transform(g.collision_debug_instance, get_global_transform());
	// }

	// if (g.area_debug_instance.is_valid()) {
	// 	RS::get_singleton()->instance_set_transform(g.area_debug_instance, get_global_transform());
	// }
}

bool TileMap3D::_octant_update(Octant *p_oct) {
	if (!p_oct->dirty) {
		return false;
	}

	_octant_clean_up(p_oct);

	if (p_oct->cells.size() == 0) {
		return true;
	}

	Map<MapTile::Tile, List<Pair<Transform3D, MapCell>>> multimesh_items;

	for (Set<MapCell>::Element *E = p_oct->cells.front(); E; E = E->next()) {
		const MapCell &cell = E->get();
		ERR_CONTINUE(cell.layer < 0 || cell.layer >= layers.size());

		const TileMapLayer &layer = layers[cell.layer];
		const Map<MapCell, MapTile>::Element *C = layer.tile_map.find(cell);
		ERR_CONTINUE(!C);

		const MapTile &mt = C->get();
		Ref<TileData3DMesh> data = tile_set->get_collection_tile(mt.tile.collection_id, mt.tile.tile_id, mt.tile.alternative_id);
		ERR_CONTINUE(data.is_null());

		Vector3 origin = _cell_to_world(cell);
		Transform3D transform = Transform3D(mt.rotation, origin);
		transform.basis.scale(Vector3(cell_scale, cell_scale, cell_scale));
		if (data->get_mesh().is_valid()) {
			Map<MapTile::Tile, List<Pair<Transform3D, MapCell>>>::Element *MME = multimesh_items.find(mt.tile);
			if (!MME) {
				MME = multimesh_items.insert(mt.tile, List<Pair<Transform3D, MapCell>>());
			}
			Pair<Transform3D, MapCell> p;
			p.first = transform * data->get_mesh_transform();
			p.second = cell;
			MME->get().push_back(p);
		}
	}

	RenderingServer *rs = RenderingServer::get_singleton();
	for (const KeyValue<MapTile::Tile, List<Pair<Transform3D, MapCell>>> &E : multimesh_items) {
		Octant::MultimeshInstance mmi;
		const MapTile::Tile &tile = E.key;
		Ref<TileData3DMesh> data = tile_set->get_collection_tile(tile.collection_id, tile.tile_id, tile.alternative_id);
		RID mm = rs->multimesh_create();
		rs->multimesh_allocate_data(mm, E.value.size(), RS::MULTIMESH_TRANSFORM_3D);
		rs->multimesh_set_mesh(mm, data->get_mesh()->get_rid());

		RID instance = rs->instance_create();
		rs->instance_set_base(instance, mm);

		int idx = 0;
		for (const Pair<Transform3D, MapCell> &F : E.value) {
			rs->multimesh_instance_set_transform(mm, idx, F.first);
			multimeshes[F.second] = instance;
			instance_indices[F.second] = idx;
			idx++;
		}

		if (is_inside_tree()) {
			rs->instance_set_scenario(instance, get_world_3d()->get_scenario());
			rs->instance_set_transform(instance, get_global_transform());
		}

		mmi.multimesh = mm;
		mmi.instance = instance;
		p_oct->multimesh_instances.push_back(mmi);
	}

	return false;
}

void TileMap3D::_insert_octant_cell(const OctantKey &p_ok, const MapCell &p_cell) {
	Map<OctantKey, Octant *>::Element *O = octant_map.find(p_ok);
	if (!O) {
		Octant *oct = memnew(Octant);

		O = octant_map.insert(p_ok, oct);
	}

	Octant &oct = *O->get();
	oct.cells.insert(p_cell);
	oct.dirty = true;
}

void TileMap3D::_mark_octants_as_dirty() {
	for (KeyValue<OctantKey, Octant *> &E : octant_map) {
		E.value->dirty = true;
	}
	_queue_octants_dirty();
}

TileMap3D::OctantKey TileMap3D::_cell_to_octant(const MapCell &p_cell) const {
	return OctantKey(
		p_cell.x / float(octant_size.x) + int(octant_center_x) * 0.5,
		p_cell.y / float(octant_size.y) + int(octant_center_y) * 0.5,
		p_cell.z / float(octant_size.z) + int(octant_center_z) * 0.5
	);
}

Vector3 TileMap3D::_cell_to_world(const MapCell &p_cell) const {
	Vector3 cell_size = tile_set->get_cell_size();
	return Vector3(
		cell_size.x * (p_cell.x + 0.5 * int(tile_set->is_cell_centered_x())),
		cell_size.y * (p_cell.y + 0.5 * int(tile_set->is_cell_centered_y())),
		cell_size.z * (p_cell.z + 0.5 * int(tile_set->is_cell_centered_z()))
	);
}

void TileMap3D::_clear_layers() {
	for (int i = 0; i < layers.size(); i++) {
		layers[i].tile_map.clear();
	}
}

void TileMap3D::set_tile_set(const Ref<TileSet3D> &p_set) {
	if (tile_set == p_set) {
        return;
    }
    if (tile_set.is_valid()) {
        tile_set->disconnect("changed", callable_mp(this, &TileMap3D::_queue_octants_dirty));
    }

    tile_set = p_set;
    if (tile_set.is_valid()) {
        tile_set->connect("changed", callable_mp(this, &TileMap3D::_queue_octants_dirty));
    }

	_mark_octants_as_dirty();
}

Ref<TileSet3D> TileMap3D::get_tile_set() const {
	return tile_set;
}

void TileMap3D::set_cell_scale(float p_scale) {
	cell_scale = p_scale;

	_mark_octants_as_dirty();
}

float TileMap3D::get_cell_scale() const {
	return cell_scale;
}

void TileMap3D::set_octant_size(const Vector3 &p_size) {
	ERR_FAIL_COND(p_size.x <= 0 || p_size.y <= 0 || p_size.z <= 0);
	octant_size = p_size;
	_recreate_octant_data();
}

Vector3 TileMap3D::get_octant_size() const {
	return octant_size;
}

void TileMap3D::set_octant_center_x(bool p_center) {
	octant_center_x = p_center;
	_recreate_octant_data();
}

bool TileMap3D::is_octant_centered_x() const {
	return octant_center_x;
}

void TileMap3D::set_octant_center_y(bool p_center) {
	octant_center_y = p_center;
	_recreate_octant_data();
}

bool TileMap3D::is_octant_centered_y() const {
	return octant_center_y;
}

void TileMap3D::set_octant_center_z(bool p_center) {
	octant_center_z = p_center;
	_recreate_octant_data();
}

bool TileMap3D::is_octant_centered_z() const {
	return octant_center_z;
}

void TileMap3D::clear() {
	_clear_octants();
	_clear_layers();
}

int TileMap3D::get_layers_count() const {
	return layers.size();
}

void TileMap3D::add_layer(int p_to_pos) {
	if (p_to_pos < 0) {
		p_to_pos = layers.size();
	}
	ERR_FAIL_INDEX(p_to_pos, (int)layers.size() + 1);

	layers.insert(p_to_pos, TileMapLayer());
	notify_property_list_changed();
}

void TileMap3D::move_layer(int p_layer, int p_to_pos) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	ERR_FAIL_INDEX(p_to_pos, layers.size() + 1);

	TileMapLayer tl = layers[p_layer];
	layers.insert(p_to_pos, tl);
	layers.remove_at(p_to_pos < p_layer ? p_layer + 1 : p_layer);
	notify_property_list_changed();
}

void TileMap3D::remove_layer(int p_layer) {
	ERR_FAIL_INDEX(p_layer, layers.size());

	layers.remove_at(p_layer);
	notify_property_list_changed();

	_queue_octants_dirty();
}

void TileMap3D::set_layer_name(int p_layer, String p_name) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	layers[p_layer].name = p_name;
}

String TileMap3D::get_layer_name(int p_layer) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), String());
	return layers[p_layer].name;
}

void TileMap3D::set_layer_enabled(int p_layer, bool p_visible) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	layers[p_layer].enabled = p_visible;
}

bool TileMap3D::is_layer_enabled(int p_layer) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), false);
	return layers[p_layer].enabled;
}

void TileMap3D::set_layer_material_override(int p_layer, const Ref<Material> &p_material_override) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	layers[p_layer].material_override = p_material_override;
}

Ref<Material> TileMap3D::get_layer_material_override(int p_layer) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), nullptr);
	return layers[p_layer].material_override;
}

void TileMap3D::set_layer_transparency(int p_layer, float p_transparency) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	layers[p_layer].transparency = p_transparency;
}

float TileMap3D::get_layer_transparency(int p_layer) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), 0.0);
	return layers[p_layer].transparency;
}

void TileMap3D::set_layer_render_layers_mask(int p_layer, uint32_t p_mask) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	layers[p_layer].render_layers = p_mask;
}

uint32_t TileMap3D::get_layer_render_layers_mask(int p_layer) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), 0);
	return layers[p_layer].render_layers;
}

void TileMap3D::set_layer_render_layer_mask_value(int p_layer, int p_render_layer_number, bool p_enable) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	ERR_FAIL_COND_MSG(p_render_layer_number < 1, "Render layer number must be between 1 and 20 inclusive.");
	ERR_FAIL_COND_MSG(p_render_layer_number > 20, "Render layer number must be between 1 and 20 inclusive.");
	uint32_t mask = layers[p_layer].render_layers;
	if (p_enable) {
		mask |= 1 << (p_render_layer_number - 1);
	} else {
		mask &= ~(1 << (p_render_layer_number - 1));
	}
	layers[p_layer].render_layers = mask;
}

bool TileMap3D::get_layer_render_layer_mask_value(int p_layer, int p_render_layer_number) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), false);
	ERR_FAIL_COND_V_MSG(p_render_layer_number < 1, false, "Render layer number must be between 1 and 20 inclusive.");
	ERR_FAIL_COND_V_MSG(p_render_layer_number > 20, false, "Render layer number must be between 1 and 20 inclusive.");
	return layers[p_layer].render_layers & (1 << (p_render_layer_number - 1));
}

void TileMap3D::set_cell(int p_layer, const Vector3i &p_position, int p_tile, int p_collection, int p_alternative, int p_rot_idx) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	ERR_FAIL_INDEX(ABS(p_position.x), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.y), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.z), (1 << 15) - 1);
	ERR_FAIL_COND(p_collection < 0 && p_tile >= 0);

	MapCell cell(p_position, p_layer);

	OctantKey ok = _cell_to_octant(cell);
	Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;

	if (p_tile < 0) {
		// Erase
		Map<MapCell, MapTile>::Element *E = tile_map.find(cell);
		if (E) {
			Map<OctantKey, Octant *>::Element *O = octant_map.find(ok);

			ERR_FAIL_NULL(O);
			Octant &oct = *O->get();
			oct.cells.erase(cell);
			oct.dirty = true;
			tile_map.erase(cell);
			_queue_octants_dirty();
		}
		return;
	}

	_insert_octant_cell(ok, cell);

	MapTile tile(p_collection, p_tile, p_alternative, p_layer);
	tile.set_ortho_rotation(p_rot_idx);
	tile_map[cell] = tile;

	_queue_octants_dirty();
}

int TileMap3D::get_cell_collection_id(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), -1);
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, -1);

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	if (!E) {
		return INVALID_ITEM;
	} else {
		return E->get().tile.collection_id;
	}
}

int TileMap3D::get_cell_tile_id(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), -1);
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, -1);

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	if (!E) {
		return INVALID_ITEM;
	} else {
		return E->get().tile.tile_id;
	}
}

int TileMap3D::get_cell_alternative_id(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), -1);
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, -1);

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	if (!E) {
		return INVALID_ITEM;
	} else {
		return E->get().tile.alternative_id;
	}
}

void TileMap3D::set_cell_orientation_index(int p_layer, const Vector3i &p_position, int p_rot_idx) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	ERR_FAIL_INDEX(ABS(p_position.x), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.y), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.z), (1 << 15) - 1);

	Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	ERR_FAIL_NULL_MSG(E, "No cell found with the given coordinates.");

	MapTile &t = E->get();
	t.set_ortho_rotation(p_rot_idx);
}

int TileMap3D::get_cell_closest_orientation_index(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), -1);
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, -1);
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, -1);

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	ERR_FAIL_NULL_V_MSG(E, -1, "No cell found with the given coordinates.");

	const MapTile &t = E->get();
	if (t.ortho_rot_idx != MapTile::NON_ORTHOGONAL_ROT) {
		return t.ortho_rot_idx;
	} else {
		return E->get().rotation.get_orthogonal_index();
	}
}

void TileMap3D::set_cell_rotation(int p_layer, const Vector3i &p_position, const Basis &p_rotation) {
	ERR_FAIL_INDEX(p_layer, layers.size());
	ERR_FAIL_INDEX(ABS(p_position.x), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.y), (1 << 15) - 1);
	ERR_FAIL_INDEX(ABS(p_position.z), (1 << 15) - 1);

	Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	ERR_FAIL_NULL_MSG(E, "No cell found with the given coordinates.");

	MapTile &t = E->get();
	t.set_rotation(p_rotation);
}

Basis TileMap3D::get_cell_rotation(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), Basis());
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, Basis());
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, Basis());
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, Basis());

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	ERR_FAIL_NULL_V_MSG(E, Basis(), "No cell found with the given coordinates.");

	return E->get().rotation;
}

bool TileMap3D::is_cell_rotation_orthogonal(int p_layer, const Vector3i &p_position) const {
	ERR_FAIL_INDEX_V(p_layer, layers.size(), false);
	ERR_FAIL_INDEX_V(ABS(p_position.x), (1 << 15) - 1, false);
	ERR_FAIL_INDEX_V(ABS(p_position.y), (1 << 15) - 1, false);
	ERR_FAIL_INDEX_V(ABS(p_position.z), (1 << 15) - 1, false);

	const Map<MapCell, MapTile> &tile_map = layers[p_layer].tile_map;
	MapCell cell(p_position, p_layer);
	const Map<MapCell, MapTile>::Element *E = tile_map.find(cell);

	ERR_FAIL_NULL_V_MSG(E, false, "No cell found with the given coordinates.");

	return E->get().ortho_rot_idx != MapTile::NON_ORTHOGONAL_ROT;
}

void TileMap3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_WORLD: {
			last_transform = get_global_transform();

			for (const KeyValue<OctantKey, Octant *> &E : octant_map) {
				_octant_enter_world(E.value);
			}
		} break;
		case NOTIFICATION_TRANSFORM_CHANGED: {
			Transform3D new_xform = get_global_transform();
			if (new_xform == last_transform) {
				break;
			}

			for (const KeyValue<OctantKey, Octant *> &E : octant_map) {
				_octant_transform(E.value);
			}

			last_transform = new_xform;
		} break;
		case NOTIFICATION_EXIT_WORLD: {
			for (const KeyValue<OctantKey, Octant *> &E : octant_map) {
				_octant_exit_world(E.value);
			}
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			_update_visibility();
		} break;
	}
}

// bool TileMap3D::_set(const StringName &p_name, const Variant &p_value) {
// }

// bool TileMap3D::_get(const StringName &p_name, Variant &r_ret) const {
// }

// void TileMap3D::_get_property_list(List<PropertyInfo> *p_list) const {
// }

void TileMap3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_tile_set", "tile_set"), &TileMap3D::set_tile_set);
	ClassDB::bind_method(D_METHOD("get_tile_set"), &TileMap3D::get_tile_set);
	ClassDB::bind_method(D_METHOD("set_cell_scale", "scale"), &TileMap3D::set_cell_scale);
	ClassDB::bind_method(D_METHOD("get_cell_scale"), &TileMap3D::get_cell_scale);
	ClassDB::bind_method(D_METHOD("set_octant_size", "size"), &TileMap3D::set_octant_size);
	ClassDB::bind_method(D_METHOD("get_octant_size"), &TileMap3D::get_octant_size);
	ClassDB::bind_method(D_METHOD("set_octant_center_x", "center"), &TileMap3D::set_octant_center_x);
	ClassDB::bind_method(D_METHOD("is_octant_centered_x"), &TileMap3D::is_octant_centered_x);
	ClassDB::bind_method(D_METHOD("set_octant_center_y", "center"), &TileMap3D::set_octant_center_y);
	ClassDB::bind_method(D_METHOD("is_octant_centered_y"), &TileMap3D::is_octant_centered_y);
	ClassDB::bind_method(D_METHOD("set_octant_center_z", "center"), &TileMap3D::set_octant_center_z);
	ClassDB::bind_method(D_METHOD("is_octant_centered_z"), &TileMap3D::is_octant_centered_z);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tile_set", PROPERTY_HINT_RESOURCE_TYPE, "TileSet3D"), "set_tile_set", "get_tile_set");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cell_scale"), "set_cell_scale", "get_cell_scale");
	ADD_GROUP("Octant", "octant_");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "octant_size"), "set_octant_size", "get_octant_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "octant_center_x"), "set_octant_center_x", "is_octant_centered_x");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "octant_center_y"), "set_octant_center_y", "is_octant_centered_y");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "octant_center_z"), "set_octant_center_z", "is_octant_centered_z");
}

TileMap3D::TileMap3D() {
	set_notify_transform(true);
}

TileMap3D::~TileMap3D() {
	clear();
}
