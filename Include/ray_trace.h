#ifndef  DEF_RAY_TRACE_H
#define  DEF_RAY_TRACE_H

#include  <volume_io/internal_volume_io.h>
#include  <bicpl.h>
#include  <graphics.h>

#define  N_ATTENUATION   3

typedef struct
{
    Light_types   type;
    Colour        colour;
    Point         position;
    Vector        direction;
    Real          attenuation[N_ATTENUATION];
} light_struct;

typedef struct
{
    int            n_lights;
    light_struct   *lights;
    Colour         ambient_light;
} lights_struct;

typedef struct
{
    Point           eye;
    Real            stereo_offset;
    Vector          line_of_sight;
    Vector          horizontal;
    Vector          vertical;
    Vector          perpendicular;
    Real            window_width;
    Real            window_height;
    Real            window_distance;
    BOOLEAN         perspective_flag;
    int             x_viewport_size;
    int             y_viewport_size;
} view_struct;

typedef struct
{
    Real                   opacity;
    colour_coding_struct   colour_coding;
    Volume                 volume;
    int                    continuity;
    BOOLEAN                fill_value_specified;
    Real                   fill_value;
    BOOLEAN                gradient_flag;
    BOOLEAN                extend_volume_flag;
} volume_info;

typedef  struct
{
    BOOLEAN                shadow_state;
    Real                   shadow_offset;
    BOOLEAN                flat_shading_flag;
    BOOLEAN                lighting_flag;
    BOOLEAN                mult_volume_flag;
    BOOLEAN                composite_volume_flag;
    int                    n_volumes;
    volume_info            *volumes;
    BOOLEAN                hit_only_once_flag;
}
render_struct;

typedef  struct
{
    int             n_clip_objects;
    object_struct   **clip_objects;
    BOOLEAN         *clip_inside_flags;
}
clip_struct;

typedef  struct
{
    render_struct      render;
    BOOLEAN            regular_object_flag;
    object_struct      *object;
    clip_struct        clip;
    Real               threshold;
    Colour             colour;
    Surfprop           spr;
    bitlist_3d_struct  done_bits;
    bitlist_3d_struct  surface_bits;
}
ray_trace_object;

#include  <ray_trace_prototypes.h>

#endif
