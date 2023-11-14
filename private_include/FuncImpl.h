#ifndef FUNCIMPL_H
#define FUNCIMPL_H

/// @file FuncImpl.h
/// @brief FuncImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl FuncImpl.h "FuncImpl.h"
/// @brief BnFunc の中身を表すクラス
///
/// - 実際には個々のタイプごとに固有の情報を持つ．
/// - このクラスは仮想関数の宣言のみ行う．
/// - ただし，派生クラスの生成関数をクラスメソッドとして用意したので
///   個々の派生クラスについて知る必要はない．
//////////////////////////////////////////////////////////////////////
class FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl() = default;

  /// @brief デストラクタ
  virtual
  ~FuncImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief カバー型のインスタンスを作る．
  ///
  /// - cube_list 中に現れるリテラル番号は input_num 未満でなければならない．
  /// - opat は '0' か '1' でなければならない．
  /// - 上記の条件に合わない場合は std::invalid_argument 例外を送出する．
  static
  FuncImpl*
  new_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief 論理式型のインスタンスを作る．
  static
  FuncImpl*
  new_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表型のインスタンスを作る．
  static
  FuncImpl*
  new_tvfunc(
    const TvFunc& func ///< [in] 真理値表
  );

  /// @brief BDD型のインスタンスを作る．
  static
  FuncImpl*
  new_bdd(
    const Bdd& bdd ///< [in] BDD
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 複製用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーを作る．
  virtual
  unique_ptr<FuncImpl>
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 種類を取り出す関数
  // この関数は例外を創出しない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  virtual
  BnFuncType
  type() const = 0;

  /// @brief カバー型の論理ノードの時 true を返す．
  virtual
  bool
  is_cover() const;

  /// @brief 論理式型の論理ノードの時 true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 真理値表型の論理ノードの時 true を返す．
  virtual
  bool
  is_tvfunc() const;

  /// @brief BDD型の論理ノードの時 true を返す．
  virtual
  bool
  is_bdd() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 個々の型に関連した情報を取り出す関数
  // この関数は場合によっては std::invalid_argument 例外を送出する．
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力カバーを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  const SopCover&
  input_cover() const;

  /// @brief 出力パタンを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  /// - ドントケアはない．
  virtual
  char
  output_pat() const;

  /// @brief 論理式を返す．
  ///
  /// - is_expr() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  Expr
  expr() const;

  /// @brief 真理値表を返す．
  ///
  /// - is_tvfunc() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  const TvFunc&
  tvfunc() const;

  /// @brief BDDを返す．
  ///
  /// - is_bdd() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  Bdd
  bdd() const;

  /// @brief 内容を出力する．
  virtual
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const = 0;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_H
