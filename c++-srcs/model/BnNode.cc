
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnNode::BnNode(
  const shared_ptr<ModelImpl>& impl,
  SizeType id
) : mImpl{impl},
    mId{id}
{
  if ( mId == BAD_ID ) {
    mImpl = nullptr;
  }
}

// @brief デストラクタ
BnNode::~BnNode()
{
}

// @brief 親の BnModel を返す．
BnModel
BnNode::parent_model() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode: invalid data"};
  }
  return BnModel{mImpl};
}

// @brief ノード名を返す．
const string&
BnNode::name() const
{
  return node_impl().name();
}

// @brief ノードの種類を返す．
BnNodeType
BnNode::type() const
{
  return node_impl().type();
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

// @brief FUNC タイプの論理ノードの時 true を返す．
bool
BnNode::is_func() const
{
  return type() == BnNodeType::TvFunc;
}

// @brief BDD タイプの論理ノードの時 true を返す．
bool
BnNode::is_bdd() const
{
  return type() == BnNodeType::Bdd;
}

// @brief DFFノードの時 true を返す．
bool
BnNode::is_dff() const
{
  return type() == BnNodeType::Dff;
}

// @brief ラッチノードの時 true を返す．
bool
BnNode::is_latch() const
{
  return type() == BnNodeType::Latch;
}

// @brief 入力番号を返す．
SizeType
BnNode::input_id() const
{
  return node_impl().input_id();
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  return node_impl().fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  return from_id(node_impl().fanin(pos));
}

// @brief ノードのファンインのノードのリストを返す．
vector<BnNode>
BnNode::fanin_list() const
{
  return from_id_list(node_impl().fanin_list());
}

// @brief ノードのプリミティブタイプを返す．
PrimType
BnNode::primitive_type() const
{
  return node_impl().primitive_type();
}

// @brief ファンインの反転属性を返す．
bool
BnNode::fanin_inv(
  SizeType pos
) const
{
  return node_impl().fanin_inv(pos);
}

// @brief カバー番号を返す．
SizeType
BnNode::cover_id() const
{
  return node_impl().cover_id();
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
  return node_impl().expr_id();
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
  return node_impl().cell_id();
}

// @brief ノードの関数番号を返す．
SizeType
BnNode::func_id() const
{
  return node_impl().func_id();
}

// @brief ノードのBDD番号を返す．
SizeType
BnNode::bdd_id() const
{
  return node_impl().bdd_id();
}

// @brief DFFノードの入力ノードを返す．
BnNode
BnNode::dff_src() const
{
  return from_id(node_impl().dff_src());
}

// @brief DFFノードのクロックノードを返す．
BnNode
BnNode::dff_clock() const
{
  return from_id(node_impl().dff_clock());
}

// @brief DFFノードのリセットノードを返す．
BnNode
BnNode::dff_reset() const
{
  return from_id(node_impl().dff_reset());
}

// @brief DFFノードのプリセットノードを返す．
BnNode
BnNode::dff_preset() const
{
  return from_id(node_impl().dff_preset());
}

// @brief DFFノードのリセット値を返す．
char
BnNode::dff_rsval() const
{
  return node_impl().dff_rsval();
}

// @brief ラッチノードの入力ノードを返す．
BnNode
BnNode::latch_src() const
{
  return from_id(node_impl().latch_src());
}

// @brief ラッチノードのイネーブルノードを返す．
BnNode
BnNode::latch_enable() const
{
  return from_id(node_impl().latch_enable());
}

// @brief ラッチノードのリセットノードを返す．
BnNode
BnNode::latch_reset() const
{
  return from_id(node_impl().latch_reset());
}

// @brief ラッチノードのプリセットノードを返す．
BnNode
BnNode::latch_preset() const
{
  return from_id(node_impl().latch_preset());
}

// @brief ラッチノードのリセット値を返す．
char
BnNode::latch_rsval() const
{
  return node_impl().latch_rsval();
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::node_impl() const
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

END_NAMESPACE_YM_BN
