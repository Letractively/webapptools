/*
    webEngine is the HTML processing library
    Copyright (C) 2009 Andrew Abramov aabramov@ptsecurity.ru

    This file is part of webEngine

    webEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    webEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with webEngine.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <strstream>
#include "storageFS.h"
#include "version.h"
#include "fsStorage.xpm"

static char* tables[] = {weObjTypeTask,
                        weObjTypeSysOption,
                        weObjTypeDictionary,
                        weObjTypeAuthInfo,
                        weObjTypeScan,
                        weObjTypeScanData,
                        weObjTypeObject,
                        weObjTypeProfile,
                        weObjTypeVulner,
                        weObjTypeVulnerDesc,
                        NULL}; // close the list with NULL

static string RecordToString(Record& rc)
{
    string retval = "";
    string tmp;

    StringList opts = rc.OptionsList();
    for (size_t j = 0; j < opts.size(); j++)
    {
        wOption opt = rc.Option(opts[j]);
        retval += opts[j];
        retval += '\x02';
        retval += boost::lexical_cast<string>(opt.Which());
        retval += '\x02';
        tmp = boost::lexical_cast<string>(opt.Value());
        if (tmp == "" || tmp[0] == '\0') {
            tmp = "";
        }
        retval += tmp;
        retval += '\x03';
    }

    return retval;
}

static wOptionVal StringToOption(const string& buff, string& name)
{
    wOptionVal retval;
    string parse = buff;
    size_t pos;
    int tp = -1;

    pos = parse.find('\x02');
    if (pos != string::npos)
    {
        name = parse.substr(0, pos);
        if (pos < parse.length())
        {
            parse = parse.substr(pos+1);
        }
        else {
            parse = "";
        }
    }
    pos = parse.find('\x02');
    if (pos != string::npos)
    {
        string val = parse.substr(0, pos);
        if (pos < parse.length())
        {
            parse = parse.substr(pos+1);
        }
        else {
            parse = "";
        }
        try {
            tp = boost::lexical_cast<int>(val);
        } catch (std::exception&) {
            tp = -1;
        }
    }
    pos = parse.find('\x02');
    string strData = "";
    if (pos == string::npos)
    {
        if(parse != "") {
            strData = parse;
        }
    }
    else {
        strData = parse.substr(0, pos);
    }
    try {
        switch(tp)
        {
        case 0:
            retval = boost::lexical_cast<char>(strData);
            break;
        case 1:
            retval = boost::lexical_cast<unsigned char>(strData);
            break;
        case 2:
            retval = boost::lexical_cast<int>(strData);
            break;
        case 3:
            retval = boost::lexical_cast<unsigned int>(strData);
            break;
        case 4:
            retval = boost::lexical_cast<long>(strData);
            break;
        case 5:
            retval = boost::lexical_cast<unsigned long>(strData);
            break;
        case 6:
            retval = boost::lexical_cast<bool>(strData);
            break;
        case 7:
            retval = boost::lexical_cast<double>(strData);
            break;
        case 8:
            retval = strData;
        }
    } catch (std::exception&) {
        tp = -1;
    }
    return retval;
}

static Record* StringToRecord(const string& buff)
{
    Record *retval = new Record;
    string parse = buff;
    size_t pos;

    pos = parse.find('\x03');
    while (pos != string::npos)
    {
        string nm;
        string val = parse.substr(0, pos);
        if (pos < parse.length())
        {
            parse = parse.substr(pos+1);
        }
        else {
            parse = "";
        }
        wOptionVal oval = StringToOption(val, nm);
        retval->Option(nm, oval);
        pos = parse.find('\x03');
    }
    if (parse != "")
    {
        string nm;
        wOptionVal oval = StringToOption(parse, nm);
        retval->Option(nm, oval);
    }

    return retval;
}

FsStorage::FsStorage( Dispatch* krnl, void* handle /*= NULL*/ ) :
    iStorage(krnl, handle)
{
    pluginInfo.IfaceName = "FsStorage";
    pluginInfo.IfaceList.push_back("FsStorage");
    pluginInfo.PluginDesc = "FileSystem storage";
    pluginInfo.PluginId = "1FBAB8DCF440";
    pluginInfo.PluginIcon = WeXpmToStringList(fsStorage_xpm, sizeof(fsStorage_xpm) / sizeof(char*) );
    db_dir = "";
    lastId = 0;
    LOG4CXX_TRACE(logger, "FsStorage plugin created");
}

