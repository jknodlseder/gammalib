/***************************************************************************
 *                   GException.cpp  -  exception handlers                 *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2006-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GException.cpp
 * @brief Exception handler interface implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#include "GException.hpp"
#include "GTools.hpp"


/***********************************************************************//**
 * @brief Exception message.
 ***************************************************************************/
const char* GExceptionHandler::what() const throw()
{
    std::string message = "*** ERROR in " + m_origin + ": " + m_message;
    return message.c_str();
}


/***********************************************************************//**
 * @brief Feature not implement
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional message.
 ***************************************************************************/
GException::feature_not_implemented::feature_not_implemented(std::string origin,
                                                             std::string message)
{
    m_origin   = origin;
    if (message.length() > 0)
        m_message = message;
    else
        m_message = "Feature not implemented.";
    m_message += " In case that you need this feature for your application"
                 " please submit a feature request on"
                 " https://sourceforge.net/projects/gammalib/,"
                 " join this error message and provide a detailed"
                 " description of your needs.";
    return;
}


/***********************************************************************//**
 * @brief Bad type
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional message.
 ***************************************************************************/
GException::bad_type::bad_type(std::string origin, std::string message)
{
    m_origin   = origin;
    m_message = "Invalud type conversion. " + message;
    return;
}


/***********************************************************************//**
 * @brief Memory allocation exception.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] num Requested amount of memory.
 ***************************************************************************/
GException::mem_alloc::mem_alloc(std::string origin, unsigned num)
{
    m_origin  = origin;
    m_message = "Memory allocation error (" + str((int)num) + " elements)";
    return;
}


/***********************************************************************//**
 * @brief Not enough nodes in node array
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] num Number of nodes in node array.
 ***************************************************************************/
GException::not_enough_nodes::not_enough_nodes(std::string origin, int num)
{
    m_origin  = origin;
    m_message = "Not enough nodes in node array (" + str(num) + " nodes).";
    return;
}


/***********************************************************************//**
 * @brief File not found
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] filename Filename.
 ***************************************************************************/
GException::file_not_found::file_not_found(std::string origin, std::string filename)
{
    m_origin  = origin;
    m_message = "File \"" + filename +"\" not found.";
    return;
}


/***********************************************************************//**
 * @brief File open error
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] filename Filename.
 ***************************************************************************/
GException::file_open_error::file_open_error(std::string origin, std::string filename)
{
    m_origin  = origin;
    m_message = "Unable to open file \"" + filename +"\"";
    return;
}


/***********************************************************************//**
 * @brief Bad CSV columns encountered
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] filename Filename.
 * @param[in] row CSV file row.
 * @param[in] cols Expected number of columns.
 * @param[in] elements Found number of columns.
 * @param[in] message Optional message.
 ***************************************************************************/
GException::cvs_bad_columns::cvs_bad_columns(std::string origin,
                                             std::string filename,
                                             int         row,
                                             int         cols,
                                             int         elements,
                                             std::string message)
{
    m_origin  = origin;
    m_message = "Inconsistent number of columns found in row "+str(row)+
                " of file \""+filename+"\" ("+str(cols)+" expected, "+
                str(elements)+" encountered). "+message;
    return;
}
