/***************************************************************************
 *                       GSkymap.cpp - Sky map class                       *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2015 by Juergen Knoedlseder                         *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GSkymap.cpp
 * @brief Sky map class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GSkymap.hpp"
#include "GHealpix.hpp"
#include "GWcsRegistry.hpp"
#include "GWcs.hpp"
#include "GFits.hpp"
#include "GFitsTableDoubleCol.hpp"
#include "GFitsImageDouble.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_CONSTRUCT_HPX                "GSkymap::GSkymap(std::string&, int&,"\
                                                       " std::string&, int&)"
#define G_CONSTRUCT_MAP        "GSkymap::GSkymap(std::string&, std::string&,"\
                      " double&, double&, double& double&, int&, int&, int&)"
#define G_NMAPS                                        "GSkymap::nmaps(int&)"
#define G_OP_UNARY_ADD                        "GSkymap::operator+=(GSkymap&)"
#define G_OP_UNARY_SUB                        "GSkymap::operator-=(GSkymap&)"
#define G_OP_UNARY_MUL                        "GSkymap::operator-=(GSkymap&)"
#define G_OP_UNARY_DIV                        "GSkymap::operator-=(GSkymap&)"
#define G_OP_ACCESS_1D                        "GSkymap::operator(int&, int&)"
#define G_OP_ACCESS_2D                  "GSkymap::operator(GSkyPixel&, int&)"
#define G_OP_VALUE                        "GSkymap::operator(GSkyDir&, int&)"
#define G_INX2DIR                                    "GSkymap::inx2dir(int&)"
#define G_PIX2DIR                              "GSkymap::pix2dir(GSkyPixel&)"
#define G_DIR2INX                                "GSkymap::dir2inx(GSkyDir&)"
#define G_DIR2PIX                                "GSkymap::dir2pix(GSkyDir&)"
#define G_SOLIDANGLE1                             "GSkymap::solidangle(int&)"
#define G_SOLIDANGLE2                       "GSkymap::solidangle(GSkyPixel&)"
#define G_EXTRACT                              "GSkymap::extract(int&, int&)"
#define G_READ                               "GSkymap::read(const GFitsHDU&)"
#define G_SET_WCS     "GSkymap::set_wcs(std::string&, std::string&, double&,"\
                              " double&, double&, double&, double&, double&,"\
                                                       " GMatrix&, GVector&)"
#define G_READ_HEALPIX                   "GSkymap::read_healpix(GFitsTable*)"
#define G_READ_WCS                           "GSkymap::read_wcs(GFitsImage*)"
#define G_ALLOC_WCS                         "GSkymap::alloc_wcs(GFitsImage*)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_READ_HEALPIX_DEBUG                          // Debug read_healpix
//#define G_READ_WCS_DEBUG                                  // Debug read_wcs

/* __ Prototype __________________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                          Constructors/destructors                       =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GSkymap::GSkymap(void)
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS file constructor
 *
 * @param[in] filename FITS file name.
 ***************************************************************************/
