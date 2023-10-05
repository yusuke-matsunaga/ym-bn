#ifndef PYBFNODE_H
#define PYBFNODE_H

/// @file PyBfNode.h
/// @brief PyBfNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBfNode PyBfNode.h "PyBfNode.h"
/// @brief Python 用の BfNode 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBfNode
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

  /// @brief BfNode を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    BfNode val ///< [in] 値
  );

  /// @brief BfNode のリストを表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyList(
    const vector<BfNode>& val_list ///< [in] 値のリスト
  );

  /// @brief PyObject が BfNode タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BfNode を表す PyObject から BfNode を取り出す．
  /// @return BfNode を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BfNode
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BfNode を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBFNODE_H
