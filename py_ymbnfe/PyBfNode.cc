
/// @file PyBfNode.cc
/// @brief Python BfNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBfNode.h"
#include "pym/PyBfModel.h"
#include "pym/PyExpr.h"
#include "pym/PyPrimType.h"
#include "pym/PyModule.h"
#include "ym/BfNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BfNodeObject
{
  PyObject_HEAD
  BfNode* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BfNode_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BfNode_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BfNode' is disabled");
  return nullptr;
}

// 終了関数
void
BfNode_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<BfNodeObject*>(self);
  delete node_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BfNode_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_parent_model(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto val = node.parent_model();
  return PyBfModel::ToPyObject(val);
}

PyObject*
BfNode_is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_input();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_logic(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_logic();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_primitive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_primitive();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_aig(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_aig();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_cover(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_cover();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_expr();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_cell(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_cell();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_is_dff(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto node = PyBfNode::Get(self);
  auto r = node.is_dff();
  return PyBool_FromLong(r);
}

PyObject*
BfNode_fanin(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto node = PyBfNode::Get(self);
    auto ans = node.fanin(pos);
    return PyBfNode::ToPyObject(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BfNode::fanin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
BfNode_fanin_inv(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto node = PyBfNode::Get(self);
    auto ans = node.fanin_inv(pos);
    return PyBool_FromLong(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BfNode::fanin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BfNode_methods[] = {
  {"is_valid", BfNode_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"parent_model", BfNode_parent_model, METH_NOARGS,
   PyDoc_STR("return parent model")},
  {"is_input", BfNode_is_input, METH_NOARGS,
   PyDoc_STR("return True if input")},
  {"is_logic", BfNode_is_logic, METH_NOARGS,
   PyDoc_STR("return True if logic node")},
  {"is_primitive", BfNode_is_primitive, METH_NOARGS,
   PyDoc_STR("return True if primitive node")},
  {"is_aig", BfNode_is_aig, METH_NOARGS,
   PyDoc_STR("return True if aig node")},
  {"is_cover", BfNode_is_cover, METH_NOARGS,
   PyDoc_STR("return True if cover node")},
  {"is_expr", BfNode_is_expr, METH_NOARGS,
   PyDoc_STR("return True if expr node")},
  {"is_cell", BfNode_is_cell, METH_NOARGS,
   PyDoc_STR("return True if cell node")},
  {"is_dff", BfNode_is_dff, METH_NOARGS,
   PyDoc_STR("return True if dff node")},
  {"fanin", BfNode_fanin, METH_VARARGS,
   PyDoc_STR("return fanin node")},
  {"fanin_inv", BfNode_fanin_inv, METH_VARARGS,
   PyDoc_STR("return 'inv' attribute of specified fanin")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BfNode_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto node = PyBfNode::Get(self);
  auto id = node.id();
  return PyLong_FromLong(id);
}

PyObject*
BfNode_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto node = PyBfNode::Get(self);
  auto name = node.name();
  return Py_BuildValue("s", name.c_str());
}

PyObject*
BfNode_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto node = PyBfNode::Get(self);
  auto type = node.type();
  ostringstream buf;
  buf << type;
  return Py_BuildValue("s", buf.str().c_str());
}

PyObject*
BfNode_input_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.input_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.input_id");
    return nullptr;
  }
}

PyObject*
BfNode_fanin_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.fanin_num();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.fanin_num");
    return nullptr;
  }
}

PyObject*
BfNode_fanin_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto fanin_list = node.fanin_list();
    auto n = fanin_list.size();
    auto obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto inode= fanin_list[i];
      auto inode_obj = PyBfNode::ToPyObject(inode);
      PyList_SET_ITEM(obj, i, inode_obj);
    }
    return obj;
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.fanin_list");
    return nullptr;
  }

}

PyObject*
BfNode_primitive_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.primitive_type();
    return PyPrimType::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.primitive_type");
    return nullptr;
  }
}

PyObject*
BfNode_cover_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.cover_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.cover_id");
    return nullptr;
  }
}

PyObject*
BfNode_expr_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.expr_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.expr_id");
    return nullptr;
  }
}

PyObject*
BfNode_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.expr();
    return PyExpr::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.expr");
    return nullptr;
  }
}

PyObject*
BfNode_cell_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.cell_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.cell_id");
    return nullptr;
  }
}

PyObject*
BfNode_dff_src(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.dff_src();
    return PyBfNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.dff_src");
    return nullptr;
  }
}

PyObject*
BfNode_dff_rval(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto node = PyBfNode::Get(self);
    auto val = node.dff_rval();
    char tmp[2];
    tmp[0] = val;
    tmp[1] = '\0';
    return Py_BuildValue("s", tmp);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BfNode.rval");
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BfNode_getsetters[] = {
  {"id", BfNode_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"name", BfNode_name, nullptr,
   PyDoc_STR("name"), nullptr},
  {"input_id", BfNode_input_id, nullptr,
   PyDoc_STR("input ID"), nullptr},
  {"fanin_num", BfNode_fanin_num, nullptr,
   PyDoc_STR("fanin num"), nullptr},
  {"fanin_list", BfNode_fanin_list, nullptr,
   PyDoc_STR("fanin list"), nullptr},
  {"primitive_type", BfNode_primitive_type, nullptr,
   PyDoc_STR("primitive type"), nullptr},
  {"cover_id", BfNode_cover_id, nullptr,
   PyDoc_STR("cover ID"), nullptr},
  {"expr_id", BfNode_expr_id, nullptr,
   PyDoc_STR("expr ID"), nullptr},
  {"expr", BfNode_expr, nullptr,
   PyDoc_STR("expr"), nullptr},
  {"cell_id", BfNode_cell_id, nullptr,
   PyDoc_STR("Cell ID"), nullptr},
  {"dff_src", BfNode_dff_src, nullptr,
   PyDoc_STR("DFF source"), nullptr},
  {"dff_rval", BfNode_dff_rval, nullptr,
   PyDoc_STR("DFF reset value")},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'BfNode' オブジェクトを使用可能にする．
bool
PyBfNode::init(
  PyObject* m
)
{
  BfNode_Type.tp_name = "BfNode";
  BfNode_Type.tp_basicsize = sizeof(BfNodeObject);
  BfNode_Type.tp_itemsize = 0;
  BfNode_Type.tp_dealloc = BfNode_dealloc;
  BfNode_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BfNode_Type.tp_doc = PyDoc_STR("BfNode object");
  BfNode_Type.tp_methods = BfNode_methods;
  BfNode_Type.tp_getset = BfNode_getsetters;
  BfNode_Type.tp_new = BfNode_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BfNode", &BfNode_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BfNode を PyObject に変換する．
PyObject*
PyBfNode::ToPyObject(
  BfNode node
)
{
  auto obj = BfNode_Type.tp_alloc(&BfNode_Type, 0);
  auto node_obj = reinterpret_cast<BfNodeObject*>(obj);
  node_obj->mPtr = new BfNode{node};
  return obj;
}

// @brief BfNode のリストを表す PyObject を作る．
PyObject*
PyBfNode::ToPyList(
  const vector<BfNode>& val_list
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

// @brief PyObject が BfNode タイプか調べる．
bool
PyBfNode::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BfNode を表す PyObject から BfNode を取り出す．
BfNode
PyBfNode::Get(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BfNodeObject*>(obj);
  return *node_obj->mPtr;
}

// @brief BfNode を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBfNode::_typeobject()
{
  return &BfNode_Type;
}

END_NAMESPACE_YM
