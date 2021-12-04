/*************************************************************************/
/*  tile_map_3d.h                                                        */
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

#ifndef TILE_MAP_3D_H
#define TILE_MAP_3D_H

#include "core/templates/local_vector.h"
#include "scene/3d/node_3d.h"
#include "scene/resources/multimesh.h"
#include "tile_set_3d.h"

class TileMap3D : public Node3D {
	GDCLASS(TileMap3D, Node3D);

private:

	Ref<TileSet3D> tile_set;
	float cell_scale = 1.0;
	Vector3 octant_size = Vector3(1, 1, 1);
	bool octant_center_x = true;
    bool octant_center_y = true;
    bool octant_center_z = true;

	struct MapTile {
		static const int NON_ORTHOGONAL_ROT = -1;

		union Tile {
			struct {
				int16_t collection_id : 16;
				int16_t tile_id : 16;
				int16_t alternative_id : 16;
				int16_t layer : 16;
			};
			uint64_t _u64t;

			_FORCE_INLINE_ bool operator<(const Tile &p_tile) const {
				return _u64t < p_tile._u64t;
			}

			Tile(int p_collection_id = -1, int p_tile_id = -1, int p_alternative_id = -1, int p_layer = -1) {
				collection_id = p_alternative_id;
				tile_id = p_tile_id;
				alternative_id = p_alternative_id;
				layer = p_layer;
			}
		};

		Tile tile;
		Basis rotation;
		int ortho_rot_idx;

		void operator=(const MapTile &t) {
			tile.collection_id = t.tile.collection_id;
			tile.tile_id = t.tile.tile_id;
			tile.alternative_id = t.tile.alternative_id;
			ortho_rot_idx = t.ortho_rot_idx;
			rotation = t.rotation;
		}

		void set_ortho_rotation(int p_index) {
			ortho_rot_idx = p_index;
			rotation.set_orthogonal_index(p_index);
		}

		void set_rotation(const Basis &p_rot) {
			rotation = p_rot;
			ortho_rot_idx = NON_ORTHOGONAL_ROT;
		}

		MapTile(int p_collection_id = -1, int p_tile_id = -1, int p_alternative_id = -1, int p_layer = -1) :
			tile(p_collection_id, p_tile_id, p_alternative_id, p_layer) {}
	};

	union MapCell {
		struct {
			int16_t x;
			int16_t y;
			int16_t z;
			int16_t layer;
		};
		uint64_t key = 0;

		_FORCE_INLINE_ bool operator<(const MapCell &p_key) const {
			return key < p_key.key;
		}

		_FORCE_INLINE_ operator Vector3i() const {
			return Vector3i(x, y, z);
		}

		_FORCE_INLINE_ operator Vector3() const {
			return Vector3(x, y, z);
		}

		MapCell(Vector3i p_vector, int p_layer = -1) {
			x = (int16_t)p_vector.x;
			y = (int16_t)p_vector.y;
			z = (int16_t)p_vector.z;
			layer = p_layer;
		}
		MapCell() {
			layer = -1;
		}
	};

	union OctantKey {
		struct {
			int16_t x;
			int16_t y;
			int16_t z;
			int16_t empty;
		};

		uint64_t key = 0;

		_FORCE_INLINE_ bool operator<(const OctantKey &p_key) const {
			return key < p_key.key;
		}

		OctantKey() {}

		OctantKey(int p_x, int p_y, int p_z) {
			x = p_x, y = p_y, z = p_z;
		}
	};

	struct Octant {
		struct OctantPhysicsLayer {
			RID body;
			Map<int, MapCell> shapes_cell;
		};

		struct MultimeshInstance {
			RID instance;
			RID multimesh;
		};

		Set<MapCell> cells;
		bool dirty = false;
		LocalVector<MultimeshInstance, int> multimesh_instances;
		List<OctantPhysicsLayer> physics;

		// struct MultimeshInstance {
		// 	RID instance;
		// 	RID multimesh;
		// 	struct Item {
		// 		int index = 0;
		// 		Transform3D transform;
		// 		IndexKey key;
		// 	};

		// 	Vector<Item> items;
		// };

		// struct NavMesh {
		// 	RID region;
		// 	Transform3D xform;
		// };

		// bool dirty = false;
		// Set<MapCell> cells;

