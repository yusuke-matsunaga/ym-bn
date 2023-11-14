
/// @file FuncImpl_Expr.cc
/// @brief FuncImpl_Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl_Expr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Expr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Expr::FuncImpl_Expr(
  const Expr& expr
) : mExpr{expr}
{
}

// @brief コピーを作る．
unique_ptr<FuncImpl>
FuncImpl_Expr::copy(
  BddMgr& bdd_mgr
) const
{
  return unique_ptr<FuncImpl>{new FuncImpl_Expr{*this}};
}

// @brief 関数の種類を返す．
BnFuncType
FuncImpl_Expr::type() const
{
  return BnFuncType::EXPR;
}

// @brief 論理式型の論理ノードの時 true を返す．
bool
FuncImpl_Expr::is_expr() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Expr::input_num() const
{
  return mExpr.input_size();
}

// @brief 論理式を返す．
Expr
FuncImpl_Expr::expr() const
{
  return mExpr;
}

// @brief 内容を出力する．
void
FuncImpl_Expr::print(
  ostream& s
) const
{
  s << "Expr" << endl
    << mExpr.rep_string() << endl;
}

END_NAMESPACE_YM_BN
