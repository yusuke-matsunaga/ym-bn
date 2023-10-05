#ifndef YM_BNIR_H
#define YM_BNIR_H

/// @file ym/bnir.h
/// @brief ym-bnir パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bnir 用の名前空間名
#define BNIR_NSNAME nsBnir

/// @brief bnir 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BNIR \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(BNIR_NSNAME)

/// @brief bnir 用の名前空間の終了
#define END_NAMESPACE_YM_BNIR \
END_NAMESPACE(BNIR_NSNAME) \
END_NAMESPACE_YM

/// @brief bnir 用の名前空間
#define BNIR_NAMESPACE YM_NAMESPACE::BNIR_NSNAME


BEGIN_NAMESPACE_YM_BNIR

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BnModel;
class BnNode;
class BnCover;

END_NAMESPACE_YM_BNIR

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief ノードの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnNodeType : std::uint8_t {
  None,   ///< [in] 不正値
  Input,  ///< [in] 入力
  Dff,    ///< [in] DFF
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

using BNIR_NAMESPACE::BnModel;
using BNIR_NAMESPACE::BnNode;
using BNIR_NAMESPACE::BnCover;

END_NAMESPACE_YM

#endif // YM_BNIR_H
