/***************************************************************************
 *                          GTools.cpp  -  GammaLib tools                  *
 * ----------------------------------------------------------------------- *
 *  copyright            : (C) 2008 by Jurgen Knodlseder                   *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* __ Includes ___________________________________________________________ */
#include "GTools.hpp"

/* __ Namespaces _________________________________________________________ */


/***************************************************************************
 *             Strip leading and trailing whitespace from string           *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
std::string strip_whitespace(const std::string arg)
{
    // Return empty string if argument is empty
    if (arg.empty())
        return arg;

    // Get start and stop
    std::string::size_type start = arg.find_first_not_of(' ');
    std::string::size_type stop  = arg.find_last_not_of(' ');

    // If there is only whitespace in string then return empty string.
    // Otherwise strip off whitespace
    std::string result;
    if (start <= stop)
        result = arg.substr(start, stop-start+1);

    // Return result
    return result;
}
