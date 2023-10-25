
/// @file BnDff.cc
/// @brief BnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnDff.h"
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnDff::BnDff(
  const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
  SizeType id                        ///< [in] DFF番号
) : mImpl{impl},
    mId{id}
{
  if ( id == BAD_ID ) {
    mImpl = nullptr;
  }
}

// @brief デストラクタ
BnDff::~BnDff()
{
}

// @brief 親の BnModel を返す．
BnModel
BnDff::parent_model() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnDff:: invalid data"};
  }
  return BnModel{mImpl};
}

// @brief 名前を返す．
const string&
BnDff::name() const
{
  return dff_impl().name();
}

// @brief 種類を返す．
BnDffType
BnDff::type() const
{
  return dff_impl().type();
}

// @brief DFFタイプの時に true を返す．
bool
BnDff::is_dff() const
{
  return dff_impl().is_dff();
}

// @brief ラッチタイプの時に true を返す．
bool
BnDff::is_latch() const
{
  return dff_impl().is_latch();
}

// @brief セルタイプの時に true を返す．
bool
BnDff::is_cell() const
{
  return dff_impl().is_cell();
}

// @brief データ入力ノードを返す．
BnNode
BnDff::data_src() const
{
  return from_id(dff_impl().data_src());
}

// @brief クロック入力ノードを返す．
BnNode
BnDff::clock() const
{
  return from_id(dff_impl().clock());
}

// @brief イネーブル入力ノードを返す．
BnNode
BnDff::enable() const
{
  return from_id(dff_impl().enable());
}

// @brief クリア入力ノードを返す．
BnNode
BnDff::clear() const
{
  return from_id(dff_impl().clear());
}

// @brief プリセット入力ノードを返す．
BnNode
BnDff::preset() const
{
  return from_id(dff_impl().preset());
}

// @brief クリアとプリセットが衝突したときの挙動を返す．
char
BnDff::rsval() const
{
  return dff_impl().rsval();
}

// @brief データ出力ノードを返す．
BnNode
BnDff::data_output() const
{
  return from_id(dff_impl().data_output());
}

// @brief セル番号を返す．
SizeType
BnDff::cell_id() const
{
  return dff_impl().cell_id();
}

// @brief セルのピンに対応するノードを返す．
BnNode
BnDff::cell_pin(
  SizeType pos
) const
{
  return from_id(dff_impl().cell_pin(pos));
}

// @brief 実体を返す．
const DffImpl&
BnDff::dff_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnDff: invalid data"};
  }
  return mImpl->dff(mId);
}

// @brief ID 番号から BnNode を作る．
BnNode
BnDff::from_id(
  SizeType id
) const
{
  return BnNode{mImpl, id};
}


END_NAMESPACE_YM_BN
