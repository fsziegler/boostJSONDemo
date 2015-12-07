//============================================================================
// Name        : bjd001.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
/*
The MIT License (MIT)

Copyright (c) 2015 Frederick S. Ziegler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "BoostJSON.h"
#include "ZiegVersion.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>
#include <map>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;
namespace zv = ZiegVersion;

using namespace std;
using namespace BoostJSONDemo;

int main(int argc, char *argv[]) {
   bpo::options_description cmdline_options;
   try
   {
      cmdline_options.add_options()
         ("version,v", "print version string")
         ("help,h", "produce help message")
         ("json,j", bpo::value<string>(), "JSON input data file (required)")
      ;
      bpo::variables_map vm;
      bpo::store(bpo::parse_command_line(argc, argv, cmdline_options), vm);
      bpo::notify(vm);

      if (vm.count("version"))
      {
         cout << "BoostJSONDemo (bjd) Program" << endl;
         cout << "Version: " << zv::GetFullVersionString() << endl;
         cout << "  Built: " << zv::BuildDate << "@" << zv::BuildTime << endl;
         cout << "   UUID: " << zv::UUID << endl;
         exit(0);
      }
      if (vm.count("help"))
      {
         cout << cmdline_options << endl;
         exit(0);
      }
      if (vm.count("json"))
      {
         string inFileStr(vm["json"].as<string>());
         if(!bfs::exists(inFileStr))
         {
            cout << "The input file " << inFileStr << " does not exist."
                  << endl;
            exit(1);
         }
         cout << "Loading file " << inFileStr << endl;
         BoostJSON bjson;
         bjson.LoadJSONFile(inFileStr);
         bjson.Dump();
      }
      else
      {
         cout << "You must specify an input file with the '-d' option" << endl;
         exit(2);
      }
   }
   catch(exception& ex)
   {
      cout << "Exception: [" << ex.what() << "] thrown." << endl;
      cout << cmdline_options << endl;
      exit(3);
   }
   catch(...)
   {
      cout << "Exception thrown." << endl;
      cout << cmdline_options << endl;
      exit(4);
   }
   return 0;
}
