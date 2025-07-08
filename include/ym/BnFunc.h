#ifndef BNFUNC_H
#define BNFUNC_H

/// @file BnFunc.h
/// @brief BnFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnFunc BnFunc.h "BnFunc.h"
/// @brief BnModel に登録された関数情報を表すクラス
///
/// BnNode の局所関数を表すことを目的としている．
///
/// 共通の属性として入力数を持つ．
///
/// 以下のタイプがある．
///
/// - プリミティブ型
///   PrimType を持つ．
///   同じ型でも入力数が異なれば別の関数と考える．
/// - カバー型
///   SopCover と出力の反転属性を持つ．
/// - 論理式型
///   Expr を持つ．
/// - 真理値表型
///   TvFunc を持つ．
/// - BDD型
///   Bdd を持つ．
//////////////////////////////////////////////////////////////////////
class BnFunc
{
  friend class ModelImpl;

public:
  //////////////////////////////////////////////////////////////////////
  // BnFunc で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を表す列挙型
  enum Type {
    None,      ///< [in] 不正値
    Primitive, ///< [in] プリミティブ型
    Cover,     ///< [in] カバー型
    Expr,      ///< [in] 論理式型
    TvFunc,    ///< [in] 真理値表型
    Bdd        ///< [in] BDD型
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
  // 共通なインターフェイス
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

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 種類を返す．
  Type
  type() const;

  /// @brief プリミティブ型の時 true を返す．
  is_primitive() const;

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

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // プリミティブ型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブ型を返す．
  PrimType
  primitive_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // カバー型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを返す．
  const SopCover&
  input_cover() const;

  /// @brief 出力の反転属性を返す．
  bool
  output_inv() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理式型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を返す．
  /// has_expr() == false の時は std::invalid_argument 例外を送出する．
  Expr
  expr() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 真理値表型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表を返す．
  const TvFunc&
  tvfunc() const;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDDを返す．
  Bdd
  bdd() const;



public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 内容を指定したコンストラクタ
  ///
  /// この関数は ModelImpl のみが使用する．
  BnFunc(
    const std::shared_ptr<const ModelImpl>& model, ///< [in] 親のモデル
    SizeType id                                    ///< [in] 関数番号
  );

  /// @brief 実装を取り出す．
  const FuncImpl&
  _impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデルの実装本体
  std::shared_ptr<const ModelImpl> mModel{nullptr};

  // 関数番号
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNFUNC_H
