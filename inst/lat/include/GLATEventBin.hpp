/***************************************************************************
 *                 GLATEventBin.hpp  -  LAT event bin class                *
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
 * @file GLATEventBin.hpp
 * @brief GLATEventBin class interface definition.
 * @author J. Knodlseder
 */

#ifndef GLATEVENTBIN_HPP
#define GLATEVENTBIN_HPP

/* __ Includes ___________________________________________________________ */
#include <iostream>
#include "GEventBin.hpp"
#include "GEnergy.hpp"
#include "GTime.hpp"
#include "GLATInstDir.hpp"

/* __ Forward declarations _______________________________________________ */
class GLATEventCube;


/***********************************************************************//**
 * @class GLATEventBin
 *
 * @brief GLATEventBin class interface defintion.
 ***************************************************************************/
class GLATEventBin : public GEventBin {

    // Friend classes
    friend class GLATEventCube;

public:
    // Constructors and destructors
    GLATEventBin(void);
    GLATEventBin(const GLATEventBin& bin);
    virtual ~GLATEventBin(void);

    // Operators
    GLATEventBin& operator= (const GLATEventBin& bin);

    // Implemented pure virtual base class methods
    void               clear(void);
    GLATEventBin*      clone(void) const;
    double             size(void) const;
    const GLATInstDir& dir(void) const { return *m_dir; }
    const GEnergy&     energy(void) const { return *m_energy; }
    const GTime&       time(void) const { return *m_time; }
    double             counts(void) const { return *m_counts; }
    double             error(void) const;
    std::string        print(void) const;

    // Other methods
    const double&  omega(void) const { return *m_omega; }
    const GEnergy& ewidth(void) const { return *m_ewidth; }
    const double&  ontime(void) const { return *m_ontime; }
    const int&     index(void) const { return m_index; }
    const int&     ipix(void) const { return m_ipix; }
    const int&     ieng(void) const { return m_ieng; }
    GLATEventCube* cube(void) const { return m_cube; }

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GLATEventBin& bin);
    void free_members(void);

    // Protected members
    GLATEventCube* m_cube;        //!< Event cube back pointer
    int            m_index;       //!< Actual skymap index
    int            m_ipix;        //!< Actual spatial index
    int            m_ieng;        //!< Actual energy index
    GEnergy*       m_energy;      //!< Pointer to bin energy
    GLATInstDir*   m_dir;         //!< Pointer to bin direction
    GTime*         m_time;        //!< Pointer to bin time
    double*        m_counts;      //!< Pointer to number of counts
    double*        m_omega;       //!< Pointer to solid angle of pixel (sr)
    GEnergy*       m_ewidth;      //!< Pointer to energy width of bin
    double*        m_ontime;      //!< Pointer to ontime of bin (seconds)
};

#endif /* GLATEVENTBIN_HPP */
