#ifndef BNFUNC_H
#define BNFUNC_H

/// @file BnFunc.h
/// @brief BnFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnFunc BnFunc.h "BnFunc.h"
/// @brief BnModel に登録された関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class BnFunc
{
  friend class ModelImpl;

private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// この関数は ModelImpl のみが使用する．
  BnFunc(
    const shared_ptr<const ModelImpl>& model, ///< [in] 親のモデル
    SizeType id                               ///< [in] 関数番号
  );


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnFunc() = default;

  /// @brief デストラクタ
  ~BnFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mModel != nullptr;
  }

  /// @brief 関数番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 種類を返す．
  BnFuncType
  type() const;

  /// @brief カバー型の時 true を返す．
  bool
  is_cover() const;

  /// @brief 論理式型の時 true を返す．
  bool
  is_expr() const;

  /// @brief 真理値表型の時 true を返す．
  bool
  is_tvfunc() const;

  /// @brief BDD型の時 true を返す．
  bool
  is_bdd() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力カバーを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  const SopCover&
  input_cover() const;

  /// @brief 出力パタンを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  /// - ドントケアはない．
  char
  output_pat() const;

  /// @brief 論理式を返す．
  ///
  /// - is_expr() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  Expr
  expr() const;

  /// @brief 真理値表を返す．
  ///
  /// - is_tvfunc() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  const TvFunc&
  tvfunc() const;

  /// @brief BDDを返す．
  ///
  /// - is_bdd() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  Bdd
  bdd() const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnFunc& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mModel == right.mModel && mId == right.mId;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnFunc& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実装を取り出す．
  const FuncImpl&
  _impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデルの実装本体
  shared_ptr<const ModelImpl> mModel{nullptr};

  // 関数番号
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNFUNC_H
