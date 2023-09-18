#ifndef BNETCOVER_H
#define BNETCOVER_H

/// @file BnCover.h
/// @brief BnCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnet.h"
#include "ym/SopCover.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BnCover BnCover.h "BnCover.h"
/// @brief blif 形式の .names 本体のカバーを表すクラス
///
/// 内容は SopCover と出力の極性からなる．
//////////////////////////////////////////////////////////////////////
class BnCover
{
public:

  /// @brief コンストラクタ
  BnCover(
    SizeType input_num,                    ///< [in] 入力数
    const vector<vector<Literal>>& icover, ///< [in] 入力カバー
    char opat = '1'                        ///< [in] 出力のパタン ( '0', '1' のみ )
  ) : mInputCover{input_num, icover},
      mOutputPat{opat}
  {
  }

  /// @brief デストラクタ
  ~BnCover() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const { return mInputCover.variable_num(); }

  /// @brief キューブ数を返す．
  SizeType
  cube_num() const { return mInputCover.cube_num(); }

  /// @brief 入力パタンを返す．
  /// @return パタンを返す．
  SopPat
  input_pat(
    SizeType cpos, ///< [in] キューブ番号 ( 0 <= cpos < cube_num() )
    SizeType ipos  ///< [in] 入力番号 ( 0 <= ipos < input_num() )
  ) const
  {
    ASSERT_COND( 0 <= cpos && cpos < cube_num() );
    ASSERT_COND( 0 <= ipos && ipos < input_num() );
    return mInputCover.get_pat(cpos, ipos);
  }

  /// @brief 入力カバーを返す．
  const SopCover&
  input_cover() const { return mInputCover; }

  /// @brief 出力パタンを返す．
  ///
  /// - すべてのキューブに対して同一のパタンとなる．
  /// - ドントケアはない．
  char
  output_pat() const { return mOutputPat; }

  /// @brief 内容を Expr に変換する．
  Expr
  expr() const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力カバー
  SopCover mInputCover;

  // 出力パタン
  char mOutputPat;

};

END_NAMESPACE_YM_BNET

#endif // BNETCOVER_H
