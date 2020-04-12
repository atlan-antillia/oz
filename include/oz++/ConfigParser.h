/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *  ConfigParser.h
 *
 *****************************************************************************/

// 2019/06/04 
// This class will parse  an ini config file, and store the information
// to std::unordered_map object.

// The configuration file will take the following format.
/*
; Comment line: Something.ini
[section1]
name1=value1
name2=value2
;;
[section2]
name1=value1
name2=value2
;;
 */

#pragma once

#include <string>
#include <list>
#include <vector>
#include <unordered_map>

#include <iostream>
#include <fstream>
#include <oz++/KeyValueSplitter.h>
#include <oz++/Exception.h>


namespace OZ {

class ConfigParser {
private:
  KeyValueSplitter splitter;

  // The key of the following map will take a string of the format 
  // sectioned_name = section + ":" + name, and a value take a string only.
  std::unordered_map<std::string, std::string> map;

public:
  ConfigParser()
  {
  }

  void parse(const std::string& filename)
  {
    std::ifstream ifs(filename);
    if (!ifs) {
      throw IException("File not found %s", filename.c_str());
    }
    std::string line;

    //Empty section
    std::string section = "";

    while (!ifs.eof()) {
      std::getline(ifs, line);
      std::string trimmed = splitter.trim(line);
      //printf("trimmed %s¥n", trimmed.c_str());
      if (trimmed.empty()) {
        //emtpy line
        continue;
      }
      if (trimmed.find(";") == 0) { 
        //comment
        continue;
      }
      
      std::string new_section= "";
      if (is_section(trimmed, new_section)) {
        section = new_section;
      } 
      
      parse_line(section, trimmed);
    }
  }

  //If a trimmed string is of the format "[Section]", then return true else false.
  bool is_section(const std::string& string, std::string& section)
  { 
    bool found = false;
    const char& head = string.at(0);
    const char& tail = string.at(string.length()-1);
    if (head == '[' && tail== ']') {
      std::string sec = string.substr(1, string.length()-2);
      // Imagine section may have a format [  Foo ], not [Foo].
      section = splitter.trim(sec); 
      //printf("Section %s\n", section.c_str()); 
      found = true;
    }
    return found;
  }

  void parse_line(const std::string section, const std::string& trimmed)
  {
      std::string colon = ":";
      std::string name, value;
      // Split a string of the format name = value to be name and value.
      if (splitter.split(trimmed, name, value)) {
        //printf("splitted '%s'|'%s'\n", name.c_str(), value.c_str());
        std::string env;
        // This is an experimental code to handle the case
        // that a value may have a format something like 
        // dataset = $(HOME)/work/dataset/
        if (splitter.find_env_variable(value.c_str(), env)) {
          //If value contains an env_liken substring something like $(HOME) 
          //printf("env '%s'\n", env.c_str());
          std::string envv = getenv(env.c_str());
          if (!envv.empty()) {
            //printf("env '%s'\n", envv.c_str());
            std::string renv= std::string("$(") + env + std::string(")");
   
            //printf("%s to be replaced to '%s'\n", renv.c_str(), envv.c_str());
            value.find(renv);
            auto pos = value.find(renv); 
            auto len = renv.length();
            if (pos != std::string::npos) {
              value.replace(pos, len, envv);
             
              std::string sectioned_key = section + colon + name;
              /*printf("sectioned_key %s expanded value '%s'\n", 
                      sectioned_key.c_str(),
	              value.c_str());
              */
              map[sectioned_key] = value;
            }
          } else {
            //Not found by getenv() function
            std::string sectioned_key = section + colon + name;
            map[sectioned_key] = value;
          }
        } else {
          //No env_liken something like $(HOME) found.
          std::string sectioned_key = section + colon + name;
          map[sectioned_key] = value;
        }
      }
  }

  ///////////////////////////////////////////////
  //Getters
  const std::string& get(const char* section, const char* name)
  {
    std::string colon=":";
    std::string sectioned_key = std::string(section) + colon
                              + std::string(name);
    return map[sectioned_key];
  }

  const std::string& get(const std::string section, const std::string& name)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    return map[sectioned_key];
  }


  const char* getString(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    return string.c_str(); 
  }

  int getInt(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    const char* cstring= string.c_str(); 
    return atoi(cstring);
  }

  float getFloat(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    const char* cstring= string.c_str();
    return atof(cstring);
  }


  bool getBool(const char* section, const char* name)
  {
    bool rc = false;
    const std::string& string = get(section,  name);
    if (string == "true" || string == "True") {
      rc = true;
    }
    return rc;
  }

  ///////////////////////////////////////////////
  //Setters
  void set(const std::string section, const std::string& name,
                 const std::string& value)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    map[sectioned_key] = value;
  }

  void setString(const std::string section, const std::string& name,
                 const std::string& value)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    map[sectioned_key] = value;
  }

  void setInt(const std::string section, const std::string& name,
                 int value)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    char buff[20];
    sprintf(buff,"%d", value);
    map[sectioned_key] = buff;
  }

  void setFloat(const std::string section, const std::string& name,
                 float value) //double value
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    char buff[100];
    sprintf(buff,"%lf", value); 
    map[sectioned_key] = buff;
  }

  void setBool(const std::string section, const std::string& name,
                 bool value)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    char buff[20];
    if (value) {
      sprintf(buff,"%s", "true");
    } else {
      sprintf(buff,"%s", "false");
    }
    map[sectioned_key] = buff;
  }

  void dump()
  {
    for(auto it = map.begin(); it != map.end(); it++) {
      std::cout << "key = " << it->first
                << ", val = " << it->second << "\n";
    }
  }

  
  static bool compareByFirst(std::pair<std::string, std::string>& a, 
                      std::pair<std::string, std::string>& b) 
  {
    if ( a.first.compare(b.first) < 0) {
      return true;
    } else {
       return false;
    }
  }

  //
  void save(std::string& filename)
  {
    std::ofstream ofs(filename);
    if (!ofs) {
      throw IException("File not found %s", filename.c_str());
    }
    // 1. Get sorted slist from the map (std::unorder_map).
    std::list<std::pair<std::string, std::string>> slist;

    sort(slist);

    std::string section = "";
    std::string colon = ":";
    std::string sep   = " = ";
    bool first_section = true;
    //
    for(auto it = slist.begin(); it != slist.end(); it++) {

      std::string sectioned_key = it->first; 

      std::string sec, key; 
      // 2. Split the sectioned_key to sec and key.
      splitter.split(sectioned_key, colon, sec, key);
      if (section != sec) {
        section = sec;
        // If found the first section.
        if (first_section) {
          // Write the following comment line which contains filename
          ofs <<"; " << filename <<std::endl;
          first_section = false;
        }
        // 3. Write the section.
        ofs<< std::endl<< "[" << section << "]" << std::endl;
      } 
      // 4. Write key = value
      ofs<< key << sep << it->second << std::endl;
    }
  }

  //
  void sort(std::list<std::pair<std::string, std::string>>& slist)
  {
    // 1. Create a list of pairs from the map. 
    for(auto it = map.begin(); it != map.end(); it++) {
      std::pair<std::string, std::string> pair(it->first, it->second);
      slist.push_back(pair);
    }
     
    // 2. Sort the list by the first of each pair.
    slist.sort(compareByFirst); 

    
    printf("Sorted list .................\n");
    for(auto it = slist.begin(); it != slist.end(); it++) {
      std::cout << "key = " << it->first
                << ", val = " << it->second << "\n";
    }
  }

};

}

