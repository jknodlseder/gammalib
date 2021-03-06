/***************************************************************************
 *           GModelSpatialDiffuseMap.cpp - Spatial map model class         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2012-2015 by Juergen Knoedlseder                         *
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
 * @file GModelSpatialDiffuseMap.cpp
 * @brief Spatial map model class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GMath.hpp"
#include "GModelSpatialDiffuseMap.hpp"
#include "GModelSpatialRegistry.hpp"

/* __ Constants __________________________________________________________ */

/* __ Globals ____________________________________________________________ */
const GModelSpatialDiffuseMap g_spatial_map_seed;
const GModelSpatialRegistry   g_spatial_map_registry(&g_spatial_map_seed);

/* __ Method name definitions ____________________________________________ */
#define G_READ                  "GModelSpatialDiffuseMap::read(GXmlElement&)"
#define G_WRITE                "GModelSpatialDiffuseMap::write(GXmlElement&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_DEBUG_CACHE              //!< Debug Monte Carlo Cache computation
//#define G_DEBUG_PREPARE            //!< Debug map preparation


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModelSpatialDiffuseMap::GModelSpatialDiffuseMap(void) :
                         GModelSpatialDiffuse()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief XML constructor
 *
 * @param[in] xml XML element.
 *
 * Constructs spatial map model by extracting information from an XML
 * element. See the method read() for more information about the expected
 * structure of the XML element.
 ***************************************************************************/
