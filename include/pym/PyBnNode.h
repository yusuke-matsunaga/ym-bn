#ifndef PYBNNODE_H
#define PYBNNODE_H

/// @file PyBnNode.h
/// @brief PyBnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnNodeConv PyBnNode.h "PyBnNode.h"
/// @brief BnNode を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyBnNodeConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode を PyObject* に変換する．
  PyObject*
  operator()(
    const BnNode& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyBnNodeDeconv PyBnNode.h "PyBnNode.h"
/// @brief BnNode を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyBnNodeDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から BnNode を取り出す．
  bool
  operator()(
    PyObject* obj,
    BnNode& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyBnNode PyBnNode.h "PyBnNode.h"
/// @brief Python 用の BnNode 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnNode
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

  /// @brief BnNode を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BnNode& val ///< [in] 値
  )
  {
    PyBnNodeConv conv;
    return conv(val);
  }

  /// @brief PyObject が BnNode タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnNode を表す PyObject から BnNode を取り出す．
  /// @return BnNode を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BnNode&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnNode を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBNNODE_H
