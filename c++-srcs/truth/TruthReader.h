#ifndef TRUTHREADER_H
#define TRUTHREADER_H

/// @file TruthReader.h
/// @brief TruthReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnfe.h"


BEGIN_NAMESPACE_YM_BNFE

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class TruthReader TruthReader.h "TruthReader.h"
/// @brief 真理値表形式のファイルを読むためのクラス
//////////////////////////////////////////////////////////////////////
class TruthReader
{
public:

  /// @brief コンストラクタ
  TruthReader() = default;

  /// @brief デストラクタ
  ~TruthReader() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表形式のファイルを読み込む．
  void
  read(
    istream& s,      ///< [in] 入力ストリーム
    ModelImpl* model ///< [in] 結果を格納するオブジェクト
  );

};

END_NAMESPACE_YM_BNFE

#endif // TRUTHREADER_H
