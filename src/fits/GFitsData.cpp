/***************************************************************************
 *         GFitsData.cpp  - FITS data handling abstract base class         *
 * ----------------------------------------------------------------------- *
 *  copyright            : (C) 2008 by Jurgen Knodlseder                   *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/

/* __ Includes ___________________________________________________________ */
#include "GException.hpp"
#include "GFitsData.hpp"

/* __ Namespaces _________________________________________________________ */

/* __ Method name definitions ____________________________________________ */

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */



/*==========================================================================
 =                                                                         =
 =                     GFitsData constructors/destructors                  =
 =                                                                         =
 ==========================================================================*/

/***************************************************************************
 *                                Constructor                              *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
GFitsData::GFitsData()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***************************************************************************
 *                              Copy constructor                           *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
GFitsData::GFitsData(const GFitsData& data)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(data);

    // Return
    return;
}


/***************************************************************************
 *                               Destructor                                *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
GFitsData::~GFitsData()
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                           GFitsData operators                           =
 =                                                                         =
 ==========================================================================*/

/***************************************************************************
 *                            Assignment operator                          *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
GFitsData& GFitsData::operator= (const GFitsData& data)
{
    // Execute only if object is not identical
    if (this != &data) {

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(data);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                         GFitsData public methods                        =
 =                                                                         =
 ==========================================================================*/

/***************************************************************************
 *                               Open Data                                 *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
void GFitsData::open(__fitsfile* fptr)
{
    // Return
    return;
}


/***************************************************************************
 *                               Close Data                                *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
void GFitsData::close(void)
{
    // Free members
    free_members();

    // Initialise members
    init_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                        GFitsData private methods                        =
 =                                                                         =
 ==========================================================================*/

/***************************************************************************
 *                         Initialise class members                        *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
void GFitsData::init_members(void)
{
    // Initialise members

    // Return
    return;
}


/***************************************************************************
 *                            Copy class members                           *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
void GFitsData::copy_members(const GFitsData& data)
{
    // Copy attributes

    // Copy data

    // Return
    return;
}


/***************************************************************************
 *                           Delete class members                          *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
void GFitsData::free_members(void)
{
    // Free memory

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             GFitsData friends                           =
 =                                                                         =
 ==========================================================================*/


/*==========================================================================
 =                                                                         =
 =                     Other functions used by GFitsData                   =
 =                                                                         =
 ==========================================================================*/
