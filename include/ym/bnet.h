#ifndef YM_BNET_H
#define YM_BNET_H

/// @file ym/bnet.h
/// @brief ym-bnet パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bnet 用の名前空間名
#define BNET_NSNAME nsBnet

/// @brief bnet 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BNET \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(BNET_NSNAME)

/// @brief bnet 用の名前空間の終了
#define END_NAMESPACE_YM_BNET \
END_NAMESPACE(BNET_NSNAME) \
END_NAMESPACE_YM

/// @brief bnet 用の名前空間
#define BNET_NAMESPACE YM_NAMESPACE::BNET_NSNAME

BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BnModel;
class BnNode;
class BnCover;

END_NAMESPACE_YM_BNET

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief ノードの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class BnNodeType : std::uint8_t {
  None,  ///< [in] 不正値
  Input, ///< [in] 入力
  Dff,   ///< [in] DFF
  Prim,  ///< [in] プリミティブ型の論理ノード
  Aig,   ///< [in] AIG型の論理ノード
  Cover, ///< [in] カバー型の論理ノード
  Expr,  ///< [in] 論理式型の論理ノード
  Cell   ///< [in] セル型の論理ノード
};

using BNET_NAMESPACE::BnModel;
using BNET_NAMESPACE::BnNode;
using BNET_NAMESPACE::BnCover;

END_NAMESPACE_YM

#endif // YM_BNET_H
