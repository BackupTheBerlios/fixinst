#ifndef __GCOPY_H__
#define __GCOPY_H__

#include "XML.h"

#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/progressbar.h>

#include <string>

using namespace Gtk;
using namespace std;

class GCopy : public Dialog {

 public:
  GCopy(XML*, string);
  int copy(string, string);
  int isAborted();

 private:
  void clickOk();
  void clickCancel();

  Button      *okButton;
  ProgressBar *pbar;
  Label       *action_label;
  XML         *xml_file;
  string      work_dir;
  int         request_break;
};

#endif
