
/// @file PyBnSeq.cc
/// @brief Python BnSeq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnSeq.h"
#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyModule.h"
#include "ym/BnSeq.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnSeqObject
{
  PyObject_HEAD
  BnSeq* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BnSeq_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnSeq_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnSeq' is disabled");
  return nullptr;
}

// 終了関数
void
BnSeq_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<BnSeqObject*>(self);
  delete node_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnSeq_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnSeq::Get(self);
  auto r = dff.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BnSeq_parent_model(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnSeq::Get(self);
  auto val = dff.parent_model();
  return PyBnModel::ToPyObject(val);
}

PyObject*
BnSeq_is_dff(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnSeq::Get(self);
  auto r = dff.is_dff();
  return PyBool_FromLong(r);
}

PyObject*
BnSeq_is_latch(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnSeq::Get(self);
  auto r = dff.is_latch();
  return PyBool_FromLong(r);
}

PyObject*
BnSeq_is_cell(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto dff = PyBnSeq::Get(self);
  auto r = dff.is_latch();
  return PyBool_FromLong(r);
}

PyObject*
BnSeq_cell_pin(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto dff = PyBnSeq::Get(self);
    auto ans = dff.cell_pin(pos);
    return PyBnNode::ToPyObject(ans);
  }
  catch ( std::invalid_argument ) {
    ostringstream buf;
    buf << "Error occured in BnSeq::cell_pin("
	<< pos << ").";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BnSeq_methods[] = {
  {"is_valid", BnSeq_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"parent_model", BnSeq_parent_model, METH_NOARGS,
   PyDoc_STR("return parent model")},
  {"is_dff", BnSeq_is_dff, METH_NOARGS,
   PyDoc_STR("return True if D-FF type")},
  {"is_latch", BnSeq_is_latch, METH_NOARGS,
   PyDoc_STR("return True if latch type")},
  {"is_cell", BnSeq_is_cell, METH_NOARGS,
   PyDoc_STR("return True if cell type")},
  {"cell_pin", BnSeq_cell_pin, METH_VARARGS,
   PyDoc_STR("return pin node")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnSeq_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnSeq::Get(self);
  auto id = dff.id();
  return PyLong_FromLong(id);
}

PyObject*
BnSeq_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnSeq::Get(self);
  auto model = dff.parent_model();
  auto name = model.seq_name(dff.id());
  return Py_BuildValue("s", name.c_str());
}

PyObject*
BnSeq_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto dff = PyBnSeq::Get(self);
  auto type = dff.type();
  ostringstream buf;
  buf << type;
  return Py_BuildValue("s", buf.str().c_str());
}

PyObject*
BnSeq_data_src(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.data_src();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.data_src");
    return nullptr;
  }
}

PyObject*
BnSeq_clock(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.clock();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.clock");
    return nullptr;
  }
}

PyObject*
BnSeq_enable(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.enable();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.enable");
    return nullptr;
  }
}

PyObject*
BnSeq_clear(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.clear();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.clear");
    return nullptr;
  }
}

PyObject*
BnSeq_preset(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.preset();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.preset");
    return nullptr;
  }
}

PyObject*
BnSeq_data_output(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.data_output();
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.data_output");
    return nullptr;
  }
}

PyObject*
BnSeq_cell_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.cell_id();
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.cell_id");
    return nullptr;
  }
}

PyObject*
BnSeq_rsval(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto dff = PyBnSeq::Get(self);
    auto val = dff.rsval();
    char tmp[2];
    tmp[0] = val;
    tmp[1] = '\0';
    return Py_BuildValue("s", tmp);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "Error in BnSeq.rsval");
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BnSeq_getsetters[] = {
  {"id", BnSeq_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"name", BnSeq_name, nullptr,
   PyDoc_STR("name"), nullptr},
  {"data_src_id", BnSeq_data_src, nullptr,
   PyDoc_STR("data src node"), nullptr},
  {"clock", BnSeq_clock, nullptr,
   PyDoc_STR("clock node"), nullptr},
  {"clear", BnSeq_clear, nullptr,
   PyDoc_STR("clear node"), nullptr},
  {"preset", BnSeq_preset, nullptr,
   PyDoc_STR("preset node"), nullptr},
  {"data_output", BnSeq_data_output, nullptr,
   PyDoc_STR("data output node"), nullptr},
  {"cell_id", BnSeq_cell_id, nullptr,
   PyDoc_STR("Cell ID"), nullptr},
  {"rsval", BnSeq_rsval, nullptr,
   PyDoc_STR("reset/set value"), nullptr},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'BnSeq' オブジェクトを使用可能にする．
bool
PyBnSeq::init(
  PyObject* m
)
{
  BnSeq_Type.tp_name = "BnSeq";
  BnSeq_Type.tp_basicsize = sizeof(BnSeqObject);
  BnSeq_Type.tp_itemsize = 0;
  BnSeq_Type.tp_dealloc = BnSeq_dealloc;
  BnSeq_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnSeq_Type.tp_doc = PyDoc_STR("BnSeq object");
  BnSeq_Type.tp_methods = BnSeq_methods;
  BnSeq_Type.tp_getset = BnSeq_getsetters;
  BnSeq_Type.tp_new = BnSeq_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnSeq", &BnSeq_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnSeq を PyObject に変換する．
PyObject*
PyBnSeq::ToPyObject(
  BnSeq node
)
{
  auto obj = BnSeq_Type.tp_alloc(&BnSeq_Type, 0);
  auto node_obj = reinterpret_cast<BnSeqObject*>(obj);
  node_obj->mPtr = new BnSeq{node};
  return obj;
}

// @brief BnSeq のリストを表す PyObject を作る．
PyObject*
PyBnSeq::ToPyList(
  const vector<BnSeq>& val_list
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

// @brief PyObject が BnSeq タイプか調べる．
bool
PyBnSeq::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnSeq を表す PyObject から BnSeq を取り出す．
BnSeq
PyBnSeq::Get(
  PyObject* obj
)
{
  auto node_obj = reinterpret_cast<BnSeqObject*>(obj);
  return *node_obj->mPtr;
}

// @brief BnSeq を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnSeq::_typeobject()
{
  return &BnSeq_Type;
}

END_NAMESPACE_YM
