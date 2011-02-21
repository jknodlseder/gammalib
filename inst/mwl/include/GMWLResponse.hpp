/***************************************************************************
 *          GMWLResponse.hpp  -  Multi-wavelength response class           *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GMWLResponse.hpp
 * @brief Multi-wavelength response class interface definition
 * @author J. Knodlseder
 */

#ifndef GMWLRESPONSE_HPP
#define GMWLRESPONSE_HPP

/* __ Includes ___________________________________________________________ */
#include "GResponse.hpp"


/***********************************************************************//**
 * @class GMWLResponse
 *
 * @brief Multi-wavelength response class
 *
 * This class implements a dummy response class for multi-wavelength
 * observations. Since the multi-wavelength instrument classes handles data
 * that a provided in photon space, no instrument response is in fact needed.
 * The dummy response implemented by this class provides a simple diagonal
 * response matrix that allows integration of multi-wavelength observations
 * using the standard instrument specific interface.
 ***************************************************************************/
class GMWLResponse : public GResponse {

public:
    // Constructors and destructors
    GMWLResponse(void);
    GMWLResponse(const GMWLResponse& rsp);
    virtual ~GMWLResponse(void);

    // Operators
    virtual GMWLResponse& operator= (const GMWLResponse & rsp);

    // Implemented pure virtual methods
    virtual void          clear(void);
    virtual GMWLResponse* clone(void) const;
    virtual bool          hasedisp(void) const { return false; }
    virtual bool          hastdisp(void) const { return false; }
    virtual double        irf(const GInstDir&     obsDir,
                              const GEnergy&      obsEng,
                              const GTime&        obsTime,
                              const GSkyDir&      srcDir,
                              const GEnergy&      srcEng,
                              const GTime&        srcTime,
                              const GObservation& obs) const { return 1.0; }
    virtual double       npred(const GSkyDir& srcDir,
                               const GEnergy& srcEng,
                               const GTime& srcTime,
                               const GObservation& obs) const { return 1.0; }
    virtual std::string  print(void) const;

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GMWLResponse& pnt);
    void free_members(void);
};

#endif /* GMWLRESPONSE_HPP */
