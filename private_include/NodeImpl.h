#ifndef NODEIMPL_H
#define NODEIMPL_H

/// @file NodelImpl.h
/// @brief NodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnNode の実装クラス
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
  BnNode::Type
  type() const = 0;

  /// @brief 入力ノードの時 true を返す．
  ///
  /// 外部入力ノードとDFFの出力ノードのどちらかの時 true となる．
  virtual
  bool
  is_input() const;

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_primary_input() const;

  /// @brief DFFの出力の時 true を返す．
  virtual
  bool
  is_dff_output() const;

  /// @brief 論理ノードの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief 入力番号を返す．
  virtual
  SizeType
  input_id() const;

  /// @brief DFF番号を返す．
  virtual
  SizeType
  dff_id() const;

  /// @brief DFFの入力ノードの番号を返す．
  virtual
  SizeType
  dff_src() const;

  /// @brief プリミティブタイプを得る．
  virtual
  PrimType
  primitive_type() const;

  /// @brief カバー情報を持っている時 true を返す．
  virtual
  bool
  has_cover() const;

  /// @brief カバーを返す．
  ///
  /// has_cover() == false の時は std::invalid_argument 例外を送出する．
  virtual
  const SopCover&
  cover() const;

  /// @brief 論理式情報を持っている時 true を返す．
  virtual
  bool
  has_expr() const;

  /// @brief 論理式を返す．
  /// has_expr() == false の時は std::invalid_argument 例外を送出する．
  virtual
  Expr
  expr() const;

  /// @brief 倫理値表を持っている時 true を返す．
  virtual
  bool
  has_tvfunc() const;

  /// @brief 真理値表を返す．
  virtual
  const TvFunc&
  tvfunc() const;

  /// @brief BDDを持っている時 true を返す．
  virtual
  bool
  has_bdd() const;

  /// @brief BDDを返す．
  virtual
  Bdd
  bdd() const;

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

  /// @brief 複製を作る．
  virtual
  unique_ptr<NodeImpl>
  copy() const = 0;

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_H
