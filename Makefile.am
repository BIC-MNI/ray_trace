# note: all files commented out do not appear in the CONGLOMERATE_SOURCE 
# variable of the original Makefile, but were part of the Makefile and 
# the source tree in other ways

SUBDIRS = Graphics 

bin_PROGRAMS = ray_trace

EXTRA_DIST = m4/mni_cxx_have_koenig_lookup.m4 \
	m4/mni_REQUIRE_LIB.m4 \
	m4/mni_REQUIRE_MNILIBS.m4 \
	m4/mni_REQUIRE_OPENINVENTOR.m4 \
	m4/smr_CGAL_MAKEFILE.m4 \
	m4/smr_OPTIONAL_LIB.m4 \
	m4/smr_REQUIRED_LIB.m4 \
	m4/smr_WITH_BUILD_PATH.m4 \
	Include/ray_trace.h \
	Include/ray_trace_prototypes.h \
	Documentation/ray_trace_tutorial.html


INCLUDES = -I$(top_srcdir)/Include \
	-I$(top_srcdir)/Graphics/Include 


AUTOMAKE_OPTIONS = check-news
ACLOCAL_AMFLAGS = -I m4

ray_trace_SOURCES =  ray_trace.c \
	intersect.c \
    trace.c \
    volume.c 

ray_trace_LDADD = Graphics/libbicgl.a
