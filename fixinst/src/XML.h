/*

Parses and stores the XML file

Example:
<?xml version="1.0"?>
<install>
  <product>Tales of Tamar</product>
  <version>0.14</version>
  <target_dir>/usr/local/games/ToT</target_dir>
  <readme>README</readme>
  <license>license.txt</license>
  <splash>splash.jpg</splash>
  <binaries>tamar</binaries>
  <files>tamar:Bobs/*:Cmaps/*:Pics/*:Sfx/*:catalogs/*:data/*:font/*</files>
</install>

*/

#ifndef __XML_H
#define __XML_H

#include <string>
#include <list>

using namespace std;

class XML {

 private:

  typedef struct {
    string product;
    string version;
    string target_dir;
    string readme;
    string license;
    string splash;
    list<string>   binaries;
    list<string>   files;
  } TSetupData;

  TSetupData *data;

 public:

	XML(string);
  string GetProduct();
  string GetVersion();
  string GetTargetDir();
  string GetReadme();
  string GetLicense();
  string GetSplash();
  list<string>   GetBinaries();
  list<string>   GetFiles();

  void SetTargetDir(string);
};

#endif
