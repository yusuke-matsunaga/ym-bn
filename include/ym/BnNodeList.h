#ifndef BNNODELIST_H
#define BNNODELIST_H

/// @file BnNodeList.h
/// @brief BnNodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnNodeListIter BnNodeListIter.h "BnNodeListIter.h"
/// @brief BnNodeList の反復子
//////////////////////////////////////////////////////////////////////
class BnNodeListIter
{
public:

  /// @brief コンストラクタ
  BnNodeListIter(
    const shared_ptr<const ModelImpl>& model, ///< [in] 親のモデル
    vector<SizeType>::const_iterator iter     ///< [in] 本当の反復子
  ) : mModel{model},
      mIter{iter}
  {
  }

  /// @brief デストラクタ
  ~BnNodeListIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す．
  BnNode
  operator*() const;

  /// @brief 進める．
  void
  operator++()
  {
    ++ mIter;
  }

  /// @brief 等価比較演算
  bool
  operator==(
    const BnNodeListIter& right
  ) const
  {
    return mModel == right.mModel && mIter == right.mIter;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    const BnNodeListIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデル
  shared_ptr<const ModelImpl> mModel;

  // 反復子
  vector<SizeType>::const_iterator mIter;

};


//////////////////////////////////////////////////////////////////////
/// @class BnNodeList BnNodeList.h "BnNodeList.h"
/// @brief BnNode のリストを表すクラス
///
/// 実際には ModelImpl とノード番号のリストを持つ．
//////////////////////////////////////////////////////////////////////
class BnNodeList
{
  friend ModelImpl;

public:

  using iterator = BnNodeListIter;

private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// これは ModelImpl しか使えない．
  BnNodeList(
    const shared_ptr<const ModelImpl>& model, ///< [in] 親のモデル
    const vector<SizeType>& id_list           ///< [in] ノード番号のリスト
  ) : mModel{model},
      mIdList{id_list}
  {
  }


public:

  /// @brief デストラクタ
  ~BnNodeList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空の時に true を返す．
  bool
  empty() const
  {
    return mIdList.empty();
  }

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mIdList.size();
  }

  /// @brief 要素を返す．
  BnNode
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
  ) const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator{mModel, mIdList.begin()};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator{mModel, mIdList.end()};
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNodeList& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mModel == right.mModel && mIdList == right.mIdList;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNodeList& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }

  /// @brief vector<BnNode> との等価比較関数
  bool
  operator==(
    const vector<BnNode>& right
  ) const;

  /// @brief vector<BnNode> との非等価比較関数
  bool
  operator!=(
    const vector<BnNode>& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モデルの実装本体
  shared_ptr<const ModelImpl> mModel{nullptr};

  // ノード番号のリスト
  vector<SizeType> mIdList;

};

/// @brief vector<BnNode> との等価比較関数
inline
bool
operator==(
  const vector<BnNode>& left,
  const BnNodeList& right
)
{
  return right.operator==(left);
}

/// @brief vector<BnNode> との非等価比較関数
inline
bool
operator!=(
  const vector<BnNode>& left,
  const BnNodeList& right
)
{
  return right.operator!=(left);
}

END_NAMESPACE_YM_BN

#endif // BNNODELIST_H
