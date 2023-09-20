#ifndef NODEIMPL_H
#define NODEIMPL_H

/// @file NodelImpl.h
/// @brief NodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnfe.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BNFE

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BfModel のノードを表すクラス
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
  BfNodeType
  type() const
  {
    return mType;
  }

  /// @brief 入力として定義されている時 true を返す．
  bool
  is_input() const
  {
    return mType == BfNodeType::Input;
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
    return false;
  }

  /// @brief プリミティブ型の論理ノードの時 true を返す．
  bool
  is_primitive() const
  {
    return mType == BfNodeType::Prim;
  }

  /// @brief AIG型の論理ノードの時 true を返す．
  bool
  is_aig() const
  {
    return mType == BfNodeType::Aig;
  }

  /// @brief カバー型の論理ノードの時 true を返す．
  bool
  is_cover() const
  {
    return mType == BfNodeType::Cover;
  }

  /// @brief 論理式型の論理ノードの時 true を返す．
  bool
  is_expr() const
  {
    return mType == BfNodeType::Expr;
  }

  /// @brief セル型の論理ノードの時 true を返す．
  bool
  is_cell() const
  {
    return mType == BfNodeType::Cell;
  }

  /// @brief DFFノードの時 true を返す．
  bool
  is_dff() const
  {
    return mType == BfNodeType::Dff;
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

  /// @brief DFFの入力ノード番号を得る．
  ///
  /// is_dff() == true の時のみ有効
  SizeType
  dff_src() const
  {
    check_dff();
    return mFaninList[0];
  }

  /// @brief リセット値を得る．
  ///
  /// is_dff() == true の時のみ有効
  char
  dff_rval() const
  {
    check_dff();
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
    mType = BfNodeType::Input;
    mExtId = iid;
  }

  /// @brief プリミティブタイプをセットする．
  void
  set_primitive(
    const vector<SizeType>& fanin_list,
    PrimType type
  )
  {
    mType = BfNodeType::Prim;
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
    mType = BfNodeType::Aig;
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
    mType = BfNodeType::Cover;
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
    mType = BfNodeType::Expr;
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
    mType = BfNodeType::Cell;
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
    mType = BfNodeType::Dff;
    mFaninList = {input_id};
    mExtId = static_cast<SizeType>(rval);
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

  /// @brief dff タイプかどうかチェックする．
  void
  check_dff() const
  {
    if ( !is_dff() ) {
      throw std::invalid_argument{"not a dff type"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの種類
  BfNodeType mType{BfNodeType::None};

  // 名前
  string mName;

  // ファンインのID番号のリスト
  vector<SizeType> mFaninList;

  // プリミティブタイプ/カバー番号/セル番号
  SizeType mExtId;

};

END_NAMESPACE_YM_BNFE

#endif // NODEIMPL_H
