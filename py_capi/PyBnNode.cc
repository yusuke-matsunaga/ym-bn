
/// @file PyBnNode.cc
/// @brief Python BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNode.h"
#include "pym/PyBnNodeList.h"
#include "pym/PyBnModel.h"
#include "pym/PyBnSeq.h"
#include "pym/PyBnFunc.h"
#include "pym/PyPrimType.h"
#include "pym/PyClibCell.h"
#include "pym/PyModule.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnNodeObject
{
  PyObject_HEAD
  BnNode mNode;
};

// Python 用のタイプ定義
PyTypeObject BnNode_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnNode_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnNode' is disabled");
  return nullptr;
}

// 終了関数
void
BnNode_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<BnNodeObject*>(self);
  node_obj->mNode.~BnNode();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnNode_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_input();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_seq_output(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_seq_output();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_logic(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_logic();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_primitive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_primitive();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_aig(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_aig();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_func(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_func();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_cell(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto r = node.is_cell();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_fanin(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto ans = node.fanin(pos);
    return PyBnNode::ToPyObject(ans);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnNode_fanin_inv(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto ans = node.fanin_inv(pos);
    return PyBool_FromLong(ans);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BnNode_methods[] = {
  {"is_valid", BnNode_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"is_input", BnNode_is_input, METH_NOARGS,
   PyDoc_STR("return True if input")},
  {"is_seq_output", BnNode_is_seq_output, METH_NOARGS,
   PyDoc_STR("return True if SEQ output")},
  {"is_logic", BnNode_is_logic, METH_NOARGS,
   PyDoc_STR("return True if logic node")},
  {"is_primitive", BnNode_is_primitive, METH_NOARGS,
   PyDoc_STR("return True if primitive node")},
  {"is_aig", BnNode_is_aig, METH_NOARGS,
   PyDoc_STR("return True if aig node")},
  {"is_func", BnNode_is_func, METH_NOARGS,
   PyDoc_STR("return True if TvFunc node")},
  {"is_cell", BnNode_is_cell, METH_NOARGS,
   PyDoc_STR("return True if cell node")},
  {"fanin", BnNode_fanin, METH_VARARGS,
   PyDoc_STR("return fanin node")},
  {"fanin_inv", BnNode_fanin_inv, METH_VARARGS,
   PyDoc_STR("return 'inv' attribute of specified fanin")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnNode_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto id = node.id();
  return PyLong_FromLong(id);
}

PyObject*
BnNode_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& node = PyBnNode::_get_ref(self);
  auto type = node.type();
  ostringstream buf;
  buf << type;
  return Py_BuildValue("s", buf.str().c_str());
}

PyObject*
BnNode_input_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto val = node.input_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what() );
    return nullptr;
  }
}

PyObject*
BnNode_seq_node(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto val = node.seq_node();
    return PyBnSeq::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnNode_fanin_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto val = node.fanin_num();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnNode_fanin_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto fanin_list = node.fanin_list();
    return PyBnNodeList::ToPyObject(fanin_list);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }

}

PyObject*
BnNode_primitive_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto val = node.primitive_type();
    return PyPrimType::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnNode_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    const auto& val = node.local_func();
    return PyBnFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnNode_cell(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& node = PyBnNode::_get_ref(self);
    auto val = node.cell();
    return PyClibCell::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BnNode_getsetters[] = {
  {"id", BnNode_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"input_id", BnNode_input_id, nullptr,
   PyDoc_STR("input ID"), nullptr},
  {"seq_node", BnNode_seq_node, nullptr,
   PyDoc_STR("SEQ node"), nullptr},
  {"fanin_num", BnNode_fanin_num, nullptr,
   PyDoc_STR("fanin num"), nullptr},
  {"fanin_list", BnNode_fanin_list, nullptr,
   PyDoc_STR("fanin list"), nullptr},
  {"primitive_type", BnNode_primitive_type, nullptr,
   PyDoc_STR("primitive type"), nullptr},
  {"local_func", BnNode_func, nullptr,
   PyDoc_STR("local function"), nullptr},
  {"cell", BnNode_cell, nullptr,
   PyDoc_STR("Cell"), nullptr},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

// 比較関数
PyObject*
BnNode_richcompfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBnNode::Check(self) &&
       PyBnNode::Check(other) ) {
    auto& val1 = PyBnNode::_get_ref(self);
    auto& val2 = PyBnNode::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

END_NONAMESPACE


// @brief 'BnNode' オブジェクトを使用可能にする．
bool
PyBnNode::init(
  PyObject* m
)
{
  BnNode_Type.tp_name = "BnNode";
  BnNode_Type.tp_basicsize = sizeof(BnNodeObject);
  BnNode_Type.tp_itemsize = 0;
  BnNode_Type.tp_dealloc = BnNode_dealloc;
  BnNode_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnNode_Type.tp_doc = PyDoc_STR("BnNode object");
  BnNode_Type.tp_richcompare = BnNode_richcompfunc;
  BnNode_Type.tp_methods = BnNode_methods;
  BnNode_Type.tp_getset = BnNode_getsetters;
  BnNode_Type.tp_new = BnNode_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnNode", &BnNode_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnNode を PyObject に変換する．
PyObject*
PyBnNode::Conv::operator()(
  const BnNode& node
)
{
  auto obj = BnNode_Type.tp_alloc(&BnNode_Type, 0);
  auto node_obj = reinterpret_cast<BnNodeObject*>(obj);
  new (&node_obj->mNode) BnNode{node};
  return obj;
}

// @brief PyObject* から BnNode を取り出す．
bool
PyBnNode::Deconv::operator()(
  PyObject* obj,
  BnNode& val
)
{
  if ( PyBnNode::Check(obj) ) {
    val = PyBnNode::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnNode タイプか調べる．
bool
PyBnNode::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnNode を表す PyObject から BnNode を取り出す．
BnNode&
PyBnNode::_get_ref(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BnNodeObject*>(obj);
  return node_obj->mNode;
}

// @brief BnNode を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNode::_typeobject()
{
  return &BnNode_Type;
}

END_NAMESPACE_YM
