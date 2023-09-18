#ifndef NODEIMPL_H
#define NODEIMPL_H

/// @file NodelImpl.h
/// @brief NodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnet.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnModel のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NodelImpl
{
public:

  /// @brief コンストラクタ
  NodeImpl(
    const string& name ///< [in] 名前
  ) : mName{name}
  {
  }

  /// @brief デストラクタ
  ~NodeImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  BnType
  type() const
  {
    if ( is_input() ) {
      return BnType::Input;
    }
    if ( is_dff() ) {
      return BnType::Dff;
    }
    if ( is_primitive() ) {
      return BnType::Prim;
    }
    if ( is_cover() ) {
      return BnType::Cover;
    }
    if ( is_cell() ) {
      return BnType::Cell;
    }
    ASSERT_NOT_REACHED;
    return BnType::None;
  }

  /// @brief 入力として定義されている時 true を返す．
  bool
  is_input() const { return mFlags.get(INPUT_BIT); }

  /// @brief プリミティブ型の論理ノードの時 true を返す．
  bool
  is_primitive() const { return mFlags.get(PRIM_BIT); }

  /// @brief カバー型の論理ノードの時 true を返す．
  bool
  is_cover() const { return mFlags.get(COVER_BIT); }

  /// @brief セル型の論理ノードの時 true を返す．
  bool
  is_cell() const { return mFlags.get(CELL_BIT); }

  /// @brief DFFノードの時 true を返す．
  bool
  is_dff() const { return mFlags.get(DFF_BIT); }

  /// @brief このシンボルの名前を返す．
  const string&
  name() const { return mName; }

  /// @brief ファンイン数を返す．
  SizeType
  fanin_num() const
  {
    return mFaninList.size();
  }

  /// @brief ファンインのノード番号を返す．
  SizeType
  fanin(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < fanin_num() );
    return mFaninList[pos];
  }

  /// @brief ファンイン番号のリストを返す．
  const vector<SizeType>&
  fanin_list() const
  {
    return mFaninList;
  }

  /// @brief プリミティブタイプを得る．
  ///
  /// is_prim() == true の時のみ有効
  PrimType
  primitive_type() const
  {
    return static_cast<PrimType>(mExtId);
  }

  /// @brief ファンインの反転属性を返す．
  ///
  /// is_aig() が true の時のみ意味を持つ．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置番号 ( 0 or 1 )
  ) const
  {
    return static_cast<bool>((mExtId >> pos) & 1);
  }

  /// @brief カバー番号を得る．
  ///
  /// is_cover() == true の時のみ有効
  SizeType
  cover_id() const { return mExtId; }

  /// @brief セル番号を得る．
  ///
  /// is_cell() == true の時のみ有効
  SizeType
  cell_id() const { return mExtId; }

  /// @brief DFFの入力ノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_src() const { return mFaninList[0]; }

  ///
  /// is_latch() == true の時のみ有効
  /// @brief リセット値を得る．
  char
  dff_rval() const { return static_cast<char>(mExtId); }


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力として定義されたことをセットする．
  void
  set_input()
  {
    mFlags.set(INPUT_BIT);
  }

  /// @brief プリミティブタイプをセットする．
  void
  set_primitive(
    const vector<SizeType>& fanin_list,
    PrimType type
  )
  {
    mFlags.set(PRIM_BIT);
    mFaninList = fanin_list;
    mExtId = static_cast<SizeType>(type);
  }

  /// @brief AIG タイプをセットする．
  void
  set_aig(
    SizeType src0,
    SizeType src1,
    bool inv0,
    bool inv1
  )
  {
    mFlags.set(AIG_BIT);
    mFaninList = { src0, src1 };
    mExtId = static_cast<SizeType>(inv0) | (static_cast<SizeType>(inv1) << 1);
  }

  /// @brief カバータイプをセットする．
  void
  set_cover(
    const vector<SizeType>& fanin_list,
    SizeType cover_id
  )
  {
    mFlags.set(COVER_BIT);
    mFaninList = fanin_list;
    mExtId = cover_id;
  }

  /// @brief セルタイプをセットする．
  void
  set_cell(
    const vector<SizeType>& fanin_list,
    SizeType cell_id
  )
  {
    mFlags.set(CELL_BIT);
    mFaninList = fanin_list;
    mExtId = cell_id;
  }

  /// @brief DFFタイプをセットする．
  void
  set_dff(
    SizeType input_id,
    char rval
  )
  {
    mFlags.set(DFF_BIT);
    mFaninList = {input_id};
    mExtId = static_cast<SizeType>(rval);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの種類
  BnNodeType mType;

  // 名前
  string mName;

  // ファンインのID番号のリスト
  vector<SizeType> mFaninList;

  // プリミティブタイプ/カバー番号/セル番号
  SizeType mExtId;

};

END_NAMESPACE_YM_BNET

#endif // NODEIMPL_H
