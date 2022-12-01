/** @file main.cpp
 *  This program runs a very simple web server, demonstrating how to serve a
 *  static web page and how to use a web link to get the microcontroller to do
 *  something simple. 
 * 
 *  Based on an examples by A. Sinha at 
 *  @c https://github.com/hippyaki/WebServers-on-ESP32-Codes
 * 
 *  @author A. Sinha
 *  @author JR Ridgely
 *  @date   2022-Mar-28 Original stuff by Sinha
 *  @date   2022-Nov-04 Modified for ME507 use by Ridgely
 *  @copyright 2022 by the authors, released under the MIT License.
 */
#include <task_webserver.h>
#include <Arduino.h>
#include "PrintStream.h"
#include <WiFi.h>
#include <WebServer.h>
#include "shares.h"

// #define USE_LAN to have the ESP32 join an existing Local Area Network or 
// #undef USE_LAN to have the ESP32 act as an access point, forming its own LAN
#undef USE_LAN

// If joining an existing LAN, get certifications from a header file which you
// should NOT push to a public repository of any kind
#ifdef USE_LAN
#include "mycerts.h"       // For access to your WiFi network; see setup_wifi()

// If the ESP32 creates its own access point, put the credentials and network
// parameters here; do not use any personally identifying or sensitive data
#else
const char* ssid = "slicer";   // SSID, network name seen on LAN lists
const char* password = "slicendice";   // ESP32 WiFi password (min. 8 characters)

/* Put IP Address details */
IPAddress local_ip (192, 168, 5, 1); // Address of ESP32 on its own network
IPAddress gateway (192, 168, 5, 1);  // The ESP32 acts as its own gateway
IPAddress subnet (255, 255, 255, 0); // Network mask; just leave this as is
#endif


/// The pin connected to an LED controlled through the Web interface
const uint8_t ledPin = 2;

#define FAST_PIN 12         ///< The GPIO pin cranking out a 500 Hz square wave


/** @brief   The web server object for this project.
 *  @details This server is responsible for responding to HTTP requests from
 *           other computers, replying with useful information.
 *
 *           It's kind of clumsy to have this object as a global, but that's
 *           the way Arduino keeps things simple to program, without the user
 *           having to write custom classes or other intermediate-level 
 *           structures. 
*/
WebServer server (80);


/** @brief   Get the WiFi running so we can serve some web pages.
 */
void setup_wifi (void)
{
#ifdef USE_LAN                           // If connecting to an existing LAN
    Serial << "Connecting to " << ssid;

    // The SSID and password should be kept secret in @c mycerts.h.
    // This file should contain the two lines,
    //   const char* ssid = "YourWiFiNetworkName";
    //   const char* password = "YourWiFiPassword";
    WiFi.begin (ssid, password);

    while (WiFi.status() != WL_CONNECTED) 
    {
        vTaskDelay (1000);
        Serial.print (".");
    }

    Serial << "connected at IP address " << WiFi.localIP () << endl;

#else                                   // If the ESP32 makes its own LAN
    Serial << "Setting up WiFi access point...";
    WiFi.mode (WIFI_AP);
    WiFi.softAPConfig (local_ip, gateway, subnet);
    WiFi.softAP (ssid, password);
    Serial << "done." << endl;
#endif
}


/** @brief   Put a web page header into an HTML string. 
 *  @details This header may be modified if the developer wants some actual
 *           @a style for her or his web page. It is intended to be a common
 *           header (and stylle) for each of the pages served by this server.
 *  @param   a_string A reference to a string to which the header is added; the
 *           string must have been created in each function that calls this one
 *  @param   page_title The title of the page
*/
void HTML_header (String& a_string, const char* page_title)
{
    a_string += "<!DOCTYPE html> <html>\n";
    a_string += "<head><meta name=\"viewport\" content=\"width=device-width,";
    a_string += " initial-scale=1.0, user-scalable=no\">\n<title> ";
    a_string += page_title;
    a_string += "</title>\n";
    a_string += "<style>html { font-family: Helvetica; display: inline-block;";
    a_string += " margin: 0px auto; text-align: center;}\n";
    a_string += "body{margin-top: 50px;} h1 {color: #4444AA;margin: 50px auto 30px;}\n";
    a_string += "p {font-size: 24px;color: #222222;margin-bottom: 10px;}\n";
    a_string += "</style>\n</head>\n";
}


