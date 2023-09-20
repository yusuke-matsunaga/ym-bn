
/// @file PyBfModel.cc
/// @brief Python BfModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBfModel.h"
#include "pym/PyBfNode.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyModule.h"
#include "pym/PyBase.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BfModelObject
{
  PyObject_HEAD
  BfModel* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BfModelType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BfModel_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BfModel' is disabled");
  return nullptr;
}

// 終了関数
void
BfModel_dealloc(
  PyObject* self
)
{
  auto model_obj = reinterpret_cast<BfModelObject*>(self);
  delete model_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BfModel_read_blif(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    "cell_library",
    nullptr
  };
  const char* filename = nullptr;
  PyObject* lib_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O!",
				    const_cast<char**>(kwlist),
				    &filename,
				    PyClibCellLibrary::_typeobject(), &lib_obj) ) {
    return nullptr;
  }
  ClibCellLibrary cell_library;
  if ( lib_obj != nullptr ) {
    cell_library = PyClibCellLibrary::Get(lib_obj);
  }
  try {
    auto model = BfModel::read_blif(filename, cell_library);
    return PyBfModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_blif(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_read_iscas89(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BfModel::read_iscas89(filename);
    return PyBfModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_read_aag(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BfModel::read_aag(filename);
    return PyBfModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_read_aig(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BfModel::read_aig(filename);
    return PyBfModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_input(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.input(pos);
    return PyBfNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "input(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_input_name(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.input_name(pos);
    return Py_BuildValue("s", val.c_str());
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "input_name(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_output(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.output(pos);
    return PyBfNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "output(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_output_name(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.output_name(pos);
    return Py_BuildValue("s", val.c_str());
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "output_name(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_logic(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.logic(pos);
    return PyBfNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "logic(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_dff(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBfModel::Get(self);
    auto val = model.dff(pos);
    return PyBfNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "dff(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BfModel_print(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "file",
    nullptr
  };
  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  auto model = PyBfModel::Get(self);
  if ( filename == nullptr ) {
    model.print(cout);
  }
  else {
    ofstream fout{filename};
    if ( fout ) {
      model.print(fout);
    }
    else {
      ostringstream buff;
      buff << filename << ": Could not open file";
      PyErr_SetString(PyExc_ValueError, buff.str().c_str());
      return nullptr;
    }
  }
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef BfModel_methods[] = {
  {"read_blif", reinterpret_cast<PyCFunction>(BfModel_read_blif),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'blif' file")},
  {"read_iscas89", reinterpret_cast<PyCFunction>(BfModel_read_iscas89),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'iscas89(.bench)' file")},
  {"read_aag", reinterpret_cast<PyCFunction>(BfModel_read_aag),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aag' file")},
  {"read_aig", reinterpret_cast<PyCFunction>(BfModel_read_aig),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aig' file")},
  {"input", BfModel_input,
   METH_VARARGS,
   PyDoc_STR("returns input node")},
  {"input_name", BfModel_input_name,
   METH_VARARGS,
   PyDoc_STR("returns input name")},
  {"output", BfModel_output,
   METH_VARARGS,
   PyDoc_STR("returns output node")},
  {"output_name", BfModel_output_name,
   METH_VARARGS,
   PyDoc_STR("returns output name")},
  {"dff", BfModel_dff,
   METH_VARARGS,
   PyDoc_STR("returns DFF node")},
  {"logic", BfModel_logic,
   METH_VARARGS,
   PyDoc_STR("returns logic node")},
  {"print", reinterpret_cast<PyCFunction>(BfModel_print),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("print contents")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BfModel_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.name();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BfModel_comment(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.comment();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BfModel_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.input_num();
  return Py_BuildValue("i", val);
}

PyObject*
BfModel_input_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.input_list();
  return PyBfNode::ToPyList(val_list);
}

PyObject*
BfModel_input_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.input_name_list();
  return PyBase::ToPyList(val_list);
}

PyObject*
BfModel_dff_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.dff_num();
  return Py_BuildValue("i", val);
}

PyObject*
BfModel_dff_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.dff_list();
  return PyBfNode::ToPyList(val_list);
}

PyObject*
BfModel_output_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.output_num();
  return Py_BuildValue("i", val);
}

PyObject*
BfModel_output_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.output_list();
  return PyBfNode::ToPyList(val_list);
}

PyObject*
BfModel_output_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.output_name_list();
  return PyBase::ToPyList(val_list);
}

PyObject*
BfModel_logic_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.logic_num();
  return Py_BuildValue("i", val);
}

PyObject*
BfModel_logic_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val_list = model.logic_list();
  return PyBfNode::ToPyList(val_list);
}

PyObject*
BfModel_cover_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.cover_num();
  return Py_BuildValue("i", val);
}

PyObject*
BfModel_expr_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBfModel::Get(self);
  auto val = model.expr_num();
  return Py_BuildValue("i", val);
}

