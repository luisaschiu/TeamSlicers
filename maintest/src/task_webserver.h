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