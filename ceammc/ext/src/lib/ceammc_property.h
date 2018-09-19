/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_H
#define CEAMMC_PROPERTY_H

#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "ceammc_property_info.h"

#include <initializer_list>
#include <iosfwd>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace ceammc {

t_symbol* SYM_DUMP();
t_symbol* SYM_PROPS_ALL();
t_symbol* SYM_PROPS_ALL_Q();

class Property {
    PropertyInfo info_;
    bool readonly_;
    bool visible_;

public:
    Property(const PropertyInfo& info, bool readonly = false);
    virtual ~Property();

    const std::string& name() const { return info_.name(); }

    bool readonly() const { return readonly_; }
    bool visible() const { return visible_; }

    virtual bool set(const AtomList&) = 0;
    virtual AtomList get() const = 0;

    void setVisible(bool v) { visible_ = v; }
    void setReadonly(bool v) { readonly_ = v; }

    PropertyInfo& info() { return info_; }
    const PropertyInfo& info() const { return info_; }

protected:
    bool readonlyCheck() const;
    bool emptyValueCheck(const AtomList& v) const;
};

class AtomProperty : public Property {
    Atom v_;

public:
    AtomProperty(const std::string& name, const Atom& def, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    Atom value() const { return v_; }
    void setValue(const Atom& v) { v_ = v; }
};

class CombinedProperty : public Property {
    std::vector<Property*> props_;

public:
    CombinedProperty(const std::string& name, std::initializer_list<Property*> props);
    bool set(const AtomList&);
    AtomList get() const;
};

class ListProperty : public Property {
    AtomList lst_;

public:
    ListProperty(const std::string& name, const AtomList& l = AtomList(), bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    const AtomList& value() const;
    AtomList& value();
};

class FloatProperty : public Property {
    float v_;

public:
    FloatProperty(const std::string& name, float init = 0, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    float value() const { return v_; }
    void setValue(float v) { v_ = v; }
};

class IntProperty : public Property {
    int v_;

public:
    IntProperty(const std::string& name, int init = 0, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    int value() const { return v_; }
    void setValue(int v) { v_ = v; }
};

class SizeTProperty : public Property {
    size_t v_;

public:
    SizeTProperty(const std::string& name, size_t init = 0, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    size_t value() const { return v_; }
    void setValue(size_t v) { v_ = v; }
};

template <typename T>
class EnumProperty : public Property {
public:
    typedef std::vector<T> ValueList;

public:
    EnumProperty(const std::string& name, T def, bool readonly = false)
        : Property(PropertyInfo(name, PropertyInfoType::LIST), readonly)
        , idx_(0)
        , def_(def)
    {
        info().setDefault(def);
        info().addEnum(def);
        allowed_.push_back(def);
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!emptyValueCheck(lst))
            return false;

        T v = atomlistToValue<T>(lst, def_);
        long idx = enumIndex(v);
        if (idx < 0) {
            ::operator<<(LIB_ERR << "invalid property value: " << v << ". valid values are: ", allowed_);
            return false;
        }

        idx_ = idx;
        return true;
    }

    AtomList get() const
    {
        return listFrom(value());
    }

    size_t numEnums() const { return allowed_.size(); }

    T value() const
    {
        if (idx_ < 0)
            return def_;
        return allowed_[idx_];
    }

    bool setValue(T v)
    {
        long idx = enumIndex(v);
        if (idx < 0)
            return false;

        idx_ = idx;
        return true;
    }

    void appendEnum(T v)
    {
        if (enumIndex(v) < 0) {
            allowed_.push_back(v);
            info().addEnum(v);
        }
    }

    long enumIndex(T v) const
    {
        for (auto it = allowed_.begin(); it != allowed_.end(); ++it) {
            if (*it == v)
                return std::distance(allowed_.begin(), it);
        }

        return -1;
    }

private:
    ValueList allowed_;
    T def_;
    int idx_;
};

class SymbolProperty : public Property {
    mutable t_symbol* value_;

public:
    SymbolProperty(const std::string& name, t_symbol* init, bool readonly = false);

    AtomList get() const;
    bool set(const AtomList& lst);

    t_symbol* value() const;
    void setValue(t_symbol* s);

    std::string str() const;
};

class SymbolEnumProperty : public EnumProperty<t_symbol*> {
public:
    SymbolEnumProperty(const std::string& name, t_symbol* init, bool readonly = false)
        : EnumProperty<t_symbol*>(name, init, readonly)
    {
        info().setType(PropertyInfoType::SYMBOL);
        info().setView(PropertyInfoView::MENU);
    }

