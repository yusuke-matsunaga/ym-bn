
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ym/BnModel.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
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

// @brief ノードの種類を返す．
BnNodeType
BnNode::type() const
{
  return _impl().type();
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  return _impl().is_input();
}

// @brief BnSeq の出力ノードの時 true を返す．
bool
BnNode::is_seq_output() const
{
  return _impl().is_seq_output();
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  return _impl().is_logic();
}

// @brief PRIM タイプの論理ノードの時 true を返す．
bool
BnNode::is_primitive() const
{
  return _impl().is_primitive();
}

// @brief AIG タイプの論理ノードの時 true を返す．
bool
BnNode::is_aig() const
{
  return type() == BnNodeType::AIG;
}

// @brief 関数タイプの論理ノードの時 true を返す．
bool
BnNode::is_func() const
{
  return type() == BnNodeType::FUNC;
}

// @brief CELL タイプの論理ノードの時 true を返す．
bool
BnNode::is_cell() const
{
  return type() == BnNodeType::CELL;
}

// @brief 入力番号を返す．
SizeType
BnNode::input_id() const
{
  return _impl().input_id();
}

// @brief 関連する BnSeq を返す．
BnSeq
BnNode::seq_node() const
{
  return BnSeq{mImpl, _impl().seq_id()};
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  return _impl().fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  return from_id(_impl().fanin(pos));
}

// @brief ノードのファンインのノードのリストを返す．
vector<BnNode>
BnNode::fanin_list() const
{
  return from_id_list(_impl().fanin_list());
}

// @brief ノードのプリミティブタイプを返す．
PrimType
BnNode::primitive_type() const
{
  return _impl().primitive_type();
}

// @brief ファンインの反転属性を返す．
bool
BnNode::fanin_inv(
  SizeType pos
) const
{
  return _impl().fanin_inv(pos);
}

// @brief ノードの関数情報を返す．
BnFunc
BnNode::local_func() const
{
  return BnFunc{mImpl, _impl().local_func_id()};
}

// @brief ノードのセルを返す．
ClibCell
BnNode::cell() const
{
  auto cell_id = _impl().cell_id();
  return mImpl->library().cell(cell_id);
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::_impl() const
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
