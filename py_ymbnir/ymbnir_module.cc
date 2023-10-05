
/// @file ymbnir_module.cc
/// @brief Python 用の bnir モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyBfModel.h"
#include "pym/PyBfNode.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymbnir_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymbnir_module = {
  PyModuleDef_HEAD_INIT,
  "ymbnir",
  PyDoc_STR("ymbnir: Extension module for bnir"),
  -1,
  ymbnir_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymbnir()
{
  auto m = PyModule::init(&ymbnir_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyBfModel::init(m) ) {
    goto error;
  }

  if ( !PyBfNode::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
