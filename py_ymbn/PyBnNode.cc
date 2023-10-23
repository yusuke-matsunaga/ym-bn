
/// @file PyBnNode.cc
/// @brief Python BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNode.h"
#include "pym/PyBnModel.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBdd.h"
#include "pym/PyPrimType.h"
#include "pym/PyModule.h"
#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnNodeObject
{
  PyObject_HEAD
  BnNode* mPtr;
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
  delete node_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnNode_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_parent_model(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto val = node.parent_model();
  return PyBnModel::ToPyObject(val);
}

PyObject*
BnNode_is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_input();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_logic(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_logic();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_primitive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_primitive();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_aig(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_aig();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_cover(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_cover();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_expr();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_cell(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_cell();
  return PyBool_FromLong(r);
}

PyObject*
BnNode_is_dff(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBnNode::Get(self);
  auto r = node.is_dff();
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
    auto node = PyBnNode::Get(self);
    auto ans = node.fanin(pos);
    return PyBnNode::ToPyObject(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BnNode::fanin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
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
    auto node = PyBnNode::Get(self);
    auto ans = node.fanin_inv(pos);
    return PyBool_FromLong(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BnNode::fanin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BnNode_methods[] = {
  {"is_valid", BnNode_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"parent_model", BnNode_parent_model, METH_NOARGS,
   PyDoc_STR("return parent model")},
  {"is_input", BnNode_is_input, METH_NOARGS,
   PyDoc_STR("return True if input")},
  {"is_logic", BnNode_is_logic, METH_NOARGS,
   PyDoc_STR("return True if logic node")},
  {"is_primitive", BnNode_is_primitive, METH_NOARGS,
   PyDoc_STR("return True if primitive node")},
  {"is_aig", BnNode_is_aig, METH_NOARGS,
   PyDoc_STR("return True if aig node")},
  {"is_cover", BnNode_is_cover, METH_NOARGS,
   PyDoc_STR("return True if cover node")},
  {"is_expr", BnNode_is_expr, METH_NOARGS,
   PyDoc_STR("return True if expr node")},
  {"is_cell", BnNode_is_cell, METH_NOARGS,
   PyDoc_STR("return True if cell node")},
  {"is_dff", BnNode_is_dff, METH_NOARGS,
   PyDoc_STR("return True if dff node")},
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
  auto node = PyBnNode::Get(self);
  auto id = node.id();
  return PyLong_FromLong(id);
}

PyObject*
BnNode_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto node = PyBnNode::Get(self);
  auto name = node.name();
  return Py_BuildValue("s", name.c_str());
}

PyObject*
BnNode_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto node = PyBnNode::Get(self);
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
    auto node = PyBnNode::Get(self);
    auto val = node.input_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.input_id");
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
    auto node = PyBnNode::Get(self);
    auto val = node.fanin_num();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.fanin_num");
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
    auto node = PyBnNode::Get(self);
    auto fanin_list = node.fanin_list();
    auto n = fanin_list.size();
    auto obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto inode= fanin_list[i];
      auto inode_obj = PyBnNode::ToPyObject(inode);
      PyList_SET_ITEM(obj, i, inode_obj);
    }
    return obj;
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.fanin_list");
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
    auto node = PyBnNode::Get(self);
    auto val = node.primitive_type();
    return PyPrimType::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.primitive_type");
    return nullptr;
  }
}

PyObject*
BnNode_cover_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.cover_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.cover_id");
    return nullptr;
  }
}

PyObject*
BnNode_expr_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.expr_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.expr_id");
    return nullptr;
  }
}

PyObject*
BnNode_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.expr();
    return PyExpr::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.expr");
    return nullptr;
  }
}

PyObject*
BnNode_cell_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.cell_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.cell_id");
    return nullptr;
  }
}

PyObject*
BnNode_func_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.func_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.func_id");
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
    auto node = PyBnNode::Get(self);
    auto& val = node.func();
    return PyTvFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.func");
    return nullptr;
  }
}

PyObject*
BnNode_bdd_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.bdd_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.bdd_id");
    return nullptr;
  }
}

PyObject*
BnNode_bdd(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.bdd();
    return PyBdd::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.bdd");
    return nullptr;
  }
}

PyObject*
BnNode_dff_src(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.dff_src();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.dff_src");
    return nullptr;
  }
}

PyObject*
BnNode_dff_clock(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.dff_clock();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.dff_clock");
    return nullptr;
  }
}

PyObject*
BnNode_dff_reset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.dff_reset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.dff_reset");
    return nullptr;
  }
}

