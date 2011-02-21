/***************************************************************************
 *               GResponse.i  -  Abstract response base class              *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GResponse.i
 * @brief Abstract response base class Python interface definition
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GResponse.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GResponse
 *
 * @brief Abstract response base class
 ***************************************************************************/
class GResponse {

public:
    // Constructors and destructors
    GResponse(void);
    GResponse(const GResponse& rsp);
    virtual ~GResponse(void);

    // Pure virtual methods
    virtual void        clear(void) = 0;
    virtual GResponse*  clone(void) const = 0;
    virtual bool        hasedisp(void) const = 0;
    virtual bool        hastdisp(void) const = 0;
    virtual double      irf(const GInstDir&     obsDir,
                            const GEnergy&      obsEng,
                            const GTime&        obsTime,
                            const GSkyDir&      srcDir,
                            const GEnergy&      srcEng,
                            const GTime&        srcTime,
                            const GObservation& obs) const = 0;
    virtual double      npred(const GSkyDir&      srcDir,
                              const GEnergy&      srcEng,
                              const GTime&        srcTime,
                              const GObservation& obs) const = 0;

    // Virtual methods
    virtual double irf(const GEvent&       event,
                       const GModelSky&    model,
                       const GEnergy&      srcEng,
                       const GTime&        srcTime,
                       const GObservation& obs) const;
    virtual double irf_ptsrc(const GInstDir&       obsDir,
                             const GEnergy&           obsEng,
                             const GTime&             obsTime,
                             const GModelPointSource& model,
                             const GEnergy&           srcEng,
                             const GTime&             srcTime,
                             const GObservation&   obs) const;
    virtual double irf_extended(const GInstDir&             obsDir,
                                const GEnergy&              obsEng,
                                const GTime&                obsTime,
                                const GModelExtendedSource& model,
                                const GEnergy&              srcEng,
                                const GTime&                srcTime,
                                const GObservation&         obs) const;
    virtual double irf_diffuse(const GInstDir&            obsDir,
                               const GEnergy&             obsEng,
                               const GTime&               obsTime,
                               const GModelDiffuseSource& model,
                               const GEnergy&             srcEng,
                               const GTime&               srcTime,
                               const GObservation&        obs) const;
    virtual double npred(const GModelSky&    model,
                         const GEnergy&      srcEng,
                         const GTime&        srcTime,
                         const GObservation& obs) const;
    virtual double npred_ptsrc(const GModelPointSource& model,
                               const GEnergy&           srcEng,
                               const GTime&             srcTime,
                               const GObservation&      obs) const;
    virtual double npred_extended(const GModelExtendedSource& model,
                                  const GEnergy&              srcEng,
                                  const GTime&                srcTime,
                                  const GObservation&         obs) const;
    virtual double npred_diffuse(const GModelDiffuseSource& model,
                                 const GEnergy&             srcEng,
                                 const GTime&               srcTime,
                                 const GObservation&        obs) const;
};


/***********************************************************************//**
 * @brief GResponse class extension
 ***************************************************************************/
%extend GResponse {
    char *__str__() {
        return tochar(self->print());
    }
};
