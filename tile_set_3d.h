/*************************************************************************/
/*  tile_set_3d.h                                                        */
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

#ifndef TILE_SET_3D_H
#define TILE_SET_3D_H

#include "core/io/resource.h"
#include "scene/3d/visual_instance_3d.h"
#include "scene/resources/box_shape_3d.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/packed_scene.h"
#include "core/templates/local_vector.h"

class TileData3D : public Resource {
    GDCLASS(TileData3D, Resource);

private:
    // Tile
    Vector3i span = Vector3i(1, 1, 1);
    Ref<Texture2D> preview;
    String source;

    // Alternative
    int alternative_id = -1;
    float probability = 1.0;
    float internal_probability = 1.0;

    void _set_alternative_id(int p_alt_id);
    int _get_alternative_id() const;
    void _set_internal_probability(float p_internal_prob);
    float _get_internal_probability() const;

    bool _changed_request = false;
    void _update_changed();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();
    void _queue_changed();

public:

    void set_span(const Vector3i &p_span);
    Vector3i get_span() const;
    void set_preview(const Ref<Texture2D> &p_preview);
    Ref<Texture2D> get_preview() const;
    void set_probability(float p_probability);
    float get_probability() const;
    void set_source(const String &p_source);
    String get_source() const;

    TileData3D(){}
    ~TileData3D(){}
};

class TileData3DMesh : public TileData3D {
    GDCLASS(TileData3DMesh, TileData3D);

public:
    struct ShapeData {
		Ref<Shape3D> shape;
		Transform3D local_transform = Transform3D();
	};
    struct PhysicsLayerTileData3D {
        int layer_id = -1;
        Vector<ShapeData> shapes;
        Vector3 linear_velocity = Vector3();
        Vector3 angular_velocity = Vector3();
    };
    struct NavigationLayerTileData3D {
        int layer_id = -1;
        Ref<NavigationMesh> navmesh;
        Transform3D local_transform;
    };

    enum LightType {
        LIGHT_TYPE_OMNI,
        LIGHT_TYPE_SPOT
    };

    struct LightTileData3D {
	    Transform3D local_transform = Transform3D();
        LightType type = LIGHT_TYPE_OMNI;
        float light_range = 5.0;
        float attenuation = 1.0;
        float spot_angle = 45.0;
        float spot_angle_attenuation = 1.0;
        RenderingServer::LightOmniShadowMode omni_shadow_mode = RenderingServer::LIGHT_OMNI_SHADOW_CUBE;
        Color color = Color(1.0, 1.0, 1.0);
        float energy = 1.0;
        float indirect_energy = 1.0;
        Ref<Texture2D> projector;
        float size = 0.0;
        bool negative = false;
        float specular = 0.5;
        bool shadow_enabled = false;
        Color shadow_color = Color(0.0, 0.0, 0.0);
        float shadow_bias = 0.2;
        float shadow_normal_bias = 1.0;
        bool shadow_reverse_cull_face = false;
        float shadow_transmittance_bias = 0.05;
        float shadow_fog_fade = 0.1;
        float shadow_blur = 1.0;
    };

private:
    Ref<Mesh> mesh;
    Transform3D mesh_transform = Transform3D();
    Ref<Material> material_override;
    float transparency = 0.0;
    GeometryInstance3D::ShadowCastingSetting cast_shadow = GeometryInstance3D::SHADOW_CASTING_SETTING_ON;
    float lod_bias = 1.0;
    bool ignore_occlusion_culling = false;

    GeometryInstance3D::GIMode gi_mode = GeometryInstance3D::GI_MODE_DISABLED;
    GeometryInstance3D::LightmapScale gi_lightmap_scale = GeometryInstance3D::LIGHTMAP_SCALE_1X;

    float visibility_range_begin = 0.0;
    float visibility_range_begin_margin = 0.0;
    float visibility_range_end = 0.0;
    float visibility_range_end_margin = 0.0;
    GeometryInstance3D::VisibilityRangeFadeMode visibility_range_fade_mode = GeometryInstance3D::VISIBILITY_RANGE_FADE_DISABLED;

