
/// @file PyBnFunc.cc
/// @brief Python BnFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnFunc.h"
#include "pym/PyBnModel.h"
#include "pym/PySopCover.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBdd.h"
#include "pym/PyModule.h"
#include "ym/BnFunc.h"
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BnFuncObject
{
  PyObject_HEAD
  BnFunc* mPtr;
};

// Python 用のタイプ定義
PyTypeObject BnFunc_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BnFunc_new(
  PyTypeObject* type,
  PyObject* Py_UNUSED(args),
  PyObject* Py_UNUSED(kwds)
)
{
  PyErr_SetString(PyExc_TypeError, "instantiation of 'BnFunc' is disabled");
  return nullptr;
}

// 終了関数
void
BnFunc_dealloc(
  PyObject* self
)
{
  auto func_obj = reinterpret_cast<BnFuncObject*>(self);
  delete func_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BnFunc_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyBnFunc::Get(self);
  auto r = func.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
BnFunc_is_cover(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyBnFunc::Get(self);
  auto r = func.is_cover();
  return PyBool_FromLong(r);
}

PyObject*
BnFunc_is_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyBnFunc::Get(self);
  auto r = func.is_expr();
  return PyBool_FromLong(r);
}

PyObject*
BnFunc_is_tvfunc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyBnFunc::Get(self);
  auto r = func.is_tvfunc();
  return PyBool_FromLong(r);
}

PyObject*
BnFunc_is_bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyBnFunc::Get(self);
  auto r = func.is_bdd();
  return PyBool_FromLong(r);
}

// メソッド定義
PyMethodDef BnFunc_methods[] = {
  {"is_valid", BnFunc_is_valid, METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"is_cover", BnFunc_is_cover, METH_NOARGS,
   PyDoc_STR("return True if cover-type")},
  {"is_expr", BnFunc_is_expr, METH_NOARGS,
   PyDoc_STR("return True if expr-type")},
  {"is_tvfunc", BnFunc_is_tvfunc, METH_NOARGS,
   PyDoc_STR("return True if tvfunc-type")},
  {"is_bdd", BnFunc_is_bdd, METH_NOARGS,
   PyDoc_STR("return True if BDD-type")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnFunc_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& func = PyBnFunc::Get(self);
  auto id = func.id();
  return PyLong_FromLong(id);
}

PyObject*
BnFunc_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& func = PyBnFunc::Get(self);
  auto type = func.type();
  ostringstream buf;
  buf << type;
  return Py_BuildValue("s", buf.str().c_str());
}

PyObject*
BnFunc_input_cover(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& func = PyBnFunc::Get(self);
    auto& val = func.input_cover();
    return PySopCover::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnFunc_output_pat(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& func = PyBnFunc::Get(self);
    auto val = func.output_pat();
    char buf[] = {val, '\0'};
    return Py_BuildValue("s", buf);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnFunc_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& func = PyBnFunc::Get(self);
    auto val = func.expr();
    return PyExpr::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnFunc_tvfunc(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& func = PyBnFunc::Get(self);
    auto val = func.tvfunc();
    return PyTvFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnFunc_bdd(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  try {
    auto& func = PyBnFunc::Get(self);
    auto val = func.bdd();
    return PyBdd::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

// getsetter 定義
PyGetSetDef BnFunc_getsetters[] = {
  {"id", BnFunc_id, nullptr,
   PyDoc_STR("ID"), nullptr},
  {"input_cover", BnFunc_input_cover, nullptr,
   PyDoc_STR("input cover"), nullptr},
  {"output_pat", BnFunc_output_pat, nullptr,
   PyDoc_STR("output pat"), nullptr},
  {"expr", BnFunc_expr, nullptr,
   PyDoc_STR("expr"), nullptr},
  {"tvfunc", BnFunc_tvfunc, nullptr,
   PyDoc_STR("tvfunc"), nullptr},
  {"bdd", BnFunc_bdd, nullptr,
   PyDoc_STR("BDD"), nullptr},
  {nullptr, nullptr, nullptr,
   nullptr, nullptr}
};

// 比較関数
PyObject*
BnFunc_richcompfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBnFunc::Check(self) &&
       PyBnFunc::Check(other) ) {
    auto& val1 = PyBnFunc::Get(self);
    auto& val2 = PyBnFunc::Get(other);
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


// @brief 'BnFunc' オブジェクトを使用可能にする．
bool
PyBnFunc::init(
  PyObject* m
)
{
  BnFunc_Type.tp_name = "BnFunc";
  BnFunc_Type.tp_basicsize = sizeof(BnFuncObject);
  BnFunc_Type.tp_itemsize = 0;
  BnFunc_Type.tp_dealloc = BnFunc_dealloc;
  BnFunc_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnFunc_Type.tp_doc = PyDoc_STR("BnFunc object");
  BnFunc_Type.tp_richcompare = BnFunc_richcompfunc;
  BnFunc_Type.tp_methods = BnFunc_methods;
  BnFunc_Type.tp_getset = BnFunc_getsetters;
  BnFunc_Type.tp_new = BnFunc_new;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BnFunc", &BnFunc_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BnFunc を PyObject に変換する．
PyObject*
PyBnFunc::ToPyObject(
  const BnFunc& func
)
{
  auto obj = BnFunc_Type.tp_alloc(&BnFunc_Type, 0);
  auto func_obj = reinterpret_cast<BnFuncObject*>(obj);
  func_obj->mPtr = new BnFunc{func};
  return obj;
}

// @brief PyObject が BnFunc タイプか調べる．
bool
PyBnFunc::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BnFunc を表す PyObject から BnFunc を取り出す．
const BnFunc&
PyBnFunc::Get(
  PyObject* obj
)
{
  auto func_obj = reinterpret_cast<BnFuncObject*>(obj);
  return *func_obj->mPtr;
}

// @brief BnFunc を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnFunc::_typeobject()
{
  return &BnFunc_Type;
}

END_NAMESPACE_YM
