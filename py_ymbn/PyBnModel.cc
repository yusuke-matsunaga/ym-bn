
/// @file PyBnModel.cc
/// @brief Python BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnNodeList.h"
#include "pym/PyBnSeq.h"
#include "pym/PyBnFunc.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyClibCell.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBdd.h"
#include "pym/PyPrimType.h"
#include "pym/PyLiteral.h"
#include "pym/PyModule.h"
#include "pym/PyBase.h"
#include "ym/BnNode.h"
#include "ym/JsonValue.h"


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
  PyObject* args,
  PyObject* kwds
)
{
  static char* kwlist[] = {
    nullptr
  };

  // 引数を取らないことをチェックする．
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
				    const_cast<char**>(kwlist)) ) {
    return nullptr;
  }

  auto obj = BnModelType.tp_alloc(&BnModelType, 0);
  auto model_obj = reinterpret_cast<BnModelObject*>(obj);
  model_obj->mPtr = new BnModel;

  return obj;
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
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what() );
    return nullptr;
  }
}

PyObject*
BnModel_read_iscas89(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };

  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_iscas89(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_read_aag(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };

  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_aag(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_read_aig(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };

  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_aig(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_read_truth(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };

  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  try {
    auto model = BnModel::read_truth(filename);
    return PyBnModel::ToPyObject(model);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
    auto& model = PyBnModel::Get(self);
    auto val = model.input(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
    auto& model = PyBnModel::Get(self);
    auto val = model.input_name(pos);
    return Py_BuildValue("s", val.c_str());
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
    auto& model = PyBnModel::Get(self);
    auto val = model.output(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
    auto& model = PyBnModel::Get(self);
    auto val = model.output_name(pos);
    return Py_BuildValue("s", val.c_str());
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
    auto& model = PyBnModel::Get(self);
    auto val = model.logic(pos);
    return PyBnNode::ToPyObject(val);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_seq(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto& model = PyBnModel::Get(self);
    auto val = model.seq(pos);
    return PyBnSeq::ToPyObject(val);
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_seq_name(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos{0};
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  try {
    auto& model = PyBnModel::Get(self);
    auto val = model.seq_name(pos);
    return Py_BuildValue("s", val.c_str());
  }
  catch ( std::out_of_range err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
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
  auto& model = PyBnModel::Get(self);
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

PyObject*
BnModel_clear(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& model = PyBnModel::Get(self);
  model.clear();
  Py_RETURN_NONE;
}

PyObject*
BnModel_set_celllibrary(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "library",
    nullptr
  };

  PyObject* obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kwlist),
				    PyClibCellLibrary::_typeobject(), &obj) ) {
    return nullptr;
  }
  auto& model = PyBnModel::Get(self);
  auto library = PyClibCellLibrary::Get(obj);
  model.set_celllibrary(library);
  Py_RETURN_NONE;
}

PyObject*
BnModel_set_option(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "option",
    nullptr
  };

  const char* option_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &option_str) ) {
    return nullptr;
  }
  auto& model = PyBnModel::Get(self);
  auto option = JsonValue::parse(option_str);
  model.set_option(option);
  Py_RETURN_NONE;
}

PyObject*
BnModel_new_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "name",
    nullptr
  };

  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|s",
				    const_cast<char**>(kwlist),
				    &name_str) ) {
    return nullptr;
  }

  auto& model = PyBnModel::Get(self);
  string name;
  if ( name_str ) {
    name = name_str;
  }
  auto node = model.new_input(name);

  return PyBnNode::ToPyObject(node);
}

