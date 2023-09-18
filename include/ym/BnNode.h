#ifndef BNNODE_H
#define BNNODE_H

/// @file BnNode.h
/// @brief BnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnet.h"


BEGIN_NAMESPACE_YM_BNET

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnNode BnNode.h "BnNode.h"
/// @brief BnModel のノードを表すクラス
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
class BnNode
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnNode() = default;

  /// @brief コンストラクタ
  BnNode(
    ModelImpl* impl, ///< [in] 実装本体
    SizeType id      ///< [in] ノード番号
  ) : mImpl{mImpl},
      mId{id}
  {
  }

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

  /// @brief CELL タイプの論理ノードの時 true を返す．
  bool
  is_cell() const;

  /// @brief DFFノードの時 true を返す．
  bool
  is_dff() const;

  /// @brief ファンイン数を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
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
  vector<BnNode>
  fanin_list() const;

  /// @brief プリミティブタイプを返す．
  ///
  /// is_primitive() が true の時のみ意味を持つ．
  PrimType
  prim_type() const;

  /// @brief ファンインの反転属性を返す．
  ///
  /// is_aig() が true の時のみ意味を持つ．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置番号 ( 0 or 1 )
  ) const;

  /// @brief カバーを返す．
  ///
  /// is_cover() が true の時のみ意味を持つ．
  const BnCover&
  cover() const;

  /// @brief ノードのセル番号を返す．
  ///
  /// is_cell() が true の時のみ意味を持つ．
  SizeType
  cell_id() const;

  /// @brief DFFノードの入力ノードを返す．
  ///
  /// is_dff() が true の時のみ意味を持つ．
  BnNode
  dff_src() const;

  /// @brief DFFノードのリセット値を返す．
  ///
  /// is_dff() が true の時のみ意味を持つ．
  char
  dff_rval() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  ModelImpl* mImpl{nullptr};

  // ノード番号
  SizeType mId{0};

};

END_NAMESPACE_YM_BNET

#endif // BNNODE_H
