#ifndef PYBFDFF_H
#define PYBFDFF_H

/// @file PyBnDff.h
/// @brief PyBnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnDff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnDff PyBnDff.h "PyBnDff.h"
/// @brief Python 用の BnDff 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnDff
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

  /// @brief BnDff を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    BnDff val ///< [in] 値
  );

  /// @brief BnDff のリストを表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyList(
    const vector<BnDff>& val_list ///< [in] 値のリスト
  );

  /// @brief PyObject が BnDff タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnDff を表す PyObject から BnDff を取り出す．
  /// @return BnDff を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BnDff
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnDff を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBFDFF_H
