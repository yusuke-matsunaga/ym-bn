
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
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
BnNode::~BnNode()
{
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
  auto id = _impl().seq_id();
  return ModelImpl::new_seq(mModel, id);
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
  auto id = _impl().fanin(pos);
  return ModelImpl::new_node(mModel, id);
}

// @brief ノードのファンインのノードのリストを返す．
BnNodeList
BnNode::fanin_list() const
{
  return ModelImpl::new_node_list(mModel, _impl().fanin_list());
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
  auto id = _impl().local_func_id();
  return ModelImpl::new_func(mModel, id);
}

// @brief ノードのセルを返す．
ClibCell
BnNode::cell() const
{
  auto cell_id = _impl().cell_id();
  return mModel->library().cell(cell_id);
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnNode: invalid data"};
  }
  return mModel->node_impl(mId);
}


//////////////////////////////////////////////////////////////////////
// クラス BnNodeListIter
//////////////////////////////////////////////////////////////////////

// @brief 内容を取り出す．
BnNode
BnNodeListIter::operator*() const
{
  return ModelImpl::new_node(mModel, *mIter);
}


//////////////////////////////////////////////////////////////////////
// クラス BnNodeList
//////////////////////////////////////////////////////////////////////

// @brief 要素を返す．
BnNode
BnNodeList::operator[](
  SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
) const
{
  if ( pos < 0 || size() <= pos ) {
    ostringstream buf;
    buf << "'pos'(" << pos << ") is out of range.";
    throw std::out_of_range{buf.str()};
  }
  return ModelImpl::new_node(mModel, mIdList[pos]);
}

// @brief vector<BnNode> との等価比較関数
bool
BnNodeList::operator==(
  const vector<BnNode>& right
) const
{
  if ( size() != right.size() ) {
    return false;
  }
  SizeType n = size();
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( (*this)[i] != right[i] ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_BN
