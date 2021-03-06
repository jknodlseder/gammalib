/***************************************************************************
 *                GCTAObservation.cpp - CTA Observation class              *
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
 * @file GCTAObservation.cpp
 * @brief CTA observation class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GObservationRegistry.hpp"
#include "GException.hpp"
#include "GFits.hpp"
#include "GGti.hpp"
#include "GTools.hpp"
#include "GIntegral.hpp"
#include "GCTAException.hpp"
#include "GCTASupport.hpp"
#include "GCTAObservation.hpp"
#include "GCTAResponseIrf.hpp"
#include "GCTAResponseCube.hpp"
#include "GCTAEventList.hpp"
#include "GCTAEventCube.hpp"
#include "GCTARoi.hpp"

/* __ Globals ____________________________________________________________ */
const GCTAObservation      g_obs_cta_seed("CTA");
const GCTAObservation      g_obs_hess_seed("HESS");
const GCTAObservation      g_obs_magic_seed("MAGIC");
const GCTAObservation      g_obs_veritas_seed("VERITAS");
const GObservationRegistry g_obs_cta_registry(&g_obs_cta_seed);
const GObservationRegistry g_obs_hess_registry(&g_obs_hess_seed);
const GObservationRegistry g_obs_magic_registry(&g_obs_magic_seed);
const GObservationRegistry g_obs_veritas_registry(&g_obs_veritas_seed);

/* __ Method name definitions ____________________________________________ */
#define G_RESPONSE_SET                "GCTAObservation::response(GResponse&)"
#define G_RESPONSE_GET                          "GCTAObservation::response()"
#define G_ROI                                        "GCTAObservation::roi()"
#define G_GTI                                        "GCTAObservation::gti()"
#define G_EBOUNDS                                "GCTAObservation::ebounds()"
#define G_READ                          "GCTAObservation::read(GXmlElement&)"
#define G_WRITE                        "GCTAObservation::write(GXmlElement&)"
#define G_LOAD           "GCTAObservation::load(std::string&, std::string&, "\
                                                              "std::string&)"
#define G_EVENTS                                  "GCTAObservation::events()"

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
 *
 * Creates an empty CTA observation.
 ***************************************************************************/
GCTAObservation::GCTAObservation(void) : GObservation()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Instrument constructor
 *
 * @param[in] instrument Instrument name.
 *
 * Creates an empty CTA observation for a given instrument. This enables
 * using the CTA specific interface for any other VHE instrument. Note that
 * each other VHE instruments needs a specific registry at the beginning
 * of the GCTAObservation.cpp file. So far the following instruments are
 * supported: CTA, HESS, VERITAS, MAGIC.
 ***************************************************************************/
