#include  <ray_trace.h>

private  void  interpolate_over_polygon(
    Point        *point,
    int          n_points,
    Point        points[],
    BOOLEAN      flat_shading_flag,
    Vector       normals[],
    Colour_flags colour_flag,
    Colour       colours[],
    Vector       *normal,
    Colour       *colour )
{
    int              i;
    Real             weights[MAX_POINTS_PER_POLYGON];
    Real             r, g, b, a;
    Vector           scaled_normal;

    if( colour_flag == PER_VERTEX_COLOURS )
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
        a = 0.0;
    }

    get_polygon_interpolation_weights( point, n_points, points, weights );

    if( flat_shading_flag )
        find_polygon_normal( n_points, points, normal );
    else
        fill_Vector( *normal, 0.0, 0.0, 0.0 );

    for_less( i, 0, n_points )
    {
        if( colour_flag == PER_VERTEX_COLOURS )
        {
            r += weights[i] * get_Colour_r_0_1(colours[i] );
            g += weights[i] * get_Colour_g_0_1(colours[i] );
            b += weights[i] * get_Colour_b_0_1(colours[i] );
            a += weights[i] * get_Colour_a_0_1(colours[i] );
        }

        if( !flat_shading_flag )
        {
            SCALE_VECTOR( scaled_normal, normals[i], weights[i] );
            ADD_VECTORS( *normal, *normal, scaled_normal );
        }
    }

    NORMALIZE_VECTOR( *normal, *normal );

    if( colour_flag == PER_VERTEX_COLOURS )
        *colour = make_rgba_Colour_0_1( r, g, b, a );
}

public  BOOLEAN  ray_intersects_a_polygon(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    BOOLEAN          flat_shading_flag,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist )
{
    polygons_struct  *polygons;
    int              size, i, object_index, point_index;
    Point            points[MAX_POINTS_PER_POLYGON];
    Vector           normals[MAX_POINTS_PER_POLYGON];
    Colour           colours[MAX_POINTS_PER_POLYGON];
    BOOLEAN          found;

    found = intersect_ray_with_object( origin, direction, object,
                                       &object_index, dist,
                                       (Real **) NULL ) > 0;

    if( found && point != (Point *) NULL )
    {
        polygons = get_polygons_ptr( object );

        GET_POINT_ON_RAY( *point, *origin, *direction, *dist );

        if( polygons->colour_flag == ONE_COLOUR )
            *colour = polygons->colours[0];
        else if( polygons->colour_flag == PER_ITEM_COLOURS )
            *colour = polygons->colours[object_index];

        size = get_polygon_points( polygons, object_index, points );

        for_less( i, 0, size )
        {
            point_index = polygons->indices[
                            POINT_INDEX(polygons->end_indices,object_index,i)];

            normals[i] = polygons->normals[point_index];

            if( polygons->colour_flag == PER_VERTEX_COLOURS )
                colours[i] = polygons->colours[point_index];
        }

        interpolate_over_polygon( point, size, points, flat_shading_flag,
                                  normals, polygons->colour_flag,
                                  colours, normal, colour );
    }

    return( found );
}

public  BOOLEAN  ray_intersects_a_quadmesh(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    BOOLEAN          flat_shading_flag,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist )
{
    quadmesh_struct  *quadmesh;
    int              i, j, m, n, p, object_index;
    int              indices[4];
    Point            points[4];
    Vector           normals[4];
    Colour           colours[4];
    BOOLEAN          found;

    found = intersect_ray_with_object( origin, direction, object,
                                       &object_index, dist,
                                       (Real **) NULL ) > 0;

    if( found && point != (Point *) NULL )
    {
        quadmesh = get_quadmesh_ptr( object );

        GET_POINT_ON_RAY( *point, *origin, *direction, *dist );

        if( quadmesh->colour_flag == ONE_COLOUR )
            *colour = quadmesh->colours[0];
        else if( quadmesh->colour_flag == PER_ITEM_COLOURS )
            *colour = quadmesh->colours[object_index];

        get_quadmesh_n_objects( quadmesh, &m, &n );

        i = object_index / n;
        j = object_index % n;

        get_quadmesh_patch_indices( quadmesh, i, j, indices );

        for_less( p, 0, 4 )
        {
            points[p] = quadmesh->points[indices[p]];
            normals[p] = quadmesh->normals[indices[p]];

            if( quadmesh->colour_flag == PER_VERTEX_COLOURS )
                colours[p] = quadmesh->colours[indices[p]];
        }

        interpolate_over_polygon( point, 4, points, flat_shading_flag,
                                  normals, quadmesh->colour_flag,
                                  colours, normal, colour );
    }

    return( found );
}

