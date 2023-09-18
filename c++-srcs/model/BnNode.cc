
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
  return impl().name();
}

// @brief ノードの種類を返す．
BnNodeType
BnNode::type() const
{
  return impl().type();
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  return type() == BnNodeType::Input;
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  auto type = this->type();
  return type == BnNodeType::Prim ||
    type == BnNodeType::Aig ||
    type == BnNodeType::Cover ||
    type == BnNodeType::Expr ||
    type == BnNodeType::Cell;
}

// @brief PRIM タイプの論理ノードの時 true を返す．
bool
BnNode::is_primitive() const
{
  return type() == BnNodeType::Prim;
}

// @brief AIG タイプの論理ノードの時 true を返す．
bool
BnNode::is_aig() const
{
  return type() == BnNodeType::Aig;
}

// @brief COVER タイプの論理ノードの時 true を返す．
bool
BnNode::is_cover() const
{
  return type() == BnNodeType::Cover;
}

// @brief EXPR タイプの論理ノードの時 true を返す．
bool
BnNode::is_expr() const
{
  return type() == BnNodeType::Expr;
}

// @brief CELL タイプの論理ノードの時 true を返す．
bool
BnNode::is_cell() const
{
  return type() == BnNodeType::Cell;
}

// @brief DFFノードの時 true を返す．
bool
BnNode::is_dff() const
{
  return type() == BnNodeType::Dff;
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  return impl().fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  return from_id(impl().fanin(pos));
}

// @brief ノードのファンインのノードのリストを返す．
vector<BnNode>
BnNode::fanin_list() const
{
  return from_id_list(impl().fanin_list());
}

// @brief ノードのプリミティブタイプを返す．
PrimType
BnNode::primitive_type() const
{
  return impl().primitive_type();
}

// @brief ファンインの反転属性を返す．
bool
BnNode::fanin_inv(
  SizeType pos
) const
{
  return impl().fanin_inv(pos);
}

// @brief カバー番号を返す．
SizeType
BnNode::cover_id() const
{
  return impl().cover_id();
}

// @brief ノードのカバーを返す．
const BnCover&
BnNode::cover() const
{
  auto id = cover_id();
  return mImpl->cover(id);
}

// @brief 論理式番号を返す．
SizeType
BnNode::expr_id() const
{
  return impl().expr_id();
}

// @brief 論理式を返す．
const Expr&
BnNode::expr() const
{
  auto id = expr_id();
  return mImpl->expr(id);
}

// @brief ノードのセル番号を返す．
SizeType
BnNode::cell_id() const
{
  return impl().cell_id();
}

// @brief DFFノードの入力ノードを返す．
BnNode
BnNode::dff_src() const
{
  return from_id(impl().dff_src());
}

// @brief DFFノードのリセット値を返す．
char
BnNode::dff_rval() const
{
  return impl().dff_rval();
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode: invalid data"};
  }
  return mImpl->node(mId);
}

// @brief ID 番号から BnNode を作る．
BnNode
BnNode::from_id(
  SizeType id
) const
{
  return BnNode{mImpl, id};
}

// @brief ID 番号のリストから vector<BnNode> を作る．
vector<BnNode>
BnNode::from_id_list(
  const vector<SizeType>& id_list
) const
{
  vector<BnNode> node_list;
  for ( auto id: id_list ) {
    node_list.push_back(from_id(id));
  }
  return node_list;
}

END_NAMESPACE_YM_BNET