GSkymap::GSkymap(const std::string& filename)
{
    // Initialise class members for clean destruction
    init_members();

    // Load skymap
    load(filename);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Healpix sky map constructor
 *
 * @param[in] coords Coordinate System (CEL or GAL).
 * @param[in] nside Nside parameter.
 * @param[in] order Pixel ordering (RING or NEST).
 * @param[in] nmaps Number of maps in set (default=1).
 *
 * @exception GException::skymap_bad_par
 *            Invalid sky map parameter.
 *
 * Constructs sky map in Healpix pixelisation.
 ***************************************************************************/
GSkymap::GSkymap(const std::string& coords,
                 const int&         nside,
                 const std::string& order,
                 const int&         nmaps)
{
    // Initialise class members for clean destruction
    init_members();

    // Check if nmaps parameter is >0
    if (nmaps < 1) {
        throw GException::skymap_bad_par(G_CONSTRUCT_HPX, nmaps,
                                         "nmaps parameter must be >0.");
    }

    // Allocate Healpix projection
    GHealpix* projection = new GHealpix(nside, order, coords);
    m_proj               = projection;

    // Set number of pixels and number of maps
    m_num_pixels = projection->npix();
    m_num_maps   = nmaps;

    // Allocate pixels
    alloc_pixels();

    // Return
    return;
}


/***********************************************************************//**
 * @brief WCS sky map constructor
 *
 * @param[in] wcs World Coordinate System.
 * @param[in] coords Coordinate System (CEL or GAL).
 * @param[in] x X coordinate of sky map centre (deg).
 * @param[in] y Y coordinate of sky map centre (deg).
 * @param[in] dx Pixel size in x direction at centre (deg/pixel).
 * @param[in] dy Pixel size in y direction at centre (deg/pixel).
 * @param[in] nx Number of pixels in x direction.
 * @param[in] ny Number of pixels in y direction.
 * @param[in] nmaps Number of maps in set (default=1).
 *
 * @exception GException::skymap_bad_par
 *            Invalid sky map parameter.
 *
 * Constructs sky map in World Coordinate System projection.
 ***************************************************************************/
GSkymap::GSkymap(const std::string& wcs,
                 const std::string& coords,
                 const double&      x,
                 const double&      y,
                 const double&      dx,
                 const double&      dy,
                 const int&         nx,
                 const int&         ny,
                 const int&         nmaps)
{
    // Initialise class members for clean destruction
    init_members();

    // Check parameters
    if (nx < 1) {
        throw GException::skymap_bad_par(G_CONSTRUCT_MAP, nx,
                                         "nx parameter must be >0.");
    }
    if (ny < 1) {
        throw GException::skymap_bad_par(G_CONSTRUCT_MAP, ny,
                                         "ny parameter must be >0.");
    }
    if (nmaps < 1) {
        throw GException::skymap_bad_par(G_CONSTRUCT_MAP, nmaps,
                                         "nmaps parameter must be >0.");
    }

    // Set WCS
    double  crval1 = x;
    double  crval2 = y;
    double  crpix1 = double(nx+1)/2.0;
    double  crpix2 = double(ny+1)/2.0;
    double  cdelt1 = dx;
    double  cdelt2 = dy;
    GMatrix cd(2,2);
    GVector pv2(21);
    set_wcs(wcs, coords, crval1, crval2, crpix1, crpix2, cdelt1, cdelt2, 
            cd, pv2);

    // Set number of pixels and number of maps
    m_num_x      = nx;
    m_num_y      = ny;
    m_num_pixels = m_num_x * m_num_y;
    m_num_maps   = nmaps;

    // Allocate pixels
    alloc_pixels();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] map Sky map.
 ***************************************************************************/
GSkymap::GSkymap(const GSkymap& map)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(map);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GSkymap::~GSkymap(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] map Sky map.
 * @return Sky map.
 ***************************************************************************/
GSkymap& GSkymap::operator=(const GSkymap& map)
{
    // Execute only if object is not identical
    if (this != &map) {

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(map);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Value setting operator
 *
 * @param[in] value Value.
 * @return Sky map.
 *
 * Sets all pixels to the specified @p value.
 ***************************************************************************/
GSkymap& GSkymap::operator=(const double& value)
{
    // Get number of pixels
    int num = m_num_pixels * m_num_maps;

    // Loop over all pixels
    for (int i = 0; i < num; ++i) {
        m_pixels[i] = value;
    }

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Map addition operator
 *
 * @param[in] map Sky map.
 * @return Sky map.
 *
 * @exception GException::invalid_value
 *            Mismatch between number of maps in skymap object.
 *
 * Adds the content of @p map to the skymap. The operator only works on sky
 * maps with an identical number of layers. The content is added by
 * bi-linearily interpolating the values in the source sky map, allowing thus
 * for a reprojection of sky map values.
 *
 * @todo The method is not optimized for speed as the transformation is done
 * for each layer separately. A private method should be introduced that
 * does the transformation, allowing the to loop more effectively over the
 * layers.
 ***************************************************************************/
GSkymap& GSkymap::operator+=(const GSkymap& map)
{
    // Check if number of layers are identical
    if (map.nmaps() != nmaps()) {
        std::string msg = "Mismatch of number of maps in skymap object"
                          " ("+gammalib::str(nmaps())+" maps in destination"
                          " map, "+gammalib::str(map.nmaps())+" in source"
                          " map.";
        throw GException::invalid_value(G_OP_UNARY_ADD, msg);
    }

    // Loop over all pixels of sky map
    for (int index = 0; index < npix(); ++index) {

        // Get sky direction of actual pixel
        GSkyDir dir = inx2dir(index);

        // Loop over all layers
        for (int layer = 0; layer < nmaps(); ++layer) {
        
            // Add value
            (*this)(index, layer) += map(dir, layer);

        } // endfor: looped over all layers
        
    } // endfor: looped over all pixels

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Value addition operator
 *
 * @param[in] value Value.
 * @return Sky map.
 *
 * Add @p value to all sky map pixels.
 ***************************************************************************/
GSkymap& GSkymap::operator+=(const double& value)
{
    // Set total number of sky map pixels
    int num = m_num_pixels * m_num_maps;

    // Loop over all pixels of sky map
    for (int i = 0; i < num; ++i) {
        m_pixels[i] += value;
    }

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Map subtraction operator
 *
 * @param[in] map Sky map.
 * @return Sky map.
 *
 * @exception GException::invalid_value
 *            Mismatch between number of maps in skymap object.
 *
 * Subtracts the content of @p map from the skymap. The operator only works
 * on sky maps with an identical number of layers. The content is subtracted
 * by bi-linearily interpolating the values in the source sky map, allowing
 * thus for a reprojection of sky map values.
 *
 * @todo The method is not optimized for speed as the transformation is done
 * for each layer separately. A private method should be introduced that
 * does the transformation, allowing the to loop more effectively over the
 * layers.
 ***************************************************************************/
GSkymap& GSkymap::operator-=(const GSkymap& map)
{
    // Check if number of layers are identical
    if (map.nmaps() != nmaps()) {
        std::string msg = "Mismatch of number of maps in skymap object"
                          " ("+gammalib::str(nmaps())+" maps in destination"
                          " map, "+gammalib::str(map.nmaps())+" in source"
                          " map.";
        throw GException::invalid_value(G_OP_UNARY_SUB, msg);
    }

    // Loop over all pixels of sky map
    for (int index = 0; index < npix(); ++index) {

        // Get sky direction of actual pixel
        GSkyDir dir = inx2dir(index);

        // Loop over all layers
        for (int layer = 0; layer < nmaps(); ++layer) {
        
            // Subtract value
            (*this)(index, layer) -= map(dir, layer);

        } // endfor: looped over all layers
        
    } // endfor: looped over all pixels

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Value subtraction operator
 *
 * @param[in] value Value.
 * @return Sky map.
 *
 * Subtracts @p value from all sky map pixels.
 ***************************************************************************/
GSkymap& GSkymap::operator-=(const double& value)
{
    // Set total number of sky map pixels
    int num = m_num_pixels * m_num_maps;

    // Loop over all pixels of sky map
    for (int i = 0; i < num; ++i) {
        m_pixels[i] -= value;
    }

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Multiplication operator
 *
 * @param[in] map Sky map.
 * @return Sky map.
 *
 * @exception GException::invalid_value
 *            Mismatch between number of maps in skymap object.
 *
 * Multiplies the content of @p map from the skymap. The operator only works
 * on sky maps with an identical number of layers. The content is multiplied
 * by bi-linearily interpolating the values in the source sky map, allowing
 * thus for a reprojection of sky map values.
 *
 * @todo The method is not optimized for speed as the transformation is done
 * for each layer separately. A private method should be introduced that
 * does the transformation, allowing to loop more effectively over the
 * layers.
 ***************************************************************************/
GSkymap& GSkymap::operator*=(const GSkymap& map)
{
    // Check if number of layers are identical
    if (map.nmaps() != nmaps()) {
        std::string msg = "Mismatch of number of maps in skymap object"
                          " ("+gammalib::str(nmaps())+" maps in destination"
                          " map, "+gammalib::str(map.nmaps())+" in source"
                          " map.";
        throw GException::invalid_value(G_OP_UNARY_MUL, msg);
    }

    // Loop over all pixels of sky map
    for (int index = 0; index < npix(); ++index) {

        // Get sky direction of actual pixel
        GSkyDir dir = inx2dir(index);

        // Loop over all layers
        for (int layer = 0; layer < nmaps(); ++layer) {

            // Subtract value
            (*this)(index, layer) *= map(dir, layer);

        } // endfor: looped over all layers

    } // endfor: looped over all pixels

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Sky map scaling operator
 *
 * @param[in] factor Scale factor.
 * @return Sky map.
 *
 * Multiplies all pixels of the sky map by the given scale @p factor.
 ***************************************************************************/
GSkymap& GSkymap::operator*=(const double& factor)
{
    // Compute total number of pixels
    int n = npix() * nmaps();

    // Loop over all pixels
    double* pixel = m_pixels;
    for (int i = 0; i < n; ++i) {
        *pixel++ *= factor;
    }

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Division operator
 *
 * @param[in] map Sky map.
 * @return Sky map.
 *
 * @exception GException::invalid_value
 *            Mismatch between number of maps in skymap object.
 *
 * Divides the content of @p map from the skymap. The operator only works
 * on sky maps with an identical number of layers. The content is divided
 * by bi-linearily interpolating the values in the source sky map, allowing
 * thus for a reprojection of sky map values.
 *
 * @todo The method is not optimized for speed as the transformation is done
 * for each layer separately. A private method should be introduced that
 * does the transformation, allowing to loop more effectively over the
 * layers.
 ***************************************************************************/
GSkymap& GSkymap::operator/=(const GSkymap& map)
{
    // Check if number of layers are identical
    if (map.nmaps() != nmaps()) {
        std::string msg = "Mismatch of number of maps in skymap object"
                          " ("+gammalib::str(nmaps())+" maps in destination"
                          " map, "+gammalib::str(map.nmaps())+" in source"
                          " map.";
        throw GException::invalid_value(G_OP_UNARY_DIV, msg);
    }

    // Loop over all pixels of sky map
    for (int index = 0; index < npix(); ++index) {

        // Get sky direction of actual pixel
        GSkyDir dir = inx2dir(index);

        // Loop over all layers
        for (int layer = 0; layer < nmaps(); ++layer) {

            // Check for division by zero
            if (map(dir,layer) == 0.0) {
                std::string msg = "Trying to divide by zero."
                                  " Map entries have to be strictly non-zero"
                                  " for division";
                throw GException::invalid_value(G_OP_UNARY_DIV, msg);
            }
            
            // Subtract value
            (*this)(index, layer) /= map(dir, layer);

        } // endfor: looped over all layers

    } // endfor: looped over all pixels

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Sky map division operator
 *
 * @param[in] factor Scale factor.
 * @return Sky map.
 *
 * @exception GException::invalid_argument
 *            Division by zero error.
 *
 * Divides all pixels of the sky map by the given @p factor.
 ***************************************************************************/
#define G_OP_UNARY_DIV2                        "GSkymap::operator/=(double&)"
GSkymap& GSkymap::operator/=(const double& factor)
{
    // Check for division by zero
    if (factor == 0.0) {
        std::string msg = "Trying to divide sky map pixels by zero.";
        throw GException::invalid_argument(G_OP_UNARY_DIV2, msg);
    }

    // Compute total number of pixels
    int n = npix() * nmaps();

    // Loop over all pixels
    double* pixel = m_pixels;
    for (int i = 0; i < n; ++i) {
        *pixel++ /= factor;
    }

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Pixel index access operator
 *
 * @param[in] index Pixel index [0,...,npix()-1].
 * @param[in] map Map index [0,...,nmaps()-1].
 * @return Sky map pixel value.
 *
 * @exception GException::out_of_range
 *            Pixel index and/or map index are outside valid range.
 *
 * Access sky map pixel by its index, where the most quickly varying axis is
 * the x axis of the map.
 ***************************************************************************/
double& GSkymap::operator()(const int& index, const int& map)
{
    // Throw an error if pixel index or map index is not in valid range
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= m_num_pixels) {
        throw GException::out_of_range(G_OP_ACCESS_1D,
                                       "Sky map pixel index",
                                       index, m_num_pixels);
    }
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_OP_ACCESS_1D,
                                       "Sky map map index",
                                       map, m_num_maps);
    }
    #endif

    // Return reference to pixel value
    return m_pixels[index+m_num_pixels*map];
}


/***********************************************************************//**
 * @brief Pixel index access operator (const variant)
 *
 * @param[in] index Pixel index [0,...,npix()-1].
 * @param[in] map Map index [0,...,nmaps()-1].
 *
 * @exception GException::out_of_range
 *            Pixel index and/or map index are outside valid range.
 *
 * Access sky map pixel by its index, where the most quickly varying axis is
 * the x axis of the map.
 ***************************************************************************/
const double& GSkymap::operator()(const int& index, const int& map) const
{
    // Throw an error if pixel index or map index is not in valid range
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= m_num_pixels) {
        throw GException::out_of_range(G_OP_ACCESS_1D,
                                       "Sky map pixel index",
                                       index, m_num_pixels);
    }
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_OP_ACCESS_1D,
                                       "Sky map map index",
                                       map, m_num_maps);
    }
    #endif

    // Return reference to pixel value
    return m_pixels[index+m_num_pixels*map];
}


/***********************************************************************//**
 * @brief Sky map pixel access operator
 *
 * @param[in] pixel Sky map pixel.
 * @param[in] map Map index [0,...,nmaps()-1].
 *
 * @exception GException::out_of_range
 *            Sky pixel and/or map index are outside valid range.
 *
 * Access sky map pixel by its 2D index (x,y) that is implemented by the
 * GSkyPixel class.
 *
 * @todo Implement proper skymap exception (actual is for matrix elements)
 ***************************************************************************/
double& GSkymap::operator()(const GSkyPixel& pixel, const int& map)
{
    // Throw an error if pixel index or map index is not in valid range
    #if defined(G_RANGE_CHECK)
    if (!contains(pixel)) {
        throw GException::out_of_range(G_OP_ACCESS_2D,
                                       int(pixel.x()), int(pixel.y()),
                                       m_num_x-1, m_num_y-1);
    }
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_OP_ACCESS_2D,
                                       "Sky map map index",
                                       map, m_num_maps);
    }
    #endif

    // Get pixel index
    int index = pix2inx(pixel);

    // Return reference to pixel value
    return m_pixels[index+m_num_pixels*map];
}


/***********************************************************************//**
 * @brief Sky map pixel access operator
 *
 * @param[in] pixel Sky map pixel.
 * @param[in] map Map index [0,...,nmaps()-1].
 *
 * @exception GException::out_of_range
 *            Sky pixel and/or map index are outside valid range.
 *
 * Access sky map pixel by its 2D index (x,y) that is implemented by the
 * GSkyPixel class.
 *
 * @todo Implement proper skymap exception (actual is for matrix elements)
 ***************************************************************************/
const double& GSkymap::operator()(const GSkyPixel& pixel, const int& map) const
{
    // Throw an error if pixel index or map index is not in valid range
    #if defined(G_RANGE_CHECK)
    if (!contains(pixel)) {
        throw GException::out_of_range(G_OP_ACCESS_2D,
                                       int(pixel.x()), int(pixel.y()),
                                       m_num_x-1, m_num_y-1);
    }
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_OP_ACCESS_2D,
                                       "Sky map map index",
                                       map, m_num_maps);
    }
    #endif

    // Get pixel index
    int index = pix2inx(pixel);

    // Return reference to pixel value
    return m_pixels[index+m_num_pixels*map];
}


/***********************************************************************//**
 * @brief Return interpolated skymap value for sky direction
 *
 * @param[in] dir Sky direction.
 * @param[in] map Map index [0,...,nmaps()-1].
 * @return Sky intensity.
 *
 * @exception GException::out_of_range
 *            Map index lies outside valid range.
 *
 * Returns the skymap value for a given sky direction, obtained by bi-linear
 * interpolation of the neighbouring pixels. If the sky direction falls
 * outside the area covered by the skymap, a value of 0 is returned.
 *
 * The method implements a computation cache that avoids recomputation of
 * interpolation indices and weights in case that the same sky direction
 * is requested several times. This speeds up use cases where skymap values
 * for various map indices have to be returned for the same sky direction.
 ***************************************************************************/
double GSkymap::operator()(const GSkyDir& dir, const int& map) const
{
    // Throw an error if the map index is not in valid range
    #if defined(G_RANGE_CHECK)
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_OP_VALUE,
                                       "Sky map map index",
                                       map, m_num_maps);
    }
    #endif

    // Initialise intensity
    double intensity = 0.0;

    // If the pre-computation cache has not been set or if the sky direction
    // has changed then compute the bilinear interpolator
    if (!m_hascache || (dir != m_last_dir)) {

        // Perform computation for HealPix map
        if (m_proj->size() == 1)  {

            // Get Healpix interpolator
            m_interpol = static_cast<GHealpix*>(m_proj)->interpolator(dir);

            // Set containment flag and store actual sky direction as last
            // direction
            m_hascache  = true;
            m_contained = true;
            m_last_dir  = dir;

        } // endif: we had a Healpix projection

        // ... otherwise perform computation for WCS map
        else {

            // Determine sky pixel. At this point an exception may occur
            // in case that the pixel cannot be represented by the
            // relevant projection. We catch this exception here
            try {

                // Determine sky pixel
                GSkyPixel pixel = dir2pix(dir);

                // Set containment flag and store actual sky direction
                // as last direction
                m_hascache  = true;
                m_contained = contains(pixel);
                m_last_dir  = dir;

                // Continue only if pixel is within the map
                if (m_contained) {

                    // Set left indices for interpolation. The left index is
                    // comprised between 0 and npixels-2. By definition, the
                    // right index is then the left index + 1
                    int inx_x = int(pixel.x());
                    int inx_y = int(pixel.y());
                    if (inx_x < 0) {
                        inx_x = 0;
                    }
                    else if (inx_x > m_num_x-2) {
                        inx_x = m_num_x - 2;
                    }
                    if (inx_y < 0) {
                        inx_y = 0;
                    }
                    else if (inx_y > m_num_y-2) {
                        inx_y = m_num_y - 2;
                    }

                    // Set weighting factors for interpolation
                    double wgt_x_right = (pixel.x() - inx_x);
                    double wgt_x_left  = 1.0 - wgt_x_right;
                    double wgt_y_right = (pixel.y() - inx_y);
                    double wgt_y_left  = 1.0 - wgt_y_right;

                    // Compute skymap pixel indices for bi-linear interpolation
                    m_interpol.index1() = inx_x + inx_y * m_num_x;
                    m_interpol.index2() = m_interpol.index1() + m_num_x;
                    m_interpol.index3() = m_interpol.index1() + 1;
                    m_interpol.index4() = m_interpol.index2() + 1;

                    // Compute weighting factors for bi-linear interpolation
                    m_interpol.weight1() = wgt_x_left  * wgt_y_left;
                    m_interpol.weight2() = wgt_x_left  * wgt_y_right;
                    m_interpol.weight3() = wgt_x_right * wgt_y_left;
                    m_interpol.weight4() = wgt_x_right * wgt_y_right;

                } // endif: pixel was contained in map
                
            } // endtry: pixel computation was successful
            catch (GException::wcs_invalid_phi_theta) {
                m_contained = false;
            }
    
        } // endelse: we had a WCS map

    } // endif: computation of the bi-linear interpolater was required

    // Compute the interpolated intensity if the pixel is contained in
    // the map
    if (m_contained) {

        // Compute map offset
        int offset = m_num_pixels * map;

        // Compute interpolated skymap value
        intensity = m_interpol(m_pixels+offset);

    } // endif: direction was contained in map

    // Return intensity
    return intensity;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

 /***********************************************************************//**
 * @brief Clear instance.
 *
 * Resets the sky map to the initial state.
 ***************************************************************************/
void GSkymap::clear(void)
{
    // Free class members
    free_members();

    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone sky map
 *
 * @return Pointer to deep copy of sky map.
 ***************************************************************************/
GSkymap* GSkymap::clone(void) const
{
    return new GSkymap(*this);
}


/***********************************************************************//**
 * @brief Set number of maps
 *
 * @param[in] nmaps Number of maps.
 *
 * @exception GException::invalid_argument
 *            Invalid number of maps specified.
 *
 * Redefines the number of maps in an GSkymap object. If the number of maps
 * is increased with respect to the existing number, additional maps with
 * pixel values of zero are append to the object. Existing map pixel values
 * are kept. If the number of maps is decreased with respect to the existing
 * number, the excedent maps are dropped. The remaining map pixel values are
 * kept.
 ***************************************************************************/
void GSkymap::nmaps(const int& nmaps)
{
    // Throw an exception if less than 1 map is required
    if (nmaps < 1) {
        std::string msg = "At least one map is required in an GSkymap object."
                          " Please specify an argument >=1.";
        throw GException::invalid_argument(G_NMAPS, msg);
    }

    // If the map has pixels and if the number of maps changes then set a new
    // number of maps. Copy over any existing information.
    if (m_num_pixels > 0 && nmaps != m_num_maps) {

        // Compute new skymap size
        int new_size = m_num_pixels * nmaps;

        // Allocate memory for new map
        double* pixels = new double[new_size];

        // Copy over existing pixels
        int num_copy = (nmaps > m_num_maps) ? m_num_maps : nmaps;
        num_copy    *= m_num_pixels;
        for (int i = 0; i < num_copy; ++i) {
            pixels[i] = m_pixels[i];
        }

        // Set any additional pixels to zero
        if (nmaps > m_num_maps) {
            for (int i = num_copy; i < new_size; ++i) {
                pixels[i] = 0.0;
            }
        }

        // Free existing pixels
        if (m_pixels != NULL) delete [] m_pixels;

        // Set pointer to stacked pixels
        m_pixels = pixels;

        // Set number of maps
        m_num_maps = nmaps;

    } // endif: map had pixels

    // Return
    return;
}


/***********************************************************************//**
 * @brief Converts pixel index into sky map pixel
 *
 * @param[in] index Pixel index [0,...,npix()-1].
 * @return Sky map pixel.
 *
 * Converts the pixel @p index into a sky map pixel (GSkyPixel).The dimension
 * of GSkyPixel will be identical to the dimension of the sky map (i.e. a 1D
 * sky map leads to a 1D GSkyPixel object, a 2D sky map leads to a 2D
 * GSkyPixel object.
 ***************************************************************************/
GSkyPixel GSkymap::inx2pix(const int& index) const
{
    // Initialise sky map pixel
    GSkyPixel pixel;

    // Get x and y indices
    if (m_num_x != 0) { //!< 2D sky map
        pixel.x(double(index % m_num_x));
        pixel.y(double(index / m_num_x));
    }
    else {              //!< 1D sky map
        pixel.index(index);
    }

    // Return pixel
    return pixel;
}


/***********************************************************************//**
 * @brief Returns sky direction of pixel
 *
 * @param[in] index Pixel index [0,...,npix()-1].
 * @return Sky direction.
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 *
 * Returns sky direction for a given pixel index.
 ***************************************************************************/
GSkyDir GSkymap::inx2dir(const int& index) const
{
    // Throw error if sky projection is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_INX2DIR, msg);
    }

    // Determine sky direction from pixel index.
    GSkyDir dir = m_proj->pix2dir(inx2pix(index));

    // Return sky direction
    return dir;
}


/***********************************************************************//**
 * @brief Returns sky direction of pixel
 *
 * @param[in] pixel Sky map pixel.
 * @return Sky direction.
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 * @exception GException::invalid_argument
 *            2D sky map pixel used to access 1D projection.
 *
 * Returns sky direction for a given sky map @p pixel.
 ***************************************************************************/
GSkyDir GSkymap::pix2dir(const GSkyPixel& pixel) const
{
    // Throw error if WCS is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_PIX2DIR, msg);
    }

    // Initialise sky direction
    GSkyDir dir;

    // If pixel size matches the projection size then perform a straight
    // forward conversion
    if (m_proj->size() == pixel.size()) {
        dir = m_proj->pix2dir(pixel);
    }

    // ... otherwise, if we have a 2D projection but a 1D pixel then
    // interpret the pixel as the linear index in the pixel array
    else if (m_proj->size() == 2) {
        dir = m_proj->pix2dir(GSkyPixel(inx2pix(int(pixel))));
    }

    // ... otherwise we have a 1D projection but a 2D pixel. There is
    // no unambiguous way to handle this case, hence we throw an exception
    else {
        std::string msg = "A 2-dimensional sky map pixel "+pixel.print()+
                          " is used to determine the sky direction for"
                          " the 1-dimensional sky projection \""+
                          m_proj->name()+"\"\n"
                          "Please specify a 1-dimensional sky map pixel.";
        throw GException::invalid_argument(G_PIX2DIR, msg);
    }

    // Return sky direction
    return dir;
}