PyObject*
BnModel_new_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "src",
    "name",
    nullptr
  };

  PyObject* src_obj = nullptr;
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|s",
				    const_cast<char**>(kwlist),
				    PyBnNode::_typeobject(), &src_obj,
				    &name_str) ) {
    return nullptr;
  }

  auto& model = PyBnModel::Get(self);
  auto& src = PyBnNode::Get(src_obj);
  string name;
  if ( name_str ) {
    name = name_str;
  }
  try {
    auto val = model.new_output(src, name);
    return Py_BuildValue("k", val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

bool
parse_input_list(
  PyObject* input_list_obj,
  vector<BnNode>& input_list
)
{
  if ( !PySequence_Check(input_list_obj) ) {
    PyErr_SetString(PyExc_TypeError, "1st argument should be a sequence type");
    return false;
  }
  SizeType n = PySequence_Size(input_list_obj);
  input_list = vector<BnNode>(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(input_list_obj, i);
    if ( !PyBnNode::Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "1st argument should be a sequence of BnNode");
      return false;
    }
    input_list[i] = PyBnNode::Get(obj1);
  }
  return true;
}

PyObject*
BnModel_new_primitive(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "type",
    "input_list",
    nullptr
  };

  PyObject* input_list_obj = nullptr;
  PyObject* type_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
				    const_cast<char**>(kwlist),
				    PyPrimType::_typeobject(), &type_obj,
				    &input_list_obj) ) {
    return nullptr;
  }

  vector<BnNode> input_list;
  if ( !parse_input_list(input_list_obj, input_list) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto type = PyPrimType::Get(type_obj);
    auto node = model.new_primitive(type, input_list);

    return PyBnNode::ToPyObject(node);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_new_aig(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "src0",
    "src1",
    "inv0",
    "inv1",
    nullptr
  };

  PyObject* src0_obj = nullptr;
  PyObject* src1_obj = nullptr;
  int inv0 = false;
  int inv1 = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!pp",
				    const_cast<char**>(kwlist),
				    PyBnNode::_typeobject(), &src0_obj,
				    PyBnNode::_typeobject(), &src1_obj,
				    &inv0, &inv1) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto src0 = PyBnNode::Get(src0_obj);
    auto src1 = PyBnNode::Get(src1_obj);
    auto node = model.new_aig(src0, src1,
			      static_cast<bool>(inv0),
			      static_cast<bool>(inv1));

    return PyBnNode::ToPyObject(node);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_new_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "func",
    "input_list",
    nullptr
  };

  PyObject* func_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
				    const_cast<char**>(kwlist),
				    PyBnFunc::_typeobject(), &func_obj,
				    &input_list_obj) ) {
    return nullptr;
  }
  vector<BnNode> input_list;
  if ( !parse_input_list(input_list_obj, input_list) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto& func = PyBnFunc::Get(func_obj);
    auto node = model.new_func(func, input_list);

    return PyBnNode::ToPyObject(node);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what() );
    return nullptr;
  }
}