		// Vector<MultimeshInstance> multimesh_instances;
		// RID static_body;
		// RID collision_debug;
		// RID collision_debug_instance;
		// Map<MapCell, NavMesh> navmesh_ids;
	};

	Map<OctantKey, Octant *> octant_map;

	struct TileMapLayer {
		String name;
		bool enabled = true;
		Ref<Material> material_override;
		float transparency = 0.0;
		uint32_t render_layers = 1; // ADD_PROPERTY(PropertyInfo(Variant::INT, "layers", PROPERTY_HINT_LAYERS_3D_RENDER), "set_layer_mask", "get_layer_mask");
		Map<MapCell, MapTile> tile_map;
	};

	LocalVector<TileMapLayer, int> layers;

	Map<MapCell, RID> multimeshes;
	Map<MapCell, int> instance_indices;

	bool awaiting_update = false;
	Transform3D last_transform;

	void _queue_octants_dirty();
	void _recreate_octant_data();
	void _update_octants_callback();
	void _update_visibility();

	void _clear_octants();
	void _octant_clean_up(Octant *p_oct);
	void _octant_enter_world(Octant *p_oct);
	void _octant_exit_world(Octant *p_oct);
	void _octant_transform(Octant *p_oct);
	bool _octant_update(Octant *p_oct);
	void _insert_octant_cell(const OctantKey &p_ok, const MapCell &p_cell);
	void _mark_octants_as_dirty();

	OctantKey _cell_to_octant(const MapCell &p_cell) const;
	Vector3 _cell_to_world(const MapCell &p_cell) const;

	void _clear_layers();

protected:
	// bool _set(const StringName &p_name, const Variant &p_value);
	// bool _get(const StringName &p_name, Variant &r_ret) const;
	// void _get_property_list(List<PropertyInfo> *p_list) const;

	void _notification(int p_what);
	static void _bind_methods();

public:
	enum {
		INVALID_ITEM = -1
	};

	void set_tile_set(const Ref<TileSet3D> &p_set);
	Ref<TileSet3D> get_tileset() const;
	void set_cell_scale(float p_scale);
	float get_cell_scale() const;
	void set_octant_size(const Vector3 &p_size);
	Vector3 get_octant_size() const;
	void set_octant_center_x(bool p_center);
	bool is_octant_centered_x() const;
	void set_octant_center_y(bool p_center);
	bool is_octant_centered_y() const;
	void set_octant_center_z(bool p_center);
	bool is_octant_centered_z() const;

	void clear();

	int get_layers_count() const;
	void add_layer(int p_to_pos = -1);
	void move_layer(int p_layer, int p_to_pos);
	void remove_layer(int p_layer);
	void set_layer_name(int p_layer, String p_name);
	String get_layer_name(int p_layer) const;
	void set_layer_enabled(int p_layer, bool p_visible);
	bool is_layer_enabled(int p_layer) const;
	void set_layer_material_override(int p_layer, const Ref<Material> &p_material_override);
	Ref<Material> get_layer_material_override(int p_layer) const;
	void set_layer_transparency(int p_layer, float p_transparency);
	float get_layer_transparency(int p_layer) const;
	void set_layer_render_layers_mask(int p_layer, uint32_t p_mask);
	uint32_t get_layer_render_layers_mask(int p_layer) const;
	void set_layer_render_layer_mask_value(int p_layer, int p_render_layer_number, bool p_enable);
	bool get_layer_render_layer_mask_value(int p_layer, int p_render_layer_number) const;

	void set_cell(int p_layer, const Vector3i &p_position, int p_tile, int p_collection = -1, int p_alternative = -1, int p_rot_idx = 0);
	int get_cell_collection_id(int p_layer, const Vector3i &p_position) const;
	int get_cell_tile_id(int p_layer, const Vector3i &p_position) const;
	int get_cell_alternative_id(int p_layer, const Vector3i &p_position) const;
	void set_cell_orientation_index(int p_layer, const Vector3i &p_position, int p_rot_idx);
	int get_cell_closest_orientation_index(int p_layer, const Vector3i &p_position) const;
	void set_cell_rotation(int p_layer, const Vector3i &p_position, const Basis &p_rotation);
	Basis get_cell_rotation(int p_layer, const Vector3i &p_position) const;
	bool is_cell_rotation_orthogonal(int p_layer, const Vector3i &p_position) const;

	// set_layer_transparency

	TileMap3D();
	~TileMap3D();
};

#endif // TILE_MAP_3D_H