/***********************************************************************//**
 * @brief Converts sky map pixel into pixel index
 *
 * @param[in] pixel Sky map pixel.
 * @return Pixel index [0,...,npix()-1].
 *
 * Converts a sky map @p pixel into the pixel index.
 ***************************************************************************/
int GSkymap::pix2inx(const GSkyPixel& pixel) const
{
    // Initialise pixel index
    int index = 0;

    // Handle 1D sky map pixel
    if (pixel.is_1D()) {
        index = int(pixel);
    }

    // Handle 2D sky map pixel
    else if (pixel.is_2D()) {

        // Get x and y indices by rounding the (x,y) values
        int ix = int(pixel.x()+0.5);
        int iy = int(pixel.y()+0.5);

        // Set index
        index = ix + iy * m_num_x;

    }

    // Return index
    return index;
}


/***********************************************************************//**
 * @brief Returns pixel index for a given sky direction
 *
 * @param[in] dir Sky direction.
 * @return Pixel index [0,...,npix()-1].
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 *
 * Returns sky map pixel index for a given sky direction.
 ***************************************************************************/
int GSkymap::dir2inx(const GSkyDir& dir) const
{
    // Throw error if WCS is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_DIR2INX, msg);
    }

    // Determine pixel index for a given sky direction
    int index = pix2inx(m_proj->dir2pix(dir));

    // Return pixel index
    return index;
}


