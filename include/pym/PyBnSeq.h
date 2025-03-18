#ifndef PYBNSEQ_H
#define PYBNSEQ_H

/// @file PyBnSeq.h
/// @brief PyBnSeq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/BnSeq.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBnSeqConv PyBnSeq.h "PyBnSeq.h"
/// @brief BnSeq を PyObject* に変換するファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyBnSeqConv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnSeq を PyObject* に変換する．
  PyObject*
  operator()(
    const BnSeq& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyBnSeqDeconv PyBnSeq.h "PyBnSeq.h"
/// @brief BnSeq を取り出すファンクタクラス
///
/// 実はただの関数
//////////////////////////////////////////////////////////////////////
class PyBnSeqDeconv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PyObject* から BnSeq を取り出す．
  bool
  operator()(
    PyObject* obj,
    BnSeq& val
  );

};


//////////////////////////////////////////////////////////////////////
/// @class PyBnSeq PyBnSeq.h "PyBnSeq.h"
/// @brief Python 用の BnSeq 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class PyBnSeq
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

  /// @brief BnSeq を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const BnSeq& val ///< [in] 値
  )
  {
    PyBnSeqConv conv;
    return conv(val);
  }

  /// @brief PyObject が BnSeq タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief BnSeq を表す PyObject から BnSeq を取り出す．
  /// @return BnSeq を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  BnSeq&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief BnSeq を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYBNSEQ_H
