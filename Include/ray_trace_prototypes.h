#ifndef  DEF_RAY_TRACE_PROTOTYPES
#define  DEF_RAY_TRACE_PROTOTYPES

public  BOOLEAN  ray_intersects_a_polygon(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    BOOLEAN          flat_shading_flag,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist );

public  BOOLEAN  ray_intersects_a_quadmesh(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    BOOLEAN          flat_shading_flag,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist );

public  BOOLEAN  ray_intersects_a_line(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist );

public  BOOLEAN  ray_intersects_a_marker(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist );

public  Colour  trace_ray(
    view_struct           *view,
    lights_struct         *lights,
    int                   n_objects,
    ray_trace_object      objects[],
    BOOLEAN               use_flags[],
    int                   x_pixel,
    int                   y_pixel,
    int                   max_depth,
    int                   x_super_sampling,
    int                   y_super_sampling );

public  void  ray_trace_scene(
    view_struct           *view,
    lights_struct         *lights,
    int                   n_objects,
    ray_trace_object      objects[],
    int                   max_depth,
    int                   x_super_sampling,
    int                   y_super_sampling,
    pixels_struct         *pixels );

public  BOOLEAN  ray_intersects_a_volume(
    Point              *origin,
    Vector             *direction,
    Volume             volume,
    int                continuity,
    bitlist_3d_struct  *done_bits,
    bitlist_3d_struct  *surface_bits,
    Real               threshold,
    Point              *point,
    Vector             *normal,
    Real               *dist );

public  void  get_range_of_volume(
    Volume    volume,
    Point     *min_point,
    Point     *max_point );
#endif