/***********************************************************************//**
 * @brief Returns sky map pixel for a given sky direction
 *
 * @param[in] dir Sky direction.
 * @return Sky map pixel.
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 *
 * Returns sky map pixel for a given sky direction.
 ***************************************************************************/
GSkyPixel GSkymap::dir2pix(const GSkyDir& dir) const
{
    // Throw error if WCS is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_DIR2PIX, msg);
    }

    // Determine pixel for a given sky direction
    GSkyPixel pixel = m_proj->dir2pix(dir);

    // Return pixel
    return pixel;
}


/***********************************************************************//**
 * @brief Returns solid angle of pixel
 *
 * @param[in] index Pixel index [0,...,npix()-1].
 * @return Solid angle (steradians)
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 *
 * Returns the solid angle of the pixel with the specified @p index.
 ***************************************************************************/
double GSkymap::solidangle(const int& index) const
{
    // Throw error if WCS is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_SOLIDANGLE1, msg);
    }

    // Determine solid angle from pixel index.
    double solidangle = m_proj->solidangle(inx2pix(index));

    // Return solid angle
    return solidangle;
}


/***********************************************************************//**
 * @brief Returns solid angle of pixel
 *
 * @param[in] pixel Sky map pixel.
 * @return Solid angle (steradians)
 *
 * @exception GException::invalid_value
 *            No valid sky projection found.
 *
 * Returns the solid angle of the specified sky map @p pixel.
 ***************************************************************************/