PyObject*
BnModel_new_cell(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "cell",
    "input_list",
    nullptr
  };

  PyObject* cell_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
				    const_cast<char**>(kwlist),
				    PyClibCell::_typeobject(), &cell_obj,
				    &input_list_obj) ) {
    return nullptr;
  }
  vector<BnNode> input_list;
  if ( !parse_input_list(input_list_obj, input_list) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto cell = PyClibCell::Get(cell_obj);
    auto node = model.new_cell(cell, input_list);

    return PyBnNode::ToPyObject(node);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_reg_cover(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "input_num",
    "cube_list",
    "opat",
    nullptr
  };

  SizeType input_num = 0;
  PyObject* cube_list_obj = nullptr;
  const char* opat_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kOs",
				    const_cast<char**>(kwlist),
				    &input_num,
				    &cube_list_obj,
				    &opat_str) ) {
    return nullptr;
  }

  if ( !PySequence_Check(cube_list_obj) ) {
    PyErr_SetString(PyExc_TypeError, "2nd argument should be a list of list of Literals");
    return nullptr;
  }
  SizeType n = PySequence_Size(cube_list_obj);
  vector<vector<Literal>> cube_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(cube_list_obj, i);
    if ( !PySequence_Check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "2nd argument should be a list of list of Literals");
      return nullptr;
    }
    SizeType m = PySequence_Size(obj1);
    cube_list[i] = vector<Literal>(m);
    for ( SizeType j = 0; j < m; ++ j ) {
      auto obj2 = PySequence_GetItem(obj1, j);
      if ( !PyLiteral::Check(obj2) ) {
	PyErr_SetString(PyExc_TypeError, "2nd argument should be a list of list of Literals");
	return nullptr;
      }
      cube_list[i][j] = PyLiteral::Get(obj2);
    }
  }
  char opat;
  if ( strcmp(opat_str, "0") == 0 ) {
    opat = '0';
  }
  else if ( strcmp(opat_str, "1") == 0 ) {
    opat = '1';
  }
  else {
    PyErr_SetString(PyExc_ValueError, "3rd argument should be '0' or '1'");
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto val = model.reg_cover(input_num, cube_list, opat);
    return PyBnFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_reg_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "expr",
    nullptr
  };

  PyObject* expr_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kwlist),
				    PyExpr::_typeobject, &expr_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto expr = PyExpr::Get(expr_obj);
    auto val = model.reg_expr(expr);
    return PyBnFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_reg_tvfunc(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "func",
    nullptr
  };

  PyObject* func_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kwlist),
				    PyTvFunc::_typeobject, &func_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto func = PyTvFunc::Get(func_obj);
    auto val = model.reg_tvfunc(func);
    return PyBnFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_reg_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "bdd",
    nullptr
  };

  PyObject* bdd_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kwlist),
				    PyBdd::_typeobject, &bdd_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto bdd = PyBdd::Get(bdd_obj);
    auto val = model.reg_bdd(bdd);
    return PyBnFunc::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }

}

