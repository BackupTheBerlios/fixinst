//
// This file contains all dialog windows for the GTK-Installer

#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "XML.h"

#include <string>

#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>

using namespace Gtk;
using namespace std;

class Welcome : public Dialog {

 public:
  Welcome(XML*, string);
};

class ShowLicense : public Dialog {

 public:
  ShowLicense(XML*, string);
};

class ShowTarget : public Dialog {

 public:
  ShowTarget(XML*, string);
  string GetPath();

 private:
  Entry *path;
};

class ShowReadme : public Dialog {

 public:
  ShowReadme(XML*, string);
};

#endif
