/***************************************************************************
 *           GOptimizerLM.i - Levenberg Marquardt optimizer class          *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2014 by Juergen Knoedlseder                         *
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
 * @file GOptimizerLM.i
 * @brief Levenberg Marquardt optimizer class Pyhton interface definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GOptimizerLM.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GOptimizerLM
 *
 * @brief GOptimizerLM class SWIG interface definition.
 ***************************************************************************/
class GOptimizerLM : public GOptimizer {
public:

    // Constructors and destructors
    GOptimizerLM(void);
    GOptimizerLM(GLog& log);
    GOptimizerLM(const GOptimizerLM& opt);
    virtual ~GOptimizerLM(void);

    // Implemented pure virtual methods
    virtual void          clear(void);
    virtual GOptimizerLM* clone(void) const;
    virtual std::string   classname(void) const;
    virtual void          optimize(GOptimizerFunction& fct, GOptimizerPars& pars);
    virtual void          errors(GOptimizerFunction& fct, GOptimizerPars& pars);
    virtual double        value(void) const;
    virtual int           status(void) const;
    virtual int           iter(void) const;
    
    // Methods
    void          max_iter(const int& max_iter);
    void          max_stalls(const int& max_stalls);
    void          max_boundary_hits(const int& max_hit);
    void          lambda_start(const double& value);
    void          lambda_inc(const double& value);
    void          lambda_dec(const double& value);
    void          eps(const double& eps);
    void          accept_dec(const double& value);
    int           max_iter(void) const;
    int           max_stalls(void) const;
    int           max_boundary_hits(void) const;
    const double& lambda_start(void) const;
    const double& lambda_inc(void) const;
    const double& lambda_dec(void) const;
    const double& eps(void) const;
    const double& accept_dec(void);
};


/***********************************************************************//**
 * @brief GOptimizerLM class extension
 ***************************************************************************/
%extend GOptimizerLM {
    GOptimizerLM copy() {
        return (*self);
    }
    const double& lambda_value(void) {
        return (self->lambda());
    }
};
