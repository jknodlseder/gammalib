/***************************************************************************
 *  GModelSpatialEllipticalGauss.cpp - Elliptical gauss source model class *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2015 by Michael Mayer                                    *
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
 * @file GModelSpatialEllipticalGauss.cpp
 * @brief Elliptical gauss model class implementation
 * @author Michael Mayer
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GMath.hpp"
#include "GModelSpatialEllipticalGauss.hpp"
#include "GModelSpatialRadialDisk.hpp"
#include "GModelSpatialRegistry.hpp"

/* __ Constants __________________________________________________________ */

/* __ Globals ____________________________________________________________ */
const GModelSpatialEllipticalGauss g_elliptical_gauss_seed;
const GModelSpatialRegistry g_elliptical_gauss_registry(&g_elliptical_gauss_seed);

/* __ Method name definitions ____________________________________________ */
#define G_READ             "GModelSpatialEllipticalGauss::read(GXmlElement&)"
#define G_WRITE           "GModelSpatialEllipticalGauss::write(GXmlElement&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModelSpatialEllipticalGauss::GModelSpatialEllipticalGauss(void) :
                              GModelSpatialElliptical()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Elliptical Gauss constructor
 *
 * @param[in] dir Sky position of gauss centre.
 * @param[in] semimajor Semi-major axis (degrees).
 * @param[in] semiminor Semi-minor axis (degrees).
 * @param[in] posangle Position angle of semi-major axis (degrees).
 *
 * Construct elliptical gauss model from sky position of the ellipse centre
 * (@p dir), the @p semimajor and @p semiminor axes, and the position
 * angle (@p posangle).
 ***************************************************************************/
GModelSpatialEllipticalGauss::GModelSpatialEllipticalGauss(const GSkyDir& dir,
                                                           const double&  semimajor,
                                                           const double&  semiminor,
                                                           const double&  posangle) :
                              GModelSpatialElliptical()
{
    // Initialise members
    init_members();

    // Assign parameters
    this->dir(dir);
    this->semiminor(semiminor);
    this->semimajor(semimajor);
    this->posangle(posangle);

    // Return
    return;
}


/***********************************************************************//**
 * @brief XML constructor
 *
 * @param[in] xml XML element.
 *
 * Constructs elliptical gauss model by extracting information from an XML
 * element. See the read() method for more information about the expected
 * structure of the XML element.
 ***************************************************************************/