GCTAObservation::GCTAObservation(const std::string& instrument) : GObservation()
{
    // Initialise members
    init_members();

    // Set instrument
    m_instrument = instrument;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Stacked cube analysis constructor
 *
 * @param[in] cntcube Counts cube file name.
 * @param[in] expcube Exposure cube file name.
 * @param[in] psfcube Psf cube file name.
 * @param[in] bkgcube Backgorund cube file name.
 *
 * Creates a CTA observation from a counts cube, an exposure cube and a Psf
 * cube.
 ***************************************************************************/
GCTAObservation::GCTAObservation(const std::string& cntcube,
                                 const std::string& expcube,
                                 const std::string& psfcube,
                                 const std::string& bkgcube) : GObservation()
{
    // Initialise members
    init_members();

    // Load data
    load(cntcube, expcube, psfcube, bkgcube);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] obs CTA observation.
 *
 * Creates a CTA observation by copying an existing CTA observation.
 ***************************************************************************/
GCTAObservation::GCTAObservation(const GCTAObservation& obs) : GObservation(obs)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(obs);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GCTAObservation::~GCTAObservation(void)
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
 * @param[in] obs CTA observation.
 *
 * Assign CTA observation to this object.
 ***************************************************************************/
GCTAObservation& GCTAObservation::operator=(const GCTAObservation& obs)
{
    // Execute only if object is not identical
    if (this != &obs) {

        // Copy base class members
        this->GObservation::operator=(obs);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(obs);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                              Public methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear CTA observation
 ***************************************************************************/
void GCTAObservation::clear(void)
{
    // Free members
    free_members();
    this->GObservation::free_members();

    // Initialise members
    this->GObservation::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone CTA observation
 *
 * @return Pointer to deep copy of CTA observation.
 ***************************************************************************/
GCTAObservation* GCTAObservation::clone(void) const
{
    return new GCTAObservation(*this);
}


/***********************************************************************//**
 * @brief Set response function
 *
 * @param[in] rsp Response function.
 *
 * @exception GException::invalid_value
 *            Specified response in not of type GCTAResponse.
 *
 * Sets the response function for the observation. The argument has to be of
 * type GCTAResponse, otherwise an exception is thrown.
 ***************************************************************************/
void GCTAObservation::response(const GResponse& rsp)
{
    // Free response
    if (m_response != NULL) delete m_response;
    m_response = NULL;

    // Get pointer on CTA response
    const GCTAResponse* cta = dynamic_cast<const GCTAResponse*>(&rsp);
    if (cta == NULL) {
        std::string msg = "Specified response function is not a CTA "
                          "response function.\n" + rsp.print();
        throw GException::invalid_value(G_RESPONSE_SET, msg);
    }

    // Clone response function
    m_response = cta->clone();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Return pointer to CTA response function
 *
 * @return Pointer to CTA response function.
 *
 * @exception GException::invalid_value
 *            No valid response found in CTA observation.
 *
 * Returns a pointer to the CTA response function. The pointer returned is
 * never NULL.
 ***************************************************************************/
const GCTAResponse* GCTAObservation::response(void) const
{
    // Throw an exception if the response pointer is not valid
    if (m_response == NULL) {
        std::string msg = "No valid response function found in CTA"
                          " observation.\n";
        throw GException::invalid_value(G_RESPONSE_GET, msg);
    }

    // Return pointer
    return m_response;
}


/***********************************************************************//**
 * @brief Set CTA response function
 *
 * @param[in] rspname Response name.
 * @param[in] caldb Calibration database.
 *
 * Sets the CTA response function by specifying a response name and a
 * calibration database. This method also loads the response function so that
 * it is available for data analysis.
 ***************************************************************************/
void GCTAObservation::response(const std::string& rspname, const GCaldb& caldb)
{
    // Free response
    if (m_response != NULL) delete m_response;
    m_response = NULL;

    // Allocate fresh response function
    GCTAResponseIrf* rsp = new GCTAResponseIrf;

    // Set calibration database
    rsp->caldb(caldb);

    // Load instrument response function
    rsp->load(rspname);

    // Store pointer
    m_response = rsp;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set CTA response function
 *
 * @param[in] expcube Exposure cube.
 * @param[in] psfcube Psf cube.
 * @param[in] bkgcube Background cube.
 *
 * Sets the CTA response function fur cube analysis by specifying the
 * exposure cube, the Psf cube and the background cube. The method also
 * copies over the ontime, the livetime and the deadtime correction factor
 * from the exposure cube.
 ***************************************************************************/
void GCTAObservation::response(const GCTACubeExposure&   expcube,
                               const GCTACubePsf&        psfcube,
                               const GCTACubeBackground& bkgcube)
{
    // Free response
    if (m_response != NULL) delete m_response;
    m_response = NULL;

    // Allocate fresh response function
    GCTAResponseCube* rsp = new GCTAResponseCube(expcube, psfcube, bkgcube);

    // Store pointer
    m_response = rsp;

    // Copy over time information from exposure cube
    ontime(expcube.ontime());
    livetime(expcube.livetime());
    deadc(expcube.deadc());

    // Return
    return;
}


/***********************************************************************//**
 * @brief Get Region of Interest
 *
 * @exception GException::invalid_value
 *            Events is not an event list or unable to open an event file.
 *
 * Extract the Region of Interest from the event list. If events are loaded
 * into the observation, the RoI is simply copied over. If no events are
 * present, the method attemps to read the RoI from the header keywords of
 * the event file. This obviously only works if a valid event file name is
 * available.
 ***************************************************************************/
GCTARoi GCTAObservation::roi(void) const
{
    // Initialise ROI
    GCTARoi roi;

    // If CTA has events then simply retrieve the ROI from the event list
    if (m_events != NULL) {

        // Extract region of interest from CTA observation
        const GCTAEventList* list = dynamic_cast<const GCTAEventList*>(m_events);
        if (list == NULL) {
            std::string msg = "The observation does not contain an event "
                              "list. An event list is needed to retrieve "
                              "the Region of Interest.";
            throw GException::invalid_value(G_ROI, msg);
        }
        roi = list->roi();

    }

    // ... otherwise try reading the ROI from the event file header
    else {

        // Try opening event file
        GFits fits;
        try {
            fits.open(m_eventfile);
        }
        catch (std::exception &e) {
            std::string msg;
            if (m_eventfile.length() == 0) {
                msg = "The observation does not contain any information about "
                      "the event file, hence the Region of Interest cannot be "
                      "determined.";
            }
            else {
                msg = "Could no open the event file \""+m_eventfile+"\". "
                      "Please check that the file name is valid.";
            }
            throw GException::invalid_value(G_ROI, msg);
        }
    
        // Get event list HDU
        const GFitsTable& hdu = *fits.table("EVENTS");

        // Read ROI from data selection keywords
        roi = gammalib::read_ds_roi(hdu);

        // Close FITS file
        fits.close();

    }

    // Return ROI
    return roi;
}


/***********************************************************************//**
 * @brief Get Good Time Intervals
 *
 * @exception GException::invalid_value
 *            Unable to open the event file or to extract GTI information
 *            from file.
 *
 * Extract the Good Time Intervals from the events. If events are loaded into
 * the observation, the GTIs are simply copied over. If no events are present
 * the method attemps to read the GTIs from the event file. This obviously
 * only works if a valid event file name is available.
 ***************************************************************************/
GGti GCTAObservation::gti(void) const
{
    // Initialise GTIs
    GGti gti;

    // If CTA has events then simply retrieve the GTIs from the events
    if (m_events != NULL) {

        // Copy over GTIs
        gti = m_events->gti();

    }

    // ... otherwise try reading the GTIs from the event file
    else {

        // Try opening event file
        GFits fits;
        try {
            fits.open(m_eventfile);
        }
        catch (std::exception &e) {
            std::string msg;
            if (m_eventfile.length() == 0) {
                msg = "The observation does not contain any information about "
                      "the event file, hence the Good Time Intervals cannot be "
                      "determined.";
            }
            else {
                msg = "Could no open the event file \""+m_eventfile+"\". "
                      "Please check that the file name is valid.";
            }
            throw GException::invalid_value(G_GTI, msg);
        }
    
        // If we have a GTI extension, then read Good Time Intervals from
        // that extension
        if (fits.contains("GTI")) {
            const GFitsTable& table = *fits.table("GTI");
            gti.read(table);
        }

        // ... otherwise build GTI from TSTART and TSTOP
        else if (fits.contains("EVENTS")) {

            // Read start and stop time
            const GFitsTable& events = *fits.table("EVENTS");
            double            tstart = events.real("TSTART");
            double            tstop  = events.real("TSTOP");

            // Create time reference from header information
            GTimeReference timeref(events);

            // Set start and stop time
            GTime start(tstart);
            GTime stop(tstop);

            // Append start and stop time as single time interval to GTI
            gti.append(start, stop);

            // Set GTI time reference
            gti.reference(timeref);

        } // endelse: GTI built from TSTART and TSTOP on event file

        // ... else throw an exception
        else {
            std::string msg = "Could no find Good Time Interval information "
                              "in the event file \""+m_eventfile+"\". Please "
                              "check that Good Time Interval information is "
                              "properly set in the event file.";
            throw GException::invalid_value(G_GTI, msg);
        }

        // Close FITS file
        fits.close();

    }

    // Return GTIs
    return gti;
}


/***********************************************************************//**
 * @brief Get energy boundaries
 *
 * @exception GException::invalid_value
 *            Unable to open the event file or to extract energy boundaries
 *            from file.
 *
 * Extract the energy boundaries from the events. If events are loaded into
 * the observation, the energy boundaries are simply copied over. If no
 * events are present the method attemps to read the energy boundaries from
 * the event file. This obviously only works if a valid event file name is
 * available.
 ***************************************************************************/
GEbounds GCTAObservation::ebounds(void) const
{
    // Initialise energy boundaries
    GEbounds ebounds;

    // If CTA has events then simply retrieve the energy boundaries from the
    // events
    if (m_events != NULL) {

        // Copy over energy boundaries
        ebounds = m_events->ebounds();

    }

    // ... otherwise try reading the energy boundaries from the event file
    else {

        // Try opening event file
        GFits fits;
        try {
            fits.open(m_eventfile);
        }
        catch (std::exception &e) {
            std::string msg;
            if (m_eventfile.length() == 0) {
                msg = "The observation does not contain any information about "
                      "the event file, hence the energy boundaries cannot be "
                      "determined.";
            }
            else {
                msg = "Could no open the event file \""+m_eventfile+"\". "
                      "Please check that the file name is valid.";
            }
            throw GException::invalid_value(G_EBOUNDS, msg);
        }

        // If file contains an EBOUNDS extension then load the energy
        // boundaries from that extension
        if (fits.contains("EBOUNDS")) {
            const GFitsTable& hdu = *fits.table("EBOUNDS");
            ebounds.read(hdu);
        }

        // ... otherwise, if file contains an EVENTS extension then load the
        // energy boundaries from the data selection keywords
        else if (fits.contains("EVENTS")) {
            const GFitsTable& hdu = *fits.table("EVENTS");
            ebounds = gammalib::read_ds_ebounds(hdu);
        }

        // ... else throw an exception
        else {
            std::string msg = "Could no find energy boundary information in "
                              "the event file \""+m_eventfile+"\". Please "
                              "check that energy boundary information is "
                              "properly set in the event file.";
            throw GException::invalid_value(G_EBOUNDS, msg);
        }
        
        // Close FITS file
        fits.close();

    }

    // Return energy boundaries
    return ebounds;
}


/***********************************************************************//**
 * @brief Read observation from XML element
 *
 * @param[in] xml XML element.
 *
 * @exception GException::xml_invalid_parnum
 *            Invalid number of parameters found in XML element.
 * @exception GException::xml_invalid_parnames
 *            Invalid parameter names found in XML element.
 *
 * Reads information for a CTA observation from an XML element. This method
 * handles to variants: a first where an event list of counts cube is
 * given and a second where the observation definition information is
 * provided by parameter tags.
 *
 * The XML format for an event list is
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="EventList" file="..."/>
 *       ...
 *     </observation>
 *
 * and for a counts cube is
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="CountsCube" file="..."/>
 *       ...
 *     </observation>
 *
 * The second variant without event information has the following format
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="Pointing" ra="..." dec="..."/>
 *       <parameter name="Deadtime" deadc="..."/>
 *       ...
 *     </observation>
 *
 * In addition, calibration information can be specified using the format
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="Calibration" database="..." response="..."/>
 *     </observation>
 *
 * If even more control is required over the response, individual file names
 * can be specified using
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="EffectiveArea"       file="..."/>
 *       <parameter name="PointSpreadFunction" file="..."/>
 *       <parameter name="EnergyDispersion"    file="..."/>
 *       <parameter name="Background"          file="..."/>
 *     </observation>
 *
 * In case that a @a CountsCube is handled, the stacked response can also be
 * provided in the format
 *
 *      <observation name="..." id="..." instrument="...">
 *        ...
 *        <parameter name="ExposureCube" file="..."/>
 *        <parameter name="PsfCube"      file="..."/>
 *      </observation>
 *   
 * Optional user energy thresholds can be specified by adding the @a emin
 * and @a emax attributes to the @p observation tag:
 *
 *     <observation name="..." id="..." instrument="..." emin="..." emax="...">
 *
 * The method does no load the events into memory but stores the file name
 * of the event file. The events are only loaded when required. This reduces
 * the memory needs for an CTA observation object and allows for loading
 * of event information upon need.
 ***************************************************************************/
void GCTAObservation::read(const GXmlElement& xml)
{
    // Clear observation
    clear();

    // Extract instrument name
    m_instrument = xml.attribute("instrument");

    // Read in user defined energy boundaries of this observation
    if (xml.attribute("emin") != "") {
        m_lo_user_thres = gammalib::todouble(xml.attribute("emin"));
    }
    if (xml.attribute("emax") != "") {
        m_hi_user_thres = gammalib::todouble(xml.attribute("emax"));
    }

    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Verify that XML element has at least 1 parameter
    if (xml.elements() < 1 || npars < 1) {
        throw GException::xml_invalid_parnum(G_READ, xml,
              "CTA observation requires at least 1 parameter.");
    }

    // First try to extract observation parameters for an event file
    int n_evfile = 0;
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        const GXmlElement* par = xml.element("parameter", i);

        // Handle EventList or CountsCube
        if ((par->attribute("name") == "EventList") ||
            (par->attribute("name") == "CountsCube")) {

            // Store event type
            m_eventtype = par->attribute("name");

            // Read eventlist file name
            std::string filename = par->attribute("file");

            // Open FITS file
            GFits fits(filename);

            // Read event attributes but do not load the events here
            // to save memory
            if (fits.contains("EVENTS")) {
                const GFitsHDU& hdu = *fits.at("EVENTS");
                read_attributes(hdu);
            }
            else {
                const GFitsHDU& hdu = *fits.at(0);
                read_attributes(hdu);
            }

            // Close FITS file
            fits.close();

            // Store event filename
            m_eventfile = filename;

            // Increment parameter counter
            n_evfile++;
        }

        // Read Background filename (needed by GCTAModelCubeBackground)
        else if (par->attribute("name") == "Background") {

            // Read background file name
            m_bgdfile = par->attribute("file");

        }

    } // endfor: looped over observation parameters

    // Analyse parameters
    bool has_evfile = (n_evfile > 0);

    // If we have an event file then verify that all required parameters
    // were found
    if (has_evfile) {
        gammalib::xml_check_par(G_READ, "EventList\" or \"CountsCube", n_evfile);
    }

    // ... otherwise extract information from observation definition parameters
    else {

        // Initialise event list definition
        GEbounds ebounds;
        GGti     gti;
        GCTARoi  roi;
        bool     has_ebounds = false;
        bool     has_gti     = false;
        bool     has_roi     = false;

        // Extract parameters (do nothing if parameter does not exist)
        if (gammalib::xml_has_par(xml, "Pointing")) {
            m_pointing.read(xml);
        }
        if (gammalib::xml_has_par(xml, "EnergyBoundaries")) {
            ebounds.read(xml);
            has_ebounds = true;
        }
        if (gammalib::xml_has_par(xml, "GoodTimeIntervals")) {
            gti.read(xml);
            has_gti = true;
        }
        if (gammalib::xml_has_par(xml, "RegionOfInterest")) {
            roi.read(xml);
            has_roi = true;
        }
        if (gammalib::xml_has_par(xml, "Deadtime")) {
            const GXmlElement* par = gammalib::xml_get_par(G_READ, xml, "Deadtime");
            m_deadc                = gammalib::todouble(par->attribute("deadc"));
        }
        else {
            m_deadc = 1.0;
        }

        // If we have at least one event list definition then create event
        // list and attach it to the observation
        if (has_ebounds || has_gti || has_roi) {

            // Create an empty event list
            GCTAEventList events;

            // Handle energy boundaries
            if (has_ebounds) {
                events.ebounds(ebounds);
            }

            // Handle GTIs
            if (has_gti) {
                events.gti(gti);
                ontime(gti.ontime());
                livetime(gti.ontime() * m_deadc);
            }

            // Handle ROIs
            if (has_roi) {
                events.roi(roi);
            }

            // Attach event list
            this->events(events);

        } // endif: handled event list information

    } // endelse: extracted information from observation definition

    // Determine response type as function of the information that is
    // provided in the XML file. The response type is determined from the
    // parameter names. An exception is thrown if the response type cannot
    // be unambigously determined
    int response_type = 0;
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        const GXmlElement* par = xml.element("parameter", i);

        // Check for response type 1 (GCTAResponseIrf)
        if ((par->attribute("name") == "EffectiveArea") ||
            (par->attribute("name") == "ARF") ||
            (par->attribute("name") == "PointSpreadFunction") ||
            (par->attribute("name") == "PSF") ||
            (par->attribute("name") == "EnergyDispersion") ||
            (par->attribute("name") == "RMF") ||
            (par->attribute("name") == "Background") ||
            (par->attribute("name") == "Calibration")) {
            if (response_type == 2) {
                throw GException::xml_invalid_parnames(G_READ, xml,
                      "Incompatible parameter names encountered in the "
                      "response definition of a CTA observation.\n");
            }
            response_type = 1;
        }
        
        // Check for response type 2 (GCTAResponseCube)
        else if ((par->attribute("name") == "ExposureCube") ||
                 (par->attribute("name") == "PsfCube")) {
            if (response_type == 1) {
                throw GException::xml_invalid_parnames(G_READ, xml,
                      "Incompatible parameter names encountered in the "
                      "response definition of a CTA observation.\n");
            }
            response_type = 2;
        }

    } // endfor: looped over all parameters
    
    // Allocate response
    switch (response_type) {
        case 1:
            m_response = new GCTAResponseIrf;
            break;
        case 2:
            m_response = new GCTAResponseCube;
            break;
        default:
            break;
    }

    // Extract response information
    if (m_response != NULL) {
        m_response->read(xml);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write observation into XML element
 *
 * @param[in] xml XML element.
 *
 * @exception GException::invalid_value
 *            No valid events found in observation.
 *
 * Writes information for a CTA observation into an XML element. This method
 * handles to variants: a first where an event list of counts cube is
 * given and a second where the observation definition information is
 * provided by parameter tags. Note that in both bases, a valid event
 * type needs to be set (either @a EventList or @a CountsCube).
 *
 * The XML format for an event list is
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="EventList" file="..."/>
 *       ...
 *     </observation>
 *
 * and for a counts cube is
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="CountsCube" file="..."/>
 *       ...
 *     </observation>
 *
 * The second variant without event information has the following format
 *
 *     <observation name="..." id="..." instrument="...">
 *       <parameter name="Pointing" ra="..." dec="..."/>
 *       <parameter name="Deadtime" deadc="..."/>
 *       ...
 *     </observation>
 *
 * In addition, calibration information can be specified using the format
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="Calibration" database="..." response="..."/>
 *     </observation>
 *
 * If even more control is required over the response, individual file names
 * can be specified using
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="EffectiveArea"       file="..."/>
 *       <parameter name="PointSpreadFunction" file="..."/>
 *       <parameter name="EnergyDispersion"    file="..."/>
 *       <parameter name="Background"          file="..."/>
 *     </observation>
 *
 * In case that a @a CountsCube is handled, the stacked response can also be
 * provided in the format
 *
 *      <observation name="..." id="..." instrument="...">
 *        ...
 *        <parameter name="ExposureCube" file="..."/>
 *        <parameter name="PsfCube"      file="..."/>
 *      </observation>
 *   
 * If user energy thresholds are defined (i.e. threshold values are >0) the
 * additional @a emin and @a emax attributes will be written to the
 * @p observation tag:
 *
 *     <observation name="..." id="..." instrument="..." emin="..." emax="...">
 *
 ***************************************************************************/
void GCTAObservation::write(GXmlElement& xml) const
{
    // Throw an exception if m_eventtype is neither "EventList" nor
    // "CountsCube"
    if ((m_eventtype != "EventList") &&
        (m_eventtype != "CountsCube")) {
        std::string msg;
        if (m_eventtype.length() == 0) {
            msg = "The observation does not contain any events, hence "
                  "it cannot be written into an XML file.";
        }
        else {
            msg = "The observation contains an unknown event type \""+
                  m_eventtype+"\". The event type needs to be either "
                  "\"EventList\" or \"CountsCube\".";
        }
        throw GException::invalid_value(G_WRITE, msg);
    }

    // Add user energy threshold attributes (if set)
    if (m_lo_user_thres > 0.0) {
        xml.attribute("emin", gammalib::str(m_lo_user_thres));
    }
    if (m_hi_user_thres > 0.0) {
        xml.attribute("emax", gammalib::str(m_hi_user_thres));
    }

    // If there is a filename then write the event information to the
    // XML file
    if (m_eventfile.length() > 0) {
        GXmlElement* par = gammalib::xml_need_par(G_WRITE, xml, m_eventtype);
        par->attribute("file", m_eventfile);
    }

    // ... otherwise write the observation definition information
    else {

        // Write pointing, energy bounds, GTIs and ROI
        m_pointing.write(xml);
        events()->ebounds().write(xml);
        events()->gti().write(xml);

        // Write ROI
        const GCTAEventList* list = dynamic_cast<const GCTAEventList*>(events());
        if (list != NULL) {
            list->roi().write(xml);
        }

        // Write deadtime correction factor
        GXmlElement* par = gammalib::xml_need_par(G_WRITE, xml, "Deadtime");
        par->attribute("deadc",  gammalib::str(m_deadc));
    }


    // Write response information
    if (m_response != NULL) {
        m_response->write(xml);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read data from FITS object
 *
 * @param[in] fits FITS object.
 ***************************************************************************/
void GCTAObservation::read(const GFits& fits)
{
    // Delete any existing event container (do not call clear() as we do not
    // want to delete the response function)
    if (m_events != NULL) delete m_events;
    m_events = NULL;

    // If FITS file contains an EVENTS extension we have an unbinned
    // observation ...
    if (fits.contains("EVENTS")) {

        // Allocate event list
        GCTAEventList* events = new GCTAEventList;

        // Assign event list as the observation's event container
        m_events = events;

        // Read event list
        events->read(fits);

        // Read observation attributes from EVENTS extension
        const GFitsHDU& hdu = *fits.at("EVENTS");
        read_attributes(hdu);

    }

    // ... otherwise we have a binned observation
    else {

        // Allocate event cube
        GCTAEventCube* events = new GCTAEventCube;

        // Assign event cube as the observation's event container
        m_events = events;

        // Read event cube
        events->read(fits);

        // Read observation attributes from primary extension
        const GFitsHDU& hdu = *fits.at(0);
        read_attributes(hdu);

    }

    // Set the event type
    set_event_type();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write CTA observation into FITS object.
 *
 * @param[in] fits FITS object.
 *
 * This method does nothing if no events are within the CTA observation.
 ***************************************************************************/
void GCTAObservation::write(GFits& fits) const
{
    // Get pointers on event list
    const GCTAEventList* list = dynamic_cast<const GCTAEventList*>(events());
    const GCTAEventCube* cube = dynamic_cast<const GCTAEventCube*>(events());

    // Case A: Observation contains an event list
    if (list != NULL) {

        // Write event list into FITS file. This method also writes
        // the GTI as they are part of the event list.
        list->write(fits);

        // Write observation attributes into EVENTS header
        GFitsHDU& hdu = *fits.at("EVENTS");
        write_attributes(hdu);

    } // endif: observation contained an event list

    // Case B: Observation contains an event cube
    else if (cube != NULL) {

        // Write events cube into FITS file. This method also writes
        // the energy boundaries and the GTI as they are also part
        // of the event cube.
        cube->write(fits);

        // Write observation attributes into primary header
        GFitsHDU& hdu = *fits.at(0);
        write_attributes(hdu);

    } // endelse: observation contained an event cube

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load event list or counts cube from FITS file
 *
 * @param[in] filename FITS file name.
 *
 * Loads either an event list or a counts cube from a FITS file.
 ***************************************************************************/
void GCTAObservation::load(const std::string& filename)
{
    // Open FITS file
    GFits fits(filename);

    // Read data
    read(fits);

    // Close FITS file
    fits.close();

    // Store event filename
    m_eventfile = filename;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load stacked cube from FITS files
 *
 * @param[in] cntcube Counts cube file name.
 * @param[in] expcube Exposure cube file name.
 * @param[in] psfcube Psf cube file name.
 * @param[in] bkgcube Background cube file name.
 *
 * Loads a counts map, an exposure cube, a Psf cube and a background cube
 * for stacked cube analysis.
 ***************************************************************************/
void GCTAObservation::load(const std::string& cntcube,
                           const std::string& expcube,
                           const std::string& psfcube,
                           const std::string& bkgcube) {

    // Load counts cube FITS file
    load(cntcube);

    // Check whether we have an event cube
    if (dynamic_cast<const GCTAEventCube*>(events()) == NULL) {
        std::string msg = "Specified file \""+cntcube+"\" is not a CTA "
                          "counts cube. Please provide a counts cube.";
        throw GException::invalid_argument(G_LOAD, msg);
    }

    // Load exposure cube
    GCTACubeExposure exposure(expcube);

    // Load Psf cube
    GCTACubePsf psf(psfcube);

    // Load background cube
    GCTACubeBackground background(bkgcube);

    // Attach exposure cube, Psf cube and background cube as response
    response(exposure, psf, background);
 
    // Return
    return;
}


/***********************************************************************//**
 * @brief Save CTA observation into FITS file.
 *
 * @param[in] filename FITS filename.
 * @param[in] clobber Overwrite existing FITS file (default=false).
 ***************************************************************************/
void GCTAObservation::save(const std::string& filename, const bool& clobber) const
{

    // Create FITS file
    GFits fits;

    // Write data into FITS file
    write(fits);

    // Save FITS file
    fits.saveto(filename, clobber);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set event container
 *
 * @param[in] events Event container.
 *
 * Set the event container for this observation by cloning the @p events.
 ***************************************************************************/
void GCTAObservation::events(const GEvents& events)
{
    // Remove an existing event container
    if (m_events != NULL) delete m_events;

    // Clone events
    m_events = events.clone();

    // Set event type
    set_event_type();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Return pointer to event container
 *
 * @return Pointer to event container (always valid).
 *
 * @exception GException::invalid_value
 *            Events could not be fetched from disk.
 *
 * Returns a pointer to the event container. If no events exist the method
 * tries to fetch events from the file specified by the m_eventfile member.
 * If this fails, an exception will be thrown.
 ***************************************************************************/
const GEvents* GCTAObservation::events(void) const
{
    // If event container is offline then bring it online now
    if (m_events == NULL) {

        // Try loading the events from FITS file. Catch any exception. Put
        // the code into a critical zone as it might be called from within
        // a parallelized thread.
        #pragma omp critical
        {
        try {
                const_cast<GCTAObservation*>(this)->load(m_eventfile);
        }
        catch (...) {
            ;
        }
        }

        // Throw an exception if the event container is still not valid
        if (m_events == NULL) {
            std::string msg;
            if (m_eventfile.length() == 0) {
                msg = "The observation does not contain any information about "
                      "the event file, hence the events cannot be fetched "
                      "from disk for access.";
            }
            else {
                msg = "Could no load the event file \""+m_eventfile+"\" into "
                      "the observation. Please check that the file name is "
                      "valid and that the file contains events.";
            }
            throw GException::invalid_value(G_EVENTS, msg);
        }

    }

    // Return pointer to event container
    return m_events;
}


/***********************************************************************//**
 * @brief Dispose events
 *
 * Drops the events from the observation. Be careful with using this method
 * as the events are not saved before being disposed.
 ***************************************************************************/
void GCTAObservation::dispose_events(void)
{
    // Delete any existing event container
    if (m_events != NULL) delete m_events;

    // Signal that we disposed the events
    m_events = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print CTA observation information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing observation information.
 ***************************************************************************/
std::string GCTAObservation::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GCTAObservation ===");

        // Append information
        result.append("\n"+gammalib::parformat("Name")+name());
        result.append("\n"+gammalib::parformat("Identifier")+id());
        result.append("\n"+gammalib::parformat("Instrument")+instrument());
        result.append("\n"+gammalib::parformat("Event file")+eventfile());
        result.append("\n"+gammalib::parformat("Event type")+m_eventtype);
        result.append("\n"+gammalib::parformat("Statistics")+statistics());
        result.append("\n"+gammalib::parformat("Ontime"));
        result.append(gammalib::str(ontime())+" s");
        result.append("\n"+gammalib::parformat("Livetime"));
        result.append(gammalib::str(livetime())+" s");
        result.append("\n"+gammalib::parformat("Deadtime correction"));
        result.append(gammalib::str(m_deadc));

        // Append user energy threshold information
        result.append("\n"+gammalib::parformat("User energy range"));
        if (m_lo_user_thres > 0.0 && m_hi_user_thres) {
            result.append(gammalib::str(m_lo_user_thres));
            result.append(" - ");
            result.append(gammalib::str(m_hi_user_thres));
            result.append(" TeV");
        }
        else if (m_lo_user_thres > 0.0) {
            result.append("> ");
            result.append(gammalib::str(m_lo_user_thres));
            result.append(" TeV");
        }
        else if (m_hi_user_thres > 0.0) {
            result.append("< ");
            result.append(gammalib::str(m_hi_user_thres));
            result.append(" TeV");
        }
        else {
            result.append("undefined");
        }

        // Append detailed information
        GChatter reduced_chatter = gammalib::reduce(chatter);
        if (reduced_chatter > SILENT) {

            // Append pointing
            result.append("\n"+pointing().print(reduced_chatter));

            // Append response
            if (m_response != NULL) {
                result.append("\n"+m_response->print(reduced_chatter));
            }
            else {
                result.append("\n"+gammalib::parformat("Response function"));
                result.append("undefined");
            }

            // Append events
            if (m_events != NULL) {
                result.append("\n"+m_events->print(reduced_chatter));
            }
        
        } // endif: appended detailed information

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
void GCTAObservation::init_members(void)
{
    // Initialise members
    m_instrument = "CTA";
    m_eventfile.clear();
    m_eventtype.clear();
    m_bgdfile.clear();
    m_response = NULL;
    m_pointing.clear();
    m_obs_id        = 0;
    m_ontime        = 0.0;
    m_livetime      = 0.0;
    m_deadc         = 1.0;
    m_ra_obj        = 0.0;
    m_dec_obj       = 0.0;
    m_lo_user_thres = 0.0;
    m_hi_user_thres = 0.0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] obs CTA observation.
 ***************************************************************************/
void GCTAObservation::copy_members(const GCTAObservation& obs)
{
    // Copy members
    m_instrument    = obs.m_instrument;
    m_eventfile     = obs.m_eventfile;
    m_eventtype     = obs.m_eventtype;
    m_bgdfile       = obs.m_bgdfile;
    m_pointing      = obs.m_pointing;
    m_obs_id        = obs.m_obs_id;
    m_ontime        = obs.m_ontime;
    m_livetime      = obs.m_livetime;
    m_deadc         = obs.m_deadc;
    m_ra_obj        = obs.m_ra_obj;
    m_dec_obj       = obs.m_dec_obj;
    m_lo_user_thres = obs.m_lo_user_thres;
    m_hi_user_thres = obs.m_hi_user_thres;

    // Clone members
    m_response = (obs.m_response != NULL) ? obs.m_response->clone() : NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GCTAObservation::free_members(void)
{
    // Free memory
    if (m_response != NULL) delete m_response;

    // Initialise pointers
    m_response = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read observation attributes
 *
 * @param[in] hdu FITS HDU.
 *
 * Reads CTA observation attributes from HDU. Mandatory attributes are
 *
 * RA_PNT   - Right Ascension of pointing
 * DEC_PNT  - Declination of pointing
 * ONTIME   - Exposure time
 * LIVETIME - Livetime
 *
 * and optional attributes are
 *
 * OBJECT   - Name of observed object
 * DEADC    - Deadtime correction
 * RA_OBJ   - Right Ascension of observed object,
 * DEC_OBJ  - Declination of observed object,
 * OBS_ID   - Observation identifier
 * ALT_PNT  - Altitude of pointing above horizon
 * AZ_PNT   - Azimuth of pointing
 *
 * Based on RA_PNT and DEC_PNT, the CTA pointing direction is set. Note that
 * DEADC is computed using DEADC=LIVETIME/ONTIME
 *
 * @todo The actual reader is a minimal reader to accomodate as many
 *       different datasets as possible. Once the CTA data format is fixed
 *       the reader should have more mandatory attributes.
 ***************************************************************************/
void GCTAObservation::read_attributes(const GFitsHDU& hdu)
{
    // Read mandatory attributes
    double ra_pnt  = hdu.real("RA_PNT");
    double dec_pnt = hdu.real("DEC_PNT");
    m_ontime   = (hdu.has_card("ONTIME"))   ? hdu.real("ONTIME") : 0.0;
    m_livetime = (hdu.has_card("LIVETIME")) ? hdu.real("LIVETIME") : 0.0;

    // Read optional attributes
    m_name     = (hdu.has_card("OBJECT"))   ? hdu.string("OBJECT") : "unknown";
    m_deadc    = (hdu.has_card("DEADC"))    ? hdu.real("DEADC") : 0.0;
    m_ra_obj   = (hdu.has_card("RA_OBJ"))   ? hdu.real("RA_OBJ") : 0.0;
    m_dec_obj  = (hdu.has_card("DEC_OBJ"))  ? hdu.real("DEC_OBJ") : 0.0;
    m_obs_id   = (hdu.has_card("OBS_ID"))   ? hdu.integer("OBS_ID") : 0;
    double alt = (hdu.has_card("ALT_PNT"))  ? hdu.real("ALT_PNT") : 0.0;
    double az  = (hdu.has_card("AZ_PNT"))   ? hdu.real("AZ_PNT") : 0.0;

    // Kluge: compute DEADC from livetime and ontime instead of using the
    // keyword value as the original event lists had this values badly
    // assigned
    if (m_ontime > 0) {
        m_deadc = m_livetime / m_ontime;
    }
    else {
        m_deadc = 0.0;
    }

    // Set pointing information
    GSkyDir pnt;
    pnt.radec_deg(ra_pnt, dec_pnt);
    m_pointing.dir(pnt);
    m_pointing.zenith(90.0-alt);
    m_pointing.azimuth(az);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write observation attributes
 *
 * @param[in] hdu FITS HDU.
 ***************************************************************************/
void GCTAObservation::write_attributes(GFitsHDU& hdu) const
{
    // Get time reference
    GTimeReference timeref = events()->gti().reference();

    // Compute some attributes
    double      ra_pnt   = m_pointing.dir().ra_deg();
    double      dec_pnt  = m_pointing.dir().dec_deg();
    double      alt      = 90.0 - m_pointing.zenith();
    double      az       = m_pointing.azimuth();
    double      tstart   = events()->tstart().convert(timeref);
    double      tstop    = events()->tstop().convert(timeref);
    double      telapse  = events()->gti().telapse();
    double      ontime   = events()->gti().ontime();
    double      deadc    = (ontime > 0.0 && livetime() > 0.0) ? 
                           livetime() / ontime : m_deadc;
    std::string utc_obs  = events()->tstart().utc();
    std::string utc_end  = events()->tstop().utc();
    std::string date_obs = utc_obs.substr(0, 10);
    std::string time_obs = utc_obs.substr(11, 8);
    std::string date_end = utc_end.substr(0, 10);
    std::string time_end = utc_end.substr(11, 8);

    // Set observation information
    hdu.card("CREATOR",  "GammaLib",   "Program which created the file");
    hdu.card("TELESCOP", instrument(), "Telescope");
    hdu.card("OBS_ID",   obs_id(),     "Observation identifier");
    hdu.card("DATE_OBS", date_obs,     "Observation start date");
    hdu.card("TIME_OBS", time_obs,     "Observation start time");
    hdu.card("DATE_END", date_end,     "Observation end date");
    hdu.card("TIME_END", time_end,     "Observation end time");

    // Set observation time information
    hdu.card("TSTART",   tstart, "[s] Mission time of start of observation");
    hdu.card("TSTOP",    tstop, "[s] Mission time of end of observation");
    timeref.write(hdu);
    hdu.card("TELAPSE",  telapse, "[s] Mission elapsed time");
    hdu.card("ONTIME",   ontime, "[s] Total good time including deadtime");
    hdu.card("LIVETIME", livetime(), "[s] Total livetime");
    hdu.card("DEADC",    deadc, "Deadtime correction factor");
    hdu.card("TIMEDEL",  1.0, "Time resolution");

    // Set pointing information
    hdu.card("OBJECT",   name(),    "Observed object");
    hdu.card("RA_OBJ",   ra_obj(),  "[deg] Target Right Ascension");
    hdu.card("DEC_OBJ",  dec_obj(), "[deg] Target Declination");
    hdu.card("RA_PNT",   ra_pnt,    "[deg] Pointing Right Ascension");
    hdu.card("DEC_PNT",  dec_pnt,   "[deg] Pointing Declination");
    hdu.card("ALT_PNT",  alt,       "[deg] Average altitude of pointing");
    hdu.card("AZ_PNT",   az,        "[deg] Average azimuth of pointing");
    hdu.card("RADECSYS", "FK5",     "Coordinate system");
    hdu.card("EQUINOX",  2000.0,    "Epoch");
    hdu.card("CONV_DEP", 0.0,       "Convergence depth of telescopes");
    hdu.card("CONV_RA",  0.0,       "[deg] Convergence Right Ascension");
    hdu.card("CONV_DEC", 0.0,       "[deg] Convergence Declination");
    hdu.card("OBSERVER", "string",  "Observer");

    // Telescope information
    hdu.card("N_TELS",   100,      "Number of telescopes in event list");
    hdu.card("TELLIST",  "string", "Telescope IDs");
    hdu.card("GEOLAT",   0.0,      "[deg] Geographic latitude of array centre");
    hdu.card("GEOLON",   0.0,      "[deg] Geographic longitude of array centre");
    hdu.card("ALTITUDE", 0.0,      "[km] Altitude of array centre");

    // Other information
    hdu.card("EUNIT",    "TeV",    "Energy unit");
    hdu.card("EVTVER",   "draft1", "Event list version number");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set event type
 *
 * Depending on the type of events that are stored in the m_events member,
 * set the event type to
 *
 *     "EventList" if m_events is of type GCTAEventList
 *     "CountsCube" if m_events is of type GCTAEventCube
 *     "Events" if m_events is not NULL but neither GCTAEventList nor GCTAEventCube
 *     "" if m_events is NULL
 *
 ***************************************************************************/
void GCTAObservation::set_event_type(void)
{
    // Continue only if events are loaded
    if (m_events != NULL) {

        // Case A: we have a list
        GCTAEventList* list = dynamic_cast<GCTAEventList*>(m_events);
        if (list != NULL) {
            m_eventtype = "EventList";
        }

        // Case B: we have a cube
        else {
            GCTAEventCube* cube = dynamic_cast<GCTAEventCube*>(m_events);
            if (cube != NULL) {
                m_eventtype = "CountsCube";
            }

            // Case C: we don't know what we have
            else {
                m_eventtype = "Events";
            }
        }

    }

    // Return
    return;
}
