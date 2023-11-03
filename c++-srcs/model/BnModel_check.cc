
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnSeq.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief 正しいカバー番号かチェックする．
void
BnModel::_check_cover(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->cover_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid cover-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief カバーと入力のサイズが合っているか調べる．
void
BnModel::_check_cover_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& cover = mImpl->cover(id);
  if ( cover.input_num() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しい論理式番号かチェックする．
void
BnModel::_check_expr(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->expr_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid expr-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 論理式と入力のサイズが合っているか調べる．
void
BnModel::_check_expr_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& expr = mImpl->expr(id);
  if ( expr.input_size() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しい関数番号かチェックする．
void
BnModel::_check_func(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->func_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid func-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 関数と入力のサイズが合っているか調べる．
void
BnModel::_check_func_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& func = mImpl->func(id);
  if ( func.input_num() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しいBDD番号かチェックする．
void
BnModel::_check_bdd(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->bdd_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid bdd-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief BDDと入力のサイズが合っているか調べる．
void
BnModel::_check_bdd_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& bdd = mImpl->bdd(id);
  if ( bdd.support_size() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 本体に登録されているライブラリとセルが属するライブラリが等しいか調べる．
void
BnModel::_check_library(
  ClibCell cell,
  const string& funcname
) const
{
  if ( library() != cell.library() ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "cell library mismatch";
    throw std::invalid_argument{buf.str()};
  }
}

END_NAMESPACE_YM_BN
