
/// @file py_ymbnfe.cc
/// @brief py_ymbnfe の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "pym/ymbn.h"


int
main(
  int argc,
  char** argv
)
{
  PyImport_AppendInittab("ymbn", &YM_NAMESPACE::PyInit_ymbn);

  return Py_BytesMain(argc, argv);
}