    PackedVector3Array occlusion_vertices;
    PackedInt32Array occlusion_indices;

    Vector<PhysicsLayerTileData3D> physics_data;

    Vector<NavigationLayerTileData3D> navigation_data;

    Vector<LightTileData3D> lights;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

public:

    void set_mesh(const Ref<Mesh> &p_mesh);
    Ref<Mesh> get_mesh() const;
    void set_mesh_transform(const Transform3D &p_transform);
    Transform3D get_mesh_transform() const;
    void set_material_override(const Ref<Material> &p_material);
    Ref<Material> get_material_override() const;
    void set_transparency(float p_transparency);
    float get_transparency() const;
    void set_cast_shadow_mode(GeometryInstance3D::ShadowCastingSetting p_mode);
    GeometryInstance3D::ShadowCastingSetting get_cast_shadow_mode() const;
    void set_lod_bias(float p_value);
    float get_lod_bias() const;
    void set_ignore_occlusion_culling(bool p_value);
    bool is_ignoring_occlusion_culling() const;

    void set_gi_mode(GeometryInstance3D::GIMode p_mode);
    GeometryInstance3D::GIMode get_gi_mode() const;
    void set_gi_lightmap_scale(GeometryInstance3D::LightmapScale p_scale);
    GeometryInstance3D::LightmapScale get_gi_lightmap_scale() const;

    void set_visibility_range_begin(float p_value);
    float get_visibility_range_begin() const;
    void set_visibility_range_begin_margin(float p_value);
    float get_visibility_range_begin_margin() const;
    void set_visibility_range_end(float p_value);
    float get_visibility_range_end() const;
    void set_visibility_range_end_margin(float p_value);
    float get_visibility_range_end_margin() const;
    void set_visibility_range_fade_mode(GeometryInstance3D::VisibilityRangeFadeMode p_mode);
    GeometryInstance3D::VisibilityRangeFadeMode get_visibility_range_fade_mode() const;

    void set_physics_data_count(int p_count);
    int get_physics_data_count() const;
	void add_physics_data(int p_index = -1);
	void move_physics_data(int p_from_index, int p_to_pos);
	void remove_physics_data(int p_index);
    int get_physics_layer_index(int p_layer) const;
    void set_physics_data_layer_id(int p_index, int p_layer);
    int get_physics_data_layer_id(int p_index) const;
    void set_physics_data_shape(int p_index, int p_shapei, const Ref<Shape3D> &p_shape);
    Ref<Shape3D> get_physics_data_shape(int p_index, int p_shapei) const;
    void set_physics_data_shape_transform(int p_index, int p_shapei, const Transform3D &p_transform);
    Transform3D get_physics_data_shape_transform(int p_index, int p_shapei) const;
    void set_physics_data_linear_velocity(int p_index, const Vector3 &p_linear_velocity);
    Vector3 get_physics_data_linear_velocity(int p_index) const;
    void set_physics_data_angular_velocity(int p_index, const Vector3 &p_angular_velocity);
    Vector3 get_physics_data_angular_velocity(int p_index) const;
    void set_physics_data_shapes(int p_index, const Vector<ShapeData> &p_shapes);
    Vector<ShapeData> get_physics_data_shapes(int p_index) const;
    void _set_physics_data_shapes(int p_index, const Array &p_shapes);
    Array _get_physics_data_shapes(int p_index) const;

    void set_navigation_data_count(int p_count);
    int get_navigation_data_count() const;
    int get_navigation_layer_index(int p_layer) const;
    void set_navigation_data_layer_id(int p_index, int p_layer);
    int get_navigation_data_layer_id(int p_index) const;
    void set_navigation_data_navmesh(int p_index, const Ref<NavigationMesh> &p_navmesh);
    Ref<NavigationMesh> get_navigation_data_navmesh(int p_index) const;
    void set_navigation_data_navmesh_transform(int p_index, const Transform3D &p_transform);
    Transform3D get_navigation_data_navmesh_transform(int p_index) const;

