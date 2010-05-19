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
#ifndef __WEIDOCUMENT_H__
#define __WEIDOCUMENT_H__

/// @file   weiParser.h
/// @brief  Processing subsystem declaration

#include <stdexcept>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "weStrings.h"
#include "weTagScanner.h"
#include "weBlob.h"
#include "weDiffLib.h"

using namespace std;

namespace webEngine {

class i_transport;
class i_response;

class iEntity;
class iDocument;

typedef boost::shared_ptr<iEntity> iEntityPtr;
typedef boost::weak_ptr<iEntity> iEntityWPtr;
typedef vector<iEntityPtr> EntityList;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @interface  iEntity
///
/// @brief  Processing Entity interface. 
///
/// @author A. Abramov
/// @date   08.06.2009
////////////////////////////////////////////////////////////////////////////////////////////////////
class iEntity : public boost::enable_shared_from_this<iEntity>
{
public:
    iEntity() {};
    virtual ~iEntity() { ClearChildren(); };

    virtual const string Attr(string);
    virtual void Attr(string, string);
    void ClearAttr(void)    { attributes.clear(); };        ///< Clears all attributes

    virtual iEntityPtr Child(string type);
    virtual iEntityPtr Child(int idx);
    virtual EntityList Children() { return chldList; };  ///< Direct access to the children collection
    void ClearChildren(void);

    virtual const string InnerText(void) = 0;
    virtual const string OuterText(void) = 0;

    iEntityPtr FindID(string id);
    EntityList FindTags(string tag);

    virtual scanner_token Parse(string tagName, tag_scanner& scanner, i_transport* processor = NULL) { return wstError; };

    virtual CmpResults* Diff(iEntity& cmp, weCmpMode mode = weCmpDefault) = 0;
    virtual weCmpState Compare(iEntity& cmp, weCmpMode mode = weCmpDefault) = 0;
    virtual bool operator==(iEntity& cmp) { return (Compare(cmp) == weCmpEqual); };

    iDocument* GetRootDocument(void);

    //@{
    /// @brief Access the Parent
    const iEntityWPtr Parent(void) const	{ return(parent);   };
    void Parent(iEntityWPtr prnt)  { parent = prnt;    };
    //}

    //@{
    /// @brief  Name property access
    ///
    /// Name property represents the entity TAG name. Some names defined for the special entities. These
    /// names are starts with '#' sign (for example @e \#text for inner text).
    const string &Name(void) const      { return(entityName);         };
    void Name(const string &EntityName) { entityName = EntityName;    };
    //@}

    //@{
    /// @brief  CompareMode property access
    ///
    /// CompareMode property defines how the entities will be compared by default, i.e by the
    /// @b == operator, or if mode not set for the IsEqual function @n
    /// WeCompareDefault for default comparison means the strict
    /// comparison mode (WeCompatreStrict). But is not recommended
    /// to set the WeCompareDefault as the default mode. @n
    /// The CompareMode property of the WeHtmlEntity used to access
    /// this variable.
    static const weCmpMode &CompareMode(void)       { return(compareMode);  };
    static void CompareMode(const weCmpMode &cMode) { compareMode = cMode;	};
    //@}

    /// @brief  Gets the entity unique ID
    const string ID(void) const {return m_entityId; };

protected:
    virtual void GenerateId(void);
    bool IsParentTag(string tag);

#ifndef __DOXYGEN__
protected:
    iEntityWPtr parent;
    AttrMap attributes;
    EntityList chldList;
    string entityName;
    string m_entityId;
    static weCmpMode compareMode;
    int startPos, endPos;
#endif //__DOXYGEN__
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @interface	iDocument
///
/// @brief  Parsing results document. 
///
/// @author A. Abramov
/// @date   08.06.2009
////////////////////////////////////////////////////////////////////////////////////////////////////
class iDocument : virtual public iEntity
{
public:
    virtual bool ParseData(boost::shared_ptr<i_response> resp, i_transport* processor = NULL) = 0;
    virtual blob& Data(void) = 0;
};

void ClearEntityList(EntityList &lst);

} // namespace webEngine

#endif //__WEIDOCUMENT_H__
