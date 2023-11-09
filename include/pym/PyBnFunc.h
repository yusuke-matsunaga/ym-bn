#ifndef PYBNFUNC_H
#define PYBNFUNC_H

/// @file PyBnFunc.h
/// @brief PyBnFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnFunc PyBnFunc.h "PyBnFunc.h"
/// @brief Python 用の BnFunc 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnFunc
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

  /// @brief BnFunc を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BnFunc& val ///< [in] 値
  );

  /// @brief PyObject が BnFunc タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnFunc を表す PyObject から BnFunc を取り出す．
  /// @return BnFunc を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  const BnFunc&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnFunc を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBNFUNC_H
