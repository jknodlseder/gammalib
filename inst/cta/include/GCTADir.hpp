/***************************************************************************
 *                   GCTADir.hpp  -  CTA direction class                   *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011 by Jurgen Knodlseder                                *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GCTADir.hpp
 * @brief CTA camera direction class interface definition
 * @author J. Knodlseder
 */

#ifndef GCTADIR_HPP
#define GCTADIR_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include <iostream>
#include "GLog.hpp"
#include "GSkyDir.hpp"
#include "GCTAPointing.hpp"
#include "GCTAInstDir.hpp"


/***********************************************************************//**
 * @class GCTADir
 *
 * @brief CTA camera direction class.
 *
 * This class implements a coordinate in the CTA camera system. The
 * coordinate is defined by the radial offset angle theta and the polar
 * angle phi. Both are stored in units of radians in this class.
 ***************************************************************************/
class GCTADir {

    // I/O friends
    friend std::ostream& operator<<(std::ostream& os, const GCTADir& dir);
    friend GLog&         operator<<(GLog& log,        const GCTADir& dir);

public:
    // Constructors and destructors
    GCTADir(void);
    GCTADir(const GCTADir& dir);
    explicit GCTADir(const GSkyDir& dir, const GCTAPointing& pnt);
    explicit GCTADir(const GCTAInstDir& dir, const GCTAPointing& pnt);
    virtual ~GCTADir(void);

    // Operators
    GCTADir& operator=(const GCTADir& dir);

    // Methods
    void          clear(void);
    GCTADir*      clone(void) const;
    void          dir(const GSkyDir& dir, const GCTAPointing& pnt);
    void          dir(const GCTAInstDir& dir, const GCTAPointing& pnt);
    const double& theta(void) const { return m_theta; }
    const double& phi(void) const { return m_phi; }
    double        theta_deg(void) const;
    double        phi_deg(void) const;
    const double& costheta(void) const;
    const double& sintheta(void) const;
    std::string   print(void) const;

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GCTADir& dir);
    void free_members(void);
    void update(void) const;

    // Protected members
    double         m_theta;      //!< Radial offset angle in radians
    double         m_phi;        //!< Polar angle in radians

    // Precomputation cache
    mutable bool   m_has_cache;  //!< Cache validity flag
    mutable double m_cos_theta;  //!< Cosine of radial offset angle
    mutable double m_sin_theta;  //!< Sine of radial offset angle
};

#endif /* GCTADIR_HPP */
