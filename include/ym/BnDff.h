#ifndef BNDFF_H
#define BNDFF_H

/// @file BnDff.h
/// @brief BnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class DffImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnDff BnDff.h "BnDff.h"
/// @brief BnModel の DFF を表すクラス
///
/// 以下の3種類のタイプがある．
/// - BnDffType::DFF:   クリア・プリセット(オプション)付きのDフリップフロップ
/// - BnDffType::LATCH: クリア・プリセット(オプション)付きのラッチ
/// - BnDfftype::CELL:  DFF/ラッチタイプのセル
///
/// セルタイプの場合にはそのセルがフリップフロップかラッチかの区別はしていない．
///
/// 以下の情報を持つ．
/// - ID番号: 同一の BnModel 内の BnDff に対してユニークとなる．
/// - 名前: 場合によっては空文字列となる．
///
/// DFF/LATCH タイプの場合は以下の情報を持つ．
/// - データ出力ノード
/// - データ入力ノード
/// - クロック/イネーブルノード
/// - クリアノード
/// - プリセットノード
/// - クリアとプロセットが衝突したときの挙動
///
/// CELL タイプの場合は以下の情報を持つ．
/// - セル番号
/// - セルの各ピンに対応するノード
///
/// 実際にはこのクラスは ModelImpl へのポインタとID番号しか持たない．
//////////////////////////////////////////////////////////////////////
class BnDff
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnDff() = default;

  /// @brief コンストラクタ
  BnDff(
    const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
    SizeType id                        ///< [in] DFF番号
  );

  /// @brief デストラクタ
  ~BnDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mImpl != nullptr;
  }

  /// @brief 親の BnModel を返す．
  BnModel
  parent_model() const;

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 名前を返す．
  const string&
  name() const;

  /// @brief 種類を返す．
  BnDffType
  type() const;

  /// @brief DFFタイプの時に true を返す．
  bool
  is_dff() const;

  /// @brief ラッチタイプの時に true を返す．
  bool
  is_latch() const;

  /// @brief セルタイプの時に true を返す．
  bool
  is_cell() const;

  /// @brief データ入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  BnNode
  data_src() const;

  /// @brief クロック入力ノードを返す．
  ///
  /// is_dff() == true
  BnNode
  clock() const;

  /// @brief イネーブル入力ノードを返す．
  ///
  /// is_latch() == true
  BnNode
  enable() const;

  /// @brief クリア入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  BnNode
  clear() const;

  /// @brief プリセット入力ノードを返す．
  ///
  /// is_dff() == true or is_latch() == true
  BnNode
  preset() const;

  /// @brief クリアとプリセットが衝突したときの挙動を返す．
  ///
  /// is_dff() == true or is_latch() == true
  char
  rsval() const;

  /// @brief データ出力ノードを返す．
  BnNode
  data_output() const;

  /// @brief セル番号を返す．
  ///
  /// is_cell() == true
  SizeType
  cell_id() const;

  /// @brief セルを返す．
  ClibCell
  cell() const;

  /// @brief セルのピンに対応するノードを返す．
  ///
  /// is_cell() == true
  BnNode
  cell_pin(
    SizeType pos ///< [in] ピン番号
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実体を返す．
  const DffImpl&
  dff_impl() const;

  /// @brief ID 番号から BnNode を作る．
  BnNode
  from_id(
    SizeType id
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  shared_ptr<ModelImpl> mImpl{nullptr};

  // ID番号
  SizeType mId{BAD_ID};


};

END_NAMESPACE_YM_BN

#endif // BNDFF_H
