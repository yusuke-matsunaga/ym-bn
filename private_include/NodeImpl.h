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
#include "ym/clib.h"


BEGIN_NAMESPACE_YM_BN

class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnModel のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class NodeImpl
{
public:

  /// @brief コンストラクタ
  NodeImpl() = default;

  /// @brief デストラクタ
  virtual
  ~NodeImpl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  virtual
  BnNodeType
  type() const = 0;

  /// @brief 入力として定義されている時 true を返す．
  bool
  is_input() const
  {
    return type() == BnNodeType::INPUT;
  }

  /// @brief BnSeq の出力の時 true を返す．
  bool
  is_seq_output() const
  {
    return type() == BnNodeType::SEQ_OUTPUT;
  }

  /// @brief 論理ノードの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief プリミティブ型の論理ノードの時 true を返す．
  bool
  is_primitive() const
  {
    return type() == BnNodeType::PRIMITIVE;
  }

  /// @brief AIG型の論理ノードの時 true を返す．
  bool
  is_aig() const
  {
    return type() == BnNodeType::AIG;
  }

  /// @brief 関数型の論理ノードの時 true を返す．
  bool
  is_func() const
  {
    return type() == BnNodeType::FUNC;
  }

  /// @brief セル型の論理ノードの時 true を返す．
  bool
  is_cell() const
  {
    return type() == BnNodeType::CELL;
  }

  /// @brief 入力番号を返す．
  virtual
  SizeType
  input_id() const;

  /// @brief SEQ 番号を返す．
  virtual
  SizeType
  seq_id() const;

  /// @brief ファンイン数を返す．
  virtual
  SizeType
  fanin_num() const;

  /// @brief ファンインのノード番号を返す．
  virtual
  SizeType
  fanin(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンイン番号のリストを返す．
  virtual
  const vector<SizeType>&
  fanin_list() const;

  /// @brief プリミティブタイプを得る．
  virtual
  PrimType
  primitive_type() const;

  /// @brief ファンインの反転属性を返す．
  virtual
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置番号 ( 0 or 1 )
  ) const;

  /// @brief 関数番号を返す．
  virtual
  SizeType
  local_func_id() const;

  /// @brief セルを得る．
  virtual
  ClibCell
  cell() const;

  /// @brief 複製を作る．
  virtual
  unique_ptr<NodeImpl>
  copy() const = 0;

#if 0
public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力として定義されたことをセットする．
  void
  set_input(
    SizeType iid ///< [in] 入力番号
  )
  {
    mType = BnNodeType::INPUT;
    mExtId = iid;
  }

  /// @brief BnSeq の出力として定義されたことをセットする．
  void
  set_seq_output(
    SizeType seq_id ///< [in] BnSeq のID番号
  )
  {
    mType = BnNodeType::SEQ_OUTPUT;
    mExtId = seq_id;
  }

  /// @brief プリミティブタイプをセットする．
  void
  set_primitive(
    const vector<SizeType>& fanin_list,
    PrimType type
  )
  {
    mType = BnNodeType::PRIMITIVE;
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
    mType = BnNodeType::AIG;
    mFaninList = { src0, src1 };
    mExtId = static_cast<SizeType>(inv0) | (static_cast<SizeType>(inv1) << 1);
  }

  /// @brief 真理値表タイプをセットする．
  void
  set_func(
    const vector<SizeType>& fanin_list,
    SizeType func_id
  )
  {
    mType = BnNodeType::FUNC;
    mFaninList = fanin_list;
    mExtId = func_id;
  }

  /// @brief セルタイプをセットする．
  void
  set_cell(
    const vector<SizeType>& fanin_list,
    ClibCell cell
  )
  {
    mType = BnNodeType::CELL;
    mFaninList = fanin_list;
    mExtId = cell_id;
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

  /// @brief seq_output タイプかどうかチェックする．
  void
  check_seq_output() const
  {
    if ( !is_seq_output() ) {
      throw std::invalid_argument{"not a seq output type"};
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

  /// @brief func タイプかどうかチェックする．
  void
  check_func() const
  {
    if ( !is_func() ) {
      throw std::invalid_argument{"not a func type"};
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの種類
  BnNodeType mType{BnNodeType::NONE};

  // ファンインのID番号のリスト
  vector<SizeType> mFaninList;

  // プリミティブタイプ/カバー番号/論理式番号/関数番号/BDD番号
  SizeType mExtId;
#endif

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_H
