/***************************************************************************
 *        GModelTemporalConst.cpp  -  Temporal constant model class        *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GModelTemporalConst.cpp
 * @brief GModelTemporalConst class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GModelTemporalConst.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_PAR                           "GModelTemporalConst::par(int) const"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GModelTemporalConst::GModelTemporalConst(void) : GModelTemporal()
{
    // Initialise private members for clean destruction
    init_members();
  
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Model from which the instance should be built.
 ***************************************************************************/
GModelTemporalConst::GModelTemporalConst(const GModelTemporalConst& model) : 
    GModelTemporal(model)
{ 
    // Initialise private members for clean destruction
    init_members();

    // Copy members
    copy_members(model);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelTemporalConst::~GModelTemporalConst(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                Operators                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] model Model which should be assigned.
 ***************************************************************************/
GModelTemporalConst& GModelTemporalConst::operator= (const GModelTemporalConst& model)
{ 
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelTemporal::operator=(model);

        // Free members
        free_members();

        // Initialise private members for clean destruction
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
 * @brief Get pointer to model parameter
 *
 * @param[in] index Parameter index.
 ***************************************************************************/
GModelPar* GModelTemporalConst::par(int index) const
{
    // If index is outside boundary then throw an error
    if (index < 0 || index >= m_npars)
        throw GException::out_of_range(G_PAR, index, 0, m_npars-1);
    
    // Return parameter pointer
    return m_par[index];
}


/***********************************************************************//**
 * @brief Evaluate function
 *
 * @param[in] obsTime Observed photon arrival time (not used).
 * @param[in] srcDir True photon arrival direction (not used).
 * @param[in] srcEng True energy of photon (not used).
 * @param[in] srcTime True photon arrival time (not used).
 * @param[in] rsp Instrument response function (not used).
 * @param[in] pnt Instrument pointing (not used).
 *
 * This method implements the temporal component of a constant model.
 * It returns a value of 1 in all cases.
 ***************************************************************************/
double GModelTemporalConst::eval(const GTime& obsTime, const GSkyDir& srcDir,
                                 const GEnergy& srcEng, const GTime& srcTime,
                                 const GResponse& rsp, const GPointing& pnt)
{
    // Return
    return 1.0;
}


/***********************************************************************//**
 * @brief Evaluate function and gradients
 *
 * @param[in] obsTime Observed photon arrival time (not used).
 * @param[in] srcDir True photon arrival direction (not used).
 * @param[in] srcEng True energy of photon (not used).
 * @param[in] srcTime True photon arrival time (not used).
 * @param[in] rsp Instrument response function (not used).
 * @param[in] pnt Instrument pointing (not used).
 *
 * This method implements the temporal component of a constant model.
 * It returns a value of 1 and a gradient of 0 in all cases.
 ***************************************************************************/
double GModelTemporalConst::eval_gradients(const GTime& obsTime,
                                           const GSkyDir& srcDir,
                                           const GEnergy& srcEng,
                                           const GTime& srcTime,
                                           const GResponse& rsp,
                                           const GPointing& pnt)
{
    // Set gradient to 0
    m_norm.gradient(0.0);

    // Return
    return 1.0;
}


/*==========================================================================
 =                                                                         =
 =                   GModelTemporalConst private methods                   =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelTemporalConst::init_members(void)
{
    // Initialise parameters
    m_npars  = 1;
    m_par[0] = &m_norm;
    
    // Initialise normalisation parameter
    m_norm = GModelPar();
    m_norm.name("Constant");
    m_norm.unit("(relative value)");
    m_norm.value(1.0);
    m_norm.fix();
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model GModelTemporalConst members which should be copied.
 ***************************************************************************/
void GModelTemporalConst::copy_members(const GModelTemporalConst& model)
{
    // Copy model parameters (we do not need to copy the rest since it is
    // static)
    m_norm  = model.m_norm;
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelTemporalConst::free_members(void)
{
    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone class
***************************************************************************/
GModelTemporalConst* GModelTemporalConst::clone(void) const
{
    return new GModelTemporalConst(*this);
}


/*==========================================================================
 =                                                                         =
 =                        GModelTemporalConst friends                       =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Put model in output stream
 *
 * @param[in] os Output stream into which the model will be dumped
 * @param[in] model Model to be dumped
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GModelTemporalConst& model)
{
    // Put observation in stream
    os << "=== GModelTemporalConst ===" << std::endl;
    os << " Number of parameters ......: " << model.m_npars << std::endl;
    for (int i = 0; i < model.m_npars; ++i) {
        if (i > 0)
            os << std::endl;
        os << " Parameter .................: " << *(model.m_par[i]);
    }
        
    // Return output stream
    return os;
}


/*==========================================================================
 =                                                                         =
 =                Other functions used by GModelTemporalConst               =
 =                                                                         =
 ==========================================================================*/
