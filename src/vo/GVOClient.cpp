/***************************************************************************
 *                     GVOClient.hpp - VO client class                     *
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
 * @file GVOClient.cpp
 * @brief VO client class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstdlib>         // std::getenv() function
#include <cstdio>          // std::fopen(), etc. functions
#include <cstring>         // std::memset() function
#include <unistd.h>        // close() function
#include <netdb.h>         // getaddrinfo() function
#include <sys/socket.h>    // socket(), connect() functions
#include "GVOClient.hpp"
#include "GException.hpp"
#include "GTools.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_REGISTER_TO_HUB                      "GVOClient::register_to_hub()"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_SHOW_MESSAGE               //!< Show posted and received messages


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GVOClient::GVOClient(void)
{
    // Initialise members
    init_members();

    // Find Hub
    find_hub();
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] client VO client.
 ***************************************************************************/
GVOClient::GVOClient(const GVOClient& client)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(client);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GVOClient::~GVOClient(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                Operators                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] client VO client.
 * @return VO client.
 ***************************************************************************/
GVOClient& GVOClient::operator= (const GVOClient& client)
{
    // Execute only if object is not identical
    if (this != &client) {

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(client);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear object.
 *
 * Reset object to a clean initial state.
 ***************************************************************************/
void GVOClient::clear(void)
{
    // Free memory and initialise members
    free_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone object
 ***************************************************************************/
GVOClient* GVOClient::clone(void) const
{
    // Clone client
    return new GVOClient(*this);
}


/***********************************************************************//**
 * @brief Register client to SAMP Hub
 *
 * Connects the VO client to the Hub. A socket to the Hub is opened and the
 * client is registered at the Hub. Metadata are sent.
 ***************************************************************************/
void GVOClient::connect(void)
{
    // Continue only if we have a Hub
    if (has_hub()) {
    
        // Register to Hub
        register_to_hub();

        // Send meta data
        send_metadata();

    } // endif: we had a Hub

    // Return
    return;
}


/***********************************************************************//**
 * @brief Unregister client from SAMP Hub
 *
 * Disconnects the VO client from the Hub. The client is unregistered and the
 * socket to the Hub is closed.
 ***************************************************************************/
void GVOClient::disconnect(void)
{
    // Continue only if we have a Hub
    if (has_hub()) {

        // Unregister from Hub
        unregister_from_hub();

        // Close socket
        if (m_socket != -1) {
            close(m_socket);
            m_socket = -1;
        }

    } // endif: we had a Hub

    // Return
    return;
}


/***********************************************************************//**
 * @brief Signals if client has Hub information
 *
 * @return True if client has Hub information, false otherwise.
 *
 * Checks if all mandatory Hub tokens are non-empty.
 ***************************************************************************/
bool GVOClient::has_hub(void) const
{
    // Return Hub information status
    return (!m_secret.empty() && !m_hub_url.empty() && !m_version.empty());
}


/***********************************************************************//**
 * @brief Execute function on server
 *
 * @param[in] request XML request string
 * @return XML response
 ***************************************************************************/
GXml GVOClient::execute(const std::string& request) const
{
    // Initialise response
    GXml xml;

    // Connect to hub
    connect_to_hub();

    // Continue only if connection has been established
    if (m_socket != -1) {

        // Post request
        post_string(request);

        // Receive response
        std::string response = receive_string();

        // Find start of XML text
        size_t start = response.find("<?xml");

        // If found then convert text into XML document
        if (start != std::string::npos) {
            xml = GXml(response.substr(start, std::string::npos));
        }

    } // endif: connection has been established

    // Return response
    return xml;
}


/***********************************************************************//**
 * @brief Print VO client information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing VO client information
 ***************************************************************************/
std::string GVOClient::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GVOClient ===");

        // Append client information
        result.append("\n"+gammalib::parformat("Name")+m_name);

        // Append Hub information
        if (has_hub()) {
            result.append("\n"+gammalib::parformat("Hub key")+m_secret);
            result.append("\n"+gammalib::parformat("Hub URL")+m_hub_url);
            result.append("\n"+gammalib::parformat("Hub host")+m_hub_host);
            result.append("\n"+gammalib::parformat("Hub port")+m_hub_port);
            result.append("\n"+gammalib::parformat("Hub path")+m_hub_path);
            result.append("\n"+gammalib::parformat("SAMP protocol version"));
            result.append(m_version);
            result.append("\n"+gammalib::parformat("Hub connection"));
            if (m_socket == -1) {
                result.append("no");
            }
            else {
                if (!m_client_key.empty()) {
                    result.append("registered as \""+m_client_id);
                    result.append("\" on Hub \""+m_hub_id);
                    result.append("\" via socket "+gammalib::str(m_socket));
                }
                else {
                    result.append("established on socket ");
                    result.append(gammalib::str(m_socket));
                }
            }
        }
        else {
            result.append("\n"+gammalib::parformat("Hub key"));
            result.append("No Hub has been found");
        }

    } // endif: chatter was not silent

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GVOClient::init_members(void)
{
    // Initialise members
    m_name = "GammaLib";
    m_secret.clear();
    m_hub_url.clear();
    m_hub_host.clear();
    m_hub_port.clear();
    m_hub_path.clear();
    m_version.clear();
    m_client_key.clear();
    m_hub_id.clear();
    m_client_id.clear();
    m_socket = -1;         // Signals no socket

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] client VO client.
 ***************************************************************************/
void GVOClient::copy_members(const GVOClient& client)
{
    // Copy members
    m_name       = client.m_name;
    m_secret     = client.m_secret;
    m_hub_url    = client.m_hub_url;
    m_hub_host   = client.m_hub_host;
    m_hub_port   = client.m_hub_port;
    m_hub_path   = client.m_hub_path;
    m_version    = client.m_version;
    m_client_key = client.m_client_key;
    m_hub_id     = client.m_hub_id;
    m_client_id  = client.m_client_id;
    m_socket     = client.m_socket;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GVOClient::free_members(void)
{
    // Unregister from Hub
    unregister_from_hub();

    // Close socket
    if (m_socket != -1) {
        close(m_socket);
        m_socket = -1;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Find SAMP Hub
 *
 * @return True if SAMP Hub has been found, false otherwise.
 *
 * Search a valid SAMP Hub and retrieve all mandatory token for this Hub.
 * The manadtory tokens are
 *
 *     samp.secret           Opaque text string required for Hub registration
 *     samp.hub.xmlrpc.url   XML-RPC endpoint for communication
 *     samp.profile.version  Version of SAMP profile
 *
 * Implements IVOA standard REC-SAMP-1.3-20120411.
 *
 * @todo If no valid Hub was found the method should attempt to start its
 * own Hub.
 ***************************************************************************/
bool GVOClient::find_hub(void)
{
    // Allocate line buffer
    const int n = 1000; 
    char      line[n];

    // Initialise find flag to false
    bool found = false;

    // Get lockfile URL
    std::string lockurl = get_hub_lockfile();

    // Continue only if a URL has been found
    if (!lockurl.empty()) {

        // If we have a file:// prefix then strip it now. This is a kluge
        // and should be remplaced by a method that allows opening any kind
        // of URL
        if (lockurl.compare(0, 7, "file://") == 0) {
            lockurl = lockurl.substr(7, std::string::npos);
        }
    
        // Open SAMP lockfile. Continue only if opening was successful
        FILE* fptr = fopen(lockurl.c_str(), "r");
        if (fptr != NULL) {

            // Parse lockfile and search for mandatory tokens
            while (fgets(line, n, fptr) != NULL) {

                // Convert line to C++ string
                std::string cline = std::string(line);

                // Check for secret key
                if (cline.compare(0, 12, "samp.secret=") == 0) {
                    m_secret = gammalib::strip_chars(cline.substr(12, std::string::npos), "\r\n");
                }

                // Check for Hub URL
                else if (cline.compare(0, 20, "samp.hub.xmlrpc.url=") == 0) {
                    m_hub_url = gammalib::strip_chars(cline.substr(20, std::string::npos), "\r\n");
                }

                // Check for profile version
                else if (cline.compare(0, 21, "samp.profile.version=") == 0) {
                    m_version = gammalib::strip_chars(cline.substr(21, std::string::npos), "\r\n");
                }

            }

            // Close SAMP lockfile
            fclose(fptr);

            // Extract host, port and path from Hub URL
            if (m_hub_url.compare(0, 7, "http://") == 0) {
                size_t length;
                size_t start = 7;
                size_t stop  = m_hub_url.find(":", start);
                size_t end   = std::string::npos;
                if (stop != std::string::npos) {
                    length = stop - start;
                }
                else {
                    length = std::string::npos;
                }
                m_hub_host = m_hub_url.substr(start, length);
                if (stop != std::string::npos) {
                    stop++;
                    end  = m_hub_url.find("/", stop);
                    if (end != std::string::npos) {
                        length = end - stop;
                    }
                    else {
                        length = std::string::npos;
                    }
                    m_hub_port = m_hub_url.substr(stop, length);
                }
                if (end != std::string::npos) {
                    end++;
                    length     = m_hub_url.length() - end;
                    m_hub_path = m_hub_url.substr(end, length);
                }
            }

            // Check for existence of mandatory tokens
            found = has_hub();

        } // endif: SAMP lockfile opened

    } // endif: URL has been found

    //TODO: Create GammaLib own Hub in case that no Hub has been found
    //if (!found) {
    //}

    // If no Hub has been found, clear all Hub related members
    if (!found) {
        m_secret.clear();
        m_hub_url.clear();
        m_version.clear();
    }

    // Return find flag
    return found;
}


/***********************************************************************//**
 * @brief Register client at SAMP Hub
 *
 * @exception GException::invalid_value
 *            Unable to connect to Hub.
 ***************************************************************************/
void GVOClient::register_to_hub(void)
{
    // Declare request
    std::string request;

    // Set metadata header
    request.append("<?xml version=\"1.0\"?>\n");
    request.append("<methodCall>\n");
    request.append("  <methodName>samp.hub.register</methodName>\n");
    request.append("  <params>\n");
    request.append("    <param><value><string>"+m_secret+"</string></value></param>\n");
    request.append("  </params>\n");
    request.append("</methodCall>\n");

    // Execute request
    GXml xml = execute(request);

    // If no error occured, extract hub and client identifiers
    if (response_is_valid(xml)) {
        m_client_key = get_response_value(xml, "samp.private-key");
        m_hub_id     = get_response_value(xml, "samp.hub-id");
        m_client_id  = get_response_value(xml, "samp.self-id");
    }

    // ... otherwise signal an error
    else {
        std::string msg = "Unable to connect to Hub (" +
                          response_error_message(xml) + ", code=" +
                          gammalib::str(response_error_code(xml)) + ").";
        throw GException::runtime_error(G_REGISTER_TO_HUB, msg);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Unregister client from SAMP Hub
 ***************************************************************************/
void GVOClient::unregister_from_hub(void)
{
    // Declare request
    std::string request;

    // Set metadata header
    request.append("<?xml version=\"1.0\"?>\n");
    request.append("<methodCall>\n");
    request.append("<methodName>samp.hub.unregister</methodName>\n");
    request.append("<params>\n");
    request.append("<param><value><string>"+m_client_key+"</string></value></param>\n");
    request.append("</params>\n");
    request.append("</methodCall>\n");

    // Execute request
    execute(request);

    // Reset Hub and client identifiers
    m_client_key.clear();
    m_hub_id.clear();
    m_client_id.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Send client metadata to SAMP Hub
 ***************************************************************************/
void GVOClient::send_metadata(void) const
{
    // Declare request
    std::string request;

    // Set metadata header
    request.append("<?xml version=\"1.0\"?>\n");
    request.append("<methodCall>\n");
    request.append("<methodName>samp.hub.declareMetadata</methodName>\n");
    request.append("<params>\n");
    request.append("<param><value><string>"+m_client_key+"</string></value></param>\n");
    request.append("<param><value><struct>\n");

    // Set SAMP name
    request.append("<member>\n");
    request.append("<name>samp.name</name>\n");
    request.append("<value><string>"+m_name+"</string></value>\n");
    request.append("</member>\n");

    // Set SAMP description text
    request.append("<member>\n");
    request.append("<name>samp.description.text</name>\n");
    request.append("<value><string>GammaLib client</string></value>\n");
    request.append("</member>\n");

    // Set SAMP icon URL
    request.append("<member>\n");
    request.append("<name>samp.icon.url</name>\n");
    request.append("<value><string>http://a.fsdn.com/allura/p/gammalib/icon</string></value>\n");
    request.append("</member>\n");

    // Set author affiliation
    request.append("<member>\n");
    request.append("<name>author.affiliation</name>\n");
    request.append("<value><string>IRAP, Toulouse, France</string></value>\n");
    request.append("</member>\n");

    // Set author e-mail
    request.append("<member>\n");
    request.append("<name>author.email</name>\n");
    request.append("<value><string>jurgen.knodlseder@irap.omp.eu</string></value>\n");
    request.append("</member>\n");

    // Set author name
    request.append("<member>\n");
    request.append("<name>author.name</name>\n");
    request.append("<value><string>Juergen Knoedlseder</string></value>\n");
    request.append("</member>\n");

    // Set metadata trailer
    request.append("</struct></value></param>\n");
    request.append("</params>\n");
    request.append("</methodCall>\n");

    // Execute request
    execute(request);

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                        Low-level private methods                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Connect to SAMP Hub
 *
 * Connects to Hub by creating a socket and connecting to this socket. The
 * method expects that a Hub has already been found. If no Hub has been found
 * (i.e. has_hub() returns false), the method does nothing.
 *
 * The method extracts the hostname and the port from the Hub endpoint URL.
 * If Hub connection fails, any created socket is closed.
 *
 * If the method is successful, m_socket will contain on exit a non-negative
 * number. If any failure occurs, m_socket will be set to -1.
 ***************************************************************************/
void GVOClient::connect_to_hub(void) const
{
    // Close any existing socket
    if (m_socket != -1) {
        close(m_socket);
        m_socket = -1;
    }

    // Continue only if host and port information was found
    if (!m_hub_host.empty() && !m_hub_port.empty()) {

        // Set hints
        struct addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        // Get server information
        struct addrinfo* servinfo;
        if (getaddrinfo(m_hub_host.c_str(), m_hub_port.c_str(),
                        &hints, &servinfo) == 0) {

            // Loop through all the results and connect to the first
            // we can
            for (struct addrinfo* ptr = servinfo; ptr != NULL; ptr = ptr->ai_next) {

                // Create socket
                m_socket = socket(ptr->ai_family,
                                  ptr->ai_socktype,
                                  ptr->ai_protocol);

                // Connect to socket if socket is valid
                if (m_socket != -1) {
                    if (::connect(m_socket,
                                  ptr->ai_addr,
                                  ptr->ai_addrlen) == -1) {
                        close(m_socket);
                        m_socket = -1;
                    }
                }
                else {
                    break;
                }

            } // endfor: looped through all results

        } // endif: server information was valid

    } // endif: host and port information found

    // Return
    return;
}


/***********************************************************************//**
 * @brief Post string content to Hub
 *
 * @param[in] content String content to post
 *
 * Posts the content of a string to the Hub.
 *
 * The method does nothing if no Hub connection has been established.
 ***************************************************************************/
void GVOClient::post_string(const std::string& content) const
{
    // Continue only if Hub connection has been established
    if (m_socket != -1) {

        // Determine content length
        int length = content.length();

        // Set prefix
        std::string prefix = "POST /"+m_hub_path+" HTTP/1.0\n"
                             "User-Agent: GammaLib\n"
                             "Content-Type: text/xml\n"
                             "Content-Length: "+gammalib::str(length)+"\n\n";

        // Build post string
        std::string post = prefix + content;

        // Debug option: show posted message
        #if defined(G_SHOW_MESSAGE)
        std::cout << post << std::endl;
        #endif

        // Send content to socket
        bool done = false;
        do {
            int length      = post.length() + 1; // +1 for terminating 0
            int sent_length = send(m_socket, post.c_str(), length+1, 0);
            if (sent_length < length) {
                post = post.substr(sent_length, std::string::npos);
            }
            else {
                done = true;
            }
        } while (!done);

    } // endif: Hub connection had been established
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Receive string content from Hub
 *
 * @return String received from Hub.
 *
 * Reads information sent by Hub into a string.
 * 
 * The method does nothing if no Hub connection has been established.
 ***************************************************************************/
std::string GVOClient::receive_string(void) const
{
    // Initialise empty string
    std::string result = "";

    // Continue only if Hub connection has been established
    if (m_socket != -1) {

        // Define buffer
        char buffer[1001];

        // Read buffer until it is empty
        int n = 0;
        do {
            n = recv(m_socket, buffer, 1000, 0);
            if (n > 0) {
                buffer[n] = '\0';
                result.append(std::string(buffer));
            }
        } while (n > 0);

        // Debug option: show received message
        #if defined(G_SHOW_MESSAGE)
        std::cout << result << std::endl;
        #endif

    } // endif: Hub connection had been established

    // Return result
    return result;
}


/***********************************************************************//**
 * @brief Returns value for a SAMP Hub response parameter
 *
 * @param[in] xml Hub response XML document.
 * @param[in] name Parameter name.
 * @return Parameter value.
 *
 * Returns value for a SAMP Hub response parameter. If the specified
 * parameter was not found or if the response structure is not compliant,
 * an empty string is returned.
 ***************************************************************************/
std::string GVOClient::get_response_value(const GXml&        xml,
                                          const std::string& name) const
{
    // Declare value
    std::string value = "";

    // Search for value of specified member
    const GXmlNode* node = xml.element("methodResponse > params > param > value > struct");
    if (node != NULL) {
        int num = node->elements("member");
        for (int i = 0; i < num; ++i) {
            const GXmlNode* member = node->element("member", i);
            std::string one_name;
            std::string one_value;
            get_name_value_pair(member, one_name, one_value);
            if (one_name == name) {
                value = one_value;
                break;
            }
        }
    }

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Extract name / value pair from XML node
 *
 * @param[in] node Pointer to XML node.
 * @param[out] name Name string.
 * @param[out] value Value string.
 *
 * Extracts a name / value pair from a XML node. If the XML node pointer is
 * NULL, the name and value strings will be empty.
 ***************************************************************************/
void GVOClient::get_name_value_pair(const GXmlNode* node,
                                    std::string&    name,
                                    std::string&    value) const
{
    // Clear name and value strings
    name.clear();
    value.clear();

    // Continue only if node is valid
    if (node != NULL) {

        // Get name node and extract text content
        const GXmlNode* ptr = node->element("name", 0);
        if (ptr != NULL) {
            const GXmlText* text = static_cast<const GXmlText*>((*ptr)[0]);
            if (text != NULL) {
                name = text->text();
            }
        }

        // Get value node and extract text content
        ptr = node->element("value", 0);
        if (ptr != NULL) {
            const GXmlText* text = static_cast<const GXmlText*>((*ptr)[0]);
            if (text != NULL) {
                value = text->text();
            }
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns SAMP Hub lockfile URL
 *
 * @return SAMP Hub lockfile URL (empty if no lockfile was found).
 *
 * Implements IVOA standard REC-SAMP-1.3-20120411.
 ***************************************************************************/
std::string GVOClient::get_hub_lockfile(void) const
{
    // Initialise result
    std::string url = "";

    // Check for existence of the SAMP_HUB environment variable first
    char* hub_ptr = std::getenv("SAMP_HUB");
    if (hub_ptr != NULL) {

        // Check for mandatory std-lockurl: prefix (no other prefixe is
        // supported so far)
        std::string lockurl = std::string(hub_ptr);
        if (lockurl.compare(0, 12, "std-lockurl:") == 0) {

            // Extract URL
            url = lockurl.substr(12, std::string::npos);

        } // endif: std-lockurl: prefix found
  
    }

    // ... otherwise the lockfile should be $HOME/.samp
    else {

        // Get user's HOME directory path as the prefix of the full
        // path. If the HOME environment variable is not set we
        // expect that .samp is in the local directory. This is non
        // standard, but prevents for creating an exception here.
        std::string prefix = "";
        char* home_ptr = std::getenv("HOME");
        if (home_ptr != NULL) {
            prefix = std::string(home_ptr) + "/";
        }

        // Set filename
        url = prefix + ".samp";

    } // endelse: no SAMP_HUB environment variable found

    // Return URL
    return url;
}


/***********************************************************************//**
 * @brief Checks if response is valid
 *
 * @return True if response is valid, false otherwise.
 ***************************************************************************/
bool GVOClient::response_is_valid(const GXml& xml) const
{
    // Initialise validity flag
    bool valid = true;

    // Check for data
    const GXmlNode* node = xml.element("methodResponse > params > param > value > struct");
    if (node == NULL) {
        valid = false;
    }

    // Return validity
    return valid;
}


/***********************************************************************//**
 * @brief Return response error code
 *
 * @return Response error core (0 if no error occured).
 ***************************************************************************/
int GVOClient::response_error_code(const GXml& xml) const
{
    // Initialise error code
    int error_code = 0;

    // Extract error code
    const GXmlNode* node = xml.element("methodResponse > fault > value > struct");
    if (node != NULL) {
        const GXmlNode* code = node->element("member[0] > value > int");
        if (code != NULL) {
            error_code = gammalib::toint(static_cast<const GXmlText*>((*code)[0])->text());
        }
    }

    // Return error code
    return error_code;
}


/***********************************************************************//**
 * @brief Return response error message
 *
 * @return Response error message (blank if no error occured).
 ***************************************************************************/
std::string GVOClient::response_error_message(const GXml& xml) const
{
    // Initialise error message
    std::string error_message;

    // Extract error message
    const GXmlNode* node = xml.element("methodResponse > fault > value > struct");
    if (node != NULL) {
        const GXmlNode* msg  = node->element("member[1] > value");
        if (msg != NULL) {
            error_message = static_cast<const GXmlText*>((*msg)[0])->text();
        }
    }

    // Return error message
    return error_message;
}