GModelSpatialDiffuseMap::GModelSpatialDiffuseMap(const GXmlElement& xml) :
                         GModelSpatialDiffuse()
{
    // Initialise members
    init_members();

    // Read information from XML element
    read(xml);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Filename constructor
 *
 * @param[in] filename File name.
 * @param[in] value Normalization factor (defaults to 1).
 * @param[in] normalize Normalize map (defaults to true).
 *
 * Constructs spatial map model by loading a skymap from the file specified
 * by @p filename and by setting the normalization @p value.
 ***************************************************************************/
GModelSpatialDiffuseMap::GModelSpatialDiffuseMap(const std::string& filename,
                                                 const double&      value,
                                                 const bool&        normalize) :
                         GModelSpatialDiffuse()
{
    // Initialise members
    init_members();

    // Set normalization parameter
    m_value.value(value);

    // Set normalization flag
    m_normalize = normalize;

    // Load skymap
    load(filename);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Sky map constructor
 *
 * @param[in] map Sky map.
 * @param[in] value Normalization factor (defaults to 1).
 * @param[in] normalize Normalize map (defaults to true).
 *
 * Constructs spatial map model by setting the sky @p map and by setting the
 * normalization @p value.
 ***************************************************************************/
GModelSpatialDiffuseMap::GModelSpatialDiffuseMap(const GSkymap& map,
                                                 const double&  value,
                                                 const bool&    normalize) :
                         GModelSpatialDiffuse()
{
    // Initialise members
    init_members();

    // Set normalization parameter
    m_value.value(value);

    // Set normalization flag
    m_normalize = normalize;

    // Set and prepare skymap
    m_map = map;
    prepare_map();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Spatial map model.
 ***************************************************************************/
GModelSpatialDiffuseMap::GModelSpatialDiffuseMap(const GModelSpatialDiffuseMap& model) :
                         GModelSpatialDiffuse(model)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(model);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelSpatialDiffuseMap::~GModelSpatialDiffuseMap(void)
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
 * @param[in] model Spatial map model.
 * @return Spatial map model.
 ***************************************************************************/
GModelSpatialDiffuseMap& GModelSpatialDiffuseMap::operator=(const GModelSpatialDiffuseMap& model)
{
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelSpatialDiffuse::operator=(model);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(model);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                            Public methods                               =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear diffuse map
 ***************************************************************************/
void GModelSpatialDiffuseMap::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GModelSpatialDiffuse::free_members();
    this->GModelSpatial::free_members();

    // Initialise members
    this->GModelSpatial::init_members();
    this->GModelSpatialDiffuse::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone diffuse map
 *
 * @return Pointer to deep copy of diffuse map model.
 ***************************************************************************/
GModelSpatialDiffuseMap* GModelSpatialDiffuseMap::clone(void) const
{
    // Clone diffuse map
    return new GModelSpatialDiffuseMap(*this);
}


/***********************************************************************//**
 * @brief Return intensity of skymap
 *
 * @param[in] photon Incident photon.
 * @return Sky map intensity.
 *
 * Returns the intensity of the skymap at the specified sky direction
 * multiplied by the normalization factor. If the sky direction falls outside
 * the skymap, an intensity of 0 is returned.
 ***************************************************************************/
double GModelSpatialDiffuseMap::eval(const GPhoton& photon) const
{
    // Get skymap intensity
    double intensity = m_map(photon.dir());

    // Return intensity times normalization factor
    return (intensity * m_value.value());
}


/***********************************************************************//**
 * @brief Return intensity of skymap and gradient
 *
 * @param[in] photon Incident photon.
 * @return Sky map intensity.
 *
 * Returns the intensity of the skymap at the specified sky direction
 * multiplied by the normalization factor. The method also sets the gradient
 * with respect to the normalization factor. If the sky direction falls
 * outside the skymap, an intensity of 0 is returned.
 ***************************************************************************/
double GModelSpatialDiffuseMap::eval_gradients(const GPhoton& photon) const
{
    // Get skymap intensity
    double intensity = m_map(photon.dir());

    // Compute partial derivatives of the parameter values
    double g_value = (m_value.is_free()) ? intensity * m_value.scale() : 0.0;

    // Set gradient to 0 (circumvent const correctness)
    const_cast<GModelSpatialDiffuseMap*>(this)->m_value.factor_gradient(g_value);

    // Return intensity times normalization factor
    return (intensity * m_value.value());
}


/***********************************************************************//**
 * @brief Returns MC sky direction
 *
 * @param[in] energy Photon energy (ignored).
 * @param[in] time Photon arrival time (ignored).
 * @param[in,out] ran Random number generator.
 * @return Sky direction.
 *
 * Returns a random sky direction according to the intensity distribution of
 * the model sky map. It makes use of a cache array that contains the
 * normalized cumulative flux values of the skymap. Using a uniform random
 * number, this cache array is scanned using a bi-section method to determine
 * the skymap pixel for which the position should be returned. To avoid
 * binning problems, the exact position within the pixel is set by a uniform
 * random number generator. In case of a 2D map, this is done by randomizing
 * the skymap pixel values (neglecting thus pixel distortions). In case of a
 * HealPix map, this is done by selecing a random sky position within the
 * HealPix map.
 *
 * @todo: Improve HealPix randomization
 ***************************************************************************/
GSkyDir GModelSpatialDiffuseMap::mc(const GEnergy& energy,
                                    const GTime&   time,
                                    GRan&          ran) const
{
    // Allocate sky direction
    GSkyDir dir;

    // Determine number of skymap pixels
    int npix = m_map.npix();

    // Continue only if there are skymap pixels
    if (npix > 0) {

        // Get pixel index from CDF
    	int index = ran.cdf(m_mc_cache);

    	// Convert sky map index to sky map pixel
    	GSkyPixel pixel = m_map.inx2pix(index);

    	// If we have a 2D pixel then randomize pixel values and convert them
        // into a sky direction
        if (pixel.is_2D()) {
            pixel.x(pixel.x() + ran.uniform() - 0.5);
            pixel.y(pixel.y() + ran.uniform() - 0.5);
            dir = m_map.pix2dir(pixel);
        }

        // ... otherwise convert pixel into sky direction and randomize
        // position. We use here a kluge to compute the radius that contains
        // a HealPix pixel by setting this radius to twice the half angle
        // subtended by the solid angle of the pixel. We then use a while
        // loop to randomize the position within that circle and to retain
        // only directions that result in the same pixel index.
        else {
            dir = m_map.pix2dir(pixel);
            GSkyDir randomized_dir;
            int     randomized_index = -1;
            double  radius           = 2.0 * std::acos(1.0 - m_map.solidangle(index)/gammalib::twopi);
            double  cosrad           = std::cos(radius);
            while (randomized_index != index) {
                randomized_dir = dir;
                double theta   = std::acos(1.0 - ran.uniform() * (1.0 - cosrad)) * gammalib::rad2deg;
                double phi     = 360.0 * ran.uniform();
                randomized_dir.rotate_deg(phi, theta);
                randomized_index = m_map.dir2inx(randomized_dir);
//std::cout << radius << " " << randomized_index << " " << index << " " << randomized_dir << std::endl;
            }
            dir = randomized_dir;
        }

    } // endif: there were pixels in sky map

    // Return sky direction
    return dir;
}


/***********************************************************************//**
 * @brief Signals whether model contains sky direction
 *
 * @param[in] dir Sky direction.
 * @param[in] margin Margin to be added to sky direction (degrees)
 * @return True if the model contains the sky direction.
 *
 * Signals whether a sky direction falls within the bounding circle of
 * the diffuse map.
 ***************************************************************************/
bool GModelSpatialDiffuseMap::contains(const GSkyDir& dir,
                                       const double&  margin) const
{
    // Initialise containment flag
    bool contains = false;

    // Continue only if radius is positive
    if (m_radius > 0.0) {

        // Compute distance to centre
        double distance = m_centre.dist_deg(dir);

        // If distance is smaller than radius plus margin we consider
        // the position to be contained within the bounding circle
        if (distance < m_radius + margin) {
            contains = true;
        }

    }

    // Return containment
    return (contains);
}


/***********************************************************************//**
 * @brief Read model from XML element
 *
 * @param[in] xml XML element.
 *
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Read the skymap information from an XML element. The XML element is
 * required to have 1 parameter named either "Normalization" or "Prefactor".
 *
 * If the attribute @a normalize="0" or @ normalize="false" is present the
 * diffuse map will not be normalised to unity flux upon loading.
 ***************************************************************************/
void GModelSpatialDiffuseMap::read(const GXmlElement& xml)
{
    // Verify that XML element has exactly 1 parameters
    if (xml.elements() != 1 || xml.elements("parameter") != 1) {
        throw GException::model_invalid_parnum(G_READ, xml,
              "Spatial map model requires exactly 1 parameter.");
    }

    // Get pointer on model parameter
    const GXmlElement* par = xml.element("parameter", 0);

    // Get value
    if (par->attribute("name") == "Normalization" ||
        par->attribute("name") == "Prefactor") {
        m_value.read(*par);
    }
    else {
        throw GException::model_invalid_parnames(G_READ, xml,
              "Spatial map model requires either \"Prefactor\" or"
              " \"Normalization\" parameter.");
    }

    // Get optional normalization attribute
    m_normalize     = true;
    m_has_normalize = false;
    if (xml.has_attribute("normalize")) {
        m_has_normalize = true;
        std::string arg = xml.attribute("normalize");
        if (arg == "0" || gammalib::tolower(arg) == "false") {
            m_normalize = false;
        }
    }

    // Load skymap
    load(xml.attribute("file"));

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write model into XML element
 *
 * @param[in] xml XML element.
 *
 * @exception GException::model_invalid_spatial
 *            Existing XML element is not of type "SpatialMap"
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Write the map cube information into an XML element. The XML element has to
 * be of type "MapCubeFunction" and will have 1 parameter leaf named either
 * "Value" or "Normalization" (default).
 ***************************************************************************/
void GModelSpatialDiffuseMap::write(GXmlElement& xml) const
{
    // Set model type
    if (xml.attribute("type") == "") {
        xml.attribute("type", "SpatialMap");
    }

    // Set model filename
    xml.attribute("file", m_filename);

    // Verify model type
    if (xml.attribute("type") != "SpatialMap") {
        throw GException::model_invalid_spatial(G_WRITE, xml.attribute("type"),
              "Spatial model is not of type \"SpatialMap\".");
    }

    // If XML element has 0 nodes then append parameter node. The name
    // of the node is "Prefactor" as this is the Fermi/LAT standard.
    // We thus assure that the XML files will be compatible with
    // Fermi/LAT.
    if (xml.elements() == 0) {
        xml.append(GXmlElement("parameter name=\"Prefactor\""));
    }

    // Verify that XML element has exactly 1 parameter
    if (xml.elements() != 1 || xml.elements("parameter") != 1) {
        throw GException::model_invalid_parnum(G_WRITE, xml,
              "Spatial map model requires exactly 1 parameter.");
    }

    // Get pointer on model parameter
    GXmlElement* par = xml.element("parameter", 0);

    // Set or update parameter
    if (par->attribute("name") == "Normalization" ||
        par->attribute("name") == "Prefactor") {
        m_value.write(*par);
    }
    else {
        throw GException::model_invalid_parnames(G_WRITE, xml,
              "Spatial map model requires either \"Prefactor\" or"
              " \"Normalization\" parameter.");
    }

    // Set optional normalization attribute
    if (m_has_normalize || !m_normalize) {
        if (m_normalize) {
            xml.attribute("normalize", "1");
        }
        else {
            xml.attribute("normalize", "0");
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print map information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String with diffuse map model information.
 ***************************************************************************/
std::string GModelSpatialDiffuseMap::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GModelSpatialDiffuseMap ===");

        // Append parameters
        result.append("\n"+gammalib::parformat("Sky map file")+m_filename);
        result.append("\n"+gammalib::parformat("Map normalization"));
        result.append(gammalib::str(m_norm)+" ph/cm2/s");
        if (normalize()) {
            result.append(" [normalized]");
        }
        result.append("\n"+gammalib::parformat("Map centre"));
        result.append(m_centre.print());
        result.append("\n"+gammalib::parformat("Map radius"));
        result.append(gammalib::str(m_radius)+" deg");
        result.append("\n"+gammalib::parformat("Number of parameters"));
        result.append(gammalib::str(size()));
        for (int i = 0; i < size(); ++i) {
            result.append("\n"+m_pars[i]->print(chatter));
        }

    } // endif: chatter was not silent

    // Return result
    return result;
}


/***********************************************************************//**
 * @brief Load skymap into the model class
 *
 * @param[in] filename Sky map file.
 *
 * Loads skymap into the model class. The method calls the protected method
 * prepare_map() that prepares the map for usage by the class.
 ***************************************************************************/
void GModelSpatialDiffuseMap::load(const std::string& filename)
{
    // Initialise skymap
    m_map.clear();

    // Store filename of skymap (for XML writing). Note that we do not
    // expand any environment variable at this level, so that if we write
    // back the XML element we write the filepath with the environment
    // variables
    m_filename = filename;

    // Load skymap
    m_map.load(gammalib::expand_env(m_filename));

    // Prepare sky map
    prepare_map();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpatialDiffuseMap::init_members(void)
{
    // Initialise Value
    m_value.clear();
    m_value.name("Prefactor");
    m_value.value(1.0);
    m_value.scale(1.0);
    m_value.range(0.001, 1000.0);
    m_value.gradient(0.0);
    m_value.fix();
    m_value.has_grad(true);

    // Set parameter pointer(s)
    m_pars.clear();
    m_pars.push_back(&m_value);

    // Initialise other members
    m_map.clear();
    m_filename.clear();
    m_mc_cache.clear();
    m_normalize     = true;
    m_has_normalize = false;
    m_norm          = 0.0;
    m_centre.clear();
    m_radius        = 0.0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model Spatial map cube model.
 ***************************************************************************/
void GModelSpatialDiffuseMap::copy_members(const GModelSpatialDiffuseMap& model)
{
    // Copy members
    m_value         = model.m_value;
    m_map           = model.m_map;
    m_filename      = model.m_filename;
    m_mc_cache      = model.m_mc_cache;
    m_normalize     = model.m_normalize;
    m_has_normalize = model.m_has_normalize;
    m_norm          = model.m_norm;
    m_centre        = model.m_centre;
    m_radius        = model.m_radius;

    // Set parameter pointer(s)
    m_pars.clear();
    m_pars.push_back(&m_value);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpatialDiffuseMap::free_members(void)
{
    // Return
    return;
}


/***********************************************************************//**
 * @brief Prepare sky map after loading
 *
 * Prepares a sky map after loading. The map is normalized so that the total
 * flux in the map amounts to 1 ph/cm2/s. Negative skymap pixels are set to
 * zero intensity.
 *
 * The method also initialises a cache for Monte Carlo sampling of the
 * skymap. This Monte Carlo cache consists of a linear array that maps a
 * value between 0 and 1 into the skymap pixel.
 *
 * Note that if the GSkymap object contains multiple maps, only the first
 * map is used.
 ***************************************************************************/
void GModelSpatialDiffuseMap::prepare_map(void)
{
    // Initialise cache, centre and radius
    m_mc_cache.clear();
    m_centre.clear();
    m_radius = 0.0;

    // Determine number of skymap pixels
    int npix = m_map.npix();

    // Continue only if there are skymap pixels
    if (npix > 0) {

        // Reserve space for all pixels in cache
        m_mc_cache.reserve(npix+1);

        // Set first cache value to 0
        m_mc_cache.push_back(0.0);

        // Initialise cache with cumulative pixel fluxes and compute total
        // flux in skymap for normalization. Negative pixels are set to
        // zero intensity in the skymap. Invalid pixels are also filtered.
        double sum = 0.0;
        for (int i = 0; i < npix; ++i) {
            double flux = m_map(i) * m_map.solidangle(i);
            if (flux < 0.0 ||
                gammalib::is_notanumber(flux) ||
                gammalib::is_infinite(flux)) {
                m_map(i) = 0.0;
                flux     = 0.0;
            }
            sum += flux;
            m_mc_cache.push_back(sum);
        }

        // Normalize fluxes in the cache so that the values in the cache
        // run from 0 to 1. Optionally also normalize the sky map.
        if (sum > 0.0) {
            if (normalize()) {
                for (int i = 0; i < npix; ++i) {
                    m_map(i)      /= sum;
                    m_mc_cache[i] /= sum;
                }
                m_norm = 1.0;
            }
            else {
                for (int i = 0; i < npix; ++i) {
                    m_mc_cache[i] /= sum;
                }
                m_norm = sum;
            }
        }

        // Make sure that last pixel in the cache is >1
        m_mc_cache[npix] = 1.0001;

        // If we have a HealPix map then set radius to 180 deg
        if (m_map.projection()->code() == "HPX") {
            m_radius = 180.0;
        }

        // ... otherwise compute map centre and radius
        else {
        
            // Get map centre
            GSkyPixel centre(m_map.nx()/2.0, m_map.ny()/2.0);
            m_centre = m_map.pix2dir(centre);

            // Determine map radius
            for (int i = 0; i < npix; ++i) {
                double radius = m_map.inx2dir(i).dist_deg(m_centre);
                if (radius > m_radius) {
                    m_radius = radius;
                }
            }

        } // endelse: computed map centre and radius

        // Dump preparation results
        #if defined(G_DEBUG_PREPARE)
        double sum_control = 0.0;
        for (int i = 0; i < npix; ++i) {
            double flux = m_map(i) * m_map.solidangle(i);
            if (flux >= 0.0) {
                sum_control += flux;
            }
        }
        std::cout << "Total flux before normalization: " << sum << std::endl;
        std::cout << "Total flux after normalization : " << sum_control << std::endl;
        #endif

        // Dump cache values for debugging
        #if defined(G_DEBUG_CACHE)
        for (int i = 0; i < npix+1; ++i) {
            std::cout << "i=" << i;
            std::cout << " c=" << m_mc_cache[i] << std::endl;
        }
        #endif

    } // endif: there were skymap pixels

    // Return
    return;
}
