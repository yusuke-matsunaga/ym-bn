#ifndef BFNODE_H
#define BFNODE_H

/// @file BfNode.h
/// @brief BfNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnfe.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BNFE

class ModelImpl;
class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class BfNode BfNode.h "BfNode.h"
/// @brief BfModel のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID
/// - 名前
/// - 種類(Input, Dff, Prim, Aig, Cover, Cell)
/// - ファンインのノード番号のリスト
/// - プリミティブタイプ(種類がPrimの時のみ)
/// - ファンインの極性(種類がAigの時のみ)
/// - カバー番号(種類がCoverの時のみ)
/// - セル番号(種類がCellの時のみ)
//////////////////////////////////////////////////////////////////////
class BfNode
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BfNode() = default;

  /// @brief コンストラクタ
  BfNode(
    const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
    SizeType id                       ///< [in] ノード番号
  ) : mImpl{impl},
      mId{id}
  {
  }

  /// @brief デストラクタ
  ~BfNode();


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

  /// @brief 親の BfModel を返す．
  BfModel
  parent_model() const;

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief ノード名を返す．
  const string&
  name() const;

  /// @brief ノードの種類を返す．
  BfNodeType
  type() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const;

  /// @brief PRIM タイプの論理ノードの時 true を返す．
  bool
  is_primitive() const;

  /// @brief AIG タイプの論理ノードの時 true を返す．
  bool
  is_aig() const;

  /// @brief COVER タイプの論理ノードの時 true を返す．
  bool
  is_cover() const;

  /// @brief EXPR タイプの論理ノードの時 true を返す．
  bool
  is_expr() const;

  /// @brief CELL タイプの論理ノードの時 true を返す．
  bool
  is_cell() const;

  /// @brief FUNC タイプの論理ノードの時 true を返す．
  bool
  is_func() const;

  /// @brief BDD タイプの論理ノードの時 true を返す．
  bool
  is_bdd() const;

  /// @brief DFFノードの時 true を返す．
  bool
  is_dff() const;

  /// @brief 入力番号を返す．
  ///
  /// is_input() が true の時のみ意味を持つ．
  SizeType
  input_id() const;

  /// @brief ファンイン数を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  BfNode
  fanin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < node_fanin_num() )
  ) const;

  /// @brief ファンインのノードのリストを返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  vector<BfNode>
  fanin_list() const;

  /// @brief プリミティブタイプを返す．
  ///
  /// is_primitive() が true の時のみ意味を持つ．
  PrimType
  primitive_type() const;

  /// @brief ファンインの反転属性を返す．
  ///
  /// is_aig() が true の時のみ意味を持つ．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置番号 ( 0 or 1 )
  ) const;

  /// @brief カバー番号を返す．
  SizeType
  cover_id() const;

  /// @brief カバーを返す．
  ///
  /// is_cover() が true の時のみ意味を持つ．
  const BfCover&
  cover() const;

  /// @brief 論理式番号を返す．
  SizeType
  expr_id() const;

  /// @brief 論理式を返す．
  const Expr&
  expr() const;

  /// @brief ノードのセル番号を返す．
  ///
  /// is_cell() が true の時のみ意味を持つ．
  SizeType
  cell_id() const;

  /// @brief ノードの関数番号を返す．
  ///
  /// is_func() が true の時のみ意味を持つ．
  SizeType
  func_id() const;

  /// @brief ノードのBDD番号を返す．
  ///
  /// is_bdd() が true の時のみ意味を持つ．
  SizeType
  bdd_id() const;

  /// @brief DFFノードの入力ノードを返す．
  ///
  /// is_dff() が true の時のみ意味を持つ．
  BfNode
  dff_src() const;

  /// @brief DFFノードのリセット値を返す．
  ///
  /// is_dff() が true の時のみ意味を持つ．
  char
  dff_rval() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BfNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl && mId == right.mId;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BfNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの実体を返す．
  const NodeImpl&
  node_impl() const;

  /// @brief ID 番号から BfNode を作る．
  BfNode
  from_id(
    SizeType id
  ) const;

  /// @brief ID 番号のリストから vector<BfNode> を作る．
  vector<BfNode>
  from_id_list(
    const vector<SizeType>& id_list
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  shared_ptr<ModelImpl> mImpl{nullptr};

  // ノード番号
  SizeType mId{0};

};

END_NAMESPACE_YM_BNFE

#endif // BFNODE_H
