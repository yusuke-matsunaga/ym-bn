
/// @file FuncImpl.cc
/// @brief FuncImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Cover.h"
#include "FuncImpl_Expr.h"
#include "FuncImpl_TvFunc.h"
#include "FuncImpl_Bdd.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief カバー型のインスタンスを作る．
FuncImpl*
FuncImpl::new_cover(
  SizeType input_num,                       ///< [in] 入力数
  const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
  char opat                                 ///< [in] 出力パタン ( '1' or '0' )
)
{
  // cube_list に含まれるリテラル番号が input_num より小さいかチェックする．
  // すき間があっても問題ない．
  for ( auto& cube: cube_list ) {
    for ( auto lit: cube ) {
      if ( lit.varid() >= input_num ) {
	throw std::invalid_argument{"'cube_list' contains an invalid literal."};
      }
    }
  }
  // opat が適正かチェックする．
  if ( opat != '1' && opat != '0' ) {
    ostringstream buf;
    buf << "'"<< opat << "' is not a valid opat value.";
    throw std::invalid_argument{buf.str()};
  }
  return new FuncImpl_Cover{input_num, cube_list, opat};
}

// @brief 論理式型のインスタンスを作る．
FuncImpl*
FuncImpl::new_expr(
  const Expr& expr
)
{
  return new FuncImpl_Expr{expr};
}

// @brief 真理値表型のインスタンスを作る．
FuncImpl*
FuncImpl::new_tvfunc(
  const TvFunc& func
)
{
  return new FuncImpl_TvFunc{func};
}

// @brief BDD型のインスタンスを作る．
FuncImpl*
FuncImpl::new_bdd(
  const Bdd& bdd
)
{
  return new FuncImpl_Bdd{bdd};
}

// @brief カバー型の論理ノードの時 true を返す．
bool
FuncImpl::is_cover() const
{
  return false;
}

// @brief 論理式型の論理ノードの時 true を返す．
bool
FuncImpl::is_expr() const
{
  return false;
}

// @brief 真理値表型の論理ノードの時 true を返す．
bool
FuncImpl::is_tvfunc() const
{
  return false;
}

// @brief BDD型の論理ノードの時 true を返す．
bool
FuncImpl::is_bdd() const
{
  return false;
}

// @brief 入力カバーを返す．
const SopCover&
FuncImpl::input_cover() const
{
  throw std::invalid_argument{"not a Cover type."};
}

// @brief 出力パタンを返す．
char
FuncImpl::output_pat() const
{
  throw std::invalid_argument{"not a Cover type."};
}

// @brief 論理式を返す．
Expr
FuncImpl::expr() const
{
  throw std::invalid_argument{"not an Expr type."};
}

// @brief 真理値表を返す．
const TvFunc&
FuncImpl::tvfunc() const
{
  throw std::invalid_argument{"not a TvFunc type."};
}

// @brief BDDを返す．
Bdd
FuncImpl::bdd() const
{
  throw std::invalid_argument{"not a BDD type."};
}

END_NAMESPACE_YM_BN
