
/// @file PyBnModel.cc
/// @brief Python BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnSeq.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyModule.h"
#include "pym/PyBase.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnModelObject
{
  PyObject_HEAD
  BnModel* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BnModelType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnModel_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnModel' is disabled");
  return nullptr;
}

// 終了関数
void
BnModel_dealloc(
  PyObject* self
)
{
  auto model_obj = reinterpret_cast<BnModelObject*>(self);
  delete model_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnModel_read_blif(
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
    auto model = BnModel::read_blif(filename, cell_library);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_blif(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_read_iscas89(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_iscas89(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_read_aag(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_aag(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_read_aig(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* filename = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_aig(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "read_iscas89(\"" << filename << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_input(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
    auto val = model.input(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "input(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_input_name(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
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
BnModel_output(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
    auto val = model.output(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "output(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_output_name(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
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
BnModel_logic(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
    auto val = model.logic(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "logic(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_seq_node(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto model = PyBnModel::Get(self);
    auto val = model.seq_node(pos);
    return PyBnSeq::ToPyObject(val);
  }
  catch ( std::invalid_argument ) {
    ostringstream buff;
    buff << "seq_node(\"" << pos << "\") failed";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
}

PyObject*
BnModel_print(
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
  auto model = PyBnModel::Get(self);
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
PyMethodDef BnModel_methods[] = {
  {"read_blif", reinterpret_cast<PyCFunction>(BnModel_read_blif),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'blif' file")},
  {"read_iscas89", reinterpret_cast<PyCFunction>(BnModel_read_iscas89),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'iscas89(.bench)' file")},
  {"read_aag", reinterpret_cast<PyCFunction>(BnModel_read_aag),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aag' file")},
  {"read_aig", reinterpret_cast<PyCFunction>(BnModel_read_aig),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aig' file")},
  {"input", BnModel_input,
   METH_VARARGS,
   PyDoc_STR("returns input node")},
  {"input_name", BnModel_input_name,
   METH_VARARGS,
   PyDoc_STR("returns input name")},
  {"output", BnModel_output,
   METH_VARARGS,
   PyDoc_STR("returns output node")},
  {"output_name", BnModel_output_name,
   METH_VARARGS,
   PyDoc_STR("returns output name")},
  {"seq_node", BnModel_seq_node,
   METH_VARARGS,
   PyDoc_STR("returns SEQ node")},
  {"logic", BnModel_logic,
   METH_VARARGS,
   PyDoc_STR("returns logic node")},
  {"print", reinterpret_cast<PyCFunction>(BnModel_print),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("print contents")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnModel_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.name();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BnModel_comment(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.comment();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BnModel_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.input_num();
  return Py_BuildValue("i", val);
}

PyObject*
BnModel_input_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val_list = model.input_list();
  return PyBnNode::ToPyList(val_list);
}

PyObject*
BnModel_input_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  SizeType n = model.input_num();
  vector<string> val_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    val_list[i] = model.input_name(i);
  }
  return PyBase::ToPyList(val_list);
}

PyObject*
BnModel_seq_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.seq_num();
  return Py_BuildValue("i", val);
}

PyObject*
BnModel_seq_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val_list = model.seq_node_list();
  return PyBnSeq::ToPyList(val_list);
}

PyObject*
BnModel_output_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.output_num();
  return Py_BuildValue("i", val);
}

PyObject*
BnModel_output_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val_list = model.output_list();
  return PyBnNode::ToPyList(val_list);
}

PyObject*
BnModel_output_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  SizeType n = model.output_num();
  vector<string> val_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    val_list[i] = model.output_name(i);
  }
  return PyBase::ToPyList(val_list);
}

PyObject*
BnModel_logic_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.logic_num();
  return Py_BuildValue("i", val);
}

PyObject*
BnModel_logic_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val_list = model.logic_list();
  return PyBnNode::ToPyList(val_list);
}

PyObject*
BnModel_cover_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.cover_num();
  return Py_BuildValue("i", val);
}

PyObject*
BnModel_expr_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto model = PyBnModel::Get(self);
  auto val = model.expr_num();
  return Py_BuildValue("i", val);
}

// getsetter 定義
PyGetSetDef BnModel_getsetters[] = {
  {"name", BnModel_name, nullptr, PyDoc_STR("name"), nullptr},
  {"comment", BnModel_comment, nullptr, PyDoc_STR("comment"), nullptr},
  {"input_num", BnModel_input_num, nullptr, PyDoc_STR("input num"), nullptr},
  {"input_list", BnModel_input_list, nullptr, PyDoc_STR("input list"), nullptr},
  {"input_name_list", BnModel_input_name_list, nullptr, PyDoc_STR("input list"), nullptr},
  {"seq_num", BnModel_seq_num, nullptr, PyDoc_STR("SEQ node num"), nullptr},
  {"seq_node_list", BnModel_seq_list, nullptr, PyDoc_STR("SEQ node list"), nullptr},
  {"output_num", BnModel_output_num, nullptr, PyDoc_STR("output num"), nullptr},
  {"output_list", BnModel_output_list, nullptr, PyDoc_STR("output list"), nullptr},
  {"output_name_list", BnModel_output_name_list, nullptr, PyDoc_STR("output list"), nullptr},
  {"logic_num", BnModel_logic_num, nullptr, PyDoc_STR("logic gate num"), nullptr},
  {"logic_list", BnModel_logic_list, nullptr, PyDoc_STR("logic gate list"), nullptr},
  {"cover_num", BnModel_cover_num, nullptr, PyDoc_STR("cover num"), nullptr},
  {"expr_num", BnModel_expr_num, nullptr, PyDoc_STR("expr num"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief 'BnModel' オブジェクトを使用可能にする．
bool
PyBnModel::init(
  PyObject* m
)
{
  BnModelType.tp_name = "BnModel";
  BnModelType.tp_basicsize = sizeof(BnModelObject);
  BnModelType.tp_itemsize = 0;
  BnModelType.tp_dealloc = BnModel_dealloc;
  BnModelType.tp_flags = Py_TPFLAGS_DEFAULT;
  BnModelType.tp_doc = PyDoc_STR("BnModel object");
  BnModelType.tp_methods = BnModel_methods;
  BnModelType.tp_getset = BnModel_getsetters;
  BnModelType.tp_new = BnModel_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnModel", &BnModelType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnModel を PyObject に変換する．
PyObject*
PyBnModel::ToPyObject(
  const BnModel& val
)
{
  auto obj = BnModelType.tp_alloc(&BnModelType, 0);
  auto model_obj = reinterpret_cast<BnModelObject*>(obj);
  model_obj->mPtr = new BnModel{val};
  return obj;
}

// @brief PyObject が BnModel タイプか調べる．
bool
PyBnModel::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnModel を表す PyObject から BnModel を取り出す．
BnModel
PyBnModel::Get(
  PyObject* obj
)
{
  auto model_obj = reinterpret_cast<BnModelObject*>(obj);
  return *model_obj->mPtr;
}

// @brief BnModel を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnModel::_typeobject()
{
  return &BnModelType;
}

END_NAMESPACE_YM
