
/// @file BnBase.cc
/// @brief BnBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnBase.h"
#include "ym/BnNode.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief 空のコンストラクタ
BnBase::BnBase()
{
}

// @brief 内容を指定したコンストラクタ
BnBase::BnBase(
  const std::shared_ptr<ModelImpl>& impl
) : mImpl{impl}
{
}

// @brief 内容を指定したコンストラクタ
BnBase::BnBase(
  std::shared_ptr<ModelImpl>&& impl
) : mImpl{impl}
{
}

// @brief デストラクタ
BnBase::~BnBase()
{
}

// @brief ModelImpl のオブジェクトをセットする．
void
BnBase::_set_impl(
  ModelImpl* impl
)
{
  mImpl = std::shared_ptr<ModelImpl>{impl};
}

// @brief ノード番号を BnNode に変換する．
BnNode
BnBase::_id2node(
  SizeType id
) const
{
  return BnNode(mImpl, id);
}

// @brief ノード番号のリストを BnNode のリストに変換する．
std::vector<BnNode>
BnBase::_id2node_list(
  const std::vector<SizeType>& id_list
) const
{
  std::vector<BnNode> node_list;
  node_list.reserve(id_list.size());
  for ( auto id: id_list ) {
    node_list.push_back(_id2node(id));
  }
  return node_list;
}

// @brief BnNode をノード番号に変換する．
SizeType
BnBase::_node2id(
  const BnNode& node
) const
{
  if ( !_check_impl(node) ) {
    throw std::invalid_argument{"node does not belong to this Model"};
  }
  return node.id();
}

// @brief BnNode のリストをノード番号のりストに変換する．
std::vector<SizeType>
BnBase::_node2id_list(
  const std::vector<BnNode>& node_list
) const
{
  std::vector<SizeType> id_list;
  id_list.reserve(node_list.size());
  for ( auto& node: node_list ) {
    auto id = _node2id(node);
    id_list.push_back(id);
  }
  return id_list;
}

// @brief 関数番号を BnFunc に変換する．
BnFunc
BnBase::_id2func(
  SizeType id
) const
{
  return BnFunc(mImpl, id);
}

// @brief 同じ ModelImpl を持つオブジェクトかチェックする．
bool
BnBase::_check_impl(
  const BnBase& base
) const
{
  return mImpl == base.mImpl;
}

// @brief BnNode のチェックを行う．
void
BnBase::_check_node(
  const BnNode& node
) const
{
  if ( !node.is_valid() ) {
    throw std::invalid_argument{"'node' is invalid."};
  }
  if ( !_check_impl(node) ) {
    throw std::invalid_argument{"'node' does not belong to this model."};
  }
}

// @brief BnFunc のチェックを行う．
void
BnBase::_check_func(
  const BnFunc& func
) const
{
  if ( !func.is_valid() ) {
    throw std::invalid_argument{"'func' is invalid."};
  }
  if ( !_check_impl(func) ) {
    throw std::invalid_argument{"'func' does not belong to this model."};
  }
}

// @brief ModelImpl を返す．
const ModelImpl&
BnBase::_model_impl() const
{
  return *mImpl;
}

// @brief ModelImpl を返す．
ModelImpl&
BnBase::_model_impl()
{
  return *mImpl;
}

END_NAMESPACE_YM_BN