    ~SymbolEnumProperty();

    bool is(const char* v) const
    {
        return value() == gensym(v);
    }
};

class FlagProperty : public Property {
    bool v_;

public:
    FlagProperty(const std::string& name);
    bool value() const { return v_; }
    AtomList get() const;
    bool set(const AtomList&);
};

template <typename T, typename V>
class AliasProperty : public Property {
    T* ptr_;
    V val_;

public:
    AliasProperty(const std::string& name, T* prop, V v)
        : Property(PropertyInfo(name, PropertyInfoType::LIST), false)
        , ptr_(prop)
        , val_(v)
    {
        info().setType(prop->info().type());
    }

    bool set(const AtomList&)
    {
        ptr_->setValue(val_);
        return true;
    }

    AtomList get() const { return listFrom(bool(ptr_->value() == val_)); }
};

typedef AliasProperty<SymbolEnumProperty, t_symbol*> SymbolEnumAlias;

typedef EnumProperty<int> IntEnumProperty;

class BoolProperty : public Property {
    bool v_;

public:
    BoolProperty(const std::string& name, bool init, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    bool value() const { return v_; }
    void setValue(bool v) { v_ = v; }
};

template <typename T>
class CallbackProperty : public Property {
public:
    typedef AtomList (T::*GetterFn)() const;
    typedef void (T::*SetterFn)(const AtomList& v);

public:
    CallbackProperty(const std::string& name, T* obj, GetterFn gf, SetterFn sf = 0)
        : Property(PropertyInfo(name, PropertyInfoType::LIST), sf == 0 ? true : false)
        , obj_(obj)
        , getter_(gf)
        , setter_(sf)
    {
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        (obj_->*setter_)(lst);
        return true;
    }

    AtomList get() const
    {
        return (obj_->*getter_)();
    }

protected:
    T* obj_;

private:
    GetterFn getter_;
    SetterFn setter_;
};

template <typename T, typename B>
class TypedCbProperty : public CallbackProperty<TypedCbProperty<T, B>> {
    typedef T (B::*TGetterFn)() const;
    typedef void (B::*SGetterFn)(const T&);

public:
    TypedCbProperty(const std::string& name, B* obj, TGetterFn gf, SGetterFn sf = 0)
        : CallbackProperty<TypedCbProperty<T, B>>(name,
              this,
              &TypedCbProperty::defGetter, sf == 0 ? 0 : &TypedCbProperty::defSetter)
        , bobj_(obj)
        , tgetter_(gf)
        , tsetter_(sf)

    {
    }

private:
    AtomList defGetter() const
    {
        T v = (this->bobj_->*tgetter_)();
        return listFrom(v);
    }

    void defSetter(const AtomList& l)
    {
        T v = atomlistToValue<T>(l, T());
        (this->bobj_->*tsetter_)(v);
    }

private:
    B* bobj_;
    TGetterFn tgetter_;
    SGetterFn tsetter_;
};

template <typename T>
class PointerProperty : public Property {
    T* vptr_;

public:
    PointerProperty(const std::string& name, T* value, bool readonly = true)
        : Property(PropertyInfo(name, PropertyInfo::toType<T>()), readonly)
        , vptr_(value)
    {
        info().setDefault(*value);
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!emptyValueCheck(lst))
            return false;

        *vptr_ = atomlistToValue(lst, T());
        return true;
    }

    AtomList get() const
    {
        return listFrom(*vptr_);
    }
};

template <typename P, typename V, typename CheckFunc>
class CheckedProperty : public Property {
public:
    CheckedProperty(const std::string& name, V value, CheckFunc fn)
        : Property(PropertyInfo(name, PropertyInfo::toType<V>()), false)
        , prop_(new P(name, value, false))
        , def_(value)
        , check_fn_(fn)
    {
        info().setDefault(value);
    }

    ~CheckedProperty()
    {
        delete prop_;
    }

    AtomList get() const
    {
        return prop_->get();
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!emptyValueCheck(lst))
            return false;

        if (!check_fn_(atomlistToValue<V>(lst, def_))) {
            LIB_ERR << "invalid value for property " << name() << ": " << lst << ". " << errorString();
            return false;
        }

        prop_->set(lst);
        return true;
    }

    void setValue(V v)
    {
        if (check_fn_(v))
            reinterpret_cast<P*>(prop_)->setValue(v);
    }

    V value()
    {
        return reinterpret_cast<P*>(prop_)->value();
    }

