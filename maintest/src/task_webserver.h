/** @file task_webserver.h
 *  This program runs a very simple web server, demonstrating how to serve a
 *  static web page and how to use a web link to get the microcontroller to do
 *  something simple. 
 * 
 *  Based on an examples by A. Sinha at 
 *  @c https://github.com/hippyaki/WebServers-on-ESP32-Codes
 * 
 *  @author A. Sinha
 *  @author JR Ridgely
 * @author Kevin Salceda
 * @author Caleb Kephart
 *  @date   2022-Mar-28 Original stuff by Sinha
 *  @date   2022-Nov-04 Modified for ME507 use by Ridgely
 *  @copyright 2022 by the authors, released under the MIT License.
 */
#ifndef task_webserver_h
#define task_webserver_h
#include <Arduino.h>
#include <PrintStream.h>
#include <WiFi.h>
#include <WebServer.h>

void setup_wifi (void);

void HTML_header (String& a_string, const char* page_title);

void handle_DocumentRoot ();

void handle_NotFound (void);

void handle_home_blade (void);

void handle_home_push (void);

void handle_home_done (void);

void handle_thick_sel (void);

void handle_quarter  (void);

void handle_half (void);

void handle_inch (void);

void handle_cut (void);

void handle_CSV (void);

void task_webserver (void* p_params);


#endif