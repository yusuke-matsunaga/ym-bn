#ifndef BNSEQ_H
#define BNSEQ_H

/// @file BnSeq.h
/// @brief BnSeq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class SeqImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnSeq BnSeq.h "BnSeq.h"
/// @brief BnModel の DFF を表すクラス
///
/// 以下の3種類のタイプがある．
/// - BnSeqType::DFF:   クリア・プリセット(オプション)付きのDフリップフロップ
/// - BnSeqType::LATCH: クリア・プリセット(オプション)付きのラッチ
/// - BnSeqtype::CELL:  DFF/ラッチタイプのセル
///
/// セルタイプの場合にはそのセルがフリップフロップかラッチかの区別はしていない．
///
/// 以下の情報を持つ．
/// - ID番号: 同一の BnModel 内の BnSeq に対してユニークとなる．
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
///
/// 空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() = false, id() = BAD_ID となる以外は
/// 全てのメンバ関数の呼び出しが std::invalid_argument 例外を送出する．
//////////////////////////////////////////////////////////////////////
class BnSeq
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnSeq() = default;

  /// @brief コンストラクタ
  BnSeq(
    const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
    SizeType id                        ///< [in] DFF番号
  );

  /// @brief デストラクタ
  ~BnSeq();


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

  /// @brief 種類を返す．
  BnSeqType
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
  /// - is_dff() == true or is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  data_src() const;

  /// @brief クロック入力ノードを返す．
  ///
  /// - is_dff() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  clock() const;

  /// @brief イネーブル入力ノードを返す．
  ///
  /// - is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  enable() const;

  /// @brief クリア入力ノードを返す．
  ///
  /// - is_dff() == true or is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  clear() const;

  /// @brief プリセット入力ノードを返す．
  ///
  /// - is_dff() == true or is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  preset() const;

  /// @brief クリアとプリセットが衝突したときの挙動を返す．
  ///
  /// - is_dff() == true or is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  char
  rsval() const;

  /// @brief データ出力ノードを返す．
  ///
  /// - is_dff() == true or is_latch() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  BnNode
  data_output() const;

  /// @brief セル番号を返す．
  ///
  /// - is_cell() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  SizeType
  cell_id() const;

  /// @brief セルを返す．
  ///
  /// - is_cell() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  ClibCell
  cell() const;

  /// @brief セルのピンに対応するノードを返す．
  ///
  /// - is_cell() == true の時のみ意味を持つ．
  /// - それ以外は std::invalid_argument 例外を送出する．
  /// - pos が範囲外の時は std::out_of_range 例外を送出する．
  BnNode
  cell_pin(
    SizeType pos ///< [in] ピン番号
  ) const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnSeq& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl && mId == right.mId;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnSeq& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実体を返す．
  const SeqImpl&
  seq_impl() const;

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

#endif // BNSEQ_H
