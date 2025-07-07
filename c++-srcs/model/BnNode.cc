
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
  const std::shared_ptr<const ModelImpl>& model,
  SizeType id
) : mModel{model},
    mId{id}
{
  if ( mModel.get() == nullptr ) {
    mId = BAD_ID;
  }
  else if ( mId == BAD_ID ) {
    mModel = nullptr;
  }
}

// @brief デストラクタ
BnNode::~BnNode()
{
}

// @brief ノードの種類を返す．
BnNode::Type
BnNode::type() const
{
  return _impl()->type();
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  return _impl()->is_input();
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  return _impl()->is_logic();
}

// @brief 外部入力ノードの時 true を返す．
bool
BnNode::is_primary_input() const
{
  return _impl()->is_primary_input();
}

// @brief DFFの出力ノードの時 true を返す．
bool
BnNode::is_dff_output() const
{
  return _impl()->is_dff_output();
}

// @brief 入力番号を返す．
SizeType
BnNode::input_id() const
{
  return _impl()->input_id();
}

// @brief DFF番号を返す．
SizeType
BnNode::dff_id() const
{
  return _impl()->dff_id();
}

// @brief DFFの入力ノードを返す．
BnNode
BnNode::dff_src() const
{
  return _impl()->dff_src();
}

// @brief プリミティブ型を返す．
PrimType
BnNode::primitive_type() const
{
  return _impl()->primitive_type();
}

// @brief カバー情報を持っている時 true を返す．
bool
BnNode::has_cover() const
{
  return _impl()->has_cover();
}

// @brief カバーを返す．
const SopCover&
BnNode::cover() const
{
  return _impl()->cover();
}

// @brief 論理式情報を持っている時 true を返す．
bool
BnNode::has_expr() const
{
  return _impl()->has_expr();
}

// @brief 論理式を返す．
Expr
BnNode::expr() const
{
  return _impl()->expr();
}

// @brief 倫理値表を持っている時 true を返す．
bool
BnNode::has_tvfunc() const
{
  return _impl()->has_tvfunc();
}

// @brief 真理値表を返す．
const TvFunc&
BnNode::tvfunc() const
{
  return _impl()->tvfunc();
}

// @brief BDDを持っている時 true を返す．
bool
BnNode::has_bdd() const
{
  return _impl()->has_bdd();
}

// @brief BDDを返す．
Bdd
BnNode::bdd() const
{
  return _impl()->bdd();
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  return _impl()->fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  auto id = _impl()->fanin(pos);
  return BnNode(mModel, id);
}

// @brief ノードのファンインのノードのリストを返す．
std::vector<BnNode>
BnNode::fanin_list() const
{
  auto& id_list = _impl()->fanin_list();
  std::vector<BnNode> node_list;
  node_list.reserve(id_list.size());
  for ( auto id: id_list ) {
    node_list.push_back(BnNode(mModel, id));
  }
  return node_list;
}

// @brief ノードの実体を返す．
const NodeImpl*
BnNode::_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnNode: invalid data"};
  }
  return mModel->node_impl(mId);
}

END_NAMESPACE_YM_BN
