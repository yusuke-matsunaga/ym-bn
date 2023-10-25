
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
  return node_impl().is_input();
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  return node_impl().is_logic();
}

// @brief PRIM タイプの論理ノードの時 true を返す．
bool
BnNode::is_primitive() const
{
  return node_impl().is_primitive();
}

// @brief AIG タイプの論理ノードの時 true を返す．
bool
BnNode::is_aig() const
{
  return type() == BnNodeType::AIG;
}

// @brief COVER タイプの論理ノードの時 true を返す．
bool
BnNode::is_cover() const
{
  return type() == BnNodeType::COVER;
}

// @brief EXPR タイプの論理ノードの時 true を返す．
bool
BnNode::is_expr() const
{
  return type() == BnNodeType::EXPR;
}

// @brief CELL タイプの論理ノードの時 true を返す．
bool
BnNode::is_cell() const
{
  return type() == BnNodeType::CELL;
}

// @brief FUNC タイプの論理ノードの時 true を返す．
bool
BnNode::is_func() const
{
  return type() == BnNodeType::TVFUNC;
}

// @brief BDD タイプの論理ノードの時 true を返す．
bool
BnNode::is_bdd() const
{
  return type() == BnNodeType::BDD;
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

// @brief ノードのセルを返す．
ClibCell
BnNode::cell() const
{
  auto library = mImpl->library();
  return library.cell(cell_id());
}

// @brief ノードの関数番号を返す．
SizeType
BnNode::func_id() const
{
  return node_impl().func_id();
}

// @brief 関数を返す．
const TvFunc&
BnNode::func() const
{
  auto id = func_id();
  return mImpl->func(id);
}

// @brief ノードのBDD番号を返す．
SizeType
BnNode::bdd_id() const
{
  return node_impl().bdd_id();
}

// @brief BDDを返す．
Bdd
BnNode::bdd() const
{
  auto id = bdd_id();
  return mImpl->bdd(id);
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
