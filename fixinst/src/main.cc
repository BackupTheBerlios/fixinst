//
// Main file of another installer (originally written for
// Tales of Tamar

// Installer headers
#include "main.h"
#include "Dialogs.h"
#include "File.h"
#include "GCopy.h"
#include "XML.h"

#include "../config.h"

// C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <list>

// GTK++ headers
#include <gtkmm/messagedialog.h>

// The defined conf file
#define SETUP_FILE "setup.xml"

using namespace std;
using namespace Gtk;

InstMain::InstMain() {}

int InstMain::Init(int argc, char *argv[], bool set_locale) {

    if(!set_locale)
      gtk_disable_setlocale();

    //TODO: Add support for gtk_init_check().
    if (gtk_init_check(&argc, &argv)) {
      init_gtkmm_internals();
      return 1;
    } else
      return 0;
}

int installX11(int argc, char *argv[]) {

  File         *files;
  XML          *xml;
  string       s = argv[0];
  string       working_dir = s.substr(0, s.find_last_of("/")+1);
  string       input;

  xml   = new XML(working_dir + SETUP_FILE);
  files = new File();  // Wee need an object for file operations

  InstMain setup;
  if (!setup.Init(argc, argv)) {
    cout << "...falling back onto console." << endl;
    return 0;
  }

  // A little gossip...
  Welcome welcome(xml, working_dir);
  if (welcome.run() != RESPONSE_OK)
    return 1;
  welcome.hide();

  // Show license...
  if (xml->GetLicense() != "") {
    ShowLicense license(xml, working_dir);
    if (license.run() != RESPONSE_OK) {
      return 1;
    }
    license.hide();
  }

  // Show target directory
  ShowTarget target(xml, working_dir);
  if (target.run() != RESPONSE_OK) {
    return 1;
  }
  target.hide();
  if (target.GetPath() != "")
    xml->SetTargetDir(target.GetPath());
  if (!files->isDir(xml->GetTargetDir())) {
    MessageDialog msg((xml->GetTargetDir() + 
                       " does not exist! Create it now?").c_str(),
                       MESSAGE_QUESTION, BUTTONS_YES_NO);
    switch(msg.run()) {
      case RESPONSE_YES: files->MkDir(xml->GetTargetDir());; break;
      case RESPONSE_NO : return 1; break;
    }
  }

  // do installation
  GCopy win(xml, working_dir);

  if ((win.run() != RESPONSE_OK) || win.isAborted()) {
    return 1;
  }
  win.hide();

  // Show README...
  if (xml->GetReadme() != "") {
    ShowReadme readme(xml, working_dir);
    if (readme.run() != RESPONSE_OK)
      return 1;
    readme.hide();
	}

  return 1;
}

int installConsole(int argc, char *argv[]) {

  File         *files;
  XML          *xml;
  list<string> flist;
  string       s = argv[0];
  string       working_dir = s.substr(0, s.find_last_of("/")+1);
  string       input;

  xml   = new XML(working_dir + SETUP_FILE);
  files = new File();  // Wee need an object for file operations

  // A little gossip...
  cout << "==== Welcome to " << PACKAGE_STRING << " ====" << endl << endl;
  cout << "I will install " << xml->GetProduct() <<
		" Version " << xml->GetVersion() << "." << endl;

  // Show license
  if (xml->GetLicense() != "") {
    cout << endl << "Hit RETURN to start";
    getline(cin, input, '\n');
    files->Less(working_dir + xml->GetLicense());
    cout << endl << "Thank you! Hit RETURN to go on";
    getline(cin, input, '\n');
    cout << endl;
  }

  // Target directory...
  cout << "Target directory [" << xml->GetTargetDir() << "] ";
  getline(cin, input, '\n');

  if (input != "")
    xml->SetTargetDir(input);

  cout << endl << endl;

  if (!files->isDir(xml->GetTargetDir())) {
    cout << xml->GetTargetDir() << " does not exist. Create [Y/n]? ";
    getline(cin, input, '\n');
    if ((input == "") || (input[0] == 'y') || (input[0] == 'Y')) {
      if (files->MkDir(xml->GetTargetDir()))
        return 0;
    } else
      return 1;

    cout << endl << endl;
  }

  // Start copying...
  cout << "I will copy: " << endl;

  flist = xml->GetFiles();
  list<string>::iterator iter = flist.begin();

  while (iter != flist.end()) {
    files->Copy(working_dir + (string)*iter,
         (string)xml->GetTargetDir()+ (string)*iter);
    iter++;
  }

  // Show README
  if (xml->GetReadme() != "") {
    cout << "Do want to read the README file [Y/n]?";
    getline(cin, input, '\n');
    if ((input == "") || (input[0] == 'y') || (input[0] == 'Y')) {
      files->Less(working_dir + xml->GetReadme());
    }
  }

  cout << endl << xml->GetProduct() << " is installed!" << endl << endl;
  return 1;
}

int main(int argc, char *argv[]) {

  if (!getenv("DISPLAY"))
    return(!installConsole(argc, argv));
  else {
    if (!installX11(argc, argv))
      installConsole(argc, argv);
  }

  return 0;
}
