/***************************************************************************
 *            GException_model.cpp  -  Model exception handlers            *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GException_model.cpp
 * @brief Implement exceptions for the model module
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GModelRegistry.hpp"
#include "GModelSpatialRegistry.hpp"
#include "GModelSpectralRegistry.hpp"
#include "GModelTemporalRegistry.hpp"
#include "GTools.hpp"


/***********************************************************************//**
 * @brief Invalid model type
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Model type that has been encountered.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid::model_invalid(std::string origin,
                                         std::string type,
                                         std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid model type \""+type+"\" encountered. " + message;

    // Add list of valid models
    GModelRegistry registry;
    if (registry.size() > 0) {
        m_message += "The following models are registered: ";
        for (int i = 0; i < registry.size(); ++i) {
            if (i > 0)
                m_message += ", ";
            m_message += "\"" + registry.name(i) + "\"";
        }
        m_message += ".";
    }
    else
        m_message += "No models are registered.";

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid spatial model type
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Spatial model type that has been encountered.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid_spatial::model_invalid_spatial(std::string origin,
                                                         std::string type,
                                                         std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid spatial model type \""+type+"\" encountered. " +
                message;

    // Add list of valid spatial models
    GModelSpatialRegistry registry;
    if (registry.size() > 0) {
        m_message += "The following models are registered: ";
        for (int i = 0; i < registry.size(); ++i) {
            if (i > 0)
                m_message += ", ";
            m_message += "\"" + registry.name(i) + "\"";
        }
        m_message += ".";
    }
    else
        m_message += "No models are registered.";

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid spectral model type
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Spectral model type that has been encountered.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid_spectral::model_invalid_spectral(std::string origin,
                                                           std::string type,
                                                           std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid spectral model type \""+type+"\" encountered. " +
                message;

    // Add list of valid spectral models
    GModelSpectralRegistry registry;
    if (registry.size() > 0) {
        m_message += "The following models are registered: ";
        for (int i = 0; i < registry.size(); ++i) {
            if (i > 0)
                m_message += ", ";
            m_message += "\"" + registry.name(i) + "\"";
        }
        m_message += ".";
    }
    else
        m_message += "No models are registered.";

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid temporal model type
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Temporal model type that has been encountered.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid_temporal::model_invalid_temporal(std::string origin,
                                                           std::string type,
                                                           std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid temporal model type \""+type+"\" encountered. " +
                message;

    // Add list of valid spectral models
    GModelTemporalRegistry registry;
    if (registry.size() > 0) {
        m_message += "The following models are registered: ";
        for (int i = 0; i < registry.size(); ++i) {
            if (i > 0)
                m_message += ", ";
            m_message += "\"" + registry.name(i) + "\"";
        }
        m_message += ".";
    }
    else
        m_message += "No models are registered.";

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid number of model parameters in XML element
 *
 * @param[in] origin Method that throws the error.
 * @param[in] xml XML element.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid_parnum::model_invalid_parnum(std::string origin,
                                                       GXmlElement xml,
                                                       std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid number of model parameters found in XML element. " +
                message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid model parameter names in XML element
 *
 * @param[in] origin Method that throws the error.
 * @param[in] xml XML element.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::model_invalid_parnames::model_invalid_parnames(std::string origin,
                                                           GXmlElement xml,
                                                           std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "Invalid model parameter names found in XML element. " +
                message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Not enough nodes in file function
 *
 * @param[in] origin Method that throws the error.
 * @param[in] filename File function filename.
 * @param[in] num Number of nodes.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::not_enough_data::not_enough_data(std::string origin,
                                             std::string filename,
                                             int         num,
                                             std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "File function \""+filename+"\" contains "+str(num)+
                " energy nodes while at least 2 are required to describe a"
                " spectral shape. "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Not enough columns in file function
 *
 * @param[in] origin Method that throws the error.
 * @param[in] filename File function filename.
 * @param[in] num Number of columns.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::not_enough_columns::not_enough_columns(std::string origin,
                                                   std::string filename,
                                                   int         num,
                                                   std::string message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = "File function \""+filename+"\" contains "+str(num)+
                " columns while at least 2 are required to define"
                " energy and intensity. "+message;

    // Return
    return;
}
