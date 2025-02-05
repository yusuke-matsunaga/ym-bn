
/// @file BnFunc.cc
/// @brief BnFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnFunc.h"
#include "ym/BnModel.h"
#include "ym/Expr.h"
#include "ym/AlgCover.h"
#include "ym/Bdd.h"
#include "FuncImpl.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief 内容を指定したコンストラクタ
BnFunc::BnFunc(
  const shared_ptr<const ModelImpl>& model,
  SizeType id
) : mModel{model},
    mId{id}
{
  if ( mId == BAD_ID ) {
    mModel = nullptr;
  }
}

// @brief デストラクタ
BnFunc::~BnFunc()
{
}

// @brief 種類を返す．
BnFuncType
BnFunc::type() const
{
  return _impl().type();
}

// @brief カバー型の時 true を返す．
bool
BnFunc::is_cover() const
{
  return _impl().is_cover();
}

// @brief 論理式型の時 true を返す．
bool
BnFunc::is_expr() const
{
  return _impl().is_expr();
}

// @brief 真理値表型の時 true を返す．
bool
BnFunc::is_tvfunc() const
{
  return _impl().is_tvfunc();
}

// @brief BDD型の時 true を返す．
bool
BnFunc::is_bdd() const
{
  return _impl().is_bdd();
}

// @brief 入力数を返す．
SizeType
BnFunc::input_num() const
{
  return _impl().input_num();
}

// @brief 入力カバーを返す．
const AlgCover&
BnFunc::input_cover() const
{
  return _impl().input_cover();
}

// @brief 出力パタンを返す．
char
BnFunc::output_pat() const
{
  return _impl().output_pat();
}

// @brief 論理式を返す．
Expr
BnFunc::expr() const
{
  return _impl().expr();
}

// @brief 真理値表を返す．
const TvFunc&
BnFunc::tvfunc() const
{
  return _impl().tvfunc();
}

// @brief BDDを返す．
Bdd
BnFunc::bdd() const
{
  return _impl().bdd();
}

// @brief 内容を出力する．
void
BnFunc::print(
  ostream& s
) const
{
  _impl().print(s);
}

// @brief 実装を取り出す．
const FuncImpl&
BnFunc::_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnFunc: invalid data"};
  }
  return mModel->func_impl(mId);
}

END_NAMESPACE_YM_BN
