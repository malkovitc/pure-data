/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "datatype_property.h"
#include "ceammc_convert.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"

#include <cstring>
#include <limits>
#include <sstream>

const DataType DataTypeProperty::dataType = data::DATA_PROPERTY;

DataTypeProperty::DataTypeProperty(t_symbol* name)
    : name_(name)
    , type_(T_FLOAT)
    , value_(0.f)
    , default_(0.f)
    , fmin_(std::numeric_limits<t_float>::lowest())
    , fmax_(std::numeric_limits<t_float>::max())
    , lmin_(std::numeric_limits<long>::lowest())
    , lmax_(std::numeric_limits<long>::max())
{
}

DataTypeProperty::DataTypeProperty(const DataTypeProperty& p)
    : name_(p.name_)
    , type_(p.type_)
    , value_(p.value_)
    , fmin_(p.fmin_)
    , fmax_(p.fmax_)
    , lmin_(p.lmin_)
    , lmax_(p.lmax_)
    , enum_(p.enum_)
{
}

DataType DataTypeProperty::type() const
{
    return dataType;
}

DataTypeProperty* DataTypeProperty::clone() const
{
    return new DataTypeProperty(*this);
}

std::string DataTypeProperty::toString() const
{
    std::ostringstream ss;
    ss << "Property: " << name_ << "\n"
       << "  type: " << type_ << "\n";

    return ss.str();
}

