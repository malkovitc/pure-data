/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "data_tree.h"
#include "ceammc_datatypes.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

DataTree::DataTree(const PdArgs& args)
    : CollectionIFace<BaseObject>(args)
    , tree_(nullptr)
{
    createOutlet();

    auto str = to_string(args.args, " ");
    if (!str.empty())
        tree_ = TreePtr(new DataTypeTree(str.c_str()));
    else
        tree_ = TreePtr(new DataTypeTree());
}

void DataTree::proto_add(const AtomList& lst)
{
    if (tree_->isArray() || tree_->isNull()) {
        auto p = tree_->cloneT<DataTypeTree>();
        TreePtr j(p);

        if (lst.isFloat()) {
            auto f = atomlistToValue<t_float>(lst, 0);
            if (!p->addFloat(f))
                OBJ_ERR << "can't add float to json: " << f;
            else
                tree_ = j;
        } else if (lst.isSymbol()) {
            auto s = atomlistToValue<t_symbol*>(lst, &s_);
            if (!p->addSymbol(s))
                OBJ_ERR << "can't add symbol to json: " << s;
            else
                tree_ = j;
        } else if (lst.isList() && lst.allOf(isFloat)) {
            if (!p->addList(lst))
                OBJ_ERR << "can't add list to json: " << lst;
            else
                tree_ = j;
        } else if (lst.isDataType(data::DATA_TREE)) {
            TreePtr jptr(lst[0]);
            if (jptr.isNull()) {
                OBJ_ERR << "invalid json data: " << lst;
                return;
            }

            if (jptr->isNull())
                return;

            if (!p->addTree(*jptr))
                OBJ_ERR << "can't add json to json: " << lst;
            else
                tree_ = j;
        } else {
            auto str = to_string(lst, " ");
            if (!p->addTree(DataTypeTree(str.c_str())))
                OBJ_ERR << "can't add json to json: " << lst;
            else
                tree_ = j;
        }
    }
}

bool DataTree::proto_remove(const AtomList& lst)
{
}

void DataTree::proto_set(const AtomList& lst)
{
    if (lst.isFloat())
        tree_ = TreePtr(new DataTypeTree(atomlistToValue<t_float>(lst, 0)));
    else if (lst.isSymbol())
        tree_ = TreePtr(new DataTypeTree(atomlistToValue<t_symbol*>(lst, &s_)));
    else if (lst.isDataType(data::DATA_TREE))
        tree_ = TreePtr(lst[0]);
    else if (lst.allOf(isFloat))
        tree_ = TreePtr(new DataTypeTree(lst.asFloats()));
    else {
        auto str = to_string(lst, " ");
        tree_ = TreePtr(new DataTypeTree(str.c_str()));
    }
}

void DataTree::proto_clear()
{
    if (!tree_->isNull())
        tree_ = TreePtr(new DataTypeTree);
}

size_t DataTree::proto_size() const
{
    return tree_->size();
}

void DataTree::onBang()
{
    this->dataTo(0, tree_);
}

void DataTree::dump() const
{
    CollectionIFace<BaseObject>::dump();
    OBJ_DBG << tree_->toString();
}

void DataTree::onDataT(const DataTPtr<DataTypeTree>& j)
{
    tree_ = j;
    dataTo(0, j);
}

void DataTree::m_find(t_symbol* s, const AtomList& l)
{
    dataTo(0, tree_->match(to_string(l).c_str()));
}

void DataTree::m_at(t_symbol* s, const AtomList& l)
{
    if (l.isFloat()) {
        auto f = atomlistToValue<t_float>(l, 0);
        if (f < 0) {
            METHOD_ERR(s) << "invalid index: " << f;
            return;
        }

        dataTo(0, tree_->at(f));
    } else if (l.isSymbol()) {
        dataTo(0, tree_->at(atomlistToValue<t_symbol*>(l, 0)));
    }
}

void DataTree::m_key(t_symbol* s, const AtomList& l)
{
    //    dataTo(0, json_->key());
}

void DataTree::m_insert(t_symbol* s, const AtomList& lst)
{
    DataTypeTree* p = tree_->cloneT<DataTypeTree>();
    TreePtr j(p);

    if (lst.size() < 1 || !lst[0].isSymbol()) {
        METHOD_ERR(s) << "usage: " << s->s_name << " KEY [VALUES]";
        return;
    }

    const char* key = lst[0].asSymbol()->s_name;

    if (lst.size() < 2) {

    } else {
        if (lst[1].isFloat()) {
            auto f = lst[1].asFloat();
            if (!p->insertFloat(key, f))
                OBJ_ERR << "can't insert float to json: " << key << " " << f;
            else
                tree_ = j;
        } else if (lst[1].isSymbol()) {
            auto s = lst[1].asSymbol();
            if (!p->insertSymbol(key, s))
                OBJ_ERR << "can't insert symbol to json: " << key << " " << s;
            else
                tree_ = j;
        } /*else if (lst.isList() && lst.allOf(isFloat)) {
        if (!p->insert(lst))
            OBJ_ERR << "can't add list to json: " << lst;
        else
            json_ = j;
    }*/
    }
}

void setup_data_tree()
{
    ColectionIFaceFactory<DataTree> obj("data.tree");
    obj.processData<DataTypeTree>();
    obj.addMethod("at", &DataTree::m_at);
    obj.addMethod("find", &DataTree::m_find);
    obj.addMethod("insert", &DataTree::m_insert);
}
