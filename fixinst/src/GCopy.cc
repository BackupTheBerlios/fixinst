#include "GCopy.h"
#include "File.h"

#include "../config.h"

#include <gtkmm/frame.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Gtk;
using namespace Glib;

// Class GCopy
GCopy::GCopy(XML *xml, string working_dir) {

  okButton             = manage(add_button(Stock::OK, RESPONSE_OK));
  Button *cancelButton = manage(add_button(Stock::CANCEL, RESPONSE_CANCEL));
  Frame  *bild         = manage(new Frame());
  VBox   *vbox         = manage(new VBox());

  pbar         = manage(new ProgressBar());
  action_label = manage(new Label());
  okButton->signal_clicked().connect(slot(*this, &GCopy::clickOk));
  cancelButton->signal_clicked().connect(slot(*this, &GCopy::clickCancel));

  xml_file = xml;
  work_dir = working_dir;

  set_border_width(10);
  set_default_size(400,300);
  // set_resizable(false);
  set_title((string)PACKAGE_STRING + " - Install");

  // add(*vbox);

  // Logo in Frame
  bild->set_border_width(10);

  Image *img = manage(new Image((working_dir + xml->GetSplash()).c_str()));
  bild->add(*img);

  // First text to label
  action_label->set_text("I will install now! Click on OK.");

  // Add components to vbox
  get_vbox()->pack_start(*bild, PACK_SHRINK);
  get_vbox()->pack_end(*pbar, PACK_SHRINK);
  get_vbox()->pack_end(*action_label, PACK_SHRINK);

  show_all_children();
  set_focus_child(*okButton);

  request_break = 0;
}

int GCopy::isAborted() {

  return (request_break);
}

void GCopy::clickOk() {

  list<string> flist;

  okButton->released();
  flist = xml_file->GetFiles();
  list<string>::iterator iter = flist.begin();

  while (iter != flist.end()) {
    action_label->set_text((xml_file->GetTargetDir()+ *iter).c_str());
    copy(work_dir + (string)*iter,
         (string)xml_file->GetTargetDir()+ (string)*iter);
    // cout << xml_file->GetTargetDir()+ *iter << endl;
    iter++;
    if (request_break) {
      iter = flist.end();
    }
  }

  // hide();
}

void GCopy::clickCancel() {

  request_break = 1;
}

int GCopy::copy(string src, string dest) {

  char                   *s;
  File                   *datei;
  char                   do_dir_copy;
  list<string>           src_list;
  list<string>::iterator src_iter;
  string                 dest_orig;

  datei = new File();

  if (dest[dest.length()-1] == '*') {
    dest = dest.substr(0, dest.length()-2);
  }

  if ((src[src.length()-1] == '*') || (src[src.length()-1] == '/')) {
    datei->MkDir(dest);
    src_list = datei->ReadDir(src.substr(0, src.length()-2),1);
    do_dir_copy = 1;
  } else {
    src_list.push_back(src);
    do_dir_copy = 0;
  }

  src_iter  = src_list.begin();
  dest_orig = dest;

  while (src_iter != src_list.end()) {
    if (do_dir_copy)
      dest = dest_orig + "/" + datei->SplitPath(*src_iter).back();
    else
      dest = dest_orig;

    action_label->set_text(("Installing " + *src_iter).c_str());

 	  ifstream in((*src_iter).c_str());
    ofstream out(dest.c_str());
    char     c;
    streampos fsize;

    if (!in) {
      // cout << "Error: Could not open " << src << endl;
      return 1;
    }

    fsize             = datei->Filesize(in);
    unsigned long cnt = 0;
    double        part;

    s = new char(128);
    while (in.get(c)) {
      out.put(c);
      cnt++;
      part = (double)cnt / (double)fsize;
      if (((part*100.0) / (int)(part*100)) == 1) {
        sprintf(s, "%i %%", (int)(part*100));
        pbar->set_fraction(part);
        pbar->set_text(s);
        MainContext::get_default()->iteration(false);
      }
    }
    datei->SetFileAttr(dest, datei->GetFileAttr(*src_iter));
    src_iter++;
    if (request_break) {
      MessageDialog dialog(*this, "Program not installed completely!",
                           MESSAGE_INFO, BUTTONS_OK, true);
			dialog.run();
      src_iter = src_list.end();
    }
  }
  delete s;
  delete datei;
}
