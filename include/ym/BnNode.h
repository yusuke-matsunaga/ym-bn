#ifndef BNNODE_H
#define BNNODE_H

/// @file BnNode.h
/// @brief BnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;
class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnNode BnNode.h "BnNode.h"
/// @brief BnModel のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
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
/// ただし，空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() は false を返し，id() は BAD_ID を返すが，
/// それ以外のメンバ関数の呼び出しは std::invalid_argument 例外を送出する．
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
///
/// 通常は BnModel のみが生成/設定を行う．
//////////////////////////////////////////////////////////////////////
class BnNode
{
  friend class ModelImpl;

private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// これは ModelImpl のみが使用する．
  BnNode(
    const shared_ptr<const ModelImpl>& model, ///< [in] 親のモデル．
    SizeType id                               ///< [in] ノード番号
  );


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnNode() = default;

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
    return mModel != nullptr;
  }

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief ノードの種類を返す．
  BnNodeType
  type() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードの時 true を返す．
  ///
  /// is_input() を包含している．
  bool
  is_primary_input() const;

  /// @brief DFFの出力ノードの時 true を返す．
  ///
  /// is_input() を包含している．
  bool
  is_sdff_output() const;

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const;

  /// @brief プリミティブ型の論理ノードの時 true を返す．
  ///
  /// is_logic() を包含している．
  bool
  is_primitive() const;

  /// @brief 関数タイプの論理ノードの時 true を返す．
  ///
  /// is_logic() を包含している．
  bool
  is_func() const;

  /// @brief 入力番号を返す．
  ///
  /// - is_primary_input() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  SizeType
  input_id() const;

  /// @brief DFF番号を返す．
  ///
  /// - is_dff_output() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  SizeType
  dff_id() const;

  /// @brief DFFの入力ノードを返す．
  ///
  /// - is_dff_output() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  BnNode
  dff_input() const;

  /// @brief ファンイン数を返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - is_logic() が false の時は 0 を返す．
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  /// - pos が範囲外の時は std::out_of_range 例外を送出する．
  BnNode
  fanin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンインのノードのリストを返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - is_logic() が false の時は空リストを返す．
  std::vector<BnNode>
  fanin_list() const;

  /// @brief ノードのプリミティブタイプを返す．
  ///
  /// - is_primitive() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  PrimType
  primitive_type() const;

  /// @brief ノードの関数情報を返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  BnFunc
  local_func() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mModel == right.mModel && mId == right.mId;
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
  const NodeImpl*
  _impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデルの実装本体
  shared_ptr<const ModelImpl> mModel{nullptr};

  // ノード番号
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNNODE_H
