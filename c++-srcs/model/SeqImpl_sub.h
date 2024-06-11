#ifndef SEQIMPL_SUB_H
#define SEQIMPL_SUB_H

/// @file SeqImpl_sub.h
/// @brief SeqImpl_sub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "SeqImpl.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class SeqImpl_FL SeqImpl_sub.h "SeqImpl_sub.h"
/// @brief DFF/Latch タイプに共通な基底クラス
//////////////////////////////////////////////////////////////////////
class SeqImpl_FL :
  public SeqImpl
{
public:

  /// @brief コンストラクタ
  SeqImpl_FL(
    char rs_val         ///< [in] リセットとプリセットが共にオンの時の値
  );

  /// @brief デストラクタ
  ~SeqImpl_FL();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief データ入力ノードを返す．
  SizeType
  data_src() const override;

  /// @brief クリア入力ノードを返す．
  SizeType
  clear() const override;

  /// @brief プリセット入力ノードを返す．
  SizeType
  preset() const override;

  /// @brief データ出力ノードを返す．
  SizeType
  data_output() const override;

  /// @brief クリアとプリセットが衝突したときの挙動を返す．
  char
  rsval() const override;

  /// @brief データ入力をセットする．
  void
  set_data_src(
    SizeType src_id
  ) override;

  /// @brief クリアをセットする．
  void
  set_clear(
    SizeType clear_id
  ) override;

  /// @brief プリセットをセットする．
  void
  set_preset(
    SizeType preset_id
  ) override;

  /// @brief データ出力をセットする．
  void
  set_data_output(
    SizeType node_id
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ入力ノード
  SizeType mDataSrc{BAD_ID};

  // クリアノード
  SizeType mClear{BAD_ID};

  // プリセットノード
  SizeType mPreset{BAD_ID};

  // データ出力ノード
  SizeType mDataOutput{BAD_ID};

  // クリアとプリセットが衝突したときの挙動を返す．
  char mRsVal{' '};

};


//////////////////////////////////////////////////////////////////////
/// @class SeqImpl_DFF SeqImpl_sub.h "SeqImpl_sub.h"
/// @brief DFF タイプの SeqImpl
//////////////////////////////////////////////////////////////////////
class SeqImpl_DFF :
  public SeqImpl_FL
{
public:

  /// @brief コンストラクタ
  SeqImpl_DFF(
    char rs_val         ///< [in] リセットとプリセットが共にオンの時の値
  );

  /// @brief デストラクタ
  ~SeqImpl_DFF();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  BnSeqType
  type() const override;

  /// @brief DFFタイプの時に true を返す．
  bool
  is_dff() const override;

  /// @brief クロック入力ノードを返す．
  SizeType
  clock() const override;

  /// @brief クロックをセットする．
  void
  set_clock(
    SizeType clock_id
  ) override;

  /// @brief 複製を作る．
  unique_ptr<SeqImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // クロックノード
  SizeType mClock{BAD_ID};

};


//////////////////////////////////////////////////////////////////////
/// @class SeqImpl_Latch SeqImpl_sub.h "SeqImpl_sub.h"
/// @brief Latch タイプの SeqImpl
//////////////////////////////////////////////////////////////////////
class SeqImpl_Latch :
  public SeqImpl_FL
{
public:

  /// @brief コンストラクタ
  SeqImpl_Latch(
    char rs_val         ///< [in] リセットとプリセットが共にオンの時の値
  );

  /// @brief デストラクタ
  ~SeqImpl_Latch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  BnSeqType
  type() const override;

  /// @brief ラッチタイプの時に true を返す．
  bool
  is_latch() const override;

  /// @brief イネーブル入力ノードを返す．
  SizeType
  enable() const override;

  /// @brief イネーブルをセットする．
  void
  set_enable(
    SizeType enable_id
  ) override;

  /// @brief 複製を作る．
  unique_ptr<SeqImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イネーブルノード
  SizeType mEnable{BAD_ID};

};


//////////////////////////////////////////////////////////////////////
/// @class SeqImpl_Cell SeqImpl_sub.h "SeqImpl_sub.h"
/// @brief セルタイプの SeqImpl
//////////////////////////////////////////////////////////////////////
class SeqImpl_Cell :
  public SeqImpl
{
public:

  /// @brief コンストラクタ
  SeqImpl_Cell(
    ClibCell cell ///< [in] セル
  );

  /// @brief デストラクタ
  ~SeqImpl_Cell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  BnSeqType
  type() const override;

  /// @brief セルタイプの時に true を返す．
  bool
  is_cell() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief セルのピンに対応するノードを返す．
  SizeType
  cell_pin(
    SizeType pos ///< [in] ピン番号
  ) const override;

  /// @brief ピンに対応するノードをセットする．
  void
  set_cell_pin(
    SizeType pos,
    SizeType node_id
  ) override;

  /// @brief 複製を作る．
  unique_ptr<SeqImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  ClibCell mCell;

  // 入出力ピンに対応するノード番号のリスト
  vector<SizeType> mPinList;

};

END_NAMESPACE_YM_BN

#endif // SEQIMPL_SUB_H
