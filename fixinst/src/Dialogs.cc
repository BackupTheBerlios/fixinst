#include "Dialogs.h"
#include "File.h"

#include "../config.h"

#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stock.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/textview.h>
#include <gtkmm/image.h>
#include <gtkmm/messagedialog.h>

#include <glibmm/ustring.h>
#include <glibmm/convert.h>

#include <fstream>
#include <iostream>

using namespace Gtk;
using namespace Glib;
using namespace std;

// Size of Dialogs
#define WIDTH 500
#define HEIGHT 400

// Class Welcome
Welcome::Welcome(XML *xml, string working_dir) {

  Button        *okButton     = manage(add_button(Stock::OK, RESPONSE_OK));
  Button        *cancelButton = manage(add_button(Stock::CANCEL,
                                                  RESPONSE_CANCEL));
  Frame         *bild         = manage(new Frame());
  Frame         *text         = manage(new Frame());
  VBox          *vbox         = manage(new VBox(true));

  set_border_width(10);
  set_default_size(WIDTH, HEIGHT);
  // set_resizable(false);
  set_title((string)PACKAGE_STRING);

  // Logo in Frame
  Image *img = manage(new Image((working_dir + xml->GetSplash()).c_str()));
  bild->add(*img);

  // Text
  text->set_border_width(10);
  vbox->set_spacing(10);

  vbox->pack_start(*(manage(new Label("Welcome to " + (string)PACKAGE_STRING,
                    ALIGN_CENTER))), PACK_EXPAND_WIDGET);
  vbox->pack_start(*(manage(new Label(((string)("I will install " +
                    xml->GetProduct() + " Version " +
                    xml->GetVersion())).c_str(), ALIGN_CENTER))),
                    PACK_EXPAND_WIDGET);
  text->add(*vbox);

  // Add components to vbox
  get_vbox()->pack_start(*bild, PACK_EXPAND_WIDGET);
  get_vbox()->pack_end(*text, PACK_EXPAND_WIDGET);

  show_all_children();
  set_focus_child(*okButton);
}

// Class ShowTarget
ShowTarget::ShowTarget(XML *xml, string working_dir) {

  Button        *okButton     = manage(add_button(Stock::OK, RESPONSE_OK));
  Button        *cancelButton = manage(add_button(Stock::CANCEL,
                                                  RESPONSE_CANCEL));
  Frame         *bild         = manage(new Frame());
  Frame         *text         = manage(new Frame());
  HBox          *hbox = manage(new HBox(true));
  VBox          *vbox         = manage(new VBox(true));

  set_border_width(10);
  set_default_size(WIDTH, HEIGHT);
  // set_resizable(false);
  set_title((string)PACKAGE_STRING + " - Select target directory");

  // Logo in Frame
  Image *img = manage(new Image((working_dir + xml->GetSplash()).c_str()));
  bild->add(*img);

  // Text
  text->set_border_width(10);
  vbox->set_spacing(10);

  vbox->pack_start(*(manage(new Label("Target directory:",
                    ALIGN_CENTER))), PACK_EXPAND_WIDGET);

  // Path
  path = manage(new Entry());
  path->set_text(xml->GetTargetDir().c_str());

  hbox->pack_start(*path);

  vbox->pack_end(*hbox, PACK_EXPAND_WIDGET);
  text->add(*vbox);

  // Add components to vbox
  get_vbox()->pack_start(*bild, PACK_EXPAND_WIDGET);
  get_vbox()->pack_end(*text, PACK_EXPAND_WIDGET);

  show_all_children();
  set_focus_child(*okButton);
}

string ShowTarget::GetPath() {

  return((string)((path->get_text()).c_str()));
}

// Class ShowLicense

ShowLicense::ShowLicense(XML *xml, string working_dir) {

  Button         *okButton     = manage(add_button(Stock::OK, RESPONSE_OK));
  Button         *cancelButton = manage(add_button(Stock::CANCEL,
                                                  RESPONSE_CANCEL));
  Frame          *bild         = manage(new Frame());

  TextView       *tview        = manage(new TextView());
  ScrolledWindow *swin         = manage(new ScrolledWindow());
  RefPtr<TextBuffer> tbuf;

  set_border_width(10);
  set_default_size(WIDTH, HEIGHT);
  // set_resizable(false);
  set_title((string)PACKAGE_STRING + " - License");

  // Logo in Frame
  bild->set_border_width(10);

  Image *img = manage(new Image((working_dir + xml->GetSplash()).c_str()));
  bild->add(*img);

  // Fill text buffer
  tbuf = TextBuffer::create();
 	ifstream in((working_dir + xml->GetLicense()).c_str());
  if (!in) {
    tbuf->set_text((string)("Could not find " + working_dir +
                            xml->GetLicense()).c_str());
  } else {
    string l_text; // The text of the license
    string temp;
    while (!in.eof()) {
      getline(in, temp, '\n');
      l_text = l_text + temp + "\n";
    }
    tbuf->set_text(locale_to_utf8(l_text));
  }

  // Text
  tview->set_buffer(tbuf);
  tview->set_editable(false);
  tview->set_cursor_visible(false);
  tview->set_wrap_mode(WRAP_WORD);
  swin->add(*tview);
  swin->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
  swin->set_shadow_type(SHADOW_IN);

  // Add components to vbox
  get_vbox()->pack_start(*bild, PACK_SHRINK);
  get_vbox()->pack_end(*swin, PACK_EXPAND_WIDGET);

  show_all_children();
  set_focus_child(*okButton);
}

// Class ShowReadme

ShowReadme::ShowReadme(XML *xml, string working_dir) {

  Button         *okButton     = manage(add_button(Stock::OK, RESPONSE_OK));
  Button         *cancelButton = manage(add_button(Stock::CANCEL,
                                                  RESPONSE_CANCEL));
  Frame          *bild         = manage(new Frame());

  TextView       *tview        = manage(new TextView());
  ScrolledWindow *swin         = manage(new ScrolledWindow());
  RefPtr<TextBuffer> tbuf;

  set_border_width(10);
  set_default_size(WIDTH, HEIGHT);
  // set_resizable(false);
  set_title((string)PACKAGE_STRING + " - Readme");

  // Logo in Frame
  bild->set_border_width(10);

  Image *img = manage(new Image((working_dir + xml->GetSplash()).c_str()));
  bild->add(*img);

  // Fill text buffer
  tbuf = TextBuffer::create();
 	ifstream in((working_dir + xml->GetReadme()).c_str());
  if (!in) {
    tbuf->set_text((string)("Could not find " + working_dir +
                            xml->GetReadme()).c_str());
  } else {
    string l_text; // The text of the README
    string temp;
    while (!in.eof()) {
      getline(in, temp, '\n');
      l_text = l_text + temp + "\n";
    }
    tbuf->set_text(locale_to_utf8(l_text));
  }

  // Text
  tview->set_buffer(tbuf);
  tview->set_editable(false);
  tview->set_cursor_visible(false);
  tview->set_wrap_mode(WRAP_WORD);
  swin->add(*tview);
  swin->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
  swin->set_shadow_type(SHADOW_IN);

  // Add components to vbox
  get_vbox()->pack_start(*bild, PACK_SHRINK);
  get_vbox()->pack_end(*swin, PACK_EXPAND_WIDGET);

  show_all_children();
  set_focus_child(*okButton);
}
