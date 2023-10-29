
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
  const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
  SizeType id                        ///< [in] ID番号
) : mImpl{impl},
    mId{id}
{
  if ( id == BAD_ID ) {
    mImpl = nullptr;
  }
}

// @brief デストラクタ
BnSeq::~BnSeq()
{
}

// @brief 親の BnModel を返す．
BnModel
BnSeq::parent_model() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnSeq:: invalid data"};
  }
  return BnModel{mImpl};
}

// @brief 名前を返す．
const string&
BnSeq::name() const
{
  return seq_impl().name();
}

// @brief 種類を返す．
BnSeqType
BnSeq::type() const
{
  return seq_impl().type();
}

// @brief DFFタイプの時に true を返す．
bool
BnSeq::is_dff() const
{
  return seq_impl().is_dff();
}

// @brief ラッチタイプの時に true を返す．
bool
BnSeq::is_latch() const
{
  return seq_impl().is_latch();
}

// @brief セルタイプの時に true を返す．
bool
BnSeq::is_cell() const
{
  return seq_impl().is_cell();
}

// @brief データ入力ノードを返す．
BnNode
BnSeq::data_src() const
{
  return from_id(seq_impl().data_src());
}

// @brief クロック入力ノードを返す．
BnNode
BnSeq::clock() const
{
  return from_id(seq_impl().clock());
}

// @brief イネーブル入力ノードを返す．
BnNode
BnSeq::enable() const
{
  return from_id(seq_impl().enable());
}

// @brief クリア入力ノードを返す．
BnNode
BnSeq::clear() const
{
  return from_id(seq_impl().clear());
}

// @brief プリセット入力ノードを返す．
BnNode
BnSeq::preset() const
{
  return from_id(seq_impl().preset());
}

// @brief クリアとプリセットが衝突したときの挙動を返す．
char
BnSeq::rsval() const
{
  return seq_impl().rsval();
}

// @brief データ出力ノードを返す．
BnNode
BnSeq::data_output() const
{
  return from_id(seq_impl().data_output());
}

// @brief セル番号を返す．
SizeType
BnSeq::cell_id() const
{
  return seq_impl().cell_id();
}

// @brief セルを返す．
ClibCell
BnSeq::cell() const
{
  auto id = cell_id();
  auto library = mImpl->library();
  return library.cell(id);
}

// @brief セルのピンに対応するノードを返す．
BnNode
BnSeq::cell_pin(
  SizeType pos
) const
{
  return from_id(seq_impl().cell_pin(pos));
}

// @brief 実体を返す．
const SeqImpl&
BnSeq::seq_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnSeq: invalid data"};
  }
  return mImpl->seq_node(mId);
}

// @brief ID 番号から BnNode を作る．
BnNode
BnSeq::from_id(
  SizeType id
) const
{
  return BnNode{mImpl, id};
}


END_NAMESPACE_YM_BN
