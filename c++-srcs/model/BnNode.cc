
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
// クラス BnNode
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
BnNode::~BnNode()
{
}

// @brief ノード名を返す．
const string&
BnNode::name() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::name(): invalid data"};
  }
  return mImpl->node_name(mId);
}

// @brief ノードの種類を返す．
BnNodeType
BnNode::type() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::type(): invalid data"};
  }
  return mImpl->node_type(mId);
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_input(): invalid data"};
  }
  return mImpl->node_type(mId) == BnNodeType::Input;
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_logic(): invalid data"};
  }
  auto type = mImpl->node_type(mId);
  return type == BnNodeType::Prim || type == BnNodeType::Cover || type == BnNodeType == Cell;
}

// @brief PRIM タイプの論理ノードの時 true を返す．
bool
BnNode::is_primitive() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_primitive(): invalid data"};
  }
  return mImpl->node_type(mId) == BnNodeType::Prim;
}

// @brief COVER タイプの論理ノードの時 true を返す．
bool
BnNode::is_cover() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_cover(): invalid data"};
  }
  return mImpl->node_type(mId) == BnNode::Cover;
}

// @brief CELL タイプの論理ノードの時 true を返す．
bool
BnNode::is_cell() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_cell(): invalid data"};
  }
  return mImpl->node_type(mId) == BnNodeType::Cell;
}

// @brief DFFノードの時 true を返す．
bool
BnNode::is_dff() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::is_dff(): invalid data"};
  }
  return mImpl->node_type(mId) == BnNodeType::Dff;
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::fanin_num(): invalid data"};
  }
  return mImpl->node_fanin_num(mId);
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::fanin(): invalid data"};
  }
  return from_id(mImpl->node_fanin(mId, pos));
}

// @brief ノードのファンインのノードのリストを返す．
vector<BnNode>
BnNode::fanin_list() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::fanin_list(): invalid data"};
  }
  return from_id_list(mImpl->node_fanin_list(mId));
}

// @brief ノードのプリミティブタイプを返す．
PrimType
BnNode::prim_type() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::prim_type(): invalid data"};
  }
  return mImpl->node_prim_type(mId);
}

// @brief ノードのカバーを返す．
const BnCover&
BnNode::cover() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::cover(): invalid data"};
  }
  return mImpl->node_cover(mId);
}

// @brief ノードのセル番号を返す．
SizeType
BnNode::cell_id() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::cell_id(): invalid data"};
  }
  return mImpl->node_cell_id(mId);
}

// @brief DFFノードの入力ノードを返す．
BnNode
BnNode::dff_src() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::dff_src(): invalid data"};
  }
  return mImpl->node_dff_src(mId);
}

// @brief DFFノードのリセット値を返す．
char
BnNode::dff_rval() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode::dff_rval(): invalid data"};
  }
  return mImpl->node_dff_rval(mId);
}

END_NAMESPACE_YM_BNET