void DataTypeProperty::setTypeFloat(t_float def)
{
    type_ = T_FLOAT;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeInt(long def)
{
    type_ = T_INT;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeBool(bool def)
{
    type_ = T_BOOL;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeSymbol(t_symbol* def)
{
    type_ = T_SYMBOL;
    default_ = def;
    value_ = default_;
}

void DataTypeProperty::setTypeList(const AtomList& def)
{
    type_ = T_LIST;
    default_ = def;
    value_ = default_;
}

bool DataTypeProperty::setBool(bool v)
{
    if (type_ != T_BOOL)
        return false;

    value_ = v;
    updateAll();
    return true;
}

bool DataTypeProperty::setFloat(t_float f)
{
    if (type_ != T_FLOAT)
        return false;

    value_ = clip<t_float>(f, fmin_, fmax_);
    updateAll();
    return true;
}

bool DataTypeProperty::setInt(long v)
{
    if (type_ != T_INT)
        return false;

    value_ = clip<long>(v, lmin_, lmax_);
    updateAll();
    return true;
}

bool DataTypeProperty::setSymbol(t_symbol* s)
{
    if (type_ != T_SYMBOL)
        return false;

    if (enum_.size() > 0 && !enum_.contains(Atom(s)))
        return false;

    value_ = s;
    updateAll();
    return true;
}

bool DataTypeProperty::setList(const AtomList& lst)
{
    if (type_ != T_LIST)
        return false;

    value_ = lst;
    updateAll();
    return true;
}

bool DataTypeProperty::setFromPdArgs(const AtomList& lst)
{
    if (isFloat()) {
        if (lst.isFloat())
            setFloat(lst[0].asFloat());
        else {
            LIB_ERR << name_ << " float argument is expected: " << lst;
            return false;
        }
    } else if (isInt()) {
        if (lst.isFloat())
            setInt(lst[0].asFloat());
        else {
            LIB_ERR << name_ << "int argument is expected: " << lst;
            return false;
        }
    } else if (isBool()) {
        if (lst.isFloat() && (lst[0].asFloat() == 0.0 || lst[0].asFloat() == 1.0)) {
            setBool(lst[0].asFloat());
        } else {
            LIB_ERR << name_ << "1 or 0 is expected: " << lst;
            return false;
        }
    } else if (isSymbol()) {
        if (lst.isSymbol())
            setSymbol(lst[0].asSymbol());
        else {
            LIB_ERR << name_ << "symbol is expected: " << lst;
            return false;
        }
    } else if (isList()) {
        setList(lst);
    } else {
        LIB_ERR << "unhandled property type: " << propertyType();
        return false;
    }

    return true;
}

void DataTypeProperty::restoreDefault()
{
    value_ = default_;
    updateAll();
}

bool DataTypeProperty::getFloat(t_float& out) const
{
    if (type_ != T_FLOAT)
        return false;

    out = boost::get<t_float>(value_);
    return true;
}

bool DataTypeProperty::getInt(long& out) const
{
    if (type_ != T_INT)
        return false;

    out = boost::get<long>(value_);
    return true;
}

bool DataTypeProperty::getBool(bool& out) const
{
    if (type_ != T_BOOL)
        return false;

    out = boost::get<bool>(value_);
    return true;
}

bool DataTypeProperty::getSymbol(t_symbol** s) const
{
    if (type_ != T_SYMBOL)
        return false;

    *s = boost::get<t_symbol*>(value_);
    return true;
}

bool DataTypeProperty::getList(AtomList& out) const
{
    if (type_ != T_LIST)
        return false;

    out = boost::get<AtomList>(value_);
    return true;
}

bool DataTypeProperty::setFloatRange(t_float min, t_float max)
{
    if (type_ != T_FLOAT)
        return false;

    auto p = std::minmax(min, max);
    fmin_ = p.first;
    fmax_ = p.second;
    default_ = clip<t_float>(boost::get<t_float>(default_), fmin_, fmax_);
    return true;
}

bool DataTypeProperty::setIntRange(long min, long max)
{
    if (type_ != T_INT)
        return false;

    auto p = std::minmax(min, max);
    lmin_ = p.first;
    lmax_ = p.second;
    default_ = clip<long>(boost::get<long>(default_), lmin_, lmax_);
    return true;
}

bool DataTypeProperty::setEnumValues(const AtomList& lst)
{
    if (type_ != T_SYMBOL)
        return false;

    enum_.clear();
    enum_.append(boost::get<t_symbol*>(default_));
    enum_.append(lst.filter(ceammc::isSymbol));
    return true;
}

static PropertyInfoType type2type(DataTypeProperty::Type t)
{
    switch (t) {
    case DataTypeProperty::T_FLOAT:
        return PropertyInfoType::FLOAT;
    case DataTypeProperty::T_INT:
        return PropertyInfoType::INTEGER;
    case DataTypeProperty::T_BOOL:
        return PropertyInfoType::BOOLEAN;
    case DataTypeProperty::T_SYMBOL:
        return PropertyInfoType::SYMBOL;
    case DataTypeProperty::T_LIST:
        return PropertyInfoType::LIST;
    }
}

PropertyInfo DataTypeProperty::info() const
{
    char* name = strchr(name_->s_name, '@');
    PropertyInfo res(name, type2type(type_));

    if (isFloat()) {
        res.setRange(fmin_, fmax_);
        res.setDefault(boost::get<t_float>(default_));
    } else if (isInt()) {
        res.setDefault((int)boost::get<long>(default_));
    } else if (isBool()) {
        res.setRange(0, 1);
        res.setDefault(boost::get<bool>(default_) ? 1 : 0);
    } else if (isSymbol()) {
        res.setDefault(boost::get<t_symbol*>(default_));
    } else if (isList()) {
        res.setDefault(boost::get<AtomList>(default_));
    }

    return res;
}

void DataTypeProperty::updateAll()
{
    if (name_->s_thing)
        pd_bang(name_->s_thing);
}

PropertyStorage::Dict& PropertyStorage::storage()
{
    static Dict dict;
    return dict;
}

std::string PropertyStorage::makeFullName(const std::string& name, _glist* cnv)
{
    std::string res;
    char buf[30];
    snprintf(buf, sizeof(buf), "%p:", (void*)cnv);
    res += buf;
    res += name;
    return res;
}

PropertyPtr::PropertyPtr(const std::string& name)
    : name_(name)
    , prop_(PropertyStorage::storage().acquire(name_))
{
}

PropertyPtr::~PropertyPtr()
{
    if (prop_)
        PropertyStorage::storage().release(name_);
}