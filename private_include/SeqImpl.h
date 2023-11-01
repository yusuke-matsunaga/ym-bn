#ifndef SEQIMPL_H
#define SEQIMPL_H

/// @file SeqImpl.h
/// @brief SeqImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class SeqImpl SeqImpl.h "SeqImpl.h"
/// @brief BnModel の DFF を表すクラス
//////////////////////////////////////////////////////////////////////
class SeqImpl
{
public:

  /// @brief コンストラクタ
  SeqImpl() = default;

  /// @brief デストラクタ
  ~SeqImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  BnSeqType
  type() const
  {
    return mType;
  }

  /// @brief DFFタイプの時に true を返す．
  bool
  is_dff() const
  {
    return type() == BnSeqType::DFF;
  }

  /// @brief ラッチタイプの時に true を返す．
  bool
  is_latch() const
  {
    return type() == BnSeqType::LATCH;
  }

  /// @brief セルタイプの時に true を返す．
  bool
  is_cell() const
  {
    return type() == BnSeqType::CELL;
  }

  /// @brief データ入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  SizeType
  data_src() const
  {
    check_dff_or_latch();
    return mPinList[0];
  }

  /// @brief クロック入力ノードを返す．
  ///
  /// is_dff() == true
  SizeType
  clock() const
  {
    check_dff();
    return mPinList[1];
  }

  /// @brief イネーブル入力ノードを返す．
  ///
  /// is_latch() == true
  SizeType
  enable() const
  {
    check_latch();
    return mPinList[1];
  }

  /// @brief クリア入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  SizeType
  clear() const
  {
    check_dff_or_latch();
    return mPinList[2];
  }

  /// @brief プリセット入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  SizeType
  preset() const
  {
    check_dff_or_latch();
    return mPinList[3];
  }

  /// @brief クリアとプリセットが衝突したときの挙動を返す．
  ///
  /// is_dff() == true or is_latch() == true
  char
  rsval() const
  {
    check_dff_or_latch();
    return static_cast<char>(mExtId);
  }

  /// @brief データ出力ノードを返す．
  SizeType
  data_output() const
  {
    check_dff_or_latch();
    return mPinList[4];
  }

  /// @brief セル番号を返す．
  ///
  /// is_cell() == true
  SizeType
  cell_id() const
  {
    check_cell();
    return mExtId;
  }

  /// @brief セルのピンに対応するノードを返す．
  ///
  /// is_cell() == true
  SizeType
  cell_pin(
    SizeType pos ///< [in] ピン番号
  ) const
  {
    check_cell();
    if ( pos < 0 || pos >= mPinList.size() ) {
      throw std::invalid_argument{"Error in SeqImpl::cell_pin(pos). pos is out of range."};
    }
    return mPinList[pos];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFタイプをセットする．
  void
  set_dff(
    char rs_val,
    SizeType output_id
  )
  {
    mType = BnSeqType::DFF;
    mPinList = {BAD_ID, BAD_ID, BAD_ID, BAD_ID, output_id};
    mExtId = static_cast<SizeType>(rs_val);
  }

  /// @brief ラッチタイプをセットする．
  void
  set_latch(
    char rs_val,
    SizeType output_id
  )
  {
    mType = BnSeqType::LATCH;
    mPinList = {BAD_ID, BAD_ID, BAD_ID, BAD_ID, output_id};
    mExtId = static_cast<SizeType>(rs_val);
  }

  /// @brief セルタイプをセットする．
  void
  set_cell(
    SizeType cell_id,
    SizeType pin_num
  )
  {
    mType = BnSeqType::CELL;
    mPinList = vector<SizeType>(pin_num, BAD_ID);
    mExtId = cell_id;
  }

  /// @brief データ入力をセットする．
  void
  set_data_src(
    SizeType src_id
  )
  {
    check_dff_or_latch();
    mPinList[0] = src_id;
  }

  /// @brief クロックをセットする．
  void
  set_clock(
    SizeType clock_id
  )
  {
    check_dff();
    mPinList[1] = clock_id;
  }

  /// @brief イネーブルをセットする．
  void
  set_enable(
    SizeType enable_id
  )
  {
    check_latch();
    mPinList[1] = enable_id;
  }

  /// @brief クリアをセットする．
  void
  set_clear(
    SizeType clear_id
  )
  {
    check_dff_or_latch();
    mPinList[2] = clear_id;
  }

  /// @brief プリセットをセットする．
  void
  set_preset(
    SizeType preset_id
  )
  {
    check_dff_or_latch();
    mPinList[3] = preset_id;
  }

  /// @brief データ出力をセットする．
  void
  set_data_output(
    SizeType node_id
  )
  {
    check_dff_or_latch();
    mPinList[4] = node_id;
  }

  /// @brief ピンに対応するノードをセットする．
  void
  set_cell_pin(
    SizeType pos,
    SizeType node_id
  )
  {
    check_cell();
    if ( pos < 0 || pos >= mPinList.size() ) {
      throw std::invalid_argument{"Error in SeqImpl::set_pin(pos, node_id). pos is out of range."};
    }
    mPinList[pos] = node_id;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dff タイプかどうかチェックする．
  void
  check_dff() const
  {
    if ( !is_dff() ) {
      throw std::invalid_argument{"not a dff type"};
    }
  }

  /// @brief ラッチタイプかどうかチェックする．
  void
  check_latch() const
  {
    if ( !is_latch() ) {
      throw std::invalid_argument{"not a latch type"};
    }
  }

  /// @brief dff タイプもしくはラッチタイプかどうかチェックする．
  void
  check_dff_or_latch() const
  {
    if ( !is_dff() && !is_latch() ) {
      throw std::invalid_argument{"neither dff nor latch type"};
    }
  }

  /// @brief セルタイプかどうかチェックする．
  void
  check_cell() const
  {
    if ( !is_cell() ) {
      throw std::invalid_argument{"not a cell type"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DFF の種類
  BnSeqType mType{BnSeqType::NONE};

  // 入出力ピンのノード番号のリスト
  vector<SizeType> mPinList;

  // rsval/セル番号
  SizeType mExtId;

};

END_NAMESPACE_YM_BN

#endif // SEQIMPL_H