PyObject*
BnNode_dff_preset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.dff_preset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.dff_preset");
    return nullptr;
  }
}

PyObject*
BnNode_dff_rsval(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.dff_rsval();
    char tmp[2];
    tmp[0] = val;
    tmp[1] = '\0';
    return Py_BuildValue("s", tmp);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.rval");
    return nullptr;
  }
}

PyObject*
BnNode_latch_src(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.latch_src();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.latch_src");
    return nullptr;
  }
}

PyObject*
BnNode_latch_enable(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.latch_enable();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.latch_enable");
    return nullptr;
  }
}

PyObject*
BnNode_latch_reset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.latch_reset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.latch_reset");
    return nullptr;
  }
}

PyObject*
BnNode_latch_preset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.latch_preset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.latch_preset");
    return nullptr;
  }
}

PyObject*
BnNode_latch_rsval(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBnNode::Get(self);
    auto val = node.latch_rsval();
    char tmp[2];
    tmp[0] = val;
    tmp[1] = '\0';
    return Py_BuildValue("s", tmp);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnNode.rval");
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BnNode_getsetters[] = {
  {"id", BnNode_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"name", BnNode_name, nullptr,
   PyDoc_STR("name"), nullptr},
  {"input_id", BnNode_input_id, nullptr,
   PyDoc_STR("input ID"), nullptr},
  {"fanin_num", BnNode_fanin_num, nullptr,
   PyDoc_STR("fanin num"), nullptr},
  {"fanin_list", BnNode_fanin_list, nullptr,
   PyDoc_STR("fanin list"), nullptr},
  {"primitive_type", BnNode_primitive_type, nullptr,
   PyDoc_STR("primitive type"), nullptr},
  {"cover_id", BnNode_cover_id, nullptr,
   PyDoc_STR("cover ID"), nullptr},
  {"expr_id", BnNode_expr_id, nullptr,
   PyDoc_STR("expr ID"), nullptr},
  {"expr", BnNode_expr, nullptr,
   PyDoc_STR("expr"), nullptr},
  {"cell_id", BnNode_cell_id, nullptr,
   PyDoc_STR("Cell ID"), nullptr},
  {"func_id", BnNode_func_id, nullptr,
   PyDoc_STR("function ID"), nullptr},
  {"bdd_id", BnNode_bdd_id, nullptr,
   PyDoc_STR("BDD ID"), nullptr},
  {"dff_src", BnNode_dff_src, nullptr,
   PyDoc_STR("DFF source"), nullptr},
  {"dff_clock", BnNode_dff_clock, nullptr,
   PyDoc_STR("DFF clock"), nullptr},
  {"dff_reset", BnNode_dff_reset, nullptr,
   PyDoc_STR("DFF reset"), nullptr},
  {"dff_preset", BnNode_dff_preset, nullptr,
   PyDoc_STR("DFF preset"), nullptr},
  {"dff_rsval", BnNode_dff_rsval, nullptr,
   PyDoc_STR("DFF reset/set value"), nullptr},
  {"latch_src", BnNode_latch_src, nullptr,
   PyDoc_STR("latch source"), nullptr},
  {"latch_enable", BnNode_latch_enable, nullptr,
   PyDoc_STR("latch enable"), nullptr},
  {"latch_reset", BnNode_latch_reset, nullptr,
   PyDoc_STR("latch reset"), nullptr},
  {"latch_preset", BnNode_latch_preset, nullptr,
   PyDoc_STR("latch preset"), nullptr},
  {"latch_rsval", BnNode_latch_rsval, nullptr,
   PyDoc_STR("latch reset/set value"), nullptr},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

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
PyBnNode::ToPyObject(
  BnNode node
)
{
  auto obj = BnNode_Type.tp_alloc(&BnNode_Type, 0);
  auto node_obj = reinterpret_cast<BnNodeObject*>(obj);
  node_obj->mPtr = new BnNode{node};
  return obj;
}

// @brief BnNode のリストを表す PyObject を作る．
PyObject*
PyBnNode::ToPyList(
  const vector<BnNode>& val_list
)
{
  SizeType n = val_list.size();
  auto obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& node = val_list[i];
    auto node_obj = ToPyObject(node);
    PyList_SetItem(obj, i, node_obj);
  }
  return obj;
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
BnNode
PyBnNode::Get(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BnNodeObject*>(obj);
  return *node_obj->mPtr;
}

// @brief BnNode を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNode::_typeobject()
{
  return &BnNode_Type;
}

END_NAMESPACE_YM
