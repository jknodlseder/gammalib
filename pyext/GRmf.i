/***************************************************************************
 *             GRmf.i - XSPEC Redistribution Matrix File class             *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2013-2015 by Juergen Knoedlseder                         *
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
 * @file GRmf.i
 * @brief XSPEC Redistribution Matrix File class
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GRmf.hpp"
%}

/* __ Includes ___________________________________________________________ */
%include "GTypemaps.i"


/***********************************************************************//**
 * @class GRmf
 *
 * @brief Redistribution Matrix File class
 ***************************************************************************/
class GRmf : public GBase {
public:
    // Constructors and destructors
    GRmf(void);
    explicit GRmf(const std::string& filename);
    GRmf(const GEbounds& etrue, const GEbounds& emeasured);
    GRmf(const GRmf& rmf);
    virtual ~GRmf(void);

    // Methods
    void                 clear(void);
    GRmf*                clone(void) const;
    std::string          classname(void) const;
    int                  size(void) const;
    int                  ntrue(void) const;
    int                  nmeasured(void) const;
    double&              at(const int& itrue, const int& imeasured);
    const GEbounds&      etrue(void) const;
    const GEbounds&      emeasured(void) const;
    GEbounds             etrue(const GEnergy& emeasured) const;
    GEbounds             emeasured(const GEnergy& etrue) const;
    const GMatrixSparse& matrix(void) const;
    void                 load(const std::string& filename);
    void                 save(const std::string& filename,
                              const bool& clobber = false,
                              const std::string& unit = "keV") const;
    void                 read(const GFitsTable& table);
    void                 write(GFits& fits,
                               const std::string& unit = "keV") const;
    const std::string&   filename(void) const;
};


/***********************************************************************//**
 * @brief GRmf class extension
 ***************************************************************************/
%extend GRmf {
    double __getitem__(int GTuple[2]) {
        return self->at(GTuple[0], GTuple[1]);
    }
    void __setitem__(int GTuple[2], const double& value) {
        self->at(GTuple[0], GTuple[1]) = value;
    }
    int __len__() {
        return (self->size());
    }
    GRmf copy() {
        return (*self);
    }
};
