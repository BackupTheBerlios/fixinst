//
// Class File offers file methods

#include "File.h"

#include <iostream>
#include <ios>
#include <fstream>

#include <dirent.h>
#include <string.h>
#include <ncurses.h>

using namespace std;

File::File() {}

int File::isDir(string filename) {

  struct stat sbuf;

  stat(filename.c_str(), &sbuf);

  return(S_ISDIR(sbuf.st_mode));
}

mode_t File::GetFileAttr(string filename) {

  struct stat sbuf;

  stat(filename.c_str(), &sbuf);

  return(sbuf.st_mode);
}

int File::SetFileAttr(string filename, mode_t t) {

  return(chmod(filename.c_str(), t));
}

list<string> File::SplitPath(string path) {

  list<string> liste;
  char *st;
  char *token;

  if (path[0] != '/')
    path = "./" + path; // Maybe path is not absolute...

  st    = strdupa(path.c_str());
  token = strtok(st, "/");
  while(token) {
    liste.push_back((string)token);
    token = strtok(NULL, "/");
  }

  return liste;
}

int File::MkDir(string dirname) {

  // First: Split the path into its parts...
  list<string>           parts = SplitPath(dirname);
  string                 substr; // Contains the rebuild path
  list<string>::iterator iter = parts.begin(); // Let's travel thru the list
  while (iter != parts.end()) {
    if ((*iter == ".") && (iter == parts.begin())) {
      substr = *iter;
    } else {
      substr = substr + "/" + *iter;
    }

    if (!isDir(substr)) {
      if (mkdir(substr.c_str(),
          S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) {
        cout << "Cannot create " << substr << endl;
        return 1;
      }
    }
    iter++;
  }

  return 0;
}

list<string> File::ReadDir(string dirname, int recursive) {

  list<string> entries;
  DIR *dp;
  struct dirent *ep;

  entries.clear();

  if ((dirname[dirname.length()-1] == '.') ||
      (dirname.find("..")  == dirname.length()-2))
    return entries;

  // cout << "Scanning " << dirname << endl;
  dp = opendir (dirname.c_str());
  if (dp != NULL) {
    while (ep = readdir (dp)) {
      string fullname = (string)(dirname + "/" + (string)ep->d_name);
			if (recursive && isDir(fullname)) {
        list<string>temp = ReadDir(fullname, recursive);
        list<string>::iterator iter = temp.begin();
        while (iter != temp.end()) {
          entries.push_back(*iter);
          iter++;
        }
      } else {
        if (!isDir(fullname)) {
          entries.push_back(fullname);
        }
      }
    }
    closedir (dp);
  }

  return entries;
}

streampos File::Filesize(ifstream &f) {

  streampos pos = f.tellg();
  f.seekg(0, ios::end);
  streampos fsize = f.tellg();
  f.seekg(pos, ios::beg);

  return fsize;
}

int File::Copy(string src, string dest) {

  char                   do_dir_copy;
  list<string>           src_list;
  list<string>::iterator src_iter;
  string                 dest_orig;

  if (dest[dest.length()-1] == '*') {
    dest = dest.substr(0, dest.length()-2);
  }

  if ((src[src.length()-1] == '*') || (src[src.length()-1] == '/')) {
    MkDir(dest);
    src_list = ReadDir(src.substr(0, src.length()-2),1);
    do_dir_copy = 1;
  } else {
    src_list.push_back(src);
    do_dir_copy = 0;
  }

  src_iter  = src_list.begin();
  dest_orig = dest;

  while (src_iter != src_list.end()) {
    if (do_dir_copy)
      dest = dest_orig + "/" + SplitPath(*src_iter).back();
    else
      dest = dest_orig;

    cout << "copying " << *src_iter << " to " << dest << endl;

 	  ifstream in((*src_iter).c_str());
    ofstream out(dest.c_str());
    char     c;
    streampos fsize;

    if (!in) {
      cout << "Error: Could not open " << src << endl;
      return 1;
    }

    fsize             = Filesize(in);
    unsigned long mod = fsize / 70;
    unsigned long cnt = 0;
    while (in.get(c)) {
      out.put(c);
      if (cnt % mod == 0)
        cout << "#";
      cnt++;
    }
    SetFileAttr(dest, GetFileAttr(*src_iter));

    cout << " Ok" << endl;

    src_iter++;
  }
}

void File::Less(string filename) {

  string out, input;
  int c, l, cnt;
  initscr();
  c = COLS;
  l = LINES;
  endwin();

 	ifstream in(filename.c_str());
  if (!in)
    return;

  cnt = 0;
  while(!in.eof()) {
    getline(in, out, '\n');
    cout << out << endl;
    cnt++;
    if (cnt % (l-2) == 0) {
      cout << endl << "Hit RETURN to go to next page";
      getline(cin, input, '\n');
      cnt = 0;
    }
  }
}

int File::MkLink(string st_old, string st_new) {

  return ((!(symlink(st_old.c_str(), st_new.c_str()))) * -1);
}
