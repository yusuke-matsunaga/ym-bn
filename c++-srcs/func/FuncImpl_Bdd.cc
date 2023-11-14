
/// @file FuncImpl_Bdd.cc
/// @brief FuncImpl_Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl_Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Bdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Bdd::FuncImpl_Bdd(
  Bdd bdd
) : mBdd{bdd}
{
}

// @brief コピーを作る．
unique_ptr<FuncImpl>
FuncImpl_Bdd::copy(
  BddMgr& bdd_mgr
) const
{
  auto my_bdd = bdd_mgr.copy(mBdd);
  return unique_ptr<FuncImpl>{new FuncImpl_Bdd{my_bdd}};
}

// @brief 関数の種類を返す．
BnFuncType
FuncImpl_Bdd::type() const
{
  return BnFuncType::BDD;
}

// @brief BDD型の論理ノードの時 true を返す．
bool
FuncImpl_Bdd::is_bdd() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Bdd::input_num() const
{
  if ( mBdd.is_zero() || mBdd.is_one() ) {
    return 0;
  }
  auto support_list = mBdd.get_support_list();
  SizeType max_var = 0;
  for ( auto var: support_list ) {
    max_var = std::max(max_var, var);
  }
  return max_var + 1;
}

// @brief BDDを返す．
Bdd
FuncImpl_Bdd::bdd() const
{
  return mBdd;
}

// @brief 内容を出力する．
void
FuncImpl_Bdd::print(
  ostream& s
) const
{
}

END_NAMESPACE_YM_BN
