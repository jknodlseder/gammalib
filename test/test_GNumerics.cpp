/***************************************************************************
 *                test_GNumerics.cpp  -  test numerics modules             *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2014 by Jurgen Knodlseder                           *
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
 * @file test_GNumerics.cpp
 * @brief Unit tests implementation for numerics module
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ostream>
#include <stdexcept>
#include <stdlib.h>
#include "test_GNumerics.hpp"

/* __ Namespaces _________________________________________________________ */

/* __ Globals ____________________________________________________________ */


/***********************************************************************//**
 * @brief Set tests
 ***************************************************************************/
void TestGNumerics::set(void)
{
    //set name
    name("GNumerics");

    // Set parameters
    m_sigma = 2.5;

    // Append tests
    append(static_cast<pfunction>(&TestGNumerics::test_integral),"Test GIntegral");
    append(static_cast<pfunction>(&TestGNumerics::test_romberg_integration),"Test Romberg integration");
    append(static_cast<pfunction>(&TestGNumerics::test_adaptive_simpson_integration),"Test adaptive Simpson integration");
    append(static_cast<pfunction>(&TestGNumerics::test_gauss_kronrod_integration),"Test Gauss-Kronrod integration");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone test suite
 *
 * @return Pointer to deep copy of test suite.
 ***************************************************************************/
TestGNumerics* TestGNumerics::clone(void) const
{
    // Clone test suite
    return new TestGNumerics(*this);
}


/***********************************************************************//**
 * @brief Test model parameter handling
 ***************************************************************************/
void TestGNumerics::test_integral(void)
{
    // Test integral and integrand allocation
    Gauss     integrand(m_sigma);
    GIntegral integral(&integrand);

    // Exit test
    return;
}

/***********************************************************************//**
 * @brief Test Romberg integration
 ***************************************************************************/
void TestGNumerics::test_romberg_integration(void)
{
    // Set-up integral
    Gauss     integrand(m_sigma);
    GIntegral integral(&integrand);

    // Integrate over the entire Gaussian
    double result = integral.romberg(-10.0*m_sigma, 10.0*m_sigma);
    test_value(result,1.0,1.0e-6,"","Gaussian integral is not 1.0 (integral="+gammalib::str(result)+")");

    // Test [-1sigma, 1sigma]
    result = integral.romberg(-m_sigma, m_sigma);
    test_value(result,0.68268948130801355,1.0e-6,"","Gaussian integral is not 0.682689 (difference="+gammalib::str((result-0.68268948130801355))+")");

    // Test [0.0, 1sigma]
    result = integral.romberg(0.0, m_sigma);
    test_value(result,0.3413447460687748,1.0e-6,"","Gaussian integral is not 0.341345 (difference="+gammalib::str((result-0.3413447460687748))+")");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Test adaptive Simpson integration
 ***************************************************************************/
void TestGNumerics::test_adaptive_simpson_integration(void)
{
    // Set-up integral
    Gauss     integrand(m_sigma);
    GIntegral integral(&integrand);

    // Integrate over the entire Gaussian
    double result = integral.adaptive_simpson(-10.0*m_sigma, 10.0*m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-1.0 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,1.0,1.0e-6,"","Gaussian integral is not 1.0 (integral="+gammalib::str(result)+")");

    // Test [-1sigma, 1sigma]
    result = integral.adaptive_simpson(-m_sigma, m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-0.68268948130801355 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,0.68268948130801355,1.0e-6,"","Gaussian integral is not 0.682689 (difference="+gammalib::str((result-0.68268948130801355))+")");

    // Test [0.0, 1sigma]
    result = integral.adaptive_simpson(0.0, m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-0.3413447460687748 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,0.3413447460687748,1.0e-6,"","Gaussian integral is not 0.341345 (difference="+gammalib::str((result-0.3413447460687748))+")");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Test Gauss-Kronrod integration
 ***************************************************************************/
void TestGNumerics::test_gauss_kronrod_integration(void)
{
    // Set-up integral
    Gauss     integrand(m_sigma);
    GIntegral integral(&integrand);

    // Integrate over the entire Gaussian
    double result = integral.gauss_kronrod(-10.0*m_sigma, 10.0*m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-1.0 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,1.0,1.0e-6,"","Gaussian integral is not 1.0 (integral="+gammalib::str(result)+")");

    // Test [-1sigma, 1sigma]
    result = integral.gauss_kronrod(-m_sigma, m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-0.68268948130801355 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,0.68268948130801355,1.0e-6,"","Gaussian integral is not 0.682689 (difference="+gammalib::str((result-0.68268948130801355))+")");

    // Test [0.0, 1sigma]
    result = integral.gauss_kronrod(0.0, m_sigma);
/*
std::cout << result << std::endl;
std::cout << result-0.3413447460687748 << std::endl;
std::cout << integral << std::endl;
*/
    test_value(result,0.3413447460687748,1.0e-6,"","Gaussian integral is not 0.341345 (difference="+gammalib::str((result-0.3413447460687748))+")");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Main test function
 ***************************************************************************/
int main(void)
{
    GTestSuites testsuite("GNumerics");

    bool was_successful=true;

    //Create a test suite
    TestGNumerics test;

    //Append to the container
    testsuite.append(test);

    //Run
    was_successful=testsuite.run();

    //save xml report
    testsuite.save("reports/GNumerics.xml");

    // Return
    return was_successful ? 0:1;
}