FsStorage::~FsStorage(void)
{
    LOG4CXX_TRACE(logger, "FsStorage plugin destroyed");
}

void* FsStorage::GetInterface( const string& ifName )
{
    LOG4CXX_TRACE(logger, "FsStorage::GetInterface " << ifName);
    if (iequals(ifName, "FsStorage"))
    {
        LOG4CXX_DEBUG(logger, "FsStorage::GetInterface found!");
        usageCount++;
        return (void*)(this);
    }
    return iStorage::GetInterface(ifName);
}

const string FsStorage::GetSetupUI( void )
{
    /// @todo: change XRC to set the fields values
    return "";
}

void FsStorage::ApplySettings( const string& xmlData )
{

}

bool FsStorage::InitStorage(const string& params)
{
    bool retval = false;

    LOG4CXX_TRACE(logger, "FsStorage::InitStorage params = " << params);
    try {
        db_dir = params;
        fs::path dir_path(db_dir);
        if ( !fs::exists(dir_path) ) {
            fs::create_directory(db_dir.c_str());
        }
        else {
            if ( ! fs::is_directory(dir_path) ) {
                dir_path = dir_path.remove_filename();
                db_dir = dir_path.string();
            }
        }
        LOG4CXX_TRACE(logger, "FsStorage::InitStorage: base dir is " << db_dir);

        int i = 0;
        while (tables[i] != NULL)
        {
            // check storage presence
            dir_path = db_dir;
            dir_path /= tables[i];
            if ( !fs::exists(dir_path) ) {
                fs::create_directory(dir_path);
        }
        else {
            if ( ! fs::is_directory(dir_path) ) {
                string msg = dir_path.string() + "isn't a directory";
                    throw std::runtime_error(msg.c_str());
                }
            }
            LOG4CXX_TRACE(logger, "FsStorage::InitStorage: " << tables[i] << " storage dir is " << dir_path.string());
            i++;
        }
    }
    catch(std::exception& e) {
        LOG4CXX_ERROR(logger, "FsStorage::InitStorage: " << e.what());
        retval = false;
    }
    return retval;
}

void FsStorage::Flush(const string& params /*= ""*/)
{
    // nothing to do. Just reimplement base class method
}

string FsStorage::GenerateID(const string& objType /*= ""*/)
{
    string retval = "";
    fs::path dir_path(db_dir);

    dir_path /= "index";

    try {
        ifstream ifs(dir_path.string().c_str());
        ifs >> lastId;
    }
    catch ( const std::exception& )
    {
        LOG4CXX_WARN(logger, "FsStorage::GenerateID no index for " << objType);
        lastId = 1;
    }
    retval = lexical_cast<string>(++lastId);
    try {
        ofstream ofs(dir_path.string().c_str());
        ofs << lastId;
    }
    catch ( const std::exception & )
    {
        LOG4CXX_ERROR(logger, "FsStorage::GenerateID can't save index for " << objType);
    }

    return retval;
}

int FsStorage::Get(Record& filter, Record& respFilter, RecordSet& results)
{
    fs::path dir_path(db_dir);
    RecordSet* dta;
    StringList reportNames;
    wOption opt;
    size_t i, j;
    RecordSet lk;

    LOG4CXX_TRACE(logger, "FsStorage::Get objType=" << filter.objectID);
    // lock the db
    LockDB();

    // if no options in the filter - get all records
    dta = Search(filter, (filter.OptionSize() == 0));

    // prepare filter list
    reportNames = respFilter.OptionsList();
    if (reportNames.size() == 0)
    {
        StringList* strct;
        strct = GetStruct(filter.objectID);
        if (strct != NULL)
        {
            reportNames = *strct;
            delete strct;
        }
    }
    for (i = 0; i < dta->size(); i++) {
        Record* objRes = new Record;
        objRes->objectID = filter.objectID;
        for (j = 0; j < reportNames.size(); j++)
        {
            opt = (*dta)[i].Option(reportNames[j]);
            objRes->Option(reportNames[j], opt.Value());
        } // and of attribute filters
        results.push_back(*objRes);
    } // end of objects search

    UnlockDB();
    return results.size();
}