    void set_light_transform(int p_index, const Transform3D &p_transform);
    Transform3D get_light_transform(int p_index) const;
    void set_light_type(int p_index, LightType p_type);
    LightType get_light_type(int p_index) const;
    void set_light_range(int p_index, float p_range);
    float get_light_range(int p_index) const;
    void set_light_attenuation(int p_index, float p_attenuation);
    float get_light_attenuation(int p_index) const;
    void set_light_spot_angle(int p_index, float p_angle);
    float get_light_spot_angle(int p_index) const;
    void set_light_spot_angle_attenuation(int p_index, float p_angle_attenuation);
    float get_light_spot_angle_attenuation(int p_index) const;
    void set_light_omni_shadow_mode(int p_index, RenderingServer::LightOmniShadowMode p_mode);
    RenderingServer::LightOmniShadowMode get_light_omni_shadow_mode(int p_index) const;
    void set_light_color(int p_index, const Color &p_color);
    Color get_light_color(int p_index) const;
    void set_light_energy(int p_index, float p_energy);
    float get_light_energy(int p_index) const;
    void set_light_indirect_energy(int p_index, float p_energy);
    float get_light_indirect_energy(int p_index) const;
    void set_light_projector(int p_index, const Ref<Texture2D> &p_projector);
    Ref<Texture2D> get_light_projector(int p_index) const;
    void set_light_size(int p_index, float p_size);
    float get_light_size(int p_index) const;
    void set_light_negative(int p_index, bool p_negative);
    bool is_light_negative(int p_index) const;
    void set_light_specular(int p_index, float p_specular);
    float get_light_specular(int p_index) const;
    void set_light_shadow_enabled(int p_index, bool p_enabled);
    bool is_light_shadow_enabled(int p_index) const;
    void set_light_shadow_color(int p_index, const Color &p_color);
    Color get_light_shadow_color(int p_index) const;
    void set_light_shadow_bias(int p_index, float p_bias);
    float get_light_shadow_bias(int p_index) const;
    void set_light_shadow_normal_bias(int p_index, float p_bias);
    float get_light_shadow_normal_bias(int p_index) const;
    void set_light_shadow_reverse_cull_face(int p_index, bool p_reverse);
    bool is_light_shadow_reverse_cull_face(int p_index) const;
    void set_light_shadow_transmittance_bias(int p_index, float p_bias);
    float get_light_shadow_transmittance_bias(int p_index) const;
    void set_light_shadow_fog_fade(int p_index, float p_fade);
    float get_light_shadow_fog_fade(int p_index) const;
    void set_light_shadow_blur(int p_index, float p_blur);
    float get_light_shadow_blur(int p_index) const;

    TileData3DMesh(){}
    ~TileData3DMesh(){}
};

class TileData3DScene : public TileData3D {
    GDCLASS(TileData3DScene, TileData3D);

private:
    Ref<PackedScene> scene;
    bool display_placeholder = false;

protected:
	static void _bind_methods();

public:
    void set_scene(const Ref<PackedScene> &p_scene);
    Ref<PackedScene> get_scene() const;
    void set_display_placeholder(bool p_display);
    bool is_displaying_placeholder() const;

    TileData3DScene(){}
    ~TileData3DScene(){}
};

class TileSet3D;

class TileSet3DCollection : public Resource {
    GDCLASS(TileSet3DCollection, Resource)
    friend TileSet3D;

public:
    enum CollectionType {
        COLLECTION_TYPE_ATLAS = 1,
        COLLECTION_TYPE_SCENES
    };

    struct TileSet3DTileAlternatives {
        Map<int, Ref<TileData3D>> tiles;
        PackedInt32Array ids;
    };

private:

    CollectionType type;
    Ref<Texture2D> icon;
    Map<int, Ref<TileData3D>> tiles;
    PackedInt32Array ids;
    Map<int, TileSet3DTileAlternatives*> alternatives;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
    TileSet3DCollection();
    ~TileSet3DCollection();
};

class TileSet3D : public Resource {
	GDCLASS(TileSet3D, Resource);

public:
    enum TileShape {
        TILE_SHAPE_CUBOID,
        TILE_SHAPE_HEXAGONAL_PRISM
    };