    virtual std::string errorString() const
    {
        return std::string();
    }

private:
    Property* prop_;
    V def_;

public:
    CheckFunc check_fn_;
};

template <typename T>
class ValidateLT {
public:
    T upper_;

public:
    ValidateLT(T upperBound)
        : upper_(upperBound)
    {
    }

    bool operator()(T v) const
    {
        return v < upper_;
    }
};

template <typename T>
class ValidateLE {
public:
    T upper_;

public:
    ValidateLE(T upperBound)
        : upper_(upperBound)
    {
    }

    bool operator()(T v) const
    {
        return v <= upper_;
    }
};

template <typename T>
class ValidateGT {
public:
    T lower_;

public:
    ValidateGT(T lowerBound)
        : lower_(lowerBound)
    {
    }

    bool operator()(T v) const
    {
        return lower_ < v;
    }
};

template <typename T>
class ValidateGE {
public:
    T lower_;

public:
    ValidateGE(T lowerBound)
        : lower_(lowerBound)
    {
    }

    bool operator()(T v) const
    {
        return lower_ <= v;
    }
};

template <typename T>
class ValidateRangeOpen {
public:
    T lower_;
    T upper_;

public:
    ValidateRangeOpen(T lowerBound, T upperBound)
        : lower_(lowerBound)
        , upper_(upperBound)
    {
    }

    bool operator()(T v) const
    {
        return lower_ < v && v < upper_;
    }
};

template <typename T>
class ValidateRangeClosed {
public:
    T lower_;
    T upper_;

public:
    ValidateRangeClosed(T lowerBound, T upperBound)
        : lower_(lowerBound)
        , upper_(upperBound)
    {
    }

    bool operator()(T v) const
    {
        return lower_ <= v && v <= upper_;
    }
};

template <typename P, typename V>
class GreaterThenProperty : public CheckedProperty<P, V, ValidateGT<V>> {
public:
    GreaterThenProperty(const std::string& name, V initValue, V minValue)
        : CheckedProperty<P, V, ValidateGT<V>>(name,
              ValidateGT<V>(minValue)(initValue) ? initValue : minValue + 1,
              minValue)
    {
        Property::info().setMin(minValue);
    }

    virtual std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be > " << this->check_fn_.lower_ << '.';
        return os.str();
    }
};

template <typename P, typename V>
class GreaterEqualProperty : public CheckedProperty<P, V, ValidateGE<V>> {
public:
    GreaterEqualProperty(const std::string& name, V initValue, V minValue)
        : CheckedProperty<P, V, ValidateGE<V>>(name, std::max(initValue, minValue), minValue)
    {
        Property::info().setMin(minValue);
    }

    std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be >= " << this->check_fn_.lower_ << '.';
        return os.str();
    }
};

template <typename P, typename V>
class LessThenProperty : public CheckedProperty<P, V, ValidateLT<V>> {
public:
    LessThenProperty(const std::string& name, V initValue, V maxValue)
        : CheckedProperty<P, V, ValidateLT<V>>(name,
              ValidateLT<V>(maxValue)(initValue) ? initValue : maxValue - 1, maxValue)
    {
        Property::info().setMax(maxValue);
    }

    std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be < " << this->check_fn_.upper_ << '.';
        return os.str();
    }
};

template <typename P, typename V>
class LessEqualProperty : public CheckedProperty<P, V, ValidateLE<V>> {
public:
    LessEqualProperty(const std::string& name, V initValue, V maxValue)
        : CheckedProperty<P, V, ValidateLE<V>>(name, std::min(initValue, maxValue), maxValue)
    {
        Property::info().setMax(maxValue);
    }

    virtual std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be <= " << this->check_fn_.upper_ << '.';
        return os.str();
    }
};

template <typename P, typename V>
class OpenRangeProperty : public CheckedProperty<P, V, ValidateRangeOpen<V>> {
public:
    OpenRangeProperty(const std::string& name, V initValue, V minValue, V maxValue)
        : CheckedProperty<P, V, ValidateRangeOpen<V>>(name,
              ValidateRangeOpen<V>(minValue, maxValue)(initValue) ? initValue : (minValue + maxValue) / 2,
              ValidateRangeOpen<V>(minValue, maxValue))
    {
        Property::info().setRange(minValue, maxValue);
    }

    virtual std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be "
           << this->check_fn_.lower_ << " < x < " << this->check_fn_.upper_
           << '.';
        return os.str();
    }
};

template <typename P, typename V>
class ClosedRangeProperty : public CheckedProperty<P, V, ValidateRangeClosed<V>> {
public:
    ClosedRangeProperty(const std::string& name, V initValue, V minValue, V maxValue)
        : CheckedProperty<P, V, ValidateRangeClosed<V>>(name,
              std::min(std::max(initValue, minValue), maxValue),
              ValidateRangeClosed<V>(minValue, maxValue))
    {
        Property::info().setRange(minValue, maxValue);
    }

