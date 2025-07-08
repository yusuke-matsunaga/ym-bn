
/// @file FuncImpl_Bdd.cc
/// @brief FuncImpl_Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief BDD型のインスタンスを作る．
FuncImpl*
FuncImpl::new_bdd(
  const Bdd& bdd
)
{
  return new FuncImpl_Bdd(bdd);
}


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
std::unique_ptr<FuncImpl>
FuncImpl_Bdd::copy(
  BddMgr& bdd_mgr
) const
{
  auto my_bdd = bdd_mgr.copy(mBdd);
  return std::unique_ptr<FuncImpl>{new FuncImpl_Bdd{my_bdd}};
}

// @brief 関数の種類を返す．
BnFunc::Type
FuncImpl_Bdd::type() const
{
  return BnFunc::Bdd;
}

// @brief BDD型の時 true を返す．
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
  return mBdd.support_size();
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
  s << "BDD" << endl;
  bdd().display(s);
}

END_NAMESPACE_YM_BN