double GSkymap::solidangle(const GSkyPixel& pixel) const
{
    // Throw error if WCS is not valid
    if (m_proj == NULL) {
        std::string msg = "Sky projection has not been defined.";
        throw GException::invalid_value(G_SOLIDANGLE2, msg);
    }

    // Initialise solid angle
    double solidangle = 0.0;

    // If pixel size matches the projection size then perform a straight
    // forward solid angle determination
    if (m_proj->size() == pixel.size()) {
        solidangle = m_proj->solidangle(pixel);
    }

    // ... otherwise, if we have a 2D projection but a 1D pixel then
    // interpret the pixel as the linear index in the pixel array
    else if (m_proj->size() == 2) {
        solidangle = m_proj->solidangle(GSkyPixel(inx2pix(int(pixel))));
    }

    // ... otherwise we have a 1D projection but a 2D pixel. There is
    // no unambiguous way to handle this case, hence we throw an exception
    else {
        std::string msg = "A 2-dimensional sky map pixel "+pixel.print()+
                          " is used to determine the solid angle for"
                          " the 1-dimensional sky projection \""+
                          m_proj->name()+"\"\n"
                          "Please specify a 1-dimensional sky map pixel.";
        throw GException::invalid_argument(G_SOLIDANGLE2, msg);
    }

    // Return solid angle
    return solidangle;
}


/***********************************************************************//**
 * @brief Set sky projection
 *
 * @param[in] proj Sky projection.
 *
 * Sets the projection from celestial to pixel coordinates. The method
 * performs a deep copy of @p proj, allowing to destroy the argument after
 * using the method.
 *
 * Warning: this method may corrupt the GSkymap object as it allows assigning
 * for example a 1D projection to a 2D skymap. Please use this method only
 * when you know what you're doing.
 *
 * @todo We may restrict this method to not allow changing the projection
 * dimension.
 ***************************************************************************/
