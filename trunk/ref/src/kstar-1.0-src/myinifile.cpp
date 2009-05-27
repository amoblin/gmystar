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

#include <iostream>
#include <fstream>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <sys/stat.h>

#include "myinifile.h"

static char MAGICDATA[] ="# This file is written by CIniFile!";
static char SPACES[]={'\t','\b',' '};

static int FileExist(const char *fileName)
{
struct stat st;
return stat(fileName,&st)==0;
}

static size_t GetFileSize(const char * fileName)
{
struct stat st;
if(stat(fileName, &st)==0)
{
return st.st_size;
}
else
{
return 0;
}
}

void EraseSpace(string &str)
{
UInt32 pos;
while((pos = str.find_first_of(SPACES))!=string::npos)
{
str.erase(pos,1);
}
#if 0//_DEBUG
cout<<str<<endl;
#endif
}

static bool SplitKeyValue(const string &strline, string &key,string &value)
{
string tmp(strline);
EraseSpace(tmp);
if (tmp.empty()) return false;
if(tmp[0]=='#') return false;
UInt32 pos= tmp.find("=");
if(pos!=string::npos)
{
key = string(tmp,0,pos);
value = string(tmp,pos+1,tmp.size()-pos);
return true;
}
return false;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Class CIniSave */
/*++++++++++++++++++++++++++++++++++++++++++++++++*/
class CIniSave
{
public:
explicit CIniSave(ofstream &of)
:outf(of)
{
}

//函子主题
void operator()( const string & strini)
{
//cout<<strini<<endl;
outf<<strini<<endl;
}

~CIniSave(){}
private:
ofstream &outf;
};



/*++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Class CIniFile */
/*++++++++++++++++++++++++++++++++++++++++++++++++*/
CIniFile::CIniFile(const char *_fileName)
{
fileName = strdup(_fileName);
if(!FileExist(fileName))
{
Create();
}
else
{
Read();
}
}

void CIniFile::Create()
{
strVect.push_back(string(MAGICDATA));
Save();
}

void CIniFile::Save()
{
ofstream of(fileName);
if(of.fail())
{
cerr<<" Usage: Create file "<< fileName << " failed!" <<endl;
return ;
}
for_each(strVect.begin(),strVect.end(),CIniSave(of));
of.close();
}

void CIniFile::Read()
{
string tmp;
ifstream inf(fileName);
if(inf.is_open())
{
while(!inf.eof())
{
getline(inf,tmp,'\n');
strVect.push_back(tmp);
}
}

#if 0//_DEBUG
strVector::iterator it = strVect.begin();
for(;it!=strVect.end();it++)
{
cout<<*it<<endl;
}
#endif

inf.close();
}

bool CIniFile::ExistSect(const char *sect)
{
strVector::iterator it = GetSectIter(sect);
return it!=strVect.end();
}

strVector::iterator CIniFile::GetSectIter(const char *sect)
{
string strSect(sect);
strSect = '[' + strSect + ']';
strVector::iterator it = strVect.begin();
while(it!=strVect.end())
{
if(it->find(strSect)!=string::npos)
break;
it++;
}
return it;
}

strVector::iterator CIniFile::GetNextSectIter(strVector::iterator &beginIter)
{
if(beginIter==strVect.end())
{
return strVect.end();
}

strVector::iterator it = beginIter;
it++;
while(it!=strVect.end())
{
if(it->find('[')!=string::npos &&it->find(']')!=string::npos)
break;
it++;
}
return it;
}

strVector::iterator CIniFile::GetNextSectIter(const char *sect)
{
strVector::iterator it = GetSectIter(sect)+1;
while(it!=strVect.end())
{
if(it->find('[')!=string::npos &&it->find(']')!=string::npos)
break;
it++;
}
return it;

}

bool CIniFile::GetSections(const char *sect, strMap &mapKeyValue)
{
strVector::iterator first = GetSectIter(sect);
if (first == strVect.end())
return false;
mapKeyValue.clear();
string strKey,strValue;
strVector::iterator last = GetNextSectIter(first);
strVector::iterator it = first+1;
typedef strMap::value_type qpair;
for(;it != last; it++)
{
if (SplitKeyValue(*it,strKey,strValue))
{
mapKeyValue.insert(qpair(strKey,strValue));
}
}
return true;
}

bool CIniFile::ExistKey(const char *sect, const char *key)
{
strVector::iterator it = GetKeyIter(sect,key);
return it!=strVect.end();
}

bool CIniFile::ReadString(const char *sect,const char *key, char *& value)
{
strMap mapKeyValue;
GetSections(sect,mapKeyValue);
strMap::iterator it = mapKeyValue.begin();
for(; it != mapKeyValue.end();it++)
{
if (it->first.compare(key)==0)
{
if(value) delete value;
value = strdup(it->second.c_str());
return true;
}
}
/*while (it != mapKeyValue.end())
{
printf("1");
if (it->first.compare(key)==0)
{
value = strdup(it->second.c_str());
return true;
}
it++;
}*/
return false;
}

int CIniFile::ReadInteger(const char *sect,const char *key,int defvalue/*=0*/)
{
char *value=NULL;
if(ReadString(sect,key,value)&& value)
{
return atoi(value);
}
delete value;
return defvalue;
}

long CIniFile::ReadLong(const char *sect,const char *key,long defvalue/*=0*/)
{
char *value=NULL;
if(ReadString(sect,key,value)&& value)
{
return atol(value);
}
delete value;
return defvalue;
}

double CIniFile::ReadFloat(const char *sect,const char *key,double defvalue/*=0.0*/)
{
char *value=NULL;
if(ReadString(sect,key,value)&&value)
{
return atof(value);
}
delete value;
return defvalue;
}

bool CIniFile::ReadBool(const char *sect,const char *key,bool defvalue/*=0*/)
{
char *value=NULL;
if(ReadString(sect,key,value)&&value)
{
return *value!='0';
}
delete value;
return defvalue;
}

strVector::iterator CIniFile::GetKeyIter(const char *sect,const char *key)
{
strVector::iterator first = GetSectIter(sect);
if (first == strVect.end())
return strVect.end();
string strKey,strValue;
strVector::iterator last = GetNextSectIter(first);
strVector::iterator it = first;
it++;
for(;it != last; it++)
{
if (SplitKeyValue(*it,strKey,strValue))
{
if (strKey.compare(key)==0) break;
}
}
if(it!=last)
return it;
else
return strVect.end();
}

bool CIniFile::EraseKey(const char *sect ,const char *key)
{
strVector::iterator it = GetKeyIter(sect,key);
if (it != strVect.end())
{
strVect.erase(it);
Save();
return true;
}
return false;
}

bool CIniFile::EraseSect(const char *sect)
{
strMap mapKeyValue;
if(GetSections(sect,mapKeyValue)&&mapKeyValue.size()==0)
{
strVector::iterator it= GetSectIter(sect);
strVect.erase(it);
Save();
return true;
}
return false;
}

bool CIniFile::CreateSect(const char *sect)
{
string strSect(sect);
strSect = '[' + strSect + ']';
if(!ExistSect(sect))
{
strVect.push_back(strSect);
Save();
return true;
}
return false;
}

bool CIniFile::CreateKey(const char *sect,const char *key,const char *value)
{
string tmp(key);
tmp = " "+ tmp +" = " + value;
if (!ExistKey(sect,key))
{
strVector::iterator it= GetNextSectIter(sect);
strVect.insert(it,tmp);
Save();
return true;
}
return false;
}

bool CIniFile::UpdateKey(const char *sect,const char *key,const char *value)
{
strVector::iterator it = GetKeyIter(sect, key);
if(it !=strVect.end())
{
UInt32 first= it->find("=");
if (first != string::npos)
{
it->replace(first + 1,it->size() - first-1,value);
Save();
return true;
}
}
return false;
}

CIniFile::~CIniFile()
{
delete fileName;
}

