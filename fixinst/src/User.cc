#include "User.h"

User::User() {

  user_data = new struct passwd;
  user_data = getpwuid(getuid());
}

User::~User() {

  delete user_data;
}

string User::GetHome() {

  return((string)user_data->pw_dir);
}

int User::isRoot() {

  return (!user_data->pw_uid);
}
