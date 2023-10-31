#ifndef BNNODE_H
#define BNNODE_H

/// @file BnNode.h
/// @brief BnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnNode BnNode.h "BnNode.h"
/// @brief BnModel のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID
/// - 名前
/// - 種類(Input, Prim, Aig, Cover, Expr, Cell, TvFunc, Bdd)
/// - ファンインのノード番号のリスト
/// - プリミティブタイプ(種類がPrimの時のみ)
/// - ファンインの極性(種類がAigの時のみ)
/// - カバー番号(種類がCoverの時のみ)
/// - 論理式番号(種類がExprの時のみ)
/// - セル番号(種類がCellの時のみ)
/// - 真理値表番号(種類がTvFuncの時のみ)
/// - BDD番号(種類がBddの時のみ)
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
//////////////////////////////////////////////////////////////////////
class BnNode
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnNode() = default;

  /// @brief コンストラクタ
  BnNode(
    const shared_ptr<ModelImpl>& impl, ///< [in] 実装本体
    SizeType id                        ///< [in] ノード番号
  );

  /// @brief デストラクタ
  ~BnNode();


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
  BnNodeType
  type() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief BnSeq の出力ノードの時 true を返す．
  bool
  is_seq_output() const;

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

  /// @brief 入力番号を返す．
  ///
  /// is_input() が true の時のみ意味を持つ．
  SizeType
  input_id() const;

  /// @brief 関連する BnSeq を返す．
  ///
  /// is_seq_output() が true の時のみ意味を持つ．
  BnSeq
  seq_node() const;

  /// @brief ファンイン数を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  /// is_logic() が false の時は 0 を返す．
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  BnNode
  fanin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < node_fanin_num() )
  ) const;

  /// @brief ファンインのノードのリストを返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  /// is_logic() が false の時は空リストを返す．
  vector<BnNode>
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
  const BnCover&
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

  /// @brief ノードのセルを返す．
  ClibCell
  cell() const;

  /// @brief ノードの関数番号を返す．
  ///
  /// is_func() が true の時のみ意味を持つ．
  SizeType
  func_id() const;

  /// @brief 関数を返す．
  ///
  /// is_func() が true の時のみ意味を持つ．
  const TvFunc&
  func() const;

  /// @brief ノードのBDD番号を返す．
  ///
  /// is_bdd() が true の時のみ意味を持つ．
  SizeType
  bdd_id() const;

  /// @brief BDDを返す．
  ///
  /// is_bdd() が true の時のみ意味を持つ．
  Bdd
  bdd() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl && mId == right.mId;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNode& right ///< [in] 比較対象のオブジェクト
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

  /// @brief ID 番号から BnNode を作る．
  BnNode
  from_id(
    SizeType id
  ) const;

  /// @brief ID 番号のリストから vector<BnNode> を作る．
  vector<BnNode>
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
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNNODE_H
