
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

BEGIN_NONAMESPACE

// @brief 範囲チェックを行う
inline
void
_check_range(
  SizeType index,
  SizeType range,
  const char* index_name
)
{
  if ( index < 0 || range <= index ) {
    ostringstream buf;
    buf << "'" << index_name << "'(" << index << ") is out-of-range";
    throw std::out_of_range{buf.str()};
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief BnNode のチェックを行う．
void
BnModel::_check_node(
  const BnNode& node
) const
{
  if ( !node.is_valid() ) {
    throw std::invalid_argument{"'node' is invalid."};
  }
  if ( !mImpl->check(node) ) {
    throw std::invalid_argument{"'node' does not belong to this model."};
  }
}

// @brief BnSeq のチェックを行う．
void
BnModel::_check_seq(
  const BnSeq& seq
) const
{
  if ( !seq.is_valid() ) {
    throw std::invalid_argument{"'seq' is invalid."};
  }
  if ( !mImpl->check(seq) ) {
    throw std::invalid_argument{"'seq' does not belong to this model."};
  }
}

// @brief BnFunc のチェックを行う．
void
BnModel::_check_func(
  const BnFunc& func
) const
{
  if ( !func.is_valid() ) {
    throw std::invalid_argument{"'func' is invalid."};
  }
  if ( !mImpl->check(func) ) {
    throw std::invalid_argument{"'func' does not belong to this model."};
  }
}

// @brief 入力番号のチェックを行う．
void
BnModel::_check_input_id(
  SizeType input_id
) const
{
  _check_range(input_id, input_num(), "input_id");
}

// @brief 出力番号のチェックを行う．
void
BnModel::_check_output_id(
  SizeType output_id
) const
{
  _check_range(output_id, output_num(), "output_id");
}

// @brief 論理ノード番号のチェックを行う．
void
BnModel::_check_logic_id(
  SizeType logic_id
) const
{
  _check_range(logic_id, logic_num(), "logic_id");
}

// @brief SEQ番号のチェックを行う．
void
BnModel::_check_seq_id(
  SizeType seq_id
) const
{
  _check_range(seq_id, seq_num(), "seq_id");
}

// @brief DFF/latch 用の rs_val が正しいかチェックする．
void
BnModel::_check_rsval(
  char rsval
) const
{
  switch ( rsval ) {
  case ' ':
  case '0':
  case '1':
  case 'X':
  case 'T':
  case 'U':
    return;
  }
  throw std::invalid_argument{"'rsval' is invalid."};
}

END_NAMESPACE_YM_BN