private  Real   get_line_alpha(
    Point     *point,
    Point     *p1,
    Point     *p2 )
{
    Real    alpha, d;
    Vector  v, offset;

    SUB_POINTS( v, *p2, *p1 );
    SUB_POINTS( offset, *point, *p1 );

    d = DOT_VECTORS( v, v );

    if( d == 0.0 )
        return( 0.0 );

    alpha = DOT_VECTORS( offset, v ) / d;

    if( alpha < 0.0 )
        alpha = 0.0;
    else if( alpha > 1.0 )
        alpha = 1.0;

    return( alpha );
}

public  BOOLEAN  ray_intersects_a_line(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist )
{
    int           line, seg, p1, p2, object_index;
    Real          alpha, r1, g1, b1, a1, r2, g2, b2, a2;
    BOOLEAN       found;
    Point         centre, point1, point2;
    lines_struct  *lines;

    found = intersect_ray_with_object( origin, direction, object,
                                       &object_index, dist,
                                       (Real **) NULL ) > 0;

    if( found && point != (Point *) NULL )
    {
        lines = get_lines_ptr( object );

        get_line_segment_index( lines, object_index, &line, &seg );

        GET_POINT_ON_RAY( *point, *origin, *direction, *dist );

        if( lines->colour_flag == ONE_COLOUR )
            *colour = lines->colours[0];
        else if( lines->colour_flag == PER_ITEM_COLOURS )
            *colour = lines->colours[line];

        p1 = lines->indices[POINT_INDEX(lines->end_indices,line,seg)];
        p2 = lines->indices[POINT_INDEX(lines->end_indices,line,seg+1)];
        point1 = lines->points[p1];
        point2 = lines->points[p2];

        alpha = get_line_alpha( point, &point1, &point2 );

        INTERPOLATE_POINTS( centre, point1, point2, alpha );
        SUB_POINTS( *normal, *point, centre );
        NORMALIZE_VECTOR( *normal, *normal );

        if( lines->colour_flag == PER_VERTEX_COLOURS )
        {
            r1 = get_Colour_r_0_1( lines->colours[p1] );
            g1 = get_Colour_g_0_1( lines->colours[p1] );
            b1 = get_Colour_b_0_1( lines->colours[p1] );
            a1 = get_Colour_a_0_1( lines->colours[p1] );
            r2 = get_Colour_r_0_1( lines->colours[p2] );
            g2 = get_Colour_g_0_1( lines->colours[p2] );
            b2 = get_Colour_b_0_1( lines->colours[p2] );
            a2 = get_Colour_a_0_1( lines->colours[p2] );

            *colour = make_rgba_Colour_0_1( r1 + (r2 - r1) * alpha,
                                            g1 + (g2 - g1) * alpha,
                                            b1 + (b2 - b1) * alpha,
                                            a1 + (a2 - a1) * alpha );
        }
    }

    return( found );
}

public  BOOLEAN  ray_intersects_a_marker(
    Point            *origin,
    Vector           *direction,
    object_struct    *object,
    Point            *point,
    Vector           *normal,
    Colour           *colour,
    Real             *dist )
{
    Real           orig, delta, t_min, t_max, t1, t2, pos1, pos2;
    int            object_index, dim;
    BOOLEAN        found;
    marker_struct  *marker;

    marker = get_marker_ptr( object );

    t_min = 0.0;
    t_max = 0.0;

    for_less( dim, 0, N_DIMENSIONS )
    {
        orig = RPoint_coord( *origin, dim );
        delta = RVector_coord( *direction, dim );
        pos1 = RPoint_coord( marker->position, dim ) - marker->size / 2.0;
        pos2 = RPoint_coord( marker->position, dim ) + marker->size / 2.0;
        if( delta < 0.0 )
        {
            t1 = (pos2 - orig) / delta;
            t2 = (pos1 - orig) / delta;
        }
        else if( delta > 0.0 )
        {
            t1 = (pos1 - orig) / delta;
            t2 = (pos2 - orig) / delta;
        }
        else
        {
            if( orig < pos1 || orig > pos2 )
                return( FALSE );
        }

        if( t1 > t_min )
            t_min = t1;
        if( dim == 0 || t2 < t_max )
            t_max = t2;
    }

    if( t1 > t2 )
        return( FALSE );

    found = intersect_ray_with_object( origin, direction, object,
                                       &object_index, dist,
                                       (Real **) NULL ) > 0;

    if( found && point != (Point *) NULL )
    {
        marker = get_marker_ptr( object );

        GET_POINT_ON_RAY( *point, *origin, *direction, *dist );

        *colour = marker->colour;

        SUB_POINTS( *normal, *point, marker->position );
        NORMALIZE_VECTOR( *normal, *normal );
    }

    return( found );
}