/** @brief   Callback function that responds to HTTP requests without a subpage
 *           name.
 *  @details When another computer contacts this ESP32 through TCP/IP port 80
 *           (the insecure Web port) with a request for the main web page, this
 *           callback function is run. It sends the main web page's text to the
 *           requesting machine.
 */
void handle_DocumentRoot ()
{
    Serial << "HTTP request from client #" << server.client () << endl;
//    start_flag.put(false);    
    String a_str;
    HTML_header (a_str, "ESP32 Web Server Test");
    a_str += "<body>\n<div id=\"webpage\">\n";
    a_str += "<h1>Slice and Dice</h1>\n";
    a_str += "Welcome!\n";
    a_str += "Recommended by top chefs such as Gordon Ramsey and Dr.Ridgely \n";
    a_str += "<p><p> <a href=\"/home_push\">Click here to home device pusher</a>\n";
    a_str += "<p><p> <a href=\"/home_blade\">Click here to home blade </a>\n";
    a_str += "<p><p> <a href=\"/home_done\">Click here to continue </a>\n";
    a_str += "</div>\n</body>\n</html>\n";

    server.send (200, "text/html", a_str); 
}


/** @brief   Respond to a request for an HTTP page that doesn't exist.
 *  @details This function produces the Error 404, Page Not Found error. 
 */
void handle_NotFound (void)
{
    server.send (404, "text/plain", "Not found");
}


/** @brief   Toggle blue LED when called by the web server.
 *  @details For testing purposes, this function turns the little blue LED on a
 *           38-pin ESP32 board on and off. It is called when someone enters
 *           @c http://server.address/toggle as the web address request from a
 *           browser.
 */
void handle_home_blade (void)
{
    home_blade_flag.put(true);
    String home_blade = "<!DOCTYPE html> <html> <head>\n";
   // toggle_page += "<meta http-equiv=\"refresh\" content=\"1; url='/'\" />\n"; // this line casues it to automatically jump back to home...
    home_blade += "</head> <body> <p> <a href='/'>Back to Main Page</a></p>"; // back to main link
    home_blade += "Homing Blade\n" ;
    server.send (200, "text/html", home_blade); // needed to create string to an HTML.
}
void handle_home_push (void)
{
    home_pusher_flag.put(true);
    String home_push = "<!DOCTYPE html> <html> <head>\n";
   // toggle_page += "<meta http-equiv=\"refresh\" content=\"1; url='/'\" />\n"; // this line casues it to automatically jump back to home...
    home_push += "</head> <body> <p> <a href='/'>Back to Main Page</a></p>"; // back to main link
    home_push += "Homing Pusher" ;
    server.send (200, "text/html", home_push); // needed to create string to an HTML.
}
void handle_home_done (void)
{

    String home_done = "<!DOCTYPE html> <html> <head>\n";
   // toggle_page += "<meta http-equiv=\"refresh\" content=\"1; url='/'\" />\n"; // this line casues it to automatically jump back to home...
    home_done += "</head> <body> <p> <a href='/'>Back to Main Page</a></p>"; // back to main link
    home_done += "Load Device With Fruit\n";
    home_done += "<p><p> <a href=\"/thick_sel\">Click Here Once Loaded</a>\n" ; // start link 
    home_done += "</body> </html>";

    server.send (200, "text/html", home_done); // needed to create string to an HTML.
}
void handle_thick_sel (void)
{   
    start_flag.put(true);
    String thick_sel = "<!DOCTYPE html> <html> <head>\n";
    //thick_sel += "</head> <body> <p> <a href='/'>Back to main page</a></p>"; // back to main link
    thick_sel += "Select Desired Thickness\n";
    thick_sel += "<p><p><p> <a href=\"/quarter_cut\">1/4 inch</a>\n" ; // start link 
    thick_sel += "<p><p><p> <a href=\"/half_cut\">1/2 inch</a>\n" ;
    thick_sel += "<p><p><p> <a href=\"/inch_cut\">1 inch</a>\n" ;
    thick_sel += "</body> </html>";

    server.send (200, "text/html", thick_sel); // needed to create string to an HTML.
}