int FsStorage::Set(Record& filter, Record& data)
{
    wOption opt;
    string fName;
    RecordSet* retlist = NULL;
    fs::path fp;
    Record sv;
    int retval = 0;

    LOG4CXX_TRACE(iLogger::GetLogger(), "FsStorage::Set(Record, Record)");

    // lock the db
    LockDB();

    retlist = Search(filter);
    if (retlist == NULL)
    {
        retlist = new RecordSet;
        retlist->clear();
    }
    if (retlist->size() > 0)
    {   // update
        LOG4CXX_TRACE(iLogger::GetLogger(), "FsStorage::Set(Record, Record) - update type=" << filter.objectID);
        for (size_t i = 0; i < retlist->size(); i ++)
        {
            (*retlist)[i].CopyOptions(&data);

            opt = (*retlist)[i].Option(weoID);
            SAFE_GET_OPTION_VAL(opt, fName, "");
            if (fName == "")
            {
                fName = GenerateID(filter.objectID);
                (*retlist)[i].Option(weoID, fName);
            }
            retval++;
        }
    }
    else {
        // insert
        LOG4CXX_TRACE(iLogger::GetLogger(), "FsStorage::Set(Record, Record) - insert type=" << data.objectID);
        sv.objectID = data.objectID;
        sv.CopyOptions(&data);
        opt = sv.Option(weoID);
        SAFE_GET_OPTION_VAL(opt, fName, "");
        if (fName == "")
        {
            fName = GenerateID(data.objectID);
            sv.Option(weoID, fName);
        }
        retlist->push_back(sv);
        retval++;
    }
    fp = fs::path(db_dir);
    fp /= filter.objectID;
    FileSave(fp, *retlist);

    FixStruct(data.objectID, sv);
    UnlockDB();
    return retval;
}

int FsStorage::Set(RecordSet& data)
{
    wOption opt;
    string fName;
    RecordSet* retlist = NULL;
    fs::path fp;
    Record sv;
    int retval = 0;
    size_t i;
    Record fl;

    LOG4CXX_TRACE(iLogger::GetLogger(), "FsStorage::Set(RecordSet)");
    for (i = 0; i < data.size(); i++)
    {
        fl.Clear();
        fl.objectID = data[i].objectID;
        opt = data[i].Option(weoID);
        fl.Option(weoID, opt.Value());
        retval += Set(fl, data[i]);
    }
    return retval;
}

int FsStorage::Delete(Record& filter)
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "FsStorage::Delete");
    LockDB();

    fs::path fp;
    RecordSet* retlist = Search(filter, false);
    int retval;
    Record del;

    for (retval = 0; retval < retlist->size(); retval++) {
        wOption opt = (*retlist)[retval].Option(weoID);
        string id;
        SAFE_GET_OPTION_VAL(opt, id, "");
        fp = fs::path(db_dir);
        fp /= filter.objectID;
        fp /= "data" + id;
        fs::remove(fp);
    }

    retval = retlist->size();

    UnlockDB();
    return retval;
}

