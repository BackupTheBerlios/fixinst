#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <list>

#include <sys/stat.h>

using namespace std;

class File {

 public:
	File();

	int isDir(string);
	mode_t GetFileAttr(string);
	int SetFileAttr(string, mode_t);
	list<string> SplitPath(string);
	int MkDir(string);
	list<string> ReadDir(string, int recursive = 0);
	streampos Filesize(ifstream&);
	int Copy(string, string);
  void Less(string);
  int MkLink(string, string);
};

#endif