    enum MainAxis {
        MAIN_AXIS_X,
        MAIN_AXIS_Y,
        MAIN_AXIS_Z
    };

private:

    TileShape tile_shape = TILE_SHAPE_CUBOID;

    Vector3 cell_size = Vector3(1, 1, 1);
    bool cell_center_x = true;
    bool cell_center_y = true;
    bool cell_center_z = true;

    MainAxis axis_main = MAIN_AXIS_Y;
    bool axis_invert_x = false;
    bool axis_invert_y = false;
    bool axis_invert_z = false;

    Map<int, Ref<TileSet3DCollection>> collections;
    PackedInt32Array collections_ids;

    bool _changed_requested = false;
    Set<int> _changed_collections;

    mutable int _cached_id = -1;
    mutable Ref<TileSet3DCollection> _cached_collection = nullptr;

    void _set_collection_count(int p_count);
    void _set_cached_collection(int p_id) const;
    void _queue_changed();
    void _queue_collection_changed(int p_idx);
    void _update_changed();
    void _update_collection_changed();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

public:

    void set_tile_shape(TileShape p_shape);
    TileShape get_tile_shape() const;
    void set_cell_size(const Vector3 &p_size);
    Vector3 get_cell_size() const;
    void set_cell_center_x(bool p_center);
    bool is_cell_centered_x() const;
    void set_cell_center_y(bool p_center);
    bool is_cell_centered_y() const;
    void set_cell_center_z(bool p_center);
    bool is_cell_centered_z() const;

    void set_main_axis(MainAxis p_main);
    MainAxis get_main_axis() const;
    void set_axis_invert_x(bool p_invert);
    bool is_axis_inverted_x() const;
    void set_axis_invert_y(bool p_invert);
    bool is_axis_inverted_y() const;
    void set_axis_invert_z(bool p_invert);
    bool is_axis_inverted_z() const;

    void clear();
    int get_collection_count() const;
    void add_collection(const Ref<TileSet3DCollection> &p_collection, int p_id, int p_pos = -1);
    int create_collection(TileSet3DCollection::CollectionType p_type, int p_id_override = -1);
    int get_next_collection_id(int p_initial = -1, int p_inc = 0) const;
    void remove_collection(int p_id);
    Ref<TileSet3DCollection> get_collection(int p_id) const;
    int get_collection_index(int p_id) const;
    void set_collection_name(int p_id, const String &p_name);
    String get_collection_name(int p_id) const;
    void set_collection_id(int p_index, int p_new_id, bool p_force = true);
    int get_collection_id(int p_index) const;
    bool has_collection_id(int p_id) const;
    TileSet3DCollection::CollectionType get_collection_type(int p_id) const;
    void set_collection_icon(int p_id, const Ref<Texture2D> &p_icon);
    Ref<Texture2D> get_collection_icon(int p_id) const;
    int get_collection_tile_count(int p_id, bool p_include_alternatives = false) const;

    int add_collection_tile(int p_collection_id, Ref<TileData3D> p_tile, int p_id_override = -1, int p_pos = -1);
    Ref<TileData3D> get_collection_tile(int p_collection_id, int p_tile_id, int p_alt_id = -1) const;
    int get_collection_tile_id(int p_collection_id, int p_tile_index);
    int add_collection_alternative_tile(int p_collection_id, int p_base_id, Ref<TileData3D> p_tile, int p_id_override = -1, int p_pos = -1);
    int get_next_tile_id(int p_collection_id, int p_initial = -1, int p_inc = 0) const;
    int get_next_alternative_tile_id(int p_collection_id, int p_base_id, int p_initial = -1, int p_inc = 0) const;
    int get_collection_alternatives_count(int p_collection_id, int p_base_id);

	TileSet3D();
	~TileSet3D();
};

VARIANT_ENUM_CAST(TileData3DMesh::LightType)
VARIANT_ENUM_CAST(TileSet3DCollection::CollectionType);
VARIANT_ENUM_CAST(TileSet3D::TileShape);
VARIANT_ENUM_CAST(TileSet3D::MainAxis);

#endif // TILE_SET_3D_H
