#ifndef NODEIMPL_H
#define NODEIMPL_H

/// @file NodelImpl.h
/// @brief NodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnModel のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NodeImpl
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
  BnNodeType
  type() const
  {
    return mType;
  }

  /// @brief 入力として定義されている時 true を返す．
  bool
  is_input() const
  {
    return mType == BnNodeType::Input;
  }

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const
  {
    if ( is_primitive() ) {
      return true;
    }
    if ( is_aig() ) {
      return true;
    }
    if ( is_cover() ) {
      return true;
    }
    if ( is_expr() ) {
      return true;
    }
    if ( is_cell() ) {
      return true;
    }
    if ( is_func() ) {
      return true;
    }
    if ( is_bdd() ) {
      return true;
    }
    return false;
  }

  /// @brief プリミティブ型の論理ノードの時 true を返す．
  bool
  is_primitive() const
  {
    return mType == BnNodeType::Prim;
  }

  /// @brief AIG型の論理ノードの時 true を返す．
  bool
  is_aig() const
  {
    return mType == BnNodeType::Aig;
  }

  /// @brief カバー型の論理ノードの時 true を返す．
  bool
  is_cover() const
  {
    return mType == BnNodeType::Cover;
  }

  /// @brief 論理式型の論理ノードの時 true を返す．
  bool
  is_expr() const
  {
    return mType == BnNodeType::Expr;
  }

  /// @brief セル型の論理ノードの時 true を返す．
  bool
  is_cell() const
  {
    return mType == BnNodeType::Cell;
  }

  /// @brief 真理値表型の論理ノードの時 true を返す．
  bool
  is_func() const
  {
    return mType == BnNodeType::TvFunc;
  }

  /// @brief BDD型の論理ノードの時 true を返す．
  bool
  is_bdd() const
  {
    return mType == BnNodeType::Bdd;
  }

  /// @brief DFFノードの時 true を返す．
  bool
  is_dff() const
  {
    return mType == BnNodeType::Dff;
  }

  /// @brief ラッチノードの時 true を返す．
  bool
  is_latch() const
  {
    return mType == BnNodeType::Latch;
  }

  /// @brief 名前を返す．
  const string&
  name() const
  {
    return mName;
  }

  /// @brief 入力番号を返す．
  ///
  /// is_input() が true の時のみ意味を持つ．
  SizeType
  input_id() const
  {
    check_input();
    return mExtId;
  }

  /// @brief ファンイン数を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  SizeType
  fanin_num() const
  {
    check_logic();
    return mFaninList.size();
  }

  /// @brief ファンインのノード番号を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  SizeType
  fanin(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const
  {
    check_logic();
    if ( pos < 0 || fanin_num() <= pos ) {
      throw std::invalid_argument{"index out of range"};
    }
    return mFaninList[pos];
  }

  /// @brief ファンイン番号のリストを返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  const vector<SizeType>&
  fanin_list() const
  {
    check_logic();
    return mFaninList;
  }

  /// @brief プリミティブタイプを得る．
  ///
  /// is_primitive() == true の時のみ有効
  PrimType
  primitive_type() const
  {
    check_primitive();
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
    check_aig();
    return static_cast<bool>((mExtId >> pos) & 1);
  }

  /// @brief カバー番号を得る．
  ///
  /// is_cover() == true の時のみ有効
  SizeType
  cover_id() const
  {
    check_cover();
    return mExtId;
  }

  /// @brief 論理式番号を得る．
  ///
  /// is_expr() == true の時のみ有効
  SizeType
  expr_id() const
  {
    check_expr();
    return mExtId;
  }

  /// @brief セル番号を得る．
  ///
  /// is_cell() == true の時のみ有効
  SizeType
  cell_id() const
  {
    check_cell();
    return mExtId;
  }

  /// @brief 関数番号を得る．
  ///
  /// is_func() == true の時のみ有効
  SizeType
  func_id() const
  {
    check_func();
    return mExtId;
  }

  /// @brief BDD番号を得る．
  ///
  /// is_bdd() == true の時のみ有効
  SizeType
  bdd_id() const
  {
    check_bdd();
    return mExtId;
  }

  /// @brief DFFの入力ノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_src() const
  {
    check_dff();
    return mFaninList[0];
  }

  /// @brief DFFのクロックノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_clock() const
  {
    check_dff();
    return mFaninList[1];
  }

  /// @brief DFFのリセットノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_reset() const
  {
    check_dff();
    return mFaninList[2];
  }

  /// @brief DFFのプリセットノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_preset() const
  {
    check_dff();
    return mFaninList[3];
  }

  /// @brief DFFのリセットとプリセットが共にオンの時の値を得る．
  ///
  /// is_dff() == true の時のみ有効
  char
  dff_rsval() const
  {
    check_dff();
    return static_cast<char>(mExtId);
  }

  /// @brief ラッチの入力ノード番号を得る．
  ///
  /// is_latch() == true の時のみ有効
  SizeType
  latch_src() const
  {
    check_latch();
    return mFaninList[0];
  }

  /// @brief ラッチのイネーブルノード番号を得る．
  ///
  /// is_latch() == true の時のみ有効
  SizeType
  latch_enable() const
  {
    check_latch();
    return mFaninList[1];
  }

  /// @brief ラッチのリセットノード番号を得る．
  ///
  /// is_latch() == true の時のみ有効
  SizeType
  latch_reset() const
  {
    check_latch();
    return mFaninList[2];
  }

  /// @brief ラッチのプリセットノード番号を得る．
  ///
  /// is_latch() == true の時のみ有効
  SizeType
  latch_preset() const
  {
    check_latch();
    return mFaninList[3];
  }

  /// @brief ラッチのリセットとプリセットが共にオンの時の値を得る．
  ///
  /// is_latch() == true の時のみ有効
  char
  latch_rsval() const
  {
    check_latch();
    return static_cast<char>(mExtId);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  void
  set_name(
    const string& name
  )
  {
    mName = name;
  }

  /// @brief 入力として定義されたことをセットする．
  void
  set_input(
    SizeType iid ///< [in] 入力番号
  )
  {
    mType = BnNodeType::Input;
    mExtId = iid;
  }

  /// @brief プリミティブタイプをセットする．
  void
  set_primitive(
    const vector<SizeType>& fanin_list,
    PrimType type
  )
  {
    mType = BnNodeType::Prim;
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
    mType = BnNodeType::Aig;
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
    mType = BnNodeType::Cover;
    mFaninList = fanin_list;
    mExtId = cover_id;
  }

  /// @brief カバータイプをセットする．
  void
  set_expr(
    const vector<SizeType>& fanin_list,
    SizeType expr_id
  )
  {
    mType = BnNodeType::Expr;
    mFaninList = fanin_list;
    mExtId = expr_id;
  }

  /// @brief セルタイプをセットする．
  void
  set_cell(
    const vector<SizeType>& fanin_list,
    SizeType cell_id
  )
  {
    mType = BnNodeType::Cell;
    mFaninList = fanin_list;
    mExtId = cell_id;
  }

  /// @brief 真理値表タイプをセットする．
  void
  set_func(
    const vector<SizeType>& fanin_list,
    SizeType func_id
  )
  {
    mType = BnNodeType::TvFunc;
    mFaninList = fanin_list;
    mExtId = func_id;
  }

  /// @brief BDDタイプをセットする．
  void
  set_bdd(
    const vector<SizeType>& fanin_list,
    SizeType bdd_id
  )
  {
    mType = BnNodeType::Bdd;
    mFaninList = fanin_list;
    mExtId = bdd_id;
  }

  /// @brief DFFタイプをセットする．
  void
  set_dff(
    char rs_val
  )
  {
    check_dff();
    mType = BnNodeType::Dff;
    mFaninList = {0, 0, 0, 0};
    mExtId = static_cast<SizeType>(rs_val);
  }

  /// @brief DFFのソースをセットする．
  void
  set_dff_src(
    SizeType src_id
  )
  {
    check_dff();
    mFaninList[0] = src_id;
  }

  /// @brief DFFのクロックをセットする．
  void
  set_dff_clock(
    SizeType clock_id
  )
  {
    check_dff();
    mFaninList[1] = clock_id;
  }

  /// @brief DFFのリセットをセットする．
  void
  set_dff_reset(
    SizeType reset_id
  )
  {
    check_dff();
    mFaninList[2] = reset_id;
  }

  /// @brief DFFのプリセットをセットする．
  void
  set_dff_preset(
    SizeType preset_id
  )
  {
    check_dff();
    mFaninList[3] = preset_id;
  }

  /// @brief ラッチタイプをセットする．
  void
  set_latch(
    char rs_val
  )
  {
    check_latch();
    mType = BnNodeType::Latch;
    mFaninList = {0, 0, 0, 0};
    mExtId = static_cast<SizeType>(rs_val);
  }

  /// @brief ラッチのソースをセットする．
  void
  set_latch_src(
    SizeType src_id
  )
  {
    check_latch();
    mFaninList[0] = src_id;
  }

  /// @brief ラッチのイネーブルをセットする．
  void
  set_latch_enable(
    SizeType enable_id
  )
  {
    check_latch();
    mFaninList[1] = enable_id;
  }

  /// @brief ラッチのリセットをセットする．
  void
  set_latch_reset(
    SizeType reset_id
  )
  {
    check_latch();
    mFaninList[2] = reset_id;
  }

  /// @brief ラッチのプリセットをセットする．
  void
  set_latch_preset(
    SizeType preset_id
  )
  {
    check_latch();
    mFaninList[3] = preset_id;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief input タイプかどうかチェックする．
  void
  check_input() const
  {
    if ( !is_input() ) {
      throw std::invalid_argument{"not an input type"};
    }
  }

  /// @brief logic タイプかどうかチェックする．
  void
  check_logic() const
  {
    if ( !is_logic() ) {
      throw std::invalid_argument{"not a logic type"};
    }
  }

  /// @brief primitive タイプかどうかチェックする．
  void
  check_primitive() const
  {
    if ( !is_primitive() ) {
      throw std::invalid_argument{"not a primitive type"};
    }
  }

  /// @brief aig タイプかどうかチェックする．
  void
  check_aig() const
  {
    if ( !is_aig() ) {
      throw std::invalid_argument{"not an aig type"};
    }
  }

  /// @brief cover タイプかどうかチェックする．
  void
  check_cover() const
  {
    if ( !is_cover() ) {
      throw std::invalid_argument{"not a cover type"};
    }
  }

  /// @brief expr タイプかどうかチェックする．
  void
  check_expr() const
  {
    if ( !is_expr() ) {
      throw std::invalid_argument{"not an expr type"};
    }
  }

  /// @brief cell タイプかどうかチェックする．
  void
  check_cell() const
  {
    if ( !is_cell() ) {
      throw std::invalid_argument{"not a cell type"};
    }
  }

  /// @brief func タイプかどうかチェックする．
  void
  check_func() const
  {
    if ( !is_func() ) {
      throw std::invalid_argument{"not a func type"};
    }
  }

  /// @brief bdd タイプかどうかチェックする．
  void
  check_bdd() const
  {
    if ( !is_bdd() ) {
      throw std::invalid_argument{"not a bdd type"};
    }
  }

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの種類
  BnNodeType mType{BnNodeType::None};

  // 名前
  string mName;

  // ファンインのID番号のリスト
  vector<SizeType> mFaninList;

  // プリミティブタイプ/カバー番号/セル番号
  SizeType mExtId;

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_H