void handle_quarter  (void)
{
    thickness_option.put(1);
    String quarter_cut = "<!DOCTYPE html> <html> <head>\n";
    quarter_cut += "</head> <body> <p> <a href='/'>Back to main page</a></p>";
    quarter_cut += "Postioning...\n";
    quarter_cut += "CAUTION: Wait till Positioning is complete before continuing!!!\n";
    quarter_cut += "<p><p><p> <a href=\"/ready_cut\">Start Cut</a>\n" ;
    quarter_cut += "</body> </html>";

    server.send (200, "text/html", quarter_cut);
}
void handle_half (void)
{
    thickness_option.put(2);
    String half_cut = "<!DOCTYPE html> <html> <head>\n";
    half_cut += "</head> <body> <p> <a href='/'>Back to main page</a></p>";
    half_cut += "Postioning...\n";
    half_cut += "CAUTION: Wait till Positioning is complete before continuing!!!\n";
    half_cut += "<p><p><p> <a href=\"/ready_cut\">Start Cut</a>\n" ;
    half_cut += "</body> </html>";

    server.send (200, "text/html", half_cut);
}
void handle_inch (void)
{
    thickness_option.put(3);
    String inch_cut = "<!DOCTYPE html> <html> <head>\n";
    inch_cut += "</head> <body> <p> <a href='/'>Back to main page</a></p>";
    inch_cut += "Postioning...\n";
    inch_cut += "CAUTION: Wait till Positioning is complete before continuing!!!\n";
    inch_cut += "<p><p><p> <a href=\"/ready_cut\">Start Cut</a>\n" ;
    inch_cut += "</body> </html>";
    
    server.send (200, "text/html", inch_cut);

}
void handle_cut (void)
{
    user_cut_flag.put(true); 
    String ready_cut = "<!DOCTYPE html> <html> <head>\n";
    ready_cut += "Cutting...";
    ready_cut += "NOTE: Return back to home page once fully cut.\n";
    ready_cut += "</head> <body> <p> <a href='/'>Back to Main Page</a></p>";
    
    server.send (200, "text/html", ready_cut);
    
}

void task_webserver (void* p_params)
{
    // The server has been created statically when the program was started and
    // is accessed as a global object because not only this function but also
    // the page handling functions referenced below need access to the server
    server.on ("/", handle_DocumentRoot);
    server.on ("/home_blade", handle_home_blade);
    server.on ("/home_push", handle_home_push);
    server.on ("/home_done", handle_home_done);
    server.on ("/thick_sel", handle_thick_sel);
    server.on ("/quarter_cut", handle_quarter);
    server.on ("/half_cut", handle_half);
    server.on ("/inch_cut", handle_inch);
    server.on ("/ready_cut", handle_cut);
    server.onNotFound (handle_NotFound);

    // Get the web server running
    server.begin ();
    Serial.println ("HTTP server started");

    for (;;)
    {
        // The web server must be periodically run to watch for page requests
        server.handleClient ();
        vTaskDelay (500);
    }
}
/**void setup () 
{
    Serial.begin (115200);
    delay (100);
    while (!Serial) { }                   // Wait for serial port to be working
    delay (1000);
    Serial.print("Got IP: ");  
    Serial << endl << F("\033[2JTesting Arduino Web Server") << endl;

    // Call function which gets the WiFi working
    setup_wifi ();

    // Create the tasks which will do exciting things...

    // Task which runs the web server. It runs at a low priority
    xTaskCreate (task_webserver, "Web Server", 8192, NULL, 2, NULL);
}
/** @brief   Arduino loop method which runs repeatedly, doing nothing much.
 */
/**void loop ()
{
    vTaskDelay (1000);
}
*/