GModelSpatialEllipticalGauss::GModelSpatialEllipticalGauss(const GXmlElement& xml) :
                              GModelSpatialElliptical()
{
    // Initialise members
    init_members();

    // Read information from XML element
    read(xml);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Elliptical gauss model.
 ***************************************************************************/
GModelSpatialEllipticalGauss::GModelSpatialEllipticalGauss(const GModelSpatialEllipticalGauss& model) :
                              GModelSpatialElliptical(model)
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
GModelSpatialEllipticalGauss::~GModelSpatialEllipticalGauss(void)
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
 * @param[in] model Elliptical gauss model.
 * @return Elliptical gauss model.
 ***************************************************************************/
GModelSpatialEllipticalGauss& GModelSpatialEllipticalGauss::operator=(const GModelSpatialEllipticalGauss& model)
{
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelSpatialElliptical::operator=(model);

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
 * @brief Clear elliptical gauss model
 ***************************************************************************/
void GModelSpatialEllipticalGauss::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GModelSpatialElliptical::free_members();
    this->GModelSpatial::free_members();

    // Initialise members
    this->GModelSpatial::init_members();
    this->GModelSpatialElliptical::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone elliptical gauss model
 *
 * @return Pointer to deep copy of elliptical gauss model.
 ***************************************************************************/
GModelSpatialEllipticalGauss* GModelSpatialEllipticalGauss::clone(void) const
{
    // Clone elliptical gauss model
    return new GModelSpatialEllipticalGauss(*this);
}


/***********************************************************************//**
 * @brief Evaluate function (in units of sr^-1)
 *
 * @param[in] theta Angular distance from disk centre (radians).
 * @param[in] posangle Position angle (counterclockwise from North) (radians).
 * @param[in] energy Photon energy.
 * @param[in] time Photon arrival time.
 * @return Model value.
 *
 * Evaluates the spatial component for an elliptical gauss source model. The
 * source model is an elliptical gaussian function
 * \f$S_{\rm p}(\theta, \phi | E, t)\f$, where
 * \f$\theta\f$ is the angular separation between elliptical gauss centre and
 * the actual location and \f$\phi\f$ the position angle with respect to the
 * model centre, counted counterclockwise from North.
 *
 * The function \f$f(\theta, \phi)\f$ is given by
 *
 * \f[
 * S_{\rm p}(\theta, \phi | E, t) = \left \{
 *  \begin{array}{l l}
 *     {\tt m\_norm}
 *     & \mbox{if} \, \, \theta \le \theta_0 \\
 *     \\
 *     0 & \mbox{else}
 *  \end{array}
 *  \right .
 * \f]
 *
 * where \f$\theta_0\f$ is the effective radius of the ellipse on the sphere
 * given by
 *
 * \f[\theta_0\ =
 *    \frac{ab}{\sqrt{b^2 \cos^2(\phi-\phi_0) + a^2 \sin^2(\phi-\phi_0)}}\f]
 *
 * and
 * \f$a\f$ is the semi-major axis of the ellipse,
 * \f$b\f$ is the semi-minor axis, and
 * \f$\phi_0\f$ is the position angle of the ellipse, counted
 * counterclockwise from North.
 *
 * The normalisation constant \f${\tt m\_norm}\f$ which is the inverse of the
 * solid angle subtended by an ellipse is given by
 *
 * @todo Quote formula for ellipse solid angle
 *
 * (see the update() method).
 ***************************************************************************/
double GModelSpatialEllipticalGauss::eval(const double&  theta,
                                          const double&  posangle,
                                          const GEnergy& energy,
                                          const GTime&   time) const
{
    // Initialise value
    double value = 0.0;

    // Continue only if we're inside circle enclosing the ellipse
    if (theta <= theta_max()) {

        // Update precomputation cache
        update();

        // Perform computations
        double sinphi = std::sin(posangle);
        double cosphi = std::cos(posangle);

        // Compute help term
        double term1 = m_term1 * cosphi * cosphi;
        double term2 = m_term2 * sinphi * sinphi;
        double term3 = m_term3 * sinphi * cosphi;

        // Compute exponent
        double exponent = theta * theta * (term1 + term2 + term3);

        // Set value
        value = m_norm * std::exp(-exponent);

        // Compile option: Check for NaN/Inf
        #if defined(G_NAN_CHECK)
        if (gammalib::is_notanumber(value) || gammalib::is_infinite(value)) {
            std::cout << "*** ERROR: GModelSpatialEllipticalGauss::eval";
            std::cout << "(theta=" << theta << "): NaN/Inf encountered";
            std::cout << "(posangle=" << posangle << "): NaN/Inf encountered";
            std::cout << " (value=" << value;
            std::cout << ", MinorAxis^2=" << m_minor2;
            std::cout << ", MaxjorAxis^2=" << m_major2;
            std::cout << ", m_norm=" << m_norm;
            std::cout << ")" << std::endl;
        }
        #endif

    } // endif: position was inside enclosing circle

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Evaluate function and gradients (in units of sr^-1)
 *
 * @param[in] theta Angular distance from gaussian centre (radians).
 * @param[in] posangle Position angle (counterclockwise from North) (radians).
 * @param[in] energy Photon energy.
 * @param[in] time Photon arrival time.
 * @return Model value.
 *
 * Evaluates the function value. No gradient computation is implemented as
 * Elliptical models will be convolved with the instrument response and thus
 * require the numerical computation of the derivatives.
 *
 * See the eval() method for more information.
 ***************************************************************************/
double GModelSpatialEllipticalGauss::eval_gradients(const double&  theta,
                                                    const double&  posangle,
                                                    const GEnergy& energy,
                                                    const GTime&   time) const
{
    // Return value
    return (eval(theta, posangle, energy, time));
}


/***********************************************************************//**
 * @brief Returns MC sky direction
 *
 * @param[in] energy Photon energy.
 * @param[in] time Photon arrival time.
 * @param[in,out] ran Random number generator.
 * @return Sky direction.
 *
 * Draws an arbitrary sky position from the 2D gauss distribution.
 ***************************************************************************/
GSkyDir GModelSpatialEllipticalGauss::mc(const GEnergy& energy,
                                         const GTime&   time,
                                         GRan&          ran) const
{
    // Update precomputation cache
    update();

    // Initialise photon
    GPhoton photon;
    photon.energy(energy);
    photon.time(time);

    // Draw gaussian offset from each axis
    double theta1 = semimajor() * ran.normal();
    double theta2 = semiminor() * ran.normal();

    // Compute total offset from model centre
    double theta = std::sqrt(theta1 * theta1 + theta2 * theta2);

    // Compute rotation angle, taking into account given position angle
    double phi = gammalib::atan2d(theta2, theta1) + posangle();

    // Rotate sky direction by offset
    GSkyDir sky_dir = dir();
    sky_dir.rotate_deg(phi , theta);

    // Set photon sky direction
    photon.dir(sky_dir);

    // Return photon direction
    return (photon.dir());
}


/***********************************************************************//**
 * @brief Return maximum model radius (in radians)
 *
 * @return Returns maximum model radius.
 *
 * Returns \f$5 \sigma\f$ as approximate edge of the Gaussian. This limit
 * is of course arbitrary, but allows to limit the integration region for
 * response computation.
 ***************************************************************************/
double GModelSpatialEllipticalGauss::theta_max(void) const
{
    // Set maximum model radius
    double theta_max = (semimajor() > semiminor())
                       ? semimajor() * gammalib::deg2rad * 5.0
                       : semiminor() * gammalib::deg2rad * 5.0;

    // Return value
    return theta_max;
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
 * Reads the elliptical gauss model information from an XML element. The XML
 * element shall have either the format 
 *
 *     <spatialModel type="EllipticalGauss">
 *       <parameter name="RA"          scale="1.0" value="83.6331" min="-360" max="360" free="1"/>
 *       <parameter name="DEC"         scale="1.0" value="22.0145" min="-90"  max="90"  free="1"/>
 *       <parameter name="PA"          scale="1.0" value="45.0"    min="-360"  max="360" free="1"/>
 *       <parameter name="MinorRadius" scale="1.0" value="0.5"     min="0.001" max="10"  free="1"/>
 *       <parameter name="MajorRadius" scale="1.0" value="2.0"     min="0.001" max="10"  free="1"/>
 *     </spatialModel>
 *
 * or
 *
 *     <spatialModel type="EllipticalGauss">
 *       <parameter name="GLON"        scale="1.0" value="83.6331" min="-360" max="360" free="1"/>
 *       <parameter name="GLAT"        scale="1.0" value="22.0145" min="-90"  max="90"  free="1"/>
 *       <parameter name="PA"          scale="1.0" value="45.0"    min="-360"  max="360" free="1"/>
 *       <parameter name="MinorRadius" scale="1.0" value="0.5"     min="0.001" max="10"  free="1"/>
 *       <parameter name="MajorRadius" scale="1.0" value="2.0"     min="0.001" max="10"  free="1"/>
 *     </spatialModel>
 *
 * @todo Implement a test of the ellipse boundary. The axes
 *       and axes minimum should be >0.
 ***************************************************************************/
void GModelSpatialEllipticalGauss::read(const GXmlElement& xml)
{
    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Verify that XML element has exactly 5 parameters
    if (xml.elements() != 5 || npars != 5) {
        throw GException::model_invalid_parnum(G_READ, xml,
              "Elliptical gauss model requires exactly 5 parameters.");
    }

    // Read gauss location
    GModelSpatialElliptical::read(xml);

    // Extract model parameters
    int  npar[2] = {0, 0};
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        const GXmlElement* par = xml.element("parameter", i);

        // Handle semiminor radius
        if (par->attribute("name") == "MinorRadius") {
            
            // Read parameter
            m_semiminor.read(*par);
            
            // Increment parameter counter
            npar[0]++;
        }

        // Handle semimajor radius
        else if (par->attribute("name") == "MajorRadius") {

        	// Read parameter
        	m_semimajor.read(*par);

        	// Increment parameter counter
        	npar[1]++;
        }

    } // endfor: looped over all parameters

    // Verify that all parameters were found
    if (npar[0] != 1 || npar[1] != 1) {
        throw GException::model_invalid_parnames(G_READ, xml,
              "Elliptical disk model requires \"MinorRadius\" and"
              " \"MajorRadius\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write model into XML element
 *
 * @param[in] xml XML element into which model information is written.
 *
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Write the elliptical gauss model information into an XML element. The XML
 * element will have the format 
 *
 *     <spatialModel type="EllipticalGauss">
 *       <parameter name="RA"          scale="1.0" value="83.6331" min="-360" max="360" free="1"/>
 *       <parameter name="DEC"         scale="1.0" value="22.0145" min="-90"  max="90"  free="1"/>
 *       <parameter name="PA"          scale="1.0" value="45.0"    min="-360"  max="360" free="1"/>
 *       <parameter name="MinorRadius" scale="1.0" value="0.5"     min="0.001" max="10"  free="1"/>
 *       <parameter name="MajorRadius" scale="1.0" value="2.0"     min="0.001" max="10"  free="1"/>
 *     </spatialModel>
 *
 ***************************************************************************/
void GModelSpatialEllipticalGauss::write(GXmlElement& xml) const
{
    // Write disk location
    GModelSpatialElliptical::write(xml);

    // If XML element has 3 nodes (which should be the location and PA nodes)
    // then append 2 parameter nodes
    if (xml.elements() == 3) {
        xml.append(GXmlElement("parameter name=\"MinorRadius\""));
        xml.append(GXmlElement("parameter name=\"MajorRadius\""));
    }

    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Verify that XML element has exactly 5 parameters
    if (xml.elements() != 5 || npars != 5) {
        throw GException::model_invalid_parnum(G_WRITE, xml,
              "Elliptical gauss model requires exactly 5 parameters.");
    }

    // Set or update model parameter attributes
    int npar[2] = {0, 0};
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        GXmlElement* par = xml.element("parameter", i);

        // Handle semiminor radius
        if (par->attribute("name") == "MinorRadius") {

        	// Write parameter
            m_semiminor.write(*par);

            // Increment parameter counter
            npar[0]++;
        }

        // Handle semimajor radius
        else if (par->attribute("name") == "MajorRadius") {

        	// Write parameter
            m_semimajor.write(*par);

            // Increment parameter counter
            npar[1]++;
        }

    } // endfor: looped over all parameters

    // Check of all required parameters are present
    if (npar[0] != 1 || npar[1] != 1) {
        throw GException::model_invalid_parnames(G_WRITE, xml,
              "Elliptical gauss model requires \"MinorRadius\" and"
              " \"MajorRadius\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing model information.
 ***************************************************************************/
std::string GModelSpatialEllipticalGauss::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GModelSpatialEllipticalGauss ===");

        // Append parameters
        result.append("\n"+gammalib::parformat("Number of parameters"));
        result.append(gammalib::str(size()));
        for (int i = 0; i < size(); ++i) {
            result.append("\n"+m_pars[i]->print(chatter));
        }

    } // endif: chatter was not silent

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpatialEllipticalGauss::init_members(void)
{
    // Initialise precomputation cache. Note that zero values flag
    // uninitialised as a zero radius is not meaningful
    m_last_minor        = 0.0;
    m_last_major        = 0.0;
    m_minor_rad         = 0.0;
    m_major_rad         = 0.0;
    m_norm              = 0.0;
    m_last_posangle     = 9999.0; // Signals that has not been initialised
    m_sin2pos           = 0.0;
    m_cospos2           = 0.0;
    m_sinpos2           = 0.0;
    m_minor2            = 0.0;
    m_major2            = 0.0;
    m_term1             = 0.0;
    m_term2             = 0.0;
    m_term3             = 0.0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model Elliptical gauss model.
 ***************************************************************************/
void GModelSpatialEllipticalGauss::copy_members(const GModelSpatialEllipticalGauss& model)
{
    // Copy precomputation cache
    m_last_minor        = model.m_last_minor;
    m_last_major        = model.m_last_major;
    m_minor_rad         = model.m_minor_rad;
    m_major_rad         = model.m_major_rad;
    m_norm              = model.m_norm;
    m_last_posangle     = model.m_last_posangle;
    m_sin2pos           = model.m_sin2pos;
    m_cospos2           = model.m_cospos2;
    m_sinpos2           = model.m_sinpos2;
    m_minor2            = model.m_minor2;
    m_major2            = model.m_major2;
    m_term1             = model.m_term1;
    m_term2             = model.m_term2;
    m_term3             = model.m_term3;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpatialEllipticalGauss::free_members(void)
{
    // Return
    return;
}


/***********************************************************************//**
 * @brief Update precomputation cache
 *
 * Computes the normalization
 * \f[
 *    {\tt m\_norm} = \frac{1}{2 \pi (1 - \cos a) (1 - \cos b)}
 * \f]
 *
 * @todo check this formula
 ***************************************************************************/
void GModelSpatialEllipticalGauss::update() const
{
    // Initialise flag if something has changed
    bool changed = false;

    // Update if one axis has changed
    if (m_last_minor != semiminor() || m_last_major != semimajor()) {

        // Signal parameter changes
        changed = true;

        // Store last values
        m_last_minor = semiminor();
        m_last_major = semimajor();

        // Compute axes in radians
        m_minor_rad = m_last_minor * gammalib::deg2rad;
        m_major_rad = m_last_major * gammalib::deg2rad;

        // Perform precomputations
        m_minor2     = m_minor_rad * m_minor_rad;
        m_major2     = m_major_rad * m_major_rad;
        double denom = gammalib::twopi * m_minor_rad * m_major_rad;
        m_norm       = (denom > 0.0) ? 1.0 / denom : 0.0;

    } // endif: update required

    // Update chache if position angle changed
    if (m_last_posangle != posangle()) {

        // Signal parameter changes
        changed = true;

        // Store last value
        m_last_posangle = posangle();

        // Compute angle in radians
        double posangle_rad = m_last_posangle * gammalib::deg2rad;

        // Compute sine and cosine
        double cospos = std::cos(posangle_rad);
        double sinpos = std::sin(posangle_rad);

        // Cache important values for further computations
        m_cospos2 = cospos * cospos;
        m_sinpos2 = sinpos * sinpos;
        m_sin2pos = std::sin(2.0 * posangle_rad);

    } // endif: position angle update required

    // Perform precomputations in case anything has changed
    if (changed) {

        // Compute help terms
        m_term1 = 0.5 * (m_cospos2 / m_minor2 + m_sinpos2 / m_major2);
        m_term2 = 0.5 * (m_cospos2 / m_major2 + m_sinpos2 / m_minor2);
        m_term3 = 0.5 * (m_sin2pos / m_major2 - m_sin2pos / m_minor2);

    } // endif: something has changed

    // Return
    return;
}
