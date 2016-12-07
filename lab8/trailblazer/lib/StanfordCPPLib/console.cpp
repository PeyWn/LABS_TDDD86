/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 */

#include <string>
#include "console.h"
#include "error.h"
#include "platform.h"
using namespace std;

static void sclTerminateHandler();

static Platform *pp = getPlatform();
static bool consoleEcho = false;
static bool consolePrintExceptions = false;
static string consoleLogFile = "";
static void (*old_terminate)() = NULL;

void clearConsole() {
   pp->clearConsole();
}

bool getConsoleEcho() {
   return consoleEcho;
}

string getConsoleLogFile() {
   return consoleLogFile;
}

bool getConsolePrintExceptions() {
    return consolePrintExceptions;
}

void setConsoleEcho(bool echo) {
   consoleEcho = echo;
}

void setConsoleFont(const string & font) {
   pp->setConsoleFont(font);
}

void setConsoleLogFile(const string & filename) {
   consoleLogFile = filename;
}

void setConsolePrintExceptions(bool printExceptions) {
    if (printExceptions && !consolePrintExceptions) {
        old_terminate = set_terminate(sclTerminateHandler);
    } else if (!printExceptions && consolePrintExceptions) {
        set_terminate(old_terminate);
    }
    consolePrintExceptions = printExceptions;
}

void setConsoleSize(double width, double height) {
   pp->setConsoleSize(width, height);
}

static void sclTerminateHandler() {
   ostream& out = cerr;
   try {
      throw;   // re-throws the exception that already occurred
   } catch (const ErrorException& ex) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** An ErrorException occurred during program execution: \n";
      msg += " *** ";
      msg += ex.what();
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw ex;
   } catch (const std::exception& ex) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** An exception occurred during program execution: \n";
      msg += " *** ";
      msg += ex.what();
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw ex;
   } catch (std::string str) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A string exception occurred during program execution: \n";
      msg += " *** \"";
      msg += str;
      msg += "\"\n ***\n";
      cout.flush();
      out << msg;
      throw str;
   } catch (char const* str) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A string exception occurred during program execution: \n";
      msg += " *** \"";
      msg += str;
      msg += "\"\n ***\n";
      cout.flush();
      out << msg;
      throw str;
   } catch (int n) {
      char buf[128];
      snprintf(buf, 128, "%d", n);
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** An int exception occurred during program execution: \n";
      msg += " *** ";
      msg += buf;
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw n;
   } catch (long l) {
      char buf[128];
      snprintf(buf, 128, "%ld", l);
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A long exception occurred during program execution: \n";
      msg += " *** ";
      msg += buf;
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw l;
   } catch (char c) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A char exception occurred during program execution: \n";
      msg += " *** '";
      msg += c;
      msg += "'\n ***\n";
      cout.flush();
      out << msg;
      throw c;
   } catch (bool b) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A bool exception occurred during program execution: \n";
      msg += " *** ";
      msg += (b ? "true" : "false");
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw b;
   } catch (double d) {
      char buf[128];
      snprintf(buf, 128, "%lf", d);
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A double exception occurred during program execution: \n";
      msg += " *** ";
      msg += buf;
      msg += "\n ***\n\n";
      cout.flush();
      out << msg;
      throw d;
   }
   abort();
}
