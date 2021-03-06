/***************************************************************************
 *        GCTAResponseIrf.i - CTA instrument response function class       *
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
 * @file GCTAResponseIrf.i
 * @brief CTA instrument response function class definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAResponseIrf.hpp"
%}


/* __ Typemaps ___________________________________________________________ */
%typemap(out) GCTAAeff* {
    if (dynamic_cast<GCTAAeff2D*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAAeff2D, 0 |  0 );
    }
    else if (dynamic_cast<GCTAAeffArf*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAAeffArf, 0 |  0 );
    }
    else if (dynamic_cast<GCTAAeffPerfTable*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAAeffPerfTable, 0 |  0 );
    }
    else {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAAeff, 0 |  0 );
    }
}
%typemap(out) GCTAPsf* {
    if (dynamic_cast<GCTAPsf2D*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAPsf2D, 0 |  0 );
    }
    else if (dynamic_cast<GCTAPsfKing*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAPsfKing, 0 |  0 );
    }
    else if (dynamic_cast<GCTAPsfPerfTable*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAPsfPerfTable, 0 |  0 );
    }
    else if (dynamic_cast<GCTAPsfVector*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAPsfVector, 0 |  0 );
    }
    else {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAPsf, 0 |  0 );
    }
}
%typemap(out) GCTAEdisp* {
    if (dynamic_cast<GCTAEdispPerfTable*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAEdispPerfTable, 0 |  0 );
    }
    else if (dynamic_cast<GCTAEdispRmf*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAEdispRmf, 0 |  0 );
    }
    else {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTAEdisp, 0 |  0 );
    }
}
%typemap(out) GCTABackground* {
    if (dynamic_cast<GCTABackground3D*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTABackground3D, 0 |  0 );
    }
    else if (dynamic_cast<GCTABackgroundPerfTable*>($1) != NULL) {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTABackgroundPerfTable, 0 |  0 );
    }
    else {
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), SWIGTYPE_p_GCTABackground, 0 |  0 );
    }
}


/***********************************************************************//**
 * @class GCTAResponseIrf
 *
 * @brief CTA instrument response function class
 ***************************************************************************/
class GCTAResponseIrf : public GCTAResponse {
public:
    // Constructors and destructors
    GCTAResponseIrf(void);
    GCTAResponseIrf(const GCTAResponseIrf& rsp);
    explicit GCTAResponseIrf(const GXmlElement& xml);
    GCTAResponseIrf(const std::string& rspname, const GCaldb& caldb);
    virtual ~GCTAResponseIrf(void);

    // Implement pure virtual base class methods
    virtual void             clear(void);
    virtual GCTAResponseIrf* clone(void) const;
    virtual std::string      classname(void) const;
    virtual bool             use_edisp(void) const;
    virtual bool             use_tdisp(void) const;
    virtual double           irf(const GEvent&       event,
                                 const GPhoton&      photon,
                                 const GObservation& obs) const;
    virtual double           npred(const GPhoton&      photon,
                                   const GObservation& obs) const;
    virtual void             read(const GXmlElement& xml);
    virtual void             write(GXmlElement& xml) const;

    // Overload virtual base class methods
    virtual double   irf_radial(const GEvent&       event,
                                const GSource&      source,
                                const GObservation& obs) const;
    virtual double   irf_elliptical(const GEvent&       event,
                                    const GSource&      source,
                                    const GObservation& obs) const;
    virtual double   irf_diffuse(const GEvent&       event,
                                 const GSource&      source,
                                 const GObservation& obs) const;
    virtual double   npred_radial(const GSource&      source,
                                  const GObservation& obs) const;
    virtual double   npred_elliptical(const GSource&      source,
                                      const GObservation& obs) const;
    virtual double   npred_diffuse(const GSource&      source,
                                   const GObservation& obs) const;
    virtual GEbounds ebounds_src(const GEnergy& obsEnergy) const;

    // Other Methods
    bool                  apply_edisp(void) const;
    void                  apply_edisp(const bool& apply_edisp) const;
    GCTAEventAtom*        mc(const double& area, const GPhoton& photon,
                             const GObservation& obs, GRan& ran) const;
    void                  caldb(const GCaldb& caldb);
    const GCaldb&         caldb(void) const;
    void                  load(const std::string& rspname);
    void                  load_aeff(const std::string& filename);
    void                  load_psf(const std::string& filename);
    void                  load_edisp(const std::string& filename);
    void                  load_background(const std::string& filename);
    void                  offset_sigma(const double& sigma);
    double                offset_sigma(void) const;
    const GCTAAeff*       aeff(void) const;
    void                  aeff(GCTAAeff* aeff);
    const GCTAPsf*        psf(void) const;
    void                  psf(GCTAPsf* psf);
    const GCTAEdisp*      edisp(void) const;
    void                  edisp(GCTAEdisp* edisp);
    const GCTABackground* background(void) const;
    void                  background(GCTABackground* background);

    // Low-level response methods
    double aeff(const double& theta,
                const double& phi,
                const double& zenith,
                const double& azimuth,
                const double& srcLogEng) const;
    double psf(const double& delta,
               const double& theta,
               const double& phi,
               const double& zenith,
               const double& azimuth,
               const double& srcLogEng) const;
    double psf_delta_max(const double& theta,
                         const double& phi,
                         const double& zenith,
                         const double& azimuth,
                         const double& srcLogEng) const;
    double edisp(const GEnergy& obsEng,
                 const double&  theta,
                 const double&  phi,
                 const double&  zenith,
                 const double&  azimuth,
                 const double&  srcLogEng) const;
    double npsf(const GSkyDir&      srcDir,
                const double&       srcLogEng,
                const GTime&        srcTime,
                const GCTAPointing& pnt,
                const GCTARoi&      roi) const;
    double nedisp(const GSkyDir&      srcDir,
                  const GEnergy&      srcEng,
                  const GTime&        srcTime,
                  const GCTAPointing& pnt,
                  const GEbounds&     ebds) const;
};


/***********************************************************************//**
 * @brief GCTAResponseIrf class extension
 ***************************************************************************/
%extend GCTAResponseIrf {
    GCTAResponseIrf copy() {
        return (*self);
    }
    GCTAResponseIrf(GResponse* rsp) {
        GCTAResponseIrf* ptr = dynamic_cast<GCTAResponseIrf*>(rsp);
        if (ptr != NULL) {
            return (ptr->clone());
        }
        else {
            throw GException::bad_type("GCTAResponseIrf(GResponse*)",
                                       "GResponse not of type GCTAResponseIrf");
        }
    }
};
