/***************************************************************************
 *                GLATEventList.cpp  -  LAT event list class               *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GLATEventList.cpp
 * @brief GLATEventList class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstdio>             // std::sprintf
#include "GLATEventList.hpp"
#include "GException.hpp"
#include "GTools.hpp"
#include "GFitsTable.hpp"
#include "GFitsTableFloatCol.hpp"
#include "GFitsTableDoubleCol.hpp"
#include "GFitsTableLongCol.hpp"
#include "GFitsTableShortCol.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_POINTER                               "GLATEventList::pointer(int)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GLATEventList::GLATEventList(void) : GEventList()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] list Event list from which the instance should be built.
 ***************************************************************************/
GLATEventList::GLATEventList(const GLATEventList& list) : GEventList(list)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(list);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GLATEventList::~GLATEventList(void)
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
 * @param[in] list Event list to be assigned.
 ***************************************************************************/
GLATEventList& GLATEventList::operator= (const GLATEventList& list)
{
    // Execute only if object is not identical
    if (this != &list) {

        // Copy base class members
        this->GEventList::operator=(list);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(list);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear object
 *
 * This method properly resets the object to an initial state.
 ***************************************************************************/
void GLATEventList::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GEventList::free_members();
    this->GEvents::free_members();

    // Initialise members
    this->GEvents::init_members();
    this->GEventList::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone object
***************************************************************************/
GLATEventList* GLATEventList::clone(void) const
{
    return new GLATEventList(*this);
}


/***********************************************************************//**
 * @brief Load LAT events from FT1 file
 *
 * @param[in] ft1name FT1 FITS filename from which events are loaded.
 ***************************************************************************/
void GLATEventList::load(const std::string& ft1name)
{
    // Clear object
    clear();

    // Allocate FITS file
    GFits file;

    // Open FT1 FITS file
    file.open(ft1name);

    // Get HDU
    GFitsTable* hdu = file.table("EVENTS");

    // Load columns
    load_ft1(hdu);

    // Close FITS file
    file.close();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns pointer to an event
 *
 * @param[in] index Event index for which pointer will be returned.
 *
 * @exception GException::out_of_range
 *            Event index not in valid range.
 *
 * This method returns a pointer on an event atom.
 ***************************************************************************/
GLATEventAtom* GLATEventList::pointer(int index)
{
    // Optionally check if the index is valid
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= m_num)
        throw GException::out_of_range(G_POINTER, index, 0, m_num-1);
    #endif

    // Return pointer
    return ((GLATEventAtom*)m_events + index);
}


/***********************************************************************//**
 * @brief Print event list information
 ***************************************************************************/
std::string GLATEventList::print(void) const
{
    // Initialise result string
    std::string result;

    // Append header
    result.append("=== GLATEventList ===\n");
    result.append(parformat("Number of events")+str(number())+"\n");

    // Append DS keywords
    result.append(parformat("Number of DS keywords")+str(m_ds_num));
    for (int i = 0; i < m_ds_num; ++i) {
        result.append("\n"+parformat(" Data selection"));
        result.append("Type="+m_ds_type[i]);
        if (m_ds_unit[i].length() > 0)
            result.append(" Unit="+m_ds_unit[i]);
        if (m_ds_reference[i].length() > 0)
            result.append(" Reference="+m_ds_reference[i]);
    }

    // Append diffuse keywords
    result.append("\n"+parformat("Number of diffuse models")+str(m_num_difrsp));
    for (int i = 0; i < m_num_difrsp; ++i) {
        result.append("\n"+parformat(" Diffuse component"));
        result.append(m_difrsp_label[i]);
    }

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
void GLATEventList::init_members(void)
{
    // Initialise members
    m_num    = 0;
    m_events = NULL;

    // Initialise diffuse models
    m_num_difrsp   = 0;
    m_difrsp_label = NULL;

    // Initialise data selection keys
    m_ds_num       = 0;
    m_ds_type      = NULL;
    m_ds_unit      = NULL;
    m_ds_value     = NULL;
    m_ds_reference = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] list GLATEventList members which should be copied.
 ***************************************************************************/
void GLATEventList::copy_members(const GLATEventList& list)
{
    // Copy attributes
    m_num        = list.m_num;
    m_num_difrsp = list.m_num_difrsp;
    m_ds_num     = list.m_ds_num;

    // If there are events then copy them
    if (m_num > 0 && list.m_events != NULL) {

        // Allocate memory for events
        m_events = new GLATEventAtom[m_num];

        // Copy events using the correct casts
        GLATEventAtom* dst = (GLATEventAtom*)m_events;
        GLATEventAtom* src = (GLATEventAtom*)list.m_events;
        for (int i = 0; i < m_num; ++i)
            *dst++ = *src++;

    }

    // If there are diffuse response model labels then copy them
    if (m_num_difrsp > 0) {

        // Allocate memory for keys
        m_difrsp_label = new std::string[m_num_difrsp];

        // Copy keys
        for (int i = 0; i < m_num_difrsp; ++i)
            m_difrsp_label[i] = list.m_difrsp_label[i];

    } // endif: there were diffuse response model labels

    // If there are data selection keys then copy them
    if (m_ds_num > 0) {

        // Allocate memory for keys
        m_ds_type      = new std::string[m_ds_num];
        m_ds_unit      = new std::string[m_ds_num];
        m_ds_value     = new std::string[m_ds_num];
        m_ds_reference = new std::string[m_ds_num];

        // Copy keys
        for (int i = 0; i < m_ds_num; ++i) {
            m_ds_type[i]      = list.m_ds_type[i];
            m_ds_unit[i]      = list.m_ds_unit[i];
            m_ds_value[i]     = list.m_ds_value[i];
            m_ds_reference[i] = list.m_ds_reference[i];
        }

    } // endif: there were data selection keys to copy

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GLATEventList::free_members(void)
{
    // Free memory.
    if (m_events       != NULL) delete [] m_events;
    if (m_difrsp_label != NULL) delete [] m_difrsp_label;
    if (m_ds_type      != NULL) delete [] m_ds_type;
    if (m_ds_unit      != NULL) delete [] m_ds_unit;
    if (m_ds_value     != NULL) delete [] m_ds_value;
    if (m_ds_reference != NULL) delete [] m_ds_reference;

    // Signal free pointers
    m_events       = NULL;
    m_difrsp_label = NULL;
    m_ds_type      = NULL;
    m_ds_unit      = NULL;
    m_ds_value     = NULL;
    m_ds_reference = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load LAT events from FITS HDU.
 *
 * @param[in] hdu Pointer to event table.
 *
 * Load the event list from an FT1 file into memory. This method loads the
 * events and also reads the data selection keywords.
 ***************************************************************************/
void GLATEventList::load_ft1(GFitsTable* hdu)
{
    // Free and initialise base class members
    this->GEvents::free_members();
    this->GEvents::init_members();

    // Free and initialise base class members
    this->GEventList::free_members();
    this->GEventList::init_members();

    // Free and initialise class members
    free_members();
    init_members();

    // Load event data
    load_events(hdu);

    // Load data selection keywords
    load_ds_keys(hdu);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load LAT events from FITS HDU.
 *
 * @param[in] hdu Pointer to event table.
 *
 * Note that this method does not handle memory deallocation.
 ***************************************************************************/
void GLATEventList::load_events(GFitsTable* hdu)
{
    // Allocate space for keyword name
    char keyword[10];

    // Continue only if HDU is valid
    if (hdu != NULL) {

        // Extract number of events in FT1 file
        m_num = hdu->integer("NAXIS2");

        // If there are events then load them
        if (m_num > 0) {

            // Allocate data
            m_events = new GLATEventAtom[m_num];

            // Get column pointers
            GFitsTableDoubleCol* ptr_time    = (GFitsTableDoubleCol*)hdu->column("TIME");
            GFitsTableFloatCol*  ptr_energy  = (GFitsTableFloatCol*)hdu->column("ENERGY");
            GFitsTableFloatCol*  ptr_ra      = (GFitsTableFloatCol*)hdu->column("RA");
            GFitsTableFloatCol*  ptr_dec     = (GFitsTableFloatCol*)hdu->column("DEC");
            GFitsTableFloatCol*  ptr_theta   = (GFitsTableFloatCol*)hdu->column("THETA");
            GFitsTableFloatCol*  ptr_phi     = (GFitsTableFloatCol*)hdu->column("PHI");
            GFitsTableFloatCol*  ptr_zenith  = (GFitsTableFloatCol*)hdu->column("ZENITH_ANGLE");
            GFitsTableFloatCol*  ptr_azimuth = (GFitsTableFloatCol*)hdu->column("EARTH_AZIMUTH_ANGLE");
            GFitsTableLongCol*   ptr_eid     = (GFitsTableLongCol*)hdu->column("EVENT_ID");
            GFitsTableLongCol*   ptr_rid     = (GFitsTableLongCol*)hdu->column("RUN_ID");
            GFitsTableShortCol*  ptr_recon   = (GFitsTableShortCol*)hdu->column("RECON_VERSION");
            GFitsTableShortCol*  ptr_calib   = (GFitsTableShortCol*)hdu->column("CALIB_VERSION");
            GFitsTableShortCol*  ptr_class   = (GFitsTableShortCol*)hdu->column("EVENT_CLASS");
            GFitsTableShortCol*  ptr_conv    = (GFitsTableShortCol*)hdu->column("CONVERSION_TYPE");
            GFitsTableDoubleCol* ptr_ltime   = (GFitsTableDoubleCol*)hdu->column("LIVETIME");

            // Copy data from columns into GLATEventAtom objects
            GLATEventAtom* ptr = (GLATEventAtom*)m_events;
            for (int i = 0; i < m_num; ++i) {
                ptr[i].m_time.met((*ptr_time)(i));
                ptr[i].m_energy.MeV((*ptr_energy)(i));
                ptr[i].m_dir.radec_deg((*ptr_ra)(i), (*ptr_dec)(i));
                ptr[i].m_theta               = (*ptr_theta)(i);
                ptr[i].m_phi                 = (*ptr_phi)(i);
                ptr[i].m_zenith_angle        = (*ptr_zenith)(i);
                ptr[i].m_earth_azimuth_angle = (*ptr_azimuth)(i);
                ptr[i].m_event_id            = (*ptr_eid)(i);
                ptr[i].m_run_id              = (*ptr_rid)(i);
                ptr[i].m_recon_version       = (*ptr_recon)(i);
                ptr[i].m_calib_version[0]    = (*ptr_calib)(i,0);
                ptr[i].m_calib_version[1]    = (*ptr_calib)(i,1);
                ptr[i].m_calib_version[2]    = (*ptr_calib)(i,2);
                ptr[i].m_event_class         = (*ptr_class)(i);
                ptr[i].m_conversion_type     = (*ptr_conv)(i);
                ptr[i].m_livetime            = (*ptr_ltime)(i);
            }

            // Extract number of diffuse response labels
            m_num_difrsp = hdu->integer("NDIFRSP");

            // Allocate diffuse response components
            if (m_num_difrsp > 0) {

                // Allocate labels
                m_difrsp_label = new std::string[m_num_difrsp];

                // Allocate components
                GLATEventAtom* ptr = (GLATEventAtom*)m_events;
                for (int i = 0; i < m_num; ++i)
                    ptr[i].m_difrsp = new double[m_num_difrsp];

                // Load diffuse columns
                for (int k = 0; k < m_num_difrsp; ++k) {

                    // Set keyword name
                    std::sprintf(keyword, "DIFRSP%d", k);

                    // Get DIFRSP label
                    try {
                        m_difrsp_label[k] = hdu->card(std::string(keyword))->string();
                    }
                    catch (GException::fits_key_not_found &e) {
                        m_difrsp_label[k] = "NONE";
                    }

                    // Get column pointer
                    GFitsTableFloatCol* ptr_dif = (GFitsTableFloatCol*)hdu->column(std::string(keyword));

                    // Copy data from columns into GLATEventAtom objects
                    GLATEventAtom* ptr = (GLATEventAtom*)m_events;
                    for (int i = 0; i < m_num; ++i)
                        ptr[i].m_difrsp[k] = (*ptr_dif)(i);

                } // endfor: looped over diffuse columns

            } // endif: allocated memory for diffuse components
            else
                m_num_difrsp = 0;

        }
        else
            m_num = 0;

    } // endif: HDU was valid

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load Data Selection keywords from FITS HDU.
 *
 * @param[in] hdu Pointer to event table.
 *
 * Note that this method does not handle memory deallocation.
 ***************************************************************************/
void GLATEventList::load_ds_keys(GFitsTable* hdu)
{
    // Continue only if HDU is valid
    if (hdu != NULL) {

        // Get number of data selection keys
        m_ds_num = hdu->integer("NDSKEYS");

        // Get data selection keys
        if (m_ds_num > 0) {

            // Allocate data selection key information
            m_ds_type      = new std::string[m_ds_num];
            m_ds_unit      = new std::string[m_ds_num];
            m_ds_value     = new std::string[m_ds_num];
            m_ds_reference = new std::string[m_ds_num];

            // Allocate space for the keyword
            char keyword[10];

            // Get columns
            for (int i = 0; i < m_ds_num; ++i) {

                // Get DSTYPnn
                try {
                    std::sprintf(keyword, "DSTYP%d", i+1);
                    m_ds_type[i] = hdu->card(std::string(keyword))->string();
                }
                catch (GException::fits_key_not_found &e) {
                    m_ds_type[i] = "";
                }

                // Get DSUNInn
                try {
                    std::sprintf(keyword, "DSUNI%d", i+1);
                    m_ds_unit[i] = hdu->card(std::string(keyword))->string();
                }
                catch (GException::fits_key_not_found &e) {
                    m_ds_unit[i] = "";
                }

                // Get DSVALnn
                try {
                    std::sprintf(keyword, "DSVAL%d", i+1);
                    m_ds_value[i] = hdu->card(std::string(keyword))->string();
                }
                catch (GException::fits_key_not_found &e) {
                    m_ds_value[i] = "";
                }

                // Get DSREFnn
                try {
                    std::sprintf(keyword, "DSREF%d", i+1);
                    m_ds_reference[i] = hdu->card(std::string(keyword))->string();
                }
                catch (GException::fits_key_not_found &e) {
                    m_ds_reference[i] = "";
                }

            } // endfor: looped over data selection keys

        } // endif: there were data selection keys
        else
            m_ds_num = 0;

    } // endif: HDU was valid

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Friends                                   =
 =                                                                         =
 ==========================================================================*/
