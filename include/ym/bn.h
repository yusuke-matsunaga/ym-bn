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
class BnNodeList;
class BnSeq;
class BnSeqList;
class BnFunc;

END_NAMESPACE_YM_BN

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief BnNode の種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnNodeType : std::uint8_t {
  NONE,       ///< 不正値
  INPUT,      ///< 入力
  SEQ_OUTPUT, ///< BnSeq の出力(疑似入力)
  PRIMITIVE,  ///< プリミティブ型の論理ノード
  AIG,        ///< AIG型の論理ノード
  FUNC,       ///< 関数型の論理ノード
  CELL        ///< セル型の論理ノード
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
  case BnNodeType::NONE:       s << "None"; break;
  case BnNodeType::INPUT:      s << "Input"; break;
  case BnNodeType::SEQ_OUTPUT: s << "Seq Output"; break;
  case BnNodeType::PRIMITIVE:  s << "Primitive"; break;
  case BnNodeType::AIG:        s << "AIG"; break;
  case BnNodeType::FUNC:       s << "Function"; break;
  case BnNodeType::CELL:       s << "Cell"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief BnFunc の種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnFuncType : std::uint8_t {
  NONE,       ///< 不正値
  COVER,      ///< カバー型
  EXPR,       ///< 論理式型
  TVFUNC,     ///< 真理値表型
  BDD         ///< BDD型
};

/// @brief BnFuncType のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  BnFuncType type
)
{
  switch ( type ) {
  case BnFuncType::NONE:       s << "None"; break;
  case BnFuncType::COVER:      s << "Cover"; break;
  case BnFuncType::EXPR:       s << "Expr"; break;
  case BnFuncType::TVFUNC:     s << "TvFunc"; break;
  case BnFuncType::BDD:        s << "BDD"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief BnSeq の種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnSeqType : std::uint8_t {
  NONE,   ///< 不正値
  DFF,    ///< D-FF
  LATCH,  ///< ラッチ
  CELL    ///< セル
};

/// @brief BnSeqType のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  BnSeqType type
)
{
  switch ( type ) {
  case BnSeqType::NONE:  s << "None"; break;
  case BnSeqType::DFF:   s << "DFF"; break;
  case BnSeqType::LATCH: s << "Latch"; break;
  case BnSeqType::CELL:  s << "Cell"; break;
  }
  return s;
}


using BN_NAMESPACE::BnModel;
using BN_NAMESPACE::BnNode;
using BN_NAMESPACE::BnNodeList;
using BN_NAMESPACE::BnSeq;
using BN_NAMESPACE::BnSeqList;
using BN_NAMESPACE::BnFunc;

END_NAMESPACE_YM

#endif // YM_BN_H
