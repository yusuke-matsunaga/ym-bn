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

/// @brief 不正なノード番号
const SizeType BAD_ID = -1;

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BnModel;
class BnNode;
class BnDff;
class BnCover;

END_NAMESPACE_YM_BN

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief BnNode の種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnNodeType : std::uint8_t {
  NONE,      ///< 不正値
  INPUT,     ///< 入力
  PRIMITIVE, ///< プリミティブ型の論理ノード
  AIG,       ///< AIG型の論理ノード
  COVER,     ///< カバー型の論理ノード
  EXPR,      ///< 論理式型の論理ノード
  CELL,      ///< セル型の論理ノード
  TVFUNC,    ///< 真理値表型の論理ノード
  BDD        ///< BDD型の論理ノード
};


//////////////////////////////////////////////////////////////////////
/// @brief BnDff の種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnDffType : std::uint8_t {
  NONE,   ///< 不正値
  DFF,    ///< D-FF
  LATCH,  ///< ラッチ
  CELL    ///< セル
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
  case BnNodeType::NONE:      s << "None"; break;
  case BnNodeType::INPUT:     s << "Input"; break;
  case BnNodeType::PRIMITIVE: s << "Primitive"; break;
  case BnNodeType::AIG:       s << "AIG"; break;
  case BnNodeType::COVER:     s << "Cover"; break;
  case BnNodeType::EXPR:      s << "Expr"; break;
  case BnNodeType::CELL:      s << "Cell"; break;
  case BnNodeType::TVFUNC:    s << "TvFunc"; break;
  case BnNodeType::BDD:       s << "BDD"; break;
  }
  return s;
}

/// @brief BnDffType のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  BnDffType type
)
{
  switch ( type ) {
  case BnDffType::NONE:  s << "None"; break;
  case BnDffType::DFF:   s << "DFF"; break;
  case BnDffType::LATCH: s << "Latch"; break;
  case BnDffType::CELL:  s << "Cell"; break;
  }
  return s;
}

using BN_NAMESPACE::BnModel;
using BN_NAMESPACE::BnNode;
using BN_NAMESPACE::BnDff;
using BN_NAMESPACE::BnCover;

END_NAMESPACE_YM

#endif // YM_BN_H
