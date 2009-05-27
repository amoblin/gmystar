/***************************************************************************
 *   Copyright (C) 2008 by duanp   *
 *   duanp@163.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef INIFILE_H
#define INIFILE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;
typedef vector<string> strVector;
typedef map<string,string> strMap;
typedef unsigned long UInt32;


class CIniFile
{
public:
explicit CIniFile(const char *_fileName);
bool ExistSect(const char *sect);
bool GetSections(const char *sect, strMap &mapKeyValue);
bool ExistKey(const char *sect, const char *key);
bool ReadString(const char *sect,const char *key, char * &value);
int ReadInteger(const char *sect,const char *key,int defvalue=0);
long ReadLong(const char *sect,const char *key,long defvalue=0);
double ReadFloat(const char *sect,const char *key,double defvalue=0.0);
bool ReadBool(const char *sect,const char *key,bool defvalue=0);
bool EraseKey(const char *sect ,const char *key);
bool EraseSect(const char *sect);
bool CreateSect(const char *sect);
bool CreateKey(const char *sect,const char *key,const char *value);
bool UpdateKey(const char *sect,const char *key,const char *value);
~CIniFile();
protected:
void Create();
void Read();
void Save();
strVector::iterator GetSectIter(const char *sect);
strVector::iterator GetNextSectIter(strVector::iterator &beginIter);
strVector::iterator GetNextSectIter(const char *sect);
strVector::iterator GetKeyIter(const char *sect,const char *key);
private:
char *fileName;
strVector strVect;
};

#endif // INIFILE_H 