PyObject*
BnModel_new_dff(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "rs_val",
    "name",
    nullptr
  };

  const char* rs_val_str = nullptr;
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|ss",
				    const_cast<char**>(kwlist),
				    &rs_val_str, &name_str) ) {
    return nullptr;
  }
  char rs_val = ' ';
  string name;
  if ( rs_val_str != nullptr ) {
    if ( strlen(rs_val_str) != 1 ) {
      PyErr_SetString(PyExc_ValueError, "Illegal value for 1st argument");
      return nullptr;
    }
    rs_val = rs_val_str[0];
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = model.new_dff(rs_val, name);

    return PyBnSeq::ToPyObject(seq);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_new_latch(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "rs_val",
    "name",
    nullptr
  };

  const char* rs_val_str = nullptr;
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|ss",
				    const_cast<char**>(kwlist),
				    &rs_val_str, &name_str) ) {
    return nullptr;
  }
  char rs_val = ' ';
  if ( rs_val_str != nullptr ) {
    if ( strlen(rs_val_str) != 1 ) {
      PyErr_SetString(PyExc_ValueError, "Illegal value for 1st argument");
      return nullptr;
    }
    rs_val = rs_val_str[0];
  }
  string name;
  if ( name_str != nullptr ) {
    name = string{name_str};
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = model.new_latch(rs_val, name);

    return PyBnSeq::ToPyObject(seq);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_new_seq_cell(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "cell",
    "name",
    nullptr
  };

  PyObject* cell_obj = nullptr;
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|s",
				    const_cast<char**>(kwlist),
				    PyClibCell::_typeobject(), &cell_obj,
				    &name_str) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto cell = PyClibCell::Get(cell_obj);
    string name;
    if ( name_str != nullptr ) {
      name = string{name_str};
    }
    auto seq = model.new_seq_cell(cell, name);

    return PyBnSeq::ToPyObject(seq);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_data_src(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "src",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto src = PyBnNode::Get(src_obj);
    model.set_data_src(seq, src);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_clock(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "clock",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  PyObject* clock_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    PyBnNode::_typeobject(), &clock_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto clock = PyBnNode::Get(clock_obj);
    model.set_clock(seq, clock);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_enable(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "enable",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto src = PyBnNode::Get(src_obj);
    model.set_enable(seq, src);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_clear(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "clear",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto src = PyBnNode::Get(src_obj);
    model.set_clear(seq, src);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_preset(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "preset",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto src = PyBnNode::Get(src_obj);
    model.set_preset(seq, src);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
BnModel_set_seq_pin(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const char* kwlist[] = {
    "seq",
    "pos",
    "node",
    nullptr
  };

  PyObject* seq_obj = nullptr;
  SizeType pos = 0;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!kO!",
				    const_cast<char**>(kwlist),
				    PyBnSeq::_typeobject(), &seq_obj,
				    &pos,
				    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }

  try {
    auto& model = PyBnModel::Get(self);
    auto seq = PyBnSeq::Get(seq_obj);
    auto src = PyBnNode::Get(src_obj);
    model.set_seq_pin(seq, pos, src);

    Py_RETURN_NONE;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef BnModel_methods[] = {
  {"read_blif",
   reinterpret_cast<PyCFunction>(BnModel_read_blif),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'blif' file")},
  {"read_iscas89",
   reinterpret_cast<PyCFunction>(BnModel_read_iscas89),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'iscas89(.bench)' file")},
  {"read_aag",
   reinterpret_cast<PyCFunction>(BnModel_read_aag),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aag' file")},
  {"read_aig",
   reinterpret_cast<PyCFunction>(BnModel_read_aig),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'aig' file")},
  {"read_truth",
   reinterpret_cast<PyCFunction>(BnModel_read_truth),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read 'truth' file")},
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
  {"seq", BnModel_seq,
   METH_VARARGS,
   PyDoc_STR("returns SEQ object")},
  {"seq_name", BnModel_seq_name,
   METH_VARARGS,
   PyDoc_STR("returns SEQ name")},
  {"logic", BnModel_logic,
   METH_VARARGS,
   PyDoc_STR("returns logic node")},
  {"print",
   reinterpret_cast<PyCFunction>(BnModel_print),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("print contents")},
  {"clear", BnModel_clear,
   METH_NOARGS,
   PyDoc_STR("clear")},
  {"set_celllibrary",
   reinterpret_cast<PyCFunction>(BnModel_set_celllibrary),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set cell library")},
  {"set_option",
   reinterpret_cast<PyCFunction>(BnModel_set_option),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set option information")},
  {"new_input",
   reinterpret_cast<PyCFunction>(BnModel_new_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new input node")},
  {"new_output",
   reinterpret_cast<PyCFunction>(BnModel_new_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new output node")},
  {"new_primitive",
   reinterpret_cast<PyCFunction>(BnModel_new_primitive),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new primitive node")},
  {"new_aig",
   reinterpret_cast<PyCFunction>(BnModel_new_aig),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new AIG node")},
  {"new_func",
   reinterpret_cast<PyCFunction>(BnModel_new_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new function node")},
  {"reg_cover",
   reinterpret_cast<PyCFunction>(BnModel_reg_cover),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("register a COVER")},
  {"reg_expr",
   reinterpret_cast<PyCFunction>(BnModel_reg_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("register an expression")},
  {"reg_tvfunc",
   reinterpret_cast<PyCFunction>(BnModel_reg_tvfunc),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("register a TVFUNC")},
  {"reg_bdd",
   reinterpret_cast<PyCFunction>(BnModel_reg_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("register a BDD")},
  {"new_dff",
   reinterpret_cast<PyCFunction>(BnModel_new_dff),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new DFF")},
  {"new_latch",
   reinterpret_cast<PyCFunction>(BnModel_new_latch),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new latch")},
  {"new_seq_cell",
   reinterpret_cast<PyCFunction>(BnModel_new_seq_cell),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new SEQ cell")},
  {"set_data_src",
   reinterpret_cast<PyCFunction>(BnModel_set_data_src),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF/latch's data-src")},
  {"set_clock",
   reinterpret_cast<PyCFunction>(BnModel_set_clock),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF's clock")},
  {"set_enable",
   reinterpret_cast<PyCFunction>(BnModel_set_enable),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set latch's enable")},
  {"set_clear",
   reinterpret_cast<PyCFunction>(BnModel_set_clear),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF/latch's clear")},
  {"set_preset",
   reinterpret_cast<PyCFunction>(BnModel_set_preset),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF/latch's preset")},
  {"set_seq_pin",
   reinterpret_cast<PyCFunction>(BnModel_set_seq_pin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set SEQ cell's pin")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BnModel_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.name();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BnModel_comment(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.comment();
  return Py_BuildValue("s", val.c_str());
}

PyObject*
BnModel_library(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.library();
  return PyClibCellLibrary::ToPyObject(val);
}

PyObject*
BnModel_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.node_num();
  return Py_BuildValue("k", val);
}

PyObject*
BnModel_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.input_num();
  return Py_BuildValue("k", val);
}

PyObject*
BnModel_input_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.input_list();
  return PyBnNodeList::ToPyObject(val);
}

PyObject*
BnModel_input_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
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
  auto& model = PyBnModel::Get(self);
  auto val = model.seq_num();
  return Py_BuildValue("k", val);
}

PyObject*
BnModel_seq_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  SizeType n = model.seq_num();
  vector<string> val_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    val_list[i] = model.seq_name(i);
  }
  return PyBase::ToPyList(val_list);
}

PyObject*
BnModel_output_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.output_num();
  return Py_BuildValue("k", val);
}

PyObject*
BnModel_output_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.output_list();
  return PyBnNodeList::ToPyObject(val);
}

PyObject*
BnModel_output_name_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
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
  auto& model = PyBnModel::Get(self);
  auto val = model.logic_num();
  return Py_BuildValue("k", val);
}

PyObject*
BnModel_logic_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.logic_list();
  return PyBnNodeList::ToPyObject(val);
}

PyObject*
BnModel_func_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.func_num();
  return Py_BuildValue("k", val);
}

#if 0
PyObject*
BnModel_option(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& model = PyBnModel::Get(self);
  auto val = model.option();
  return Py_BuildValue("i", val);
}
#endif

// getsetter 定義
PyGetSetDef BnModel_getsetters[] = {
  {"name", BnModel_name, nullptr, PyDoc_STR("name"), nullptr},
  {"comment", BnModel_comment, nullptr, PyDoc_STR("comment"), nullptr},
  {"library", BnModel_library, nullptr, PyDoc_STR("Cell Library"), nullptr},
  {"node_num", BnModel_node_num, nullptr, PyDoc_STR("node num"), nullptr},
  {"input_num", BnModel_input_num, nullptr, PyDoc_STR("input num"), nullptr},
  {"input_list", BnModel_input_list, nullptr, PyDoc_STR("input list"), nullptr},
  {"input_name_list", BnModel_input_name_list, nullptr, PyDoc_STR("input name list"), nullptr},
  {"seq_num", BnModel_seq_num, nullptr, PyDoc_STR("SEQ node num"), nullptr},
  {"seq_name_list", BnModel_seq_name_list, nullptr, PyDoc_STR("SEQ name list"), nullptr},
  {"output_num", BnModel_output_num, nullptr, PyDoc_STR("output num"), nullptr},
  {"output_list", BnModel_output_list, nullptr, PyDoc_STR("output list"), nullptr},
  {"output_name_list", BnModel_output_name_list, nullptr, PyDoc_STR("output name list"), nullptr},
  {"logic_num", BnModel_logic_num, nullptr, PyDoc_STR("logic gate num"), nullptr},
  {"logic_list", BnModel_logic_list, nullptr, PyDoc_STR("logic gate list"), nullptr},
  {"func_num", BnModel_func_num, nullptr, PyDoc_STR("function num"), nullptr},
  //{"option", BnModel_option, nullptr, PyDoc_STR("option JSON object"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

PyObject*
BnModel_richcompfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBnModel::Check(self) &&
       PyBnModel::Check(other) ) {
    auto& val1 = PyBnModel::Get(self);
    auto& val2 = PyBnModel::Get(other);
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
  BnModelType.tp_richcompare = BnModel_richcompfunc;
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
BnModel&
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
