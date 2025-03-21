
/// @file PyBnNodeList.cc
/// @brief Python BnNodeList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNodeList.h"
#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyModule.h"
#include "ym/BnNodeList.h"
#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnNodeListObject
{
  PyObject_HEAD
  BnNodeList mNodeList;
};

// Python 用のタイプ定義
PyTypeObject BnNodeList_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnNodeList_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnNodeList' is disabled");
  return nullptr;
}

// 終了関数
void
BnNodeList_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<BnNodeListObject*>(self);
  node_obj->mNodeList.~BnNodeList();
  Py_TYPE(self)->tp_free(self);
}

// メソッド定義
PyMethodDef BnNodeList_methods[] = {
  {nullptr, nullptr, 0, nullptr}
};

// getsetter 定義
PyGetSetDef BnNodeList_getsetters[] = {
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

Py_ssize_t
BnNodeList_length(
  PyObject* self
)
{
  auto& val = PyBnNodeList::_get_ref(self);
  return val.size();
}

PyObject*
BnNodeList_item(
  PyObject* self,
  Py_ssize_t index
)
{
  auto& val = PyBnNodeList::_get_ref(self);
  long index1 = ( index >= 0 ) ? index : val.size() + index;
  auto ans = val[index1];
  return PyBnNode::ToPyObject(ans);
}

// シーケンス型のメソッド
PySequenceMethods BnNodeList_sequence = {
  .sq_length = BnNodeList_length,
  .sq_item = BnNodeList_item
};

BEGIN_NONAMESPACE

bool
check_node_list(
  PyObject* obj
)
{
  if ( !PySequence_Check(obj) ) {
    return false;
  }
  SizeType n = PySequence_Size(obj);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(obj, i);
    if ( !PyBnNode::Check(obj1) ) {
      return false;
    }
  }
  return true;
}

vector<BnNode>
get_node_list(
  PyObject* obj
)
{
  SizeType n = PySequence_Size(obj);
  vector<BnNode> ans_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(obj, i);
    auto& node = PyBnNode::_get_ref(obj1);
    ans_list[i] = node;
  }
  return ans_list;
}

END_NONAMESPACE

// 比較関数
PyObject*
BnNodeList_richcompfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBnNodeList::Check(self) ) {
    auto& val1 = PyBnNodeList::_get_ref(self);
    if ( PyBnNodeList::Check(other) ) {
      auto& val2 = PyBnNodeList::_get_ref(other);
      if ( op == Py_EQ ) {
	return PyBool_FromLong(val1 == val2);
      }
      if ( op == Py_NE ) {
	return PyBool_FromLong(val1 != val2);
      }
    }
    else if ( check_node_list(other) ) {
      auto val2 = get_node_list(other);
      if ( op == Py_EQ ) {
	return PyBool_FromLong(val1 == val2);
      }
      if ( op == Py_NE ) {
	return PyBool_FromLong(val1 != val2);
      }
    }
  }
  else if ( check_node_list(self) ) {
    auto val1 = get_node_list(self);
    if ( PyBnNodeList::Check(other) ) {
      auto& val2 = PyBnNodeList::_get_ref(other);
      if ( op == Py_EQ ) {
	return PyBool_FromLong(val1 == val2);
      }
      if ( op == Py_NE ) {
	return PyBool_FromLong(val1 != val2);
      }
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

END_NONAMESPACE


// @brief 'BnNodeList' オブジェクトを使用可能にする．
bool
PyBnNodeList::init(
  PyObject* m
)
{
  BnNodeList_Type.tp_name = "BnNodeList";
  BnNodeList_Type.tp_basicsize = sizeof(BnNodeListObject);
  BnNodeList_Type.tp_itemsize = 0;
  BnNodeList_Type.tp_dealloc = BnNodeList_dealloc;
  BnNodeList_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnNodeList_Type.tp_doc = PyDoc_STR("BnNodeList object");
  BnNodeList_Type.tp_richcompare = BnNodeList_richcompfunc;
  BnNodeList_Type.tp_methods = BnNodeList_methods;
  BnNodeList_Type.tp_getset = BnNodeList_getsetters;
  BnNodeList_Type.tp_new = BnNodeList_new;
  BnNodeList_Type.tp_as_sequence = &BnNodeList_sequence;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnNodeList", &BnNodeList_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnNodeList を PyObject に変換する．
PyObject*
PyBnNodeList::Conv::operator()(
  const BnNodeList& node_list
)
{
  auto obj = BnNodeList_Type.tp_alloc(&BnNodeList_Type, 0);
  auto node_obj = reinterpret_cast<BnNodeListObject*>(obj);
  new (&node_obj->mNodeList) BnNodeList{node_list};
  return obj;
}

// @brief PyObject* から BnNodeList を取り出す．
bool
PyBnNodeList::Deconv::operator()(
  PyObject* obj,
  BnNodeList& val
)
{
  if ( PyBnNodeList::Check(obj) ) {
    val = PyBnNodeList::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnNodeList タイプか調べる．
bool
PyBnNodeList::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnNodeList を表す PyObject から BnNodeList を取り出す．
BnNodeList&
PyBnNodeList::_get_ref(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BnNodeListObject*>(obj);
  return node_obj->mNodeList;
}

// @brief BnNodeList を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNodeList::_typeobject()
{
  return &BnNodeList_Type;
}

END_NAMESPACE_YM