// getsetter 定義
PyGetSetDef BfModel_getsetters[] = {
  {"name", BfModel_name, nullptr, PyDoc_STR("name"), nullptr},
  {"comment", BfModel_comment, nullptr, PyDoc_STR("comment"), nullptr},
  {"input_num", BfModel_input_num, nullptr, PyDoc_STR("input num"), nullptr},
  {"input_list", BfModel_input_list, nullptr, PyDoc_STR("input list"), nullptr},
  {"input_name_list", BfModel_input_name_list, nullptr, PyDoc_STR("input list"), nullptr},
  {"dff_num", BfModel_dff_num, nullptr, PyDoc_STR("DFF num"), nullptr},
  {"dff_list", BfModel_dff_list, nullptr, PyDoc_STR("DFF list"), nullptr},
  {"output_num", BfModel_output_num, nullptr, PyDoc_STR("output num"), nullptr},
  {"output_list", BfModel_output_list, nullptr, PyDoc_STR("output list"), nullptr},
  {"output_name_list", BfModel_output_name_list, nullptr, PyDoc_STR("output list"), nullptr},
  {"logic_num", BfModel_logic_num, nullptr, PyDoc_STR("logic gate num"), nullptr},
  {"logic_list", BfModel_logic_list, nullptr, PyDoc_STR("logic gate list"), nullptr},
  {"cover_num", BfModel_cover_num, nullptr, PyDoc_STR("cover num"), nullptr},
  {"expr_num", BfModel_expr_num, nullptr, PyDoc_STR("expr num"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'BfModel' オブジェクトを使用可能にする．
bool
PyBfModel::init(
  PyObject* m
)
{
  BfModelType.tp_name = "BfModel";
  BfModelType.tp_basicsize = sizeof(BfModelObject);
  BfModelType.tp_itemsize = 0;
  BfModelType.tp_dealloc = BfModel_dealloc;
  BfModelType.tp_flags = Py_TPFLAGS_DEFAULT;
  BfModelType.tp_doc = PyDoc_STR("BfModel object");
  BfModelType.tp_methods = BfModel_methods;
  BfModelType.tp_getset = BfModel_getsetters;
  BfModelType.tp_new = BfModel_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BfModel", &BfModelType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BfModel を PyObject に変換する．
PyObject*
PyBfModel::ToPyObject(
  const BfModel& val
)
{
  auto obj = BfModelType.tp_alloc(&BfModelType, 0);
  auto model_obj = reinterpret_cast<BfModelObject*>(obj);
  model_obj->mPtr = new BfModel{val};
  return obj;
}

// @brief PyObject が BfModel タイプか調べる．
bool
PyBfModel::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BfModel を表す PyObject から BfModel を取り出す．
BfModel
PyBfModel::Get(
  PyObject* obj
)
{
  auto model_obj = reinterpret_cast<BfModelObject*>(obj);
  return *model_obj->mPtr;
}

// @brief BfModel を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBfModel::_typeobject()
{
  return &BfModelType;
}

END_NAMESPACE_YM
