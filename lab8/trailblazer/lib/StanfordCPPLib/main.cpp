/*
 * File: main.cpp
 * --------------
 * This file defines a default version of the Main function that takes
 * the argc and argv arguments.  This function must be defined in its
 * own module to ensure that it is loaded only if the client doesn't
 * supply one.
 */

#include <exception>
#include <iostream>
#include <string>

using namespace std;

int Main(int, char *[]) {
   extern int Main();
   return Main();
}

void __StanfordCPPLib_terminate_handler() {
   try {
      throw;   // re-throws the exception that already occurred
   } catch (const std::exception& ex) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** An exception occurred during program execution: \n";
      msg += " *** ";
      msg += ex.what();
      msg += "\n ***\n\n";
      cerr << msg;
      throw ex;
   } catch (std::string str) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A string exception occurred during program execution: \n";
      msg += " *** \"";
      msg += str;
      msg += "\"\n ***\n";
      cerr << msg;
      throw str;
   } catch (char const* str) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A string exception occurred during program execution: \n";
      msg += " *** \"";
      msg += str;
      msg += "\"\n ***\n";
      cerr << msg;
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
      cerr << msg;
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
      cerr << msg;
      throw l;
   } catch (char c) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A char exception occurred during program execution: \n";
      msg += " *** '";
      msg += c;
      msg += "'\n ***\n";
      cerr << msg;
      throw c;
   } catch (bool b) {
      string msg = "\n ***\n";
      msg += " *** STANFORD C++ LIBRARY \n";
      msg += " *** A bool exception occurred during program execution: \n";
      msg += " *** ";
      msg += (b ? "true" : "false");
      msg += "\n ***\n\n";
      cerr << msg;
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
      cerr << msg;
      throw d;
   }
   abort();
}

void __StanfordCPPLib_set_terminate() {
   set_terminate(__StanfordCPPLib_terminate_handler);
}
