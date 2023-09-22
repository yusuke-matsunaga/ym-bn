
/// @file BfNode.cc
/// @brief BfNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BfNode.h"
#include "ym/BfModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNFE

//////////////////////////////////////////////////////////////////////
// クラス BfNode
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
BfNode::~BfNode()
{
}

// @brief 親の BfModel を返す．
BfModel
BfNode::parent_model() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BfNode: invalid data"};
  }
  return BfModel{mImpl};
}

// @brief ノード名を返す．
const string&
BfNode::name() const
{
  return node_impl().name();
}

// @brief ノードの種類を返す．
BfNodeType
BfNode::type() const
{
  return node_impl().type();
}

// @brief 入力ノードの時 true を返す．
bool
BfNode::is_input() const
{
  return type() == BfNodeType::Input;
}

// @brief 論理ノードの時 true を返す．
bool
BfNode::is_logic() const
{
  auto type = this->type();
  return type == BfNodeType::Prim ||
    type == BfNodeType::Aig ||
    type == BfNodeType::Cover ||
    type == BfNodeType::Expr ||
    type == BfNodeType::Cell;
}

// @brief PRIM タイプの論理ノードの時 true を返す．
bool
BfNode::is_primitive() const
{
  return type() == BfNodeType::Prim;
}

// @brief AIG タイプの論理ノードの時 true を返す．
bool
BfNode::is_aig() const
{
  return type() == BfNodeType::Aig;
}

// @brief COVER タイプの論理ノードの時 true を返す．
bool
BfNode::is_cover() const
{
  return type() == BfNodeType::Cover;
}

// @brief EXPR タイプの論理ノードの時 true を返す．
bool
BfNode::is_expr() const
{
  return type() == BfNodeType::Expr;
}

// @brief CELL タイプの論理ノードの時 true を返す．
bool
BfNode::is_cell() const
{
  return type() == BfNodeType::Cell;
}

// @brief FUNC タイプの論理ノードの時 true を返す．
bool
BfNode::is_func() const
{
  return type() == BfNodeType::TvFunc;
}

// @brief BDD タイプの論理ノードの時 true を返す．
bool
BfNode::is_bdd() const
{
  return type() == BfNodeType::Bdd;
}

// @brief DFFノードの時 true を返す．
bool
BfNode::is_dff() const
{
  return type() == BfNodeType::Dff;
}

// @brief 入力番号を返す．
SizeType
BfNode::input_id() const
{
  return node_impl().input_id();
}

// @brief ノードのファンイン数を返す．
SizeType
BfNode::fanin_num() const
{
  return node_impl().fanin_num();
}

// @brief ノードのファンインのノードを返す．
BfNode
BfNode::fanin(
  SizeType pos
) const
{
  return from_id(node_impl().fanin(pos));
}

// @brief ノードのファンインのノードのリストを返す．
vector<BfNode>
BfNode::fanin_list() const
{
  return from_id_list(node_impl().fanin_list());
}

// @brief ノードのプリミティブタイプを返す．
PrimType
BfNode::primitive_type() const
{
  return node_impl().primitive_type();
}

// @brief ファンインの反転属性を返す．
bool
BfNode::fanin_inv(
  SizeType pos
) const
{
  return node_impl().fanin_inv(pos);
}

// @brief カバー番号を返す．
SizeType
BfNode::cover_id() const
{
  return node_impl().cover_id();
}

// @brief ノードのカバーを返す．
const BfCover&
BfNode::cover() const
{
  auto id = cover_id();
  return mImpl->cover(id);
}

// @brief 論理式番号を返す．
SizeType
BfNode::expr_id() const
{
  return node_impl().expr_id();
}

// @brief 論理式を返す．
const Expr&
BfNode::expr() const
{
  auto id = expr_id();
  return mImpl->expr(id);
}

// @brief ノードのセル番号を返す．
SizeType
BfNode::cell_id() const
{
  return node_impl().cell_id();
}

// @brief ノードの関数番号を返す．
SizeType
BfNode::func_id() const
{
  return node_impl().func_id();
}

// @brief ノードのBDD番号を返す．
SizeType
BfNode::bdd_id() const
{
  return node_impl().bdd_id();
}

// @brief DFFノードの入力ノードを返す．
BfNode
BfNode::dff_src() const
{
  return from_id(node_impl().dff_src());
}

// @brief DFFノードのリセット値を返す．
char
BfNode::dff_rval() const
{
  return node_impl().dff_rval();
}

// @brief ノードの実体を返す．
const NodeImpl&
BfNode::node_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BfNode: invalid data"};
  }
  return mImpl->node(mId);
}

// @brief ID 番号から BfNode を作る．
BfNode
BfNode::from_id(
  SizeType id
) const
{
  return BfNode{mImpl, id};
}

// @brief ID 番号のリストから vector<BfNode> を作る．
vector<BfNode>
BfNode::from_id_list(
  const vector<SizeType>& id_list
) const
{
  vector<BfNode> node_list;
  for ( auto id: id_list ) {
    node_list.push_back(from_id(id));
  }
  return node_list;
}

END_NAMESPACE_YM_BNFE