void GSkymap::projection(const GSkyProjection& proj)
{
    // Free any existing WCS
    if (m_proj != NULL) delete m_proj;

    // Clone input WCS
    m_proj = proj.clone();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Verifies if sky direction falls in map
 *
 * @param[in] dir Sky direction.
 *
 * This method checks if the specified sky direction falls within the pixels
 * covered by the skymap. The method uses the dir2xy method to convert the
 * sky direction into 2D pixel indices, and then checks whether the pixel
 * indices fall in the skymap.
 ***************************************************************************/
bool GSkymap::contains(const GSkyDir& dir) const
{
    // Convert sky direction into sky pixel
    GSkyPixel pixel = dir2pix(dir);
    
    // Return location flag
    return (contains(pixel));
}


/***********************************************************************//**
 * @brief Checks if sky map pixel falls in map
 *
 * @param[in] pixel Sky map pixel.
 * @return Trus if pixels is within map, false otherwise.
 *
 * Checks whether the specified sky map @p pixel falls within the skymap
 * or not. 
 ***************************************************************************/
bool GSkymap::contains(const GSkyPixel& pixel) const
{
    // Initialise containment flag
    bool inmap = false;

    // Test 1D pixels
    if (pixel.is_1D()) {
        if (pixel.index()+0.5 >= 0.0 && pixel.index()-0.5 < m_num_pixels) {
            inmap = true;
        }
    }

    // Test 2D pixels
    else if (pixel.is_2D()) {

        // If pixel is in range then set containment flag to true
        if ((pixel.x()+0.5 >= 0.0 && pixel.x()-0.5 < m_num_x) &&
            (pixel.y()+0.5 >= 0.0 && pixel.y()-0.5 < m_num_y)) {
            inmap = true;
        }

    }

    // Return containment flag
    return inmap;
}


/***********************************************************************//**
 * @brief Extract maps into a new sky map object
 *
 * @param[in] map First map to extract
 * @param[in] nmaps Number of maps to extract (default: 1)
 * @return Extracted map(s).
 *
 * @exception GException::out_of_range
 *            First map index outside valid range
 * @exception GException::invalid_argument
 *            Requested number of maps are not available.
 *
 * Extracts @p nmaps sky maps starting from @p map from the sky map.
 ***************************************************************************/
GSkymap GSkymap::extract(const int& map, const int& nmaps) const
{
    // Throw an exception if the first map index is invalid
    if (map < 0 || map >= m_num_maps) {
        throw GException::out_of_range(G_EXTRACT, "Sky map index", map,
                                       m_num_maps);
    }
 
    // Throw an exception if the number of maps is invalid
    if (nmaps < 0) {
        std::string msg = "The number of maps to extract cannot be negative "
                          "(nmaps="+gammalib::str(nmaps)+").";
        throw GException::invalid_argument(G_EXTRACT, msg);
    }
    if (nmaps > m_num_maps-map) {
        std::string msg = "The number of maps to extract ("+
                          gammalib::str(nmaps)+") exceeds the number of maps "
                          "that are available ("+
                          gammalib::str(m_num_maps-map)+").";
        throw GException::invalid_argument(G_EXTRACT, msg);
    }

    // Compute memory size for extracted map
    int n_size = m_num_pixels * nmaps;

    // Allocate memory for extracted maps (handle the case that the
    // extracted map can be empty)
    double* pixels = NULL;
    if (n_size > 0) {
        pixels = new double[n_size];
    }

    // Extract pixels
    double *src = m_pixels + map*m_num_pixels;
    double *dst = pixels;
    for (int i = 0; i < n_size; ++i) {
        *dst++ = *src++;
    }

    // Create a copy of the map
    GSkymap result = *this;

    // Delete pixels from that map
    if (result.m_pixels != NULL) delete [] result.m_pixels;

    // Attach copied pixels to the map
    result.m_pixels = pixels;

    // Set number of maps
    result.m_num_maps = nmaps;

    // Return map
    return result;
}


/***********************************************************************//**
 * @brief Stack all maps into a single map
 *
 * @return Stacked map.
 *
 * The methods replaces the sky map by a version with only a single map
 * by summing over the pixel values for all maps. If the sky map has no
 * pixels or there is only a single map in the object, the method does
 * nothing.
 ***************************************************************************/
void GSkymap::stack_maps(void)
{
    // Continue only if the map has pixels and if there is more than 1 map
    if (m_num_pixels > 0 && m_num_maps > 1) {

        // Allocate memory for stacked map
        double* pixels = new double[m_num_pixels];

        // Stack map and save in memory
        for (int i = 0; i < m_num_pixels; ++i) {
            double sum = 0.0;
            for (int k = 0; k < m_num_maps; ++k) {
                sum += (*this)(i,k);
            }
            pixels[i] = sum;
        }

        // Free existing pixels
        if (m_pixels != NULL) delete [] m_pixels;

        // Set pointer to stacked pixels
        m_pixels = pixels;

        // Set number of maps to 1
        m_num_maps = 1;
    
    } // endif: map had pixels

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load skymap from FITS file.
 *
 * @param[in] filename FITS file name..
 *
 * Loads HEALPix and non HEALPix skymaps. First searches for HEALPix map in
 * FITS file by scanning all HDUs for PIXTYPE=HEALPIX. If no HEALPix map has
 * been found then search load first non-empty image.
 *
 * @todo Do we have to restrict a HEALPix map to a BinTable and a WCS map
 * to a Double precision image???
 ***************************************************************************/
void GSkymap::load(const std::string& filename)
{
    // Free memory and initialise members
    free_members();
    init_members();

    // Open FITS file
    GFits fits(filename);

    // Get number of HDUs
    int num = fits.size();

    // Initialize load flag
    bool loaded = false;

    // First search for HEALPix extension. We can skip the first extension
    // since this is always an image and a HEALPix map is stored in a
    // binary table
    for (int extno = 1; extno < num; ++extno) {

        // Get reference to HDU
        const GFitsHDU& hdu = *fits.at(extno);
        
        // If PIXTYPE keyword equals "HEALPIX" then load map
        if (hdu.has_card("PIXTYPE") && hdu.string("PIXTYPE") == "HEALPIX") {
            read_healpix(static_cast<const GFitsTable&>(hdu));
            loaded = true;
            break;
        }

    } // endfor: looped over HDUs

    // If we have not found a HEALPIX map then search now for image.
    // Skip empty images
    if (!loaded) {
        for (int extno = 0; extno < num; ++extno) {

            // Get referene to HDU
            const GFitsHDU& hdu = *fits.at(extno);

            // Skip if extension is not an image
            if (extno > 0) {
                if (hdu.string("XTENSION") != "IMAGE")
                    continue;
            }

            // Load WCS map
            read_wcs(static_cast<const GFitsImage&>(hdu));
            //loaded = true;
            break;

        } // endfor: looped over HDUs
    } // endif: no HEALPix map found

    // Close FITS file
    fits.close();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Save skymap into FITS file.
 *
 * @param[in] filename FITS file name.
 * @param[in] clobber Overwrite existing file? (true=yes)
 *
 * The method does nothing if the skymap holds no valid WCS.
 ***************************************************************************/
void GSkymap::save(const std::string& filename, bool clobber) const
{
    // Continue only if we have data to save
    if (m_proj != NULL) {

        // Initialise HDU pointer
        GFitsHDU* hdu = NULL;

        // Case A: Skymap is Healpix
        if (m_proj->code() == "HPX") {
            hdu = create_healpix_hdu();
        }

        // Case B: Skymap is not Healpix
        else {
            hdu = create_wcs_hdu();
        }

        // Create FITS file and save it to disk
        if (hdu != NULL) {
            GFits fits;
            fits.append(*hdu);
            fits.saveto(filename, clobber);
        }

        // Delete HDU
        if (hdu != NULL) delete hdu;

    } // endif: we had data to save

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read skymap from FITS HDU
 *
 * @param[in] hdu FITS HDU.
 ***************************************************************************/
void GSkymap::read(const GFitsHDU& hdu)
{
    // Free memory and initialise members
    free_members();
    init_members();

    // Initialize load flag
    bool loaded = false;

    // If PIXTYPE keyword equals "HEALPIX" then load map
    if (hdu.has_card("PIXTYPE") && hdu.string("PIXTYPE") == "HEALPIX") {
        read_healpix(static_cast<const GFitsTable&>(hdu));
        loaded = true;
    }

    // ... otherwise try loading as non HEALPix map
    if (!loaded) {

        // Load only if HDU contains an image
        if (hdu.exttype() == 0) {
            read_wcs(static_cast<const GFitsImage&>(hdu));
            //loaded = true;
        }

    } // endif

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write skymap into FITS file
 *
 * @param[in] file FITS file pointer.
 ***************************************************************************/
void GSkymap::write(GFits& file) const
{
    // Continue only if we have data to save
    if (m_proj != NULL) {

        // Initialise HDU pointer
        GFitsHDU* hdu = NULL;

        // Case A: Skymap is Healpix
        if (m_proj->code() == "HPX") {
            hdu = create_healpix_hdu();
        }

        // Case B: Skymap is not Healpix
        else {
            hdu = create_wcs_hdu();
        }

        // Append HDU to FITS file.
        if (hdu != NULL) {
            file.append(*hdu);
        }

        // Delete HDU
        if (hdu != NULL) delete hdu;

    } // endif: we had data to save

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print sky map
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing sky map information.
 ***************************************************************************/
std::string GSkymap::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GSkymap ===");

        // Append information
        result.append("\n"+gammalib::parformat("Number of pixels"));
        result.append(gammalib::str(m_num_pixels));
        result.append("\n"+gammalib::parformat("Number of maps"));
        result.append(gammalib::str(m_num_maps));
        if (m_proj != NULL && m_proj->size() == 2) {
            result.append("\n"+gammalib::parformat("X axis dimension"));
            result.append(gammalib::str(m_num_x));
            result.append("\n"+gammalib::parformat("Y axis dimension"));
            result.append(gammalib::str(m_num_y));
        }

        // Append sky projection information
        if (gammalib::reduce(chatter) > SILENT) {
            if (m_proj != NULL) {
                result.append("\n"+m_proj->print(gammalib::reduce(chatter)));
            }
            else {
                result.append("\n"+gammalib::parformat("Sky projection"));
                result.append("not defined");
            }
        }

    } // endif: chatter was not silent

    // Return result
    return result;
}

/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GSkymap::init_members(void)
{
    // Initialise members
    m_num_pixels = 0;
    m_num_maps   = 0;
    m_num_x      = 0;
    m_num_y      = 0;
    m_proj       = NULL;
    m_pixels     = NULL;

    // Initialise computation cache
    m_hascache  = false;
    m_contained = false;
    m_last_dir.clear();
    m_interpol.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Allocate skymap pixels
 ***************************************************************************/
void GSkymap::alloc_pixels(void)
{
    // Compute data size
    int size = m_num_pixels * m_num_maps;

    // Continue only if there are pixels
    if (size > 0) {

        // Allocate pixels and initialize them to 0
        m_pixels = new double[size];
        for (int i = 0; i < size; ++i) {
            m_pixels[i] = 0.0;
        }

    } // endif: there were pixels

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] map Sky map.
 ***************************************************************************/
void GSkymap::copy_members(const GSkymap& map)
{
    // Copy attributes
    m_num_pixels = map.m_num_pixels;
    m_num_maps   = map.m_num_maps;
    m_num_x      = map.m_num_x;
    m_num_y      = map.m_num_y;

    // Copy computation cache
    m_hascache  = map.m_hascache;
    m_contained = map.m_contained;
    m_last_dir  = map.m_last_dir;
    m_interpol  = map.m_interpol;

    // Clone sky projection if it is valid
    if (map.m_proj != NULL) m_proj = map.m_proj->clone();

    // Compute data size
    int size = m_num_pixels * m_num_maps;

    // Copy pixels
    if (size > 0 && map.m_pixels != NULL) {
        alloc_pixels();
        for (int i = 0; i < size; ++i) {
            m_pixels[i] = map.m_pixels[i];
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GSkymap::free_members(void)
{
    // Free memory
    if (m_proj   != NULL) delete m_proj;
    if (m_pixels != NULL) delete [] m_pixels;

    // Signal free pointers
    m_proj       = NULL;
    m_pixels     = NULL;

    // Reset number of pixels
    m_num_pixels = 0;
    m_num_maps   = 0;
    m_num_x      = 0;
    m_num_y      = 0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set World Coordinate System
 *
 * @param[in] wcs World Coordinate System code.
 * @param[in] coords Coordinate system.
 * @param[in] crval1 X value of reference pixel.
 * @param[in] crval2 Y value of reference pixel.
 * @param[in] crpix1 X index of reference pixel.
 * @param[in] crpix2 Y index of reference pixel.
 * @param[in] cdelt1 Increment in x direction at reference pixel (deg).
 * @param[in] cdelt2 Increment in y direction at reference pixel (deg).
 * @param[in] cd Astrometry parameters (2x2 matrix, deg/pixel).
 * @param[in] pv2 Projection parameters (length WCS type dependent).
 *
 * @exception GException::wcs_invalid
 *            Invalid wcs parameter (World Coordinate System not known).
 *
 * This method sets the WCS projection pointer based on the WCS code and
 * sky map parameters. It makes use of the GWcsRegistry class to allocate
 * the correct derived class. Note that this method does not support the
 * HPX projection.
 *
 * @todo Remove cd and pv2 parameters.
 ***************************************************************************/
void GSkymap::set_wcs(const std::string& wcs, const std::string& coords,
                      const double& crval1, const double& crval2,
                      const double& crpix1, const double& crpix2,
                      const double& cdelt1, const double& cdelt2,
                      const GMatrix& cd, const GVector& pv2)
{
    // Convert WCS to upper case
    std::string uwcs = gammalib::toupper(wcs);
    
    // Check if HPX was requested (since this is not allowed)
    if (uwcs == "HPX") {
       throw GException::wcs_invalid(G_SET_WCS, uwcs,
                                     "Method not valid for HPX projection.");
    }
    
    // ... otherwise get projection from registry
    else {
        // Allocate WCS registry
        GWcsRegistry registry;
        
        // Allocate projection from registry
        GWcs* projection = registry.alloc(uwcs);
        m_proj           = projection;

        // Signal if projection type is not known
        if (projection == NULL) {
            std::string message = "Projection code not known. "
                                  "Should be one of "+registry.list()+".";
            throw GException::wcs_invalid(G_SET_WCS, uwcs, message);
        }

        // Setup WCS
        projection->set(coords, crval1, crval2, crpix1, crpix2,
                        cdelt1, cdelt2);
    
    } // endelse: got projection from registry

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read Healpix data from FITS table.
 *
 * @param[in] table FITS table.
 *
 * HEALPix data may be stored in various formats depending on the 
 * application that has writted the data. HEALPix IDL, for example, may
 * store the data in vectors of length 1024 if the number of pixels is
 * a multiple of 1024. On the other hand, vectors may also be used to store
 * several HEALPix maps into a single column. Alternatively, multiple maps
 * may be stored in multiple columns.
 ***************************************************************************/
void GSkymap::read_healpix(const GFitsTable& table)
{
    // Determine number of rows and columns in table
    int nrows = table.nrows();
    int ncols = table.ncols();
    #if defined(G_READ_HEALPIX_DEBUG)
    std::cout << "nrows=" << nrows << " ncols=" << ncols << std::endl;
    #endif

    // Allocate Healpix projection
    m_proj = new GHealpix;

    // Read WCS information from FITS header
    m_proj->read(table);

    // Set number of pixels based on NSIDE parameter
    m_num_pixels = static_cast<GHealpix*>(m_proj)->npix();
    #if defined(G_READ_HEALPIX_DEBUG)
    std::cout << "m_num_pixels=" << m_num_pixels << std::endl;
    #endif

    // Number of map pixels has to be a multiple of the number of
    // rows in column
    if (m_num_pixels % nrows != 0) {
        throw GException::skymap_bad_size(G_READ_HEALPIX, nrows,
                                          m_num_pixels);
    }

    // Determine vector length for HEALPix data storage
    int nentry = m_num_pixels / nrows;
    #if defined(G_READ_HEALPIX_DEBUG)
    std::cout << "nentry=" << nentry << std::endl;
    #endif

    // Determine number of maps from the number of maps that fit into
    // all columns. Only count columns that can fully hold the map.
    m_num_maps = 0;
    for (int icol = 0; icol < ncols; ++icol) {
        const GFitsTableCol* col = table[icol];
        if (col->number() % nentry == 0) {
            m_num_maps += col->number() / nentry;
        }
    }
    #if defined(G_READ_HEALPIX_DEBUG)
    std::cout << "m_num_maps=" << m_num_maps << std::endl;
    #endif

    // Allocate pixels to hold the map
    alloc_pixels();

    // Initialise map counter
    int imap = 0;

    // Loop over all columns
    for (int icol = 0; icol < ncols; ++icol) {

        // Get next column
        const GFitsTableCol* col = table[icol];

        // Only consider columns that can fully hold maps
        if (col->number() % nentry == 0) {

            // Determine number of maps in column
            int num = col->number() / nentry;

            // Loop over all maps in column
            int inx_start = 0;
            int inx_end   = nentry;
            for (int i = 0; i < num; ++i) {

                // Load map
                double *ptr = m_pixels + m_num_pixels*imap;
                for (int row = 0; row < col->length(); ++row) {
                    for (int inx = inx_start; inx < inx_end; ++inx) {
                        *ptr++ = col->real(row,inx);
                    }
                }
                #if defined(G_READ_HEALPIX_DEBUG)
                std::cout << "Load map=" << imap << " index="
                          << inx_start << "-" << inx_end << std::endl;
                #endif

                // Increment index range
                inx_start  = inx_end;
                inx_end   += nentry;

                // Increment map counter
                imap++;

                // Break if we have loaded all maps
                if (imap >= m_num_maps) {
                    break;
                }

            } // endfor: looped over all maps in column
        } // endif: column could fully hold maps

        // Break if we have loaded all maps
        if (imap >= m_num_maps) {
            break;
        }

    } // endfor: looped over all columns

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read WCS image from FITS HDU
 *
 * @param[in] image FITS image.
 *
 * @exception GException::skymap_bad_image_dim
 *            WCS image has invalid dimension (naxis=2 or 3).
 ***************************************************************************/
void GSkymap::read_wcs(const GFitsImage& image)
{
    // Allocate WCS
    alloc_wcs(image);

    // Read projection information from FITS header
    m_proj->read(image);

    // Extract map dimension and number of maps from image
    if (image.naxis() == 2) {
        m_num_x    = image.naxes(0);
        m_num_y    = image.naxes(1);
        m_num_maps = 1;
    }
    else if (image.naxis() >= 3) {
        m_num_x    = image.naxes(0);
        m_num_y    = image.naxes(1);
        m_num_maps = image.naxes(2);
    }
    else {
        throw GException::skymap_bad_image_dim(G_READ_WCS, image.naxis());
    }
    #if defined(G_READ_WCS_DEBUG)
    std::cout << "m_num_x=" << m_num_x << std::endl;
    std::cout << "m_num_y=" << m_num_y << std::endl;
    std::cout << "m_num_maps=" << m_num_maps << std::endl;
    #endif

    // Compute number of pixels
    m_num_pixels = m_num_x * m_num_y;
    #if defined(G_READ_WCS_DEBUG)
    std::cout << "m_num_pixels=" << m_num_pixels << std::endl;
    #endif

    // Allocate pixels to hold the map
    alloc_pixels();

    // Read image
    if (image.naxis() == 2) {
        double* ptr = m_pixels;
        for (int iy = 0; iy < m_num_y; ++iy) {
            for (int ix = 0; ix < m_num_x; ++ix) {
                *ptr++ = image.pixel(ix,iy);
            }
        }
    }
    else {
        double* ptr = m_pixels;
        for (int imap = 0; imap < m_num_maps; ++imap) {
            for (int iy = 0; iy < m_num_y; ++iy) {
                for (int ix = 0; ix < m_num_x; ++ix) {
                    *ptr++ = image.pixel(ix,iy,imap);
                }
            }
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Allocate WCS class
 *
 * @param[in] image FITS image.
 *
 * @exception GException::fits_key_not_found
 *            Unable to find required FITS header keyword.
 * @exception GException::skymap_bad_ctype
 *            CTYPE1 and CTYPE2 keywords are incompatible.
 * @exception GException::wcs_invalid
 *            WCS projection of FITS file not supported by GammaLib.
 ***************************************************************************/
void GSkymap::alloc_wcs(const GFitsImage& image)
{
    // Get standard keywords
    std::string ctype1 = image.string("CTYPE1");
    std::string ctype2 = image.string("CTYPE2");

    // Extract projection type
    std::string xproj = ctype1.substr(5,3);
    std::string yproj = ctype2.substr(5,3);

    // Check that projection type is identical on both axes
    if (xproj != yproj) {
        throw GException::skymap_bad_ctype(G_ALLOC_WCS,
                                           ctype1, ctype2);
    }

    // Allocate WCS registry
    GWcsRegistry registry;
    
    // Allocate projection from registry
    m_proj = registry.alloc(xproj);
    
    // Signal if projection type is not known
    if (m_proj == NULL) {
        std::string message = "Projection code not known. "
                              "Should be one of "+registry.list()+".";
        throw GException::wcs_invalid(G_ALLOC_WCS, xproj, message);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Create FITS HDU containing Healpix data
 *
 * This method allocates a binary table HDU that contains the Healpix data.
 * Deallocation of the table has to be done by the client.
 ***************************************************************************/
GFitsBinTable* GSkymap::create_healpix_hdu(void) const
{
    // Initialise result to NULL pointer
    GFitsBinTable* hdu = NULL;

    // Compute size of Healpix data
    int size = m_num_pixels * m_num_maps;

    // Continue only if we have pixels
    if (size > 0) {

        // Set number of rows and columns
        int rows   = m_num_pixels;
        int number = m_num_maps;

        // Create column to hold Healpix data
        GFitsTableDoubleCol column = GFitsTableDoubleCol("DATA", rows, number);

        // Fill data into column
        double* ptr = m_pixels;
        for (int inx = 0; inx < number; ++inx) {
            for (int row = 0; row < rows; ++row) {
                column(row,inx) = *ptr++;
            }
        }

        // Create HDU that contains Healpix map in a binary table
        hdu = new GFitsBinTable(rows);
        hdu->append(column);

    } // endif: there were pixels

    // ... otherwise create an empty header
    else {
        hdu = new GFitsBinTable;
    }

    // Set extension name
    hdu->extname("HEALPIX");

    // If we have WCS information then write into FITS header
    if (m_proj != NULL) m_proj->write(*hdu);

    // Set additional keywords
    hdu->card("NBRBINS", m_num_maps, "Number of HEALPix maps");

    // Return HDU
    return hdu;
}


/***********************************************************************//**
 * @brief Create FITS HDU containing WCS image
 *
 * This method allocates an image HDU that contains the WCS image data.
 * Deallocation of the image has to be done by the client.
 *
 * @todo Set additional keywords.
 ***************************************************************************/
GFitsImageDouble* GSkymap::create_wcs_hdu(void) const
{
    // Initialise result to NULL pointer
    GFitsImageDouble* hdu = NULL;

    // Compute size of Healpix data
    int size = m_num_pixels * m_num_maps;

    // Continue only if we have pixels
    if (size > 0) {

        // Set axis parameters for image construction
        int naxis   = (m_num_maps == 1) ? 2 : 3;
        int naxes[] = {m_num_x, m_num_y, m_num_maps};

        // Allocate image
        hdu = new GFitsImageDouble(naxis, naxes);

        // Store data in image
        if (naxis == 2) {
            double* ptr = m_pixels;
            for (int iy = 0; iy < m_num_y; ++iy) {
                for (int ix = 0; ix < m_num_x; ++ix) {
                    (*hdu)(ix,iy) = *ptr++;
                }
            }
        }
        else {
            double* ptr = m_pixels;
            for (int imap = 0; imap < m_num_maps; ++imap) {
                for (int iy = 0; iy < m_num_y; ++iy) {
                    for (int ix = 0; ix < m_num_x; ++ix) {
                        (*hdu)(ix,iy,imap) = *ptr++;
                    }
                }
            }
        }

    } // endif: there were pixels

    // ... otherwise create an empty header
    else {
        hdu = new GFitsImageDouble;
    }

    // Set extension name
    hdu->extname("IMAGE");

    // If we have sky projection information then write into FITS header
    if (m_proj != NULL) m_proj->write(*hdu);

    // Set additional keywords
    //TODO

    // Return HDU
    return hdu;
}


/*==========================================================================
 =                                                                         =
 =                                 Friends                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Computes square root of skymap elements
 *
 * @param[in] map Skymap.
 * @return Skymap containing the square root of every element.
 ***************************************************************************/
GSkymap sqrt(const GSkymap& map)
{
    // Initialise result vector
    GSkymap result(map);

    // Loop over all maps
    for (int i = 0; i < map.nmaps(); ++i) {

        // Loop over all bins
        for (int j = 0; j < map.npix(); ++j) {

            // Get the content from the bin
            double content = map(j,i);

    		// Check if content is not negative
    		if (content < 0.0) {
                std::string msg = "Negative value envountered."
                                  " Cannot take the sqrt from a negative value";
                throw GException::invalid_value(G_OP_UNARY_DIV, msg);
            }

            // Set content of the result map
            result(j,i) = std::sqrt(content);

        } // endfor: Loop over all bins

    } // endfor: Loop over all maps

    // Return vector
    return result;
}
