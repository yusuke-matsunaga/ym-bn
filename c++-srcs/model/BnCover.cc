
/// @file BnCover.cc
/// @brief BnCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnCover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnCover
//////////////////////////////////////////////////////////////////////

Expr
BnCover::expr() const
{
  auto expr = mInputCover.expr();
  if ( output_pat() == '0' ) {
    expr = ~expr;
  }
  return expr;
}

// @brief 内容を出力する．
void
BnCover::print(
  ostream& s
) const
{
  for ( SizeType c = 0; c < cube_num(); ++ c ) {
    for ( SizeType i = 0; i < input_num(); ++ i ) {
      s << input_pat(c, i);
    }
    if ( input_num() > 0 ) {
      s << ' ';
    }
    s << output_pat() << endl;
  }
}

END_NAMESPACE_YM_BN
