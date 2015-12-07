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
#include <boost/regex.hpp>

namespace BoostJSONDemo
{

BoostJSON::BoostJSON()
{
}

BoostJSON::~BoostJSON()
{
}

void BoostJSON::LoadJSONFile(const string& jsonFileName)
{
   try
   {
      read_json(jsonFileName.c_str(), m_pt);
   }
   catch(exception& ex)
   {
      cout << "Exception " << ex.what() << " thrown trying to open JSON file "
            << jsonFileName << endl;
   }
   catch(...)
   {
      cout << "Unknown exception thrown trying to open JSON file "
            << jsonFileName << endl;
   }
}

void Indent(int cnt)
{
   for(int i=0; cnt>i; ++i)
   {
      cout << "  ";
   }
}

void BoostJSON::CoutCode(ptree::const_iterator itr, string& outStr) const
{
   outStr.clear();
   outStr.append(0 < itr->first.size() ? "[S" : "[E");
   outStr.append(0 < itr->second.data().size() ? "S" : "E");
   outStr.append(0 < itr->second.size() ? "S] " : "E] ");
}

void BoostJSON::CoutTypeStr(ptree::const_iterator itr) const
{
   ptree::const_iterator itrCpy(itr);
   switch(GetObjectType(itrCpy))
   {
   case kObject:
      cout << "{object} ";
      break;
   case kArray:
      cout << "{array object} ";
      break;
   case kValue:
      cout << "{";
      cout << GetValueTypeStr(GetValueType(itrCpy));
      cout << "} ";
      break;
   case kStrValuePair:
      cout << "{";
      cout << GetValueTypeStr(GetValueType(itrCpy->second.data()));
      cout << "} ";
      break;
   case kUNKNOWNObjType:
   default:
      cout << "{unknown} ";
      break;
   }
}

void BoostJSON::Dump(const ptree& pt, int cnt) const
{
   static const string EmptyStr("[empty]");
   string outStr;
   for (ptree::const_iterator itr = pt.begin(); pt.end() != itr; ++itr)
   {
      CoutCode(itr, outStr);
      Indent(cnt);
      if(0 < itr->first.size())
      {
         cout << outStr << itr->first;
         cout << (kObject == GetObjectType(itr) ? "{object}" : "{string}");
      }
      else
      {
         cout << outStr << EmptyStr;
      }
      cout << " : ";
      if(0 < GetChildCount(itr))
      {
         cout << endl;
         Dump(itr->second, cnt+1);
      }
      else
      {
         cout
               << (0 < itr->second.data().size() ? itr->second.data() : EmptyStr);
         ptree::const_iterator itrCpy(itr);
         CoutTypeStr(itrCpy);
         cout << endl;
      }
   }
}

void BoostJSON::Dump() const
{
   Dump(m_pt, 0);
}

const ptree& BoostJSON::getPt() const
{
   return m_pt;
}

bool BoostJSON::IsObject(ptree::const_iterator& itr) const
{
   return (kJSONObjectType == GetValueType(itr));
}

bool BoostJSON::IsArray(ptree::const_iterator& itr) const
{
   return (kJSONArrayType == GetValueType(itr));
}

bool BoostJSON::IsValue(ptree::const_iterator& itr) const
{
   return (kValue == GetObjectType(itr));
}

bool BoostJSON::IsStrValuePair(ptree::const_iterator &itr) const
{
   return (kStrValuePair == GetObjectType(itr));
}

/*
            Second
   First    data()   Children Case
   -------+--------+--------+-------
   empty  | empty  | empty  | DNE
   empty  | empty  | exist  | DNE
*   empty  | string | empty  | value (in array)
   empty  | string | exist  | DNE
*   string | empty  | empty  | object (empty)
*   string | empty  | exist  | object
   string | string | empty  | string:value pair
   string | string | exist  | DNE
*/
TObjType BoostJSON::GetObjectType(ptree::const_iterator &itr) const
{
   bool first(0 < itr->first.size());
   bool childName(0 < itr->second.data().size());
   bool children(0 < itr->second.size());
   if(!first && childName && !children)
   {
      return kValue;
   }
   if (first && !childName)
   {
      if(children)
      {
         ptree::const_iterator objItr(itr->second.begin());
         switch(GetObjectType(objItr))
         {
         case kObject:
         case kStrValuePair:
            return kObject;
         case kValue:
            return kArray;
         default:
            break;
         }
      }
      else
      {
         return kObject;
      }
   }
   if(first && childName && !children)
   {
     return kStrValuePair;
   }
   return kUNKNOWNObjType;
}

const boost::regex kTrueTypeRegexStr("true");
const boost::regex kFalseTypeRegexStr("false");
const boost::regex kNullTypeRegexStr("null");
const boost::regex kNumberTypeRegexStr("[0-9\\+\\-\\.eE]+");
// see http://www.regular-expressions.info/posixbrackets.html
const boost::regex kStringTypeRegexStr(
      "[[:alnum:][:punct:][:space:]]+");

TJSONValueType BoostJSON::GetValueType(const string& valStr) const
{
   if(boost::regex_match(valStr, kTrueTypeRegexStr))
   {
      return kJSONTrueType;
   }
   if(boost::regex_match(valStr, kFalseTypeRegexStr))
   {
      return kJSONFalseType;
   }
   if(boost::regex_match(valStr, kNullTypeRegexStr))
   {
      return kJSONNullType;
   }
   if(boost::regex_match(valStr, kNumberTypeRegexStr))
   {
      return kJSONNumberType;
   }
   if(boost::regex_match(valStr, kStringTypeRegexStr))
   {
      return kJSONStringType;
   }
   return kUNKNOWNJSONValueType;
}

TJSONValueType BoostJSON::GetValueType(ptree::const_iterator& itr) const
{
   TObjType objType(GetObjectType(itr));
   switch(objType)
   {
   case kObject:
      return kJSONObjectType;
   case kArray:
      return kJSONArrayType;
   case kValue:
   {
      return GetValueType(itr->second.data());
   }
   case kStrValuePair:
      return kJSONStringType; // assumes "string" in "string/value" pair
   default:
      break;
   }
   return kUNKNOWNJSONValueType;
}

const string& BoostJSON::GetValueTypeStr(TJSONValueType type) const
{
   static const string kJSONString("string");
   static const string kJSONNumber("number");
   static const string kJSONObject("object");
   static const string kJSONArray("array");
   static const string kJSONTrue("true");
   static const string kJSONFalse("false");
   static const string kJSONNull("null");
   static const string kUNKNOWNJSONValue("unknown");
   switch(type)
   {
   case kJSONStringType:
      return kJSONString;
   case kJSONNumberType:
      return kJSONNumber;
   case kJSONObjectType:
      return kJSONObject;
   case kJSONArrayType:
      return kJSONArray;
   case kJSONTrueType:
      return kJSONTrue;
   case kJSONFalseType:
      return kJSONFalse;
   case kJSONNullType:
      return kJSONNull;
   case kUNKNOWNJSONValueType:
   default:
      break;
   }
   return kUNKNOWNJSONValue;
}

size_t BoostJSON::GetChildCount(ptree::const_iterator& itr) const
{
   return (*itr).second.size();
}

}
