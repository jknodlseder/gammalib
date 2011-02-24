/***************************************************************************
 * GCTAModelRadialAcceptance.i  -  Radial acceptance model class python I/F*
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
 * @file GCTAModelRadialAcceptance.i
 * @brief Radial acceptance model class Python interface definition
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAModelRadialAcceptance.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GCTAModelRadialAcceptance
 *
 * @brief Radial acceptance model class
 ***************************************************************************/
class GCTAModelRadialAcceptance : public GModelData {

public:
    // Constructors and destructors
    GCTAModelRadialAcceptance(void);
    explicit GCTAModelRadialAcceptance(const GXmlElement& xml);
    explicit GCTAModelRadialAcceptance(const GCTAModelRadial& radial,
                                       const GModelSpectral& spectral);
    GCTAModelRadialAcceptance(const GCTAModelRadialAcceptance& model);
    virtual ~GCTAModelRadialAcceptance(void);

    // Implemented pure virtual methods
    virtual void                       clear(void);
    virtual GCTAModelRadialAcceptance* clone(void) const;
    virtual std::string                type(void) const;
    virtual double                     eval(const GEvent& event,
                                            const GObservation& obs) const;
    virtual double                     eval_gradients(const GEvent& event,
                                                      const GObservation& obs) const;
    virtual double                     npred(const GEnergy& obsEng, const GTime& obsTime,
                                             const GObservation& obs) const;
    virtual GCTAEventList*             mc(const GObservation& obs, GRan& ran) const;
    virtual void                       read(const GXmlElement& xml);
    virtual void                       write(GXmlElement& xml) const;

    // Other methods
    GCTAModelRadial* radial(void)   const;
    GModelSpectral*  spectral(void) const;
    GModelTemporal*  temporal(void) const;
};


/***********************************************************************//**
 * @brief GCTAModelRadial class extension
 ***************************************************************************/
%extend GCTAModelRadial {
};
