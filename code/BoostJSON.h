/*
 * BoostJSON.h
 *
 *  Created on: Dec 5, 2015
 *      Author: fred

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

#ifndef BOOSTJSON_H_
#define BOOSTJSON_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;

namespace BoostJSONDemo
{

enum TValueType
{
   kString,
   kNumber,
   kObject,
   kArray,
   kTrue,
   kFalse,
   kNull,
   kUNKNOWNValueType,
};

class BoostJSON
{
public:
   BoostJSON();
   virtual ~BoostJSON();

   void LoadJSONFile(const string& jsonFileName);
   void Dump(const ptree& pt, int cnt) const;
   void Dump() const;
   const ptree& getPt() const;

   bool IsObject(ptree::const_iterator &itr) const;
   bool IsString(ptree::const_iterator &itr) const;
   bool IsArray(ptree::const_iterator &itr) const;
   bool IsValue(ptree::const_iterator &itr) const;
   TValueType GetValueType(ptree::const_iterator &itr) const;
   size_t GetChildCount(ptree::const_iterator &itr) const;

private:
   ptree m_pt;
};

}

#endif /* BOOSTJSON_H_ */
