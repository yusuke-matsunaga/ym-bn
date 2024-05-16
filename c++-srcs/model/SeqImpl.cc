
/// @file SeqImpl.cc
/// @brief SeqImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "SeqImpl.h"
#include "SeqImpl_sub.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス SeqImpl
//////////////////////////////////////////////////////////////////////

// @brief データ入力ノードを返す．
SizeType
SeqImpl::data_src() const
{
  throw std::invalid_argument{"neither dff nor latch type"};
  return BAD_ID;
}

// @brief クロック入力ノードを返す．
SizeType
SeqImpl::clock() const
{
  throw std::invalid_argument{"not a dff type"};
  return BAD_ID;
}

// @brief イネーブル入力ノードを返す．
SizeType
SeqImpl::enable() const
{
  throw std::invalid_argument{"not a latch type"};
  return BAD_ID;
}

// @brief クリア入力ノードを返す．
SizeType
SeqImpl::clear() const
{
  throw std::invalid_argument{"neither dff nor latch type"};
  return BAD_ID;
}

// @brief プリセット入力ノードを返す．
SizeType
SeqImpl::preset() const
{
  throw std::invalid_argument{"neither dff nor latch type"};
  return BAD_ID;
}

// @brief クリアとプリセットが衝突したときの挙動を返す．
char
SeqImpl::rsval() const
{
  throw std::invalid_argument{"neither dff nor latch type"};
  return ' ';
}

// @brief データ出力ノードを返す．
SizeType
SeqImpl::data_output() const
{
  throw std::invalid_argument{"neither dff nor latch type"};
  return BAD_ID;
}

// @brief セルを返す．
ClibCell
SeqImpl::cell() const
{
  throw std::invalid_argument{"not a cell type"};
  return {};
}

// @brief セルのピンに対応するノードを返す．
SizeType
SeqImpl::cell_pin(
  SizeType pos
) const
{
  throw std::invalid_argument{"not a cell type"};
  return BAD_ID;
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_FL
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_FL::SeqImpl_FL(
  SizeType output_id,
  char rs_val
) : mDataOutput{output_id},
    mRsVal{rs_val}
{
}

// @brief デストラクタ
SeqImpl_FL::~SeqImpl_FL()
{
}

// @brief データ入力ノードを返す．
SizeType
SeqImpl_FL::data_src() const
{
  return mDataSrc;
}

// @brief クリア入力ノードを返す．
SizeType
SeqImpl_FL::clear() const
{
  return mClear;
}

// @brief プリセット入力ノードを返す．
SizeType
SeqImpl_FL::preset() const
{
  return mPreset;
}

// @brief データ出力ノードを返す．
SizeType
SeqImpl_FL::data_output() const
{
  return mDataOutput;
}

// @brief クリアとプリセットが衝突したときの挙動を返す．
char
SeqImpl::rsval() const
{
  return mRsVal;
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_DFF
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_DFF::SeqImpl_DFF(
  SizeType output_id,
  char rs_val
) : SeqImpl_FL{output_id, rs_val}
{
}

// @brief デストラクタ
SeqImpl_DFF::~SeqImpl_DFF()
{
}

// @brief 種類を返す．
BnSeqType
SeqImpl_DFF::type() const
{
  return BnSeqType::DFF;
}

// @brief クロック入力ノードを返す．
SizeType
SeqImpl_DFF::clock() const
{
  return mClock;
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_Latch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_Latch::SeqImpl_Latch(
  SizeType output_id,
  char rs_val
) : SeqImpl_FL{output_id, rs_val}
{
}

// @brief デストラクタ
SeqImpl_Latch::~SeqImpl_Latch()
{
}

// @brief 種類を返す．
BnSeqType
SeqImpl_Latch::type() const
{
  return BnSeqType::LATCH;
}

// @brief イネーブル入力ノードを返す．
SizeType
SeqImpl_Latch::enable() const
{
  return mEnable;
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_Cell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_Cell::SeqImpl_Cell(
  ClibCell cell
) : mCell{cell}
{
}


// @brief デストラクタ
SeqImpl_Cell::~SeqImpl_Cell()
{
}

// @brief 種類を返す．
BnSeqType
SeqImpl_Cell::type() const
{
  return BnSeqType::CELL;
}

// @brief セルを返す．
ClibCell
SeqImpl_Cell::cell() const
{
  return mCell;
}

// @brief セルのピンに対応するノードを返す．
SizeType
SeqImpl_Cell::cell_pin(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < mPinList.size() );
  return mPinList[pos];
}

END_NAMESPACE_YM_BN
