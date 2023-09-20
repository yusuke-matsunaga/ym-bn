#ifndef YM_BNFE_H
#define YM_BNFE_H

/// @file ym/bnfe.h
/// @brief ym-bnfe パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bnfe 用の名前空間名
#define BNFE_NSNAME nsBnfe

/// @brief bnfe 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BNFE \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(BNFE_NSNAME)

/// @brief bnfe 用の名前空間の終了
#define END_NAMESPACE_YM_BNFE \
END_NAMESPACE(BNFE_NSNAME) \
END_NAMESPACE_YM

/// @brief bnfe 用の名前空間
#define BNFE_NAMESPACE YM_NAMESPACE::BNFE_NSNAME

BEGIN_NAMESPACE_YM_BNFE

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BfModel;
class BfNode;
class BfCover;

END_NAMESPACE_YM_BNFE

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief ノードの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BfNodeType : std::uint8_t {
  None,  ///< [in] 不正値
  Input, ///< [in] 入力
  Dff,   ///< [in] DFF
  Prim,  ///< [in] プリミティブ型の論理ノード
  Aig,   ///< [in] AIG型の論理ノード
  Cover, ///< [in] カバー型の論理ノード
  Expr,  ///< [in] 論理式型の論理ノード
  Cell   ///< [in] セル型の論理ノード
};

/// @brief BfNodeType のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  BfNodeType type
)
{
  switch ( type ) {
  case BfNodeType::None:  s << "None"; break;
  case BfNodeType::Input: s << "Input"; break;
  case BfNodeType::Dff:   s << "Dff"; break;
  case BfNodeType::Prim:  s << "Prim"; break;
  case BfNodeType::Aig:   s << "Aig"; break;
  case BfNodeType::Cover: s << "Cover"; break;
  case BfNodeType::Expr:  s << "Expr"; break;
  case BfNodeType::Cell:  s << "Cell"; break;
  }
  return s;
}

using BNFE_NAMESPACE::BfModel;
using BNFE_NAMESPACE::BfNode;
using BNFE_NAMESPACE::BfCover;

END_NAMESPACE_YM

#endif // YM_BNFE_H
