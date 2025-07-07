
/// @file FuncImpl_TvFunc.cc
/// @brief FuncImpl_TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_TvFunc.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief 真理値表型のインスタンスを作る．
FuncImpl*
FuncImpl::new_tvfunc(
  const TvFunc& func
)
{
  return new FuncImpl_TvFunc(func);
}


//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_TvFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_TvFunc::FuncImpl_TvFunc(
  const TvFunc& func
) : mTvFunc{func}
{
}

// @brief コピーを作る．
std::unique_ptr<FuncImpl>
FuncImpl_TvFunc::copy(
  BddMgr& bdd_mgr
) const
{
  return std::unique_ptr<FuncImpl>{new FuncImpl_TvFunc{*this}};
}

// @brief 関数の種類を返す．
BnFuncType
FuncImpl_TvFunc::type() const
{
  return BnFuncType::TVFUNC;
}

// @brief 真理値表型の論理ノードの時 true を返す．
bool
FuncImpl_TvFunc::is_tvfunc() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_TvFunc::input_num() const
{
  return mTvFunc.input_num();
}

// @brief 真理値表を返す．
const TvFunc&
FuncImpl_TvFunc::tvfunc() const
{
  return mTvFunc;
}

// @brief 内容を出力する．
void
FuncImpl_TvFunc::print(
  ostream& s
) const
{
  s << "TvFunc" << endl
    << mTvFunc << endl;
}

END_NAMESPACE_YM_BN
