## Process this file with automake to produce Makefile.in

# Set include directories
INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/src \
           -I$(top_srcdir)/src/support \
           -I$(top_srcdir)/inst/cta/include \
           -I$(top_srcdir)/inst/cta/src

# Define sources for this directory
sources = src/GCTASupport.cpp \
          src/GCTAException.cpp \
          src/GCTAObservation.cpp \
          src/GCTAOnOffObservation.cpp \
          src/GCTAOnOffObservations.cpp \
          src/GCTAEventList.cpp \
          src/GCTAEventAtom.cpp \
          src/GCTAEventCube.cpp \
          src/GCTAEventBin.cpp \
          src/GCTAResponse.cpp \
          src/GCTAResponseIrf.cpp \
          src/GCTAResponseCube.cpp \
          src/GCTAResponse_helpers.cpp \
          src/GCTAResponseTable.cpp \
          src/GCTAAeff.cpp \
          src/GCTAAeffPerfTable.cpp \
          src/GCTAAeffArf.cpp \
          src/GCTAAeff2D.cpp \
          src/GCTAPsf.cpp \
          src/GCTAPsfPerfTable.cpp \
          src/GCTAPsfVector.cpp \
          src/GCTAPsf2D.cpp \
          src/GCTAPsfKing.cpp \
          src/GCTAEdisp.cpp \
          src/GCTAEdispPerfTable.cpp \
          src/GCTAEdispRmf.cpp \
          src/GCTAEdisp2D.cpp \
          src/GCTABackground.cpp \
          src/GCTABackgroundPerfTable.cpp \
          src/GCTABackground3D.cpp \
          src/GCTACubeExposure.cpp \
          src/GCTACubeBackground.cpp \
          src/GCTACubePsf.cpp \
          src/GCTACubeSource.cpp \
          src/GCTACubeSourcePoint.cpp \
          src/GCTACubeSourceDiffuse.cpp \
          src/GCTAInstDir.cpp \
          src/GCTARoi.cpp \
          src/GCTAPointing.cpp \
          src/GCTAModelCubeBackground.cpp \
          src/GCTAModelIrfBackground.cpp \
          src/GCTAModelRadialRegistry.cpp \
          src/GCTAModelRadial.cpp \
          src/GCTAModelRadialGauss.cpp \
          src/GCTAModelRadialPolynom.cpp \
          src/GCTAModelRadialProfile.cpp \
          src/GCTAModelRadialAcceptance.cpp

# Define headers to be installed
pkginclude_HEADERS = include/GCTATypemaps.hpp \
                     include/GCTAException.hpp \
                     include/GCTAObservation.hpp \
                     include/GCTAOnOffObservation.hpp \
                     include/GCTAOnOffObservations.hpp \
                     include/GCTAEventList.hpp \
                     include/GCTAEventAtom.hpp \
                     include/GCTAEventCube.hpp \
                     include/GCTAEventBin.hpp \
                     include/GCTAPointing.hpp \
                     include/GCTAInstDir.hpp \
                     include/GCTARoi.hpp \
                     include/GCTAResponse.hpp \
                     include/GCTAResponseIrf.hpp \
                     include/GCTAResponseCube.hpp \
                     include/GCTAResponseTable.hpp \
                     include/GCTAAeff.hpp \
                     include/GCTAAeffPerfTable.hpp \
                     include/GCTAAeffArf.hpp \
                     include/GCTAAeff2D.hpp \
                     include/GCTAPsf.hpp \
                     include/GCTAPsfPerfTable.hpp \
                     include/GCTAPsfVector.hpp \
                     include/GCTAPsf2D.hpp \
                     include/GCTAPsfKing.hpp \
                     include/GCTAEdisp.hpp \
                     include/GCTAEdispPerfTable.hpp \
                     include/GCTAEdispRmf.hpp \
                     include/GCTAEdisp2D.hpp \
                     include/GCTABackground.hpp \
                     include/GCTABackgroundPerfTable.hpp \
                     include/GCTABackground3D.hpp \
                     include/GCTACubeExposure.hpp \
                     include/GCTACubePsf.hpp \
                     include/GCTACubeBackground.hpp \
                     include/GCTACubeSource.hpp \
                     include/GCTACubeSourcePoint.hpp \
                     include/GCTACubeSourceDiffuse.hpp \
                     include/GCTAModelCubeBackground.hpp \
                     include/GCTAModelIrfBackground.hpp \
                     include/GCTAModelRadialRegistry.hpp \
                     include/GCTAModelRadial.hpp \
                     include/GCTAModelRadialGauss.hpp \
                     include/GCTAModelRadialPolynom.hpp \
                     include/GCTAModelRadialProfile.hpp \
                     include/GCTAModelRadialAcceptance.hpp \
                     include/GCTALib.hpp

# Calibration files to distribute
caldb_pinx = caldb/caldb.indx
caldb_perf = caldb/cta_dummy_irf.dat
caldb_indx = caldb/data/cta/e/caldb.indx
caldb_irf1 = caldb/data/cta/e/bcf/IFAE20120510_50h/irf_file.fits
caldb_irf2 = caldb/data/cta/e/bcf/IFAE20120510_50h_King/irf_file.fits

# Build libtool library
noinst_LTLIBRARIES = libcta.la
libcta_la_SOURCES = $(sources)
libcta_la_LDFLAGS = -version-info $(GAMMALIB_LT_VERSION)

# Distribute calibration database files
EXTRA_DIST     = $(caldb_pinx) $(caldb_perf) $(caldb_indx) $(caldb_irf1) $(caldb_irf2)
caldbpinxdir   = $(datadir)/caldb/data/cta/dummy
caldbpinx_DATA = $(caldb_pinx)
caldbperfdir   = $(datadir)/caldb/data/cta/dummy/bcf
caldbperf_DATA = $(caldb_perf)
caldbindxdir   = $(datadir)/caldb/data/cta/e
caldbindx_DATA = $(caldb_indx)
caldbirf1dir   = $(datadir)/caldb/data/cta/e/bcf/IFAE20120510_50h
caldbirf1_DATA = $(caldb_irf1)
caldbirf2dir   = $(datadir)/caldb/data/cta/e/bcf/IFAE20120510_50h_King
caldbirf2_DATA = $(caldb_irf2)
