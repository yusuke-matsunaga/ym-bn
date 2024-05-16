
/// @file BnSeq.cc
/// @brief BnSeq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnSeq.h"
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnSeq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnSeq::BnSeq(
  const shared_ptr<const ModelImpl>& model,
  SizeType id
) : mModel{model},
    mId{id}
{
  if ( id == BAD_ID ) {
    mModel = nullptr;
  }
}

// @brief デストラクタ
BnSeq::~BnSeq()
{
}

// @brief 名前を返す．
string
BnSeq::name() const
{
  return mModel->seq_name(mId);
}

// @brief 種類を返す．
BnSeqType
BnSeq::type() const
{
  return _impl().type();
}

// @brief DFFタイプの時に true を返す．
bool
BnSeq::is_dff() const
{
  return _impl().is_dff();
}

// @brief ラッチタイプの時に true を返す．
bool
BnSeq::is_latch() const
{
  return _impl().is_latch();
}

// @brief セルタイプの時に true を返す．
bool
BnSeq::is_cell() const
{
  return _impl().is_cell();
}

// @brief データ入力ノードを返す．
BnNode
BnSeq::data_src() const
{
  return ModelImpl::new_node(mModel, _impl().data_src());
}

// @brief クロック入力ノードを返す．
BnNode
BnSeq::clock() const
{
  return ModelImpl::new_node(mModel, _impl().clock());
}

// @brief イネーブル入力ノードを返す．
BnNode
BnSeq::enable() const
{
  return ModelImpl::new_node(mModel, _impl().enable());
}

// @brief クリア入力ノードを返す．
BnNode
BnSeq::clear() const
{
  return ModelImpl::new_node(mModel, _impl().clear());
}

// @brief プリセット入力ノードを返す．
BnNode
BnSeq::preset() const
{
  return ModelImpl::new_node(mModel, _impl().preset());
}

// @brief クリアとプリセットが衝突したときの挙動を返す．
char
BnSeq::rsval() const
{
  return _impl().rsval();
}

// @brief データ出力ノードを返す．
BnNode
BnSeq::data_output() const
{
  return ModelImpl::new_node(mModel, _impl().data_output());
}

// @brief セルを返す．
ClibCell
BnSeq::cell() const
{
  return _impl().cell();
}

// @brief セルのピンに対応するノードを返す．
BnNode
BnSeq::cell_pin(
  SizeType pos
) const
{
  return ModelImpl::new_node(mModel, _impl().cell_pin(pos));
}

// @brief 実体を返す．
const SeqImpl&
BnSeq::_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnSeq: invalid data"};
  }
  return mModel->seq_impl(mId);
}

END_NAMESPACE_YM_BN
