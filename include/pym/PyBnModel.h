#ifndef PYBFMODEL_H
#define PYBFMODEL_H

/// @file PyBfModel.h
/// @brief PyBfModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnModelConv PyBnModel.h "PyBnModel.h"
/// @brief BnModel を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyBnModelConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnModel を PyObject* に変換する．
  PyObject*
  operator()(
    const BnModel& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyBnModel PyBnModel.h "PyBnModel.h"
/// @brief Python 用の BnModel 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnModel
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief BnModel を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BnModel& val ///< [in] 値
  )
  {
    PyBnModelConv conv;
    return conv(val);
  }

  /// @brief PyObject が BnModel タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnModel を表す PyObject から BnModel を取り出す．
  /// @return BnModel を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BnModel&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnModel を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBFMODEL_H
