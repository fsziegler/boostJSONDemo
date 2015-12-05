/*
 * BoostJSON.cpp
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

#include "BoostJSON.h"

namespace BoostJSONDemo
{

BoostJSON::BoostJSON()
{
//   boost::property_tree::read_json("data.json", pt);
}

BoostJSON::~BoostJSON()
{
   // TODO Auto-generated destructor stub
}

void BoostJSON::LoadJSONFile(const string& jsonFileName)
{
   read_json(jsonFileName.c_str(), m_pt);
}

void Indent(int cnt)
{
   for(int i=0; cnt>i; ++i)
   {
      cout << "  ";
   }
}

void BoostJSON::Dump(const ptree& pt, int cnt) const
{
   static const string EmptyStr("empty");
   for (ptree::const_iterator itr = pt.begin(); pt.end() != itr; ++itr)
   {
      Indent(cnt);
      cout << ((0 < itr->first.size()) ? itr->first : EmptyStr);
      if(0 < GetChildCount(itr))
      {
         cout << endl;
         Dump(itr->second, cnt+1);
      }
      else
      {
         cout << " : "
               << (0 < itr->second.data().size() ? itr->second.data() : EmptyStr)
               << endl;
      }
   }
}

void BoostJSON::Dump() const
{
   int cnt(0);
   Dump(m_pt, 0);
}

const ptree& BoostJSON::getPt() const
{
   return m_pt;
}

bool BoostJSON::IsObject(ptree::const_iterator& itr) const
{
   return( 0 < GetChildCount(itr));
}

bool BoostJSON::IsArray(ptree::const_iterator& itr) const
{
}

bool BoostJSON::IsString(ptree::const_iterator& itr) const
{
}

bool BoostJSON::IsValue(ptree::const_iterator& itr) const
{
}

TValueType BoostJSON::GetValueType(ptree::const_iterator& itr) const
{
   if(IsObject(itr))
   {
     return kObject;
   }
   //   kString,
//   kNumber,
//   ,
//   kArray,
//   kTrue,
//   kFalse,
//   kNull,
//   kUNKNOWNValueType,
}

size_t BoostJSON::GetChildCount(ptree::const_iterator& itr) const
{
//   const ptree& pt = (*itr).second;
   return (*itr).second.size();
}

}
