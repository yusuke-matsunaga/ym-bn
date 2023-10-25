
/// @file PyBnDff.cc
/// @brief Python BnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnDff.h"
#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyModule.h"
#include "ym/BnDff.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnDffObject
{
  PyObject_HEAD
  BnDff* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BnDff_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnDff_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnDff' is disabled");
  return nullptr;
}

// 終了関数
void
BnDff_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<BnDffObject*>(self);
  delete node_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnDff_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnDff::Get(self);
  auto r = dff.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BnDff_parent_model(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnDff::Get(self);
  auto val = dff.parent_model();
  return PyBnModel::ToPyObject(val);
}

PyObject*
BnDff_is_dff(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnDff::Get(self);
  auto r = dff.is_dff();
  return PyBool_FromLong(r);
}

PyObject*
BnDff_is_latch(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnDff::Get(self);
  auto r = dff.is_latch();
  return PyBool_FromLong(r);
}

PyObject*
BnDff_is_cell(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnDff::Get(self);
  auto r = dff.is_latch();
  return PyBool_FromLong(r);
}

PyObject*
BnDff_cell_pin(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto dff = PyBnDff::Get(self);
    auto ans = dff.cell_pin(pos);
    return PyBnNode::ToPyObject(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BnDff::cell_pin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BnDff_methods[] = {
  {"is_valid", BnDff_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"parent_model", BnDff_parent_model, METH_NOARGS,
   PyDoc_STR("return parent model")},
  {"is_dff", BnDff_is_dff, METH_NOARGS,
   PyDoc_STR("return True if D-FF type")},
  {"is_latch", BnDff_is_latch, METH_NOARGS,
   PyDoc_STR("return True if latch type")},
  {"is_cell", BnDff_is_cell, METH_NOARGS,
   PyDoc_STR("return True if cell type")},
  {"cell_pin", BnDff_cell_pin, METH_VARARGS,
   PyDoc_STR("return pin node")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnDff_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnDff::Get(self);
  auto id = dff.id();
  return PyLong_FromLong(id);
}

PyObject*
BnDff_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnDff::Get(self);
  auto name = dff.name();
  return Py_BuildValue("s", name.c_str());
}

PyObject*
BnDff_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnDff::Get(self);
  auto type = dff.type();
  ostringstream buf;
  buf << type;
  return Py_BuildValue("s", buf.str().c_str());
}

PyObject*
BnDff_data_src(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.data_src();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.data_src");
    return nullptr;
  }
}

PyObject*
BnDff_clock(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.clock();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.clock");
    return nullptr;
  }
}

PyObject*
BnDff_enable(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.enable();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.enable");
    return nullptr;
  }
}

PyObject*
BnDff_clear(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.clear();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.clear");
    return nullptr;
  }
}

PyObject*
BnDff_preset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.preset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.preset");
    return nullptr;
  }
}

PyObject*
BnDff_data_output(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.data_output();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.data_output");
    return nullptr;
  }
}

PyObject*
BnDff_cell_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.cell_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.cell_id");
    return nullptr;
  }
}

PyObject*
BnDff_rsval(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnDff::Get(self);
    auto val = dff.rsval();
    char tmp[2];
    tmp[0] = val;
    tmp[1] = '\0';
    return Py_BuildValue("s", tmp);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnDff.rsval");
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BnDff_getsetters[] = {
  {"id", BnDff_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"name", BnDff_name, nullptr,
   PyDoc_STR("name"), nullptr},
  {"data_src_id", BnDff_data_src, nullptr,
   PyDoc_STR("data src node"), nullptr},
  {"clock", BnDff_clock, nullptr,
   PyDoc_STR("clock node"), nullptr},
  {"clear", BnDff_clear, nullptr,
   PyDoc_STR("clear node"), nullptr},
  {"preset", BnDff_preset, nullptr,
   PyDoc_STR("preset node"), nullptr},
  {"data_output", BnDff_data_output, nullptr,
   PyDoc_STR("data output node"), nullptr},
  {"cell_id", BnDff_cell_id, nullptr,
   PyDoc_STR("Cell ID"), nullptr},
  {"rsval", BnDff_rsval, nullptr,
   PyDoc_STR("reset/set value"), nullptr},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'BnDff' オブジェクトを使用可能にする．
bool
PyBnDff::init(
  PyObject* m
)
{
  BnDff_Type.tp_name = "BnDff";
  BnDff_Type.tp_basicsize = sizeof(BnDffObject);
  BnDff_Type.tp_itemsize = 0;
  BnDff_Type.tp_dealloc = BnDff_dealloc;
  BnDff_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnDff_Type.tp_doc = PyDoc_STR("BnDff object");
  BnDff_Type.tp_methods = BnDff_methods;
  BnDff_Type.tp_getset = BnDff_getsetters;
  BnDff_Type.tp_new = BnDff_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnDff", &BnDff_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnDff を PyObject に変換する．
PyObject*
PyBnDff::ToPyObject(
  BnDff node
)
{
  auto obj = BnDff_Type.tp_alloc(&BnDff_Type, 0);
  auto node_obj = reinterpret_cast<BnDffObject*>(obj);
  node_obj->mPtr = new BnDff{node};
  return obj;
}

// @brief BnDff のリストを表す PyObject を作る．
PyObject*
PyBnDff::ToPyList(
  const vector<BnDff>& val_list
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

// @brief PyObject が BnDff タイプか調べる．
bool
PyBnDff::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnDff を表す PyObject から BnDff を取り出す．
BnDff
PyBnDff::Get(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BnDffObject*>(obj);
  return *node_obj->mPtr;
}

// @brief BnDff を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnDff::_typeobject()
{
  return &BnDff_Type;
}

END_NAMESPACE_YM
