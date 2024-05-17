#ifndef SEQIMPL_H
#define SEQIMPL_H

/// @file SeqImpl.h
/// @brief SeqImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class SeqImpl SeqImpl.h "SeqImpl.h"
/// @brief BnModel の DFF を表すクラス
///
/// 大きく分けて
/// - DFF
/// - LATCH
/// - CELL
/// の３種類がある．
/// 個々のタイプごとに異なるデータを持つ．
//////////////////////////////////////////////////////////////////////
class SeqImpl
{
public:

  /// @brief コンストラクタ
  SeqImpl() = default;

  /// @brief デストラクタ
  virtual
  ~SeqImpl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  BnSeqType
  type() const = 0;

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
  virtual
  SizeType
  data_src() const;

  /// @brief クロック入力ノードを返す．
  ///
  /// is_dff() == true
  virtual
  SizeType
  clock() const;

  /// @brief イネーブル入力ノードを返す．
  ///
  /// is_latch() == true
  virtual
  SizeType
  enable() const;

  /// @brief クリア入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  virtual
  SizeType
  clear() const;

  /// @brief プリセット入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  virtual
  SizeType
  preset() const;

  /// @brief クリアとプリセットが衝突したときの挙動を返す．
  ///
  /// is_dff() == true or is_latch() == true
  virtual
  char
  rsval() const;

  /// @brief データ出力ノードを返す．
  virtual
  SizeType
  data_output() const;

  /// @brief セルを返す．
  ///
  /// is_cell() == true
  virtual
  ClibCell
  cell() const;

  /// @brief セルのピンに対応するノードを返す．
  ///
  /// is_cell() == true
  virtual
  SizeType
  cell_pin(
    SizeType pos ///< [in] ピン番号
  ) const;

  /// @brief 複製を作る．
  virtual
  unique_ptr<SeqImpl>
  copy() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

#if 0
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
    ClibCell cell,
    SizeType pin_num
  )
  {
    mType = BnSeqType::CELL;
    mPinList = vector<SizeType>(pin_num, BAD_ID);
    mExtId = cell;
  }
#endif

  /// @brief データ入力をセットする．
  virtual
  void
  set_data_src(
    SizeType src_id
  );

  /// @brief クロックをセットする．
  virtual
  void
  set_clock(
    SizeType clock_id
  );

  /// @brief イネーブルをセットする．
  virtual
  void
  set_enable(
    SizeType enable_id
  );

  /// @brief クリアをセットする．
  virtual
  void
  set_clear(
    SizeType clear_id
  );

  /// @brief プリセットをセットする．
  virtual
  void
  set_preset(
    SizeType preset_id
  );

  /// @brief データ出力をセットする．
  virtual
  void
  set_data_output(
    SizeType node_id
  );

  /// @brief ピンに対応するノードをセットする．
  virtual
  void
  set_cell_pin(
    SizeType pos,
    SizeType node_id
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

#if 0
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
#endif

};

END_NAMESPACE_YM_BN

#endif // SEQIMPL_H