    virtual std::string errorString() const
    {
        std::ostringstream os;
        os << "Value shoud be "
           << this->check_fn_.lower_ << " <= x <= " << this->check_fn_.upper_
           << '.';
        return os.str();
    }
};

template <typename P, typename V>
class InitProperty : public Property {
    P* prop_;

public:
    InitProperty(P* prop)
        : Property(PropertyInfo(prop->name(), PropertyInfo::toType<V>()), false)
        , prop_(prop)
    {
        info().setDefault(prop->info().defaultValue());
    }

    ~InitProperty()
    {
        delete prop_;
    }

    V value() const
    {
        return prop_->value();
    }

    AtomList get() const
    {
        return prop_->get();
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        prop_->set(lst);
        prop_->setReadonly(true);
        setReadonly(true);
        return true;
    }

    const P* prop() const
    {
        return prop_;
    }

    P* prop()
    {
        return prop_;
    }
};

typedef GreaterEqualProperty<IntProperty, int> IntPropertyMinEq;
typedef GreaterThenProperty<IntProperty, int> IntPropertyMin;
typedef LessEqualProperty<IntProperty, int> IntPropertyMaxEq;
typedef LessThenProperty<IntProperty, int> IntPropertyMax;
typedef OpenRangeProperty<IntProperty, int> IntPropertyOpenRange;
typedef ClosedRangeProperty<IntProperty, int> IntPropertyClosedRange;

typedef GreaterEqualProperty<SizeTProperty, size_t> SizeTPropertyMinEq;
typedef GreaterThenProperty<SizeTProperty, size_t> SizeTPropertyMin;
typedef LessEqualProperty<SizeTProperty, size_t> SizeTPropertyMaxEq;
typedef LessThenProperty<SizeTProperty, size_t> SizeTPropertyMax;
typedef OpenRangeProperty<SizeTProperty, size_t> SizeTPropertyOpenRange;
typedef ClosedRangeProperty<SizeTProperty, size_t> SizeTPropertyClosedRange;

typedef GreaterEqualProperty<FloatProperty, float> FloatPropertyMinEq;
typedef GreaterThenProperty<FloatProperty, float> FloatPropertyMin;
typedef LessEqualProperty<FloatProperty, float> FloatPropertyMaxEq;
typedef LessThenProperty<FloatProperty, float> FloatPropertyMax;
typedef OpenRangeProperty<FloatProperty, float> FloatPropertyOpenRange;
typedef ClosedRangeProperty<FloatProperty, float> FloatPropertyClosedRange;

typedef InitProperty<AtomProperty, Atom> InitAtomProperty;
typedef InitProperty<ListProperty, AtomList> InitListProperty;
typedef InitProperty<IntProperty, int> InitIntProperty;
typedef InitProperty<SizeTProperty, size_t> InitSizeTProperty;
typedef InitProperty<BoolProperty, bool> InitBoolProperty;
typedef InitProperty<SymbolProperty, t_symbol*> InitSymbolProperty;

#define CEAMMC_FLOAT_PROPERTY_DECLARE(_name) typedef InitProperty<_name, float> Init##_name
#define CEAMMC_INT_PROPERTY_DECLARE(_name) typedef InitProperty<_name, int> Init##_name
#define CEAMMC_SIZE_T_PROPERTY_DECLARE(_name) typedef InitProperty<_name, size_t> Init##_name

CEAMMC_FLOAT_PROPERTY_DECLARE(FloatProperty);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyMin);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyMax);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyMinEq);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyMaxEq);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyOpenRange);
CEAMMC_FLOAT_PROPERTY_DECLARE(FloatPropertyClosedRange);

CEAMMC_INT_PROPERTY_DECLARE(IntProperty);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyMin);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyMax);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyMinEq);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyMaxEq);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyOpenRange);
CEAMMC_INT_PROPERTY_DECLARE(IntPropertyClosedRange);

CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTProperty);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyMin);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyMax);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyMinEq);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyMaxEq);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyOpenRange);
CEAMMC_SIZE_T_PROPERTY_DECLARE(SizeTPropertyClosedRange);

#undef CEAMMC_FLOAT_PROPERTY_DECLARE
#undef CEAMMC_INT_PROPERTY_DECLARE
#undef CEAMMC_SIZE_T_PROPERTY_DECLARE
}

#endif // CEAMMC_PROPERTY_H
