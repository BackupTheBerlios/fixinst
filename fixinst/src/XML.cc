#include "XML.h"

#include <libxml/parser.h>

#include <iostream>

using namespace std;

XML::XML(string docname) {

  xmlDocPtr  doc;
  xmlNodePtr node;

  doc = xmlParseFile(docname.c_str());
  if (!doc) {
    cerr << "Error: Could not parse XML document!" << endl;
    return;
  }

  node = xmlDocGetRootElement(doc);
  if (!node) {
    cerr << "Doh! Empty XML document!" << endl;
    xmlFreeDoc(doc);
    return;
  }

  if (xmlStrcmp(node->name, (const xmlChar*) "install")) {
    cerr << "Are you sure you have the right XML document? I am not!" << endl;
    xmlFreeDoc(doc);
    return;
  }
  node = node->xmlChildrenNode;
  data = new TSetupData;

  while (node) {
    if (xmlStrcmp(node->name, (const xmlChar*) "text")) {
      string s = (char*)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
      s = s.substr(s.find("\n")+1, s.find_last_of("\n")-1);
      if (!xmlStrcmp(node->name, (const xmlChar*) "product")) {
        data->product = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "version")) {
        data->version = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "target_dir")) {
        if (s[s.length()-1] != '/')
          s = s + "/";
        data->target_dir = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "readme")) {
        data->readme = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "license")) {
        data->license = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "splash")) {
        data->splash = s;
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "binaries")) {
        while (s.find(":") < s.length()) {
          data->binaries.push_back(s.substr(0, s.find(":")));
          s = s.substr(s.find(":")+1, s.length());
        }
        if (s != "")  //Maybe there is one entry left in the list...
          data->binaries.push_back(s);
      }
      if (!xmlStrcmp(node->name, (const xmlChar*) "files")) {
        while (s.find(":") < s.length()) {
          data->files.push_back(s.substr(0, s.find(":")));
          s = s.substr(s.find(":")+1, s.length());
        }
        if (s != "")  //Maybe there is one entry left in the list...
          data->files.push_back(s);
      }
    }
    node = node->next;
  }

  xmlFreeDoc(doc);
}

string       XML::GetProduct()   { return data->product; }
string       XML::GetVersion()   { return data->version; }
string       XML::GetTargetDir() { return data->target_dir; }
string       XML::GetReadme()    { return data->readme; }
string       XML::GetLicense()   { return data->license; }
string       XML::GetSplash()    { return data->splash; }
list<string> XML::GetBinaries()  { return data->binaries; }
list<string> XML::GetFiles()     { return data->files; }

void XML::SetTargetDir(string dir) {

	if (dir.find_last_of("/") != dir.length()-1)
    dir = dir + "/";

  data->target_dir = dir;
}

