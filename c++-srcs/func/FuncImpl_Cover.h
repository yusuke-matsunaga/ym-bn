#ifndef FUNCIMPL_COVER_H
#define FUNCIMPL_COVER_H

/// @file FuncImpl_Cover.h
/// @brief FuncImpl_Cover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_Cover FuncImpl_Cover.h "FuncImpl_Cover.h"
/// @brief カバー型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_Cover :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_Cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief デストラクタ
  ~FuncImpl_Cover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーを作る．
  FuncImpl*
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr
  ) const override;

  /// @brief 関数の種類を返す．
  BnFuncType
  type() const override;

  /// @brief カバー型の論理ノードの時 true を返す．
  bool
  is_cover() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief 入力カバーを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  const SopCover&
  input_cover() const override;

  /// @brief 出力パタンを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  /// - ドントケアはない．
  char
  output_pat() const override;

  /// @brief 論理式を返す．
  Expr
  expr() const override;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力カバー
  SopCover mInputCover;

  // 出力パタン
  char mOutputPat;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_COVER_H
