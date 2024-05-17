
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

// @brief データ入力をセットする．
void
SeqImpl::set_data_src(
  SizeType src_id
)
{
  throw std::invalid_argument{"neither dff nor latch type"};
}

// @brief クロックをセットする．
void
SeqImpl::set_clock(
  SizeType clock_id
)
{
  throw std::invalid_argument{"not a dff type"};
}

// @brief イネーブルをセットする．
void
SeqImpl::set_enable(
  SizeType enable_id
)
{
  throw std::invalid_argument{"not a latch type"};
}

// @brief クリアをセットする．
void
SeqImpl::set_clear(
  SizeType clear_id
)
{
  throw std::invalid_argument{"neither dff nor latch type"};
}

// @brief プリセットをセットする．
void
SeqImpl::set_preset(
  SizeType preset_id
)
{
  throw std::invalid_argument{"neither dff nor latch type"};
}

// @brief データ出力をセットする．
void
SeqImpl::set_data_output(
  SizeType node_id
)
{
  throw std::invalid_argument{"neither dff nor latch type"};
}

// @brief ピンに対応するノードをセットする．
void
SeqImpl::set_cell_pin(
  SizeType pos,
  SizeType node_id
)
{
  throw std::invalid_argument{"not a cell type"};
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_FL
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_FL::SeqImpl_FL(
  char rs_val
) : mRsVal{rs_val}
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
SeqImpl_FL::rsval() const
{
  return mRsVal;
}

// @brief データ入力をセットする．
void
SeqImpl_FL::set_data_src(
  SizeType src_id
)
{
  mDataSrc = src_id;
}

// @brief クリアをセットする．
void
SeqImpl_FL::set_clear(
  SizeType clear_id
)
{
  mClear = clear_id;
}

// @brief プリセットをセットする．
void
SeqImpl_FL::set_preset(
  SizeType preset_id
)
{
  mPreset = preset_id;
}

// @brief データ出力をセットする．
void
SeqImpl_FL::set_data_output(
  SizeType node_id
)
{
  mDataOutput = node_id;
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_DFF
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_DFF::SeqImpl_DFF(
  char rs_val
) : SeqImpl_FL{rs_val}
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

// @brief クロックをセットする．
void
SeqImpl_DFF::set_clock(
  SizeType clock_id
)
{
  mClock = clock_id;
}

// @brief 複製を作る．
unique_ptr<SeqImpl>
SeqImpl_DFF::copy() const
{
  return unique_ptr<SeqImpl>{new SeqImpl_DFF{rsval()}};
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_Latch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_Latch::SeqImpl_Latch(
  char rs_val
) : SeqImpl_FL{rs_val}
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

// @brief イネーブルをセットする．
void
SeqImpl_Latch::set_enable(
  SizeType enable_id
)
{
  mEnable = enable_id;
}

// @brief 複製を作る．
unique_ptr<SeqImpl>
SeqImpl_Latch::copy() const
{
  return unique_ptr<SeqImpl>{new SeqImpl_Latch{rsval()}};
}


//////////////////////////////////////////////////////////////////////
// クラス SeqImpl_Cell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SeqImpl_Cell::SeqImpl_Cell(
  ClibCell cell
) : mCell{cell},
  mPinList(cell.pin_num())
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

// @brief ピンに対応するノードをセットする．
void
SeqImpl_Cell::set_cell_pin(
  SizeType pos,
  SizeType node_id
)
{
  if ( pos < 0 || pos >= mPinList.size() ) {
    throw std::invalid_argument{"Error in SeqImpl::set_pin(pos, node_id). pos is out of range."};
  }
  mPinList[pos] = node_id;
}

// @brief 複製を作る．
unique_ptr<SeqImpl>
SeqImpl_Cell::copy() const
{
  return unique_ptr<SeqImpl>{new SeqImpl_Cell{cell()}};
}

END_NAMESPACE_YM_BN
