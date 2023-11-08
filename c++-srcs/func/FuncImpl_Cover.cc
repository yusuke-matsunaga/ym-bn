
/// @file FuncImpl_Cover.cc
/// @brief FuncImpl_Cover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl_Cover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Cover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Cover::FuncImpl_Cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
) : mInputCover{input_num, cube_list},
    mOutputPat{opat}
{
}

// @brief コピーを作る．
FuncImpl*
FuncImpl_Cover::copy(
  BddMgr& bdd_mgr
) const
{
  return new FuncImpl_Cover{*this};
}

// @brief 関数の種類を返す．
BnFuncType
FuncImpl_Cover::type() const
{
  return BnFuncType::COVER;
}

// @brief カバー型の論理ノードの時 true を返す．
bool
FuncImpl_Cover::is_cover() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Cover::input_num() const
{
  return mInputCover.variable_num();
}

// @brief 入力カバーを返す．
const SopCover&
FuncImpl_Cover::input_cover() const
{
  return mInputCover;
}

// @brief 出力パタンを返す．
char
FuncImpl_Cover::output_pat() const
{
  return mOutputPat;
}

// @brief 論理式を返す．
Expr
FuncImpl_Cover::expr() const
{
  auto expr = mInputCover.expr();
  if ( output_pat() == '0' ) {
    expr = ~expr;
  }
  return expr;
}

// @brief 内容を出力する．
void
FuncImpl_Cover::print(
  ostream& s
) const
{
  auto nc = mInputCover.cube_num();
  auto ni = mInputCover.variable_num();
  s << "Cover" << endl;
  for ( SizeType c = 0; c < nc; ++ c ) {
    for ( SizeType i = 0; i < ni; ++ i ) {
      s << mInputCover.get_pat(c, i);
    }
    if ( ni > 0 ) {
      s << ' ';
    }
    s << mOutputPat << endl;
  }
}

END_NAMESPACE_YM_BN
