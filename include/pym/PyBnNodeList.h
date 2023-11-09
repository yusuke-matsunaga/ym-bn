#ifndef PYBNNODELIST_H
#define PYBNNODELIST_H

/// @file PyBnNodeList.h
/// @brief PyBnNodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnNodeList.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnNodeList PyBnNodeList.h "PyBnNodeList.h"
/// @brief Python 用の BnNodeList 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnNodeList
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

  /// @brief BnNodeList を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BnNodeList& val ///< [in] 値
  );

  /// @brief PyObject が BnNodeList タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnNodeList を表す PyObject から BnNodeList を取り出す．
  /// @return BnNodeList を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  const BnNodeList&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnNodeList を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBNNODELIST_H
