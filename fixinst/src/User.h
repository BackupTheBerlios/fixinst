#ifndef __USER_H__
#define __USER_H__

#include <string>

#include <pwd.h>

using namespace std;

class User {

 public:
  User();
  ~User();

  string GetHome();
  int    isRoot();

 private:
  struct passwd *user_data;
};

#endif
