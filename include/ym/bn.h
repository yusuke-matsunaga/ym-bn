#ifndef YM_BN_H
#define YM_BN_H

/// @file ym/bn.h
/// @brief ym-bn パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bn 用の名前空間名
#define BN_NSNAME nsBn

/// @brief bn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(BN_NSNAME)

/// @brief bn 用の名前空間の終了
#define END_NAMESPACE_YM_BN \
END_NAMESPACE(BN_NSNAME) \
END_NAMESPACE_YM

/// @brief bn 用の名前空間
#define BN_NAMESPACE YM_NAMESPACE::BN_NSNAME


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BnModel;
class BnNode;
class BnCover;

END_NAMESPACE_YM_BN

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief ノードの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnNodeType : std::uint8_t {
  None,   ///< [in] 不正値
  Input,  ///< [in] 入力
  Dff,    ///< [in] DFF
  Latch,  ///< [in] ラッチ
  Prim,   ///< [in] プリミティブ型の論理ノード
  Aig,    ///< [in] AIG型の論理ノード
  Cover,  ///< [in] カバー型の論理ノード
  Expr,   ///< [in] 論理式型の論理ノード
  Cell,   ///< [in] セル型の論理ノード
  TvFunc, ///< [in] 真理値表型の論理ノード
  Bdd     ///< [in] BDD型の論理ノード
};

/// @brief BnNodeType のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  BnNodeType type
)
{
  switch ( type ) {
  case BnNodeType::None:   s << "None"; break;
  case BnNodeType::Input:  s << "Input"; break;
  case BnNodeType::Dff:    s << "Dff"; break;
  case BnNodeType::Latch:  s << "Latch"; break;
  case BnNodeType::Prim:   s << "Prim"; break;
  case BnNodeType::Aig:    s << "Aig"; break;
  case BnNodeType::Cover:  s << "Cover"; break;
  case BnNodeType::Expr:   s << "Expr"; break;
  case BnNodeType::Cell:   s << "Cell"; break;
  case BnNodeType::TvFunc: s << "TvFunc"; break;
  case BnNodeType::Bdd:    s << "Bdd"; break;
  }
  return s;
}

using BN_NAMESPACE::BnModel;
using BN_NAMESPACE::BnNode;
using BN_NAMESPACE::BnCover;

END_NAMESPACE_YM

#endif // YM_BN_H
