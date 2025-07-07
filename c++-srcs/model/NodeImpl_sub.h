#ifndef NODEIMPL_SUB_H
#define NODEIMPL_SUB_H

/// @file NodeImpl_sub.h
/// @brief NodeImpl_sub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "NodeImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_Input NodeImpl_sub.h "NodeImpl_sub.h"
/// @brief 入力ノード用の NodeImpl の基底クラス
//////////////////////////////////////////////////////////////////////
class NodeImpl_Input :
  public NodeImpl
{
public:

  /// @brief コンストラクタ
  NodeImpl_Input();

  /// @brief デストラクタ
  ~NodeImpl_Input();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの時 true を返す．
  ///
  /// 外部入力ノードとDFFの出力ノードのどちらかの時 true となる．
  virtual
  bool
  is_input() const;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_Input NodeImpl_sub.h "NodeImpl_sub.h"
/// @brief 外部入力用の NodeImpl
//////////////////////////////////////////////////////////////////////
class NodeImpl_PrimaryInput :
  public NodeImpl_Input
{
public:

  /// @brief コンストラクタ
  NodeImpl_PrimaryInput(
    SizeType iid ///< [in] 入力番号
  );

  /// @brief デストラクタ
  ~NodeImpl_PrimaryInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  BnNodeType
  type() const override;

  /// @brief 外部入力ノードの時 true を返す．
  bool
  is_primary_input() const override;

  /// @brief 入力番号を返す．
  SizeType
  input_id() const override;

  /// @brief 複製を作る．
  unique_ptr<NodeImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号
  SizeType mInputId;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_DffOutput NodeImpl_sub.h "NodeImpl_sub.h"
/// @brief DFFの出力ノード用の NodeImpl
//////////////////////////////////////////////////////////////////////
class NodeImpl_DffOutput :
  public NodeImpl_Input
{
public:

  /// @brief コンストラクタ
  NodeImpl_DffOutput(
    SizeType dff_id, ///< [in] DFF番号
    SizeType src_id  ///< [in] 入力のノード番号
  );

  /// @brief デストラクタ
  ~NodeImpl_DffOutput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  BnNodeType
  type() const override;

  /// @brief DFFの出力の時 true を返す．
  bool
  is_dff_output() const override;

  /// @brief DFF番号を返す．
  SizeType
  dff_id() const override;

  /// @brief DFFの入力ノードの番号を返す．
  SizeType
  dff_src() const override;

  /// @brief 複製を作る．
  unique_ptr<NodeImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DFF番号
  SizeType mDffId;

  // 入力のノード番号
  SizeType mSrcId;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_Logic NodeImpl_Logic.h "NodeImpl_Logic.h"
/// @brief 論理ノード用の NodeImpl の基底クラス
//////////////////////////////////////////////////////////////////////
class NodeImpl_Logic :
  public NodeImpl
{
public:

  /// @brief コンストラクタ
  NodeImpl_Logic(
    const vector<SizeType>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief デストラクタ
  ~NodeImpl_Logic();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const override;

  /// @brief ファンイン数を返す．
  SizeType
  fanin_num() const override;

  /// @brief ファンインのノード番号を返す．
  SizeType
  fanin(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const override;

  /// @brief ファンイン番号のリストを返す．
  const vector<SizeType>&
  fanin_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインのリスト
  vector<SizeType> mFaninList;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_Primitive NodeImpl_Primitive.h "NodeImpl_Primitive.h"
/// @brief Primitive 用の NodeImpl
//////////////////////////////////////////////////////////////////////
class NodeImpl_Primitive :
  public NodeImpl_Logic
{
public:

  /// @brief コンストラクタ
  NodeImpl_Primitive(
    PrimType prim_type,                ///< [in] プリミティブタイプ
    const vector<SizeType>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief デストラクタ
  ~NodeImpl_Primitive();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  BnNodeType
  type() const override;

  /// @brief プリミティブタイプを得る．
  PrimType
  primitive_type() const override;

  /// @brief 複製を作る．
  unique_ptr<NodeImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブタイプ
  PrimType mPrimType;

};


//////////////////////////////////////////////////////////////////////
/// @class NodeImpl_Func NodeImpl_Func.h "NodeImpl_Func.h"
/// @brief Func 用の NodeImpl
//////////////////////////////////////////////////////////////////////
class NodeImpl_Func :
  public NodeImpl_Logic
{
public:

  /// @brief コンストラクタ
  NodeImpl_Func(
    SizeType func_id,                  ///< [in] 関数番号
    const vector<SizeType>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief デストラクタ
  ~NodeImpl_Func();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  BnNodeType
  type() const override;

  /// @brief 関数型の論理ノードの時 true を返す．
  bool
  is_func() const override;

  /// @brief 関数番号を返す．
  SizeType
  local_func_id() const override;

  /// @brief 複製を作る．
  unique_ptr<NodeImpl>
  copy() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数番号
  SizeType mFuncId;

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_SUB_H