RecordSet* FsStorage::Search(Record& filter, bool all/* = false*/)
{
    RecordSet* retlist = new RecordSet;
    RecordSet* datalist = NULL;
    fs::path dir_path(db_dir);
    Record* data;
    StringList objProps;
    wOption opt;
    string stValue, stData;
    size_t j;

    dir_path /= filter.objectID;

    fs::directory_iterator end_itr; // default construction yields past-the-end
    for ( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
    {
        if (starts_with(itr->path().filename(), "data"))
        {
            data = FileRead(itr->path().string());
            bool skip = false;
            if (!all)
            {
                objProps = filter.OptionsList();
                for (j = 0; j < objProps.size(); j++)
                {
                    opt = filter.Option(objProps[j]);
                    stValue = boost::lexical_cast<std::string>(opt.Value());

                    opt = data->Option(objProps[j]);
                    stData = boost::lexical_cast<std::string>(opt.Value());

                    if (stData != stValue)
                    {
                        skip = true;
                    }
                } // compare options
            } // take all files
            if (!skip)
            {
                data->objectID = filter.objectID;
                retlist->push_back(*data);
            }
            else {
                delete data;
            }

        }
    }

    return retlist;
}

Record* FsStorage::FileRead(const string& fname)
{
    Record *retval = NULL;

    LOG4CXX_TRACE(logger, "FsStorage::FileRead " << fname);
    try{
        size_t fsize = fs::file_size(fs::path(fname));
        ifstream ifs(fname.c_str());
        char* buff = new char[fsize + 10];
        if(buff != NULL)
        {
            memset(buff, 0, fsize+10);
            ifs.read(buff, fsize);
            retval = StringToRecord(buff);
            delete buff;
        }
    }
    catch ( const std::exception & e )
    {
        LOG4CXX_ERROR(logger, "FsStorage::FileRead error: " << e.what());
        if (retval != NULL)
        {
            delete retval;
            retval = NULL;
        }
    }

    return retval;
}

int FsStorage::FileSave(const fs::path& fspath, const RecordSet& content)
{
    fs::path fp;
    int retval = 0;
    string id;
    wOption opt;

    // save data
    LOG4CXX_TRACE(logger, "FsStorage::FileSave recordset " << fp.string());
    try {
        for (retval = 0; retval < content.size(); retval++)
        {
            Record rc = content[retval];
            opt = rc.Option(weoID);
            SAFE_GET_OPTION_VAL(opt, id, "");
            fp = fspath;
            fp /= "data" + id;

            ofstream ofs(fp.string().c_str());
            ofs << RecordToString(rc);
        }
    }
    catch(const std::exception& e)
    {
        LOG4CXX_ERROR(logger, "FsStorage::FileSave can't save " << fp.string() << " : " << e.what());
    }
    return retval;
}

StringList* FsStorage::GetStruct(const string& nspace)
{
    fs::path fp(db_dir);
    StringList *structNames = NULL;
    string sdata;


    fp /= nspace;
    fp /= "struct";
    try
    {
        size_t fsize = fs::file_size(fp);
        ifstream is(fp.string().c_str());
        char* buff = new char[fsize + 10];
        if(buff != NULL)
        {
            memset(buff, 0, fsize+10);
            is.read(buff, fsize);
            structNames = StringToSList(buff);
            delete buff;
        }
    }
    catch(std::exception& e)
    {
        LOG4CXX_WARN(iLogger::GetLogger(), "FsStorage::GetStruct " << nspace << " structure error: " << e.what());
        if (structNames)
        {
            delete structNames;
            structNames = NULL;
        }
    }

    return structNames;
}

void FsStorage::FixStruct(const string& nspace, Record& strt)
{
    StringMap::iterator obj;
    StringList::iterator lst;
    string stData;
    StringList* structNames;
    StringList flNames;
    size_t i;

    fs::path fp(db_dir);
    fp /= nspace;
    fp /= "struct";

    structNames = GetStruct(nspace);
    if (structNames == NULL)
    {
        structNames = new StringList;
    }
    flNames = strt.OptionsList();
    for (i = 0 ; i < flNames.size(); i++)
    {
        lst = find(structNames->begin(), structNames->end(), flNames[i]);
        if (lst == structNames->end())
        {
            structNames->push_back(flNames[i]);
        }
    }
    try
    {
        ofstream os(fp.string().c_str());
        os << SListToString(*structNames);
        // archive and stream closed when destructor are called
    }
    catch(std::exception& e)
    {
        LOG4CXX_WARN(iLogger::GetLogger(), "FsStorage::FixStruct save " << nspace << " structure error: " << e.what());
        return;
    }
}

void FsStorage::LockDB()
{
    fs::path locker(db_dir);
    locker /= "lock";

    LOG4CXX_TRACE(logger, "FsStorage::LockDB");
    while (exists(locker))
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    {
        ofstream os(locker.string().c_str());
        os << "lock";
        // archive and stream closed when destructor are called
    }
    LOG4CXX_TRACE(logger, "FsStorage::LockDB - locked");
}

void FsStorage::UnlockDB()
{
    fs::path locker(db_dir);
    locker /= "lock";

    LOG4CXX_TRACE(logger, "FsStorage::UnlockDB");
    if (exists(locker))
    {
        fs::remove(locker);
    }
}