#ifndef __MAIN__H__
#define __MAIN__H__

#include <gtkmm/main.h>

using namespace Gtk;

class InstMain : public Main {

 public:
  InstMain();
  int Init(int, char**, bool set_locale = true);
};

#endif
