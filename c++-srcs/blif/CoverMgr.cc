
/// @file CoverMgr.cc
/// @brief CoverMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CoverMgr.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_BLIF

BEGIN_NONAMESPACE

// キー生成関数その１
inline
std::string
key_func(
  SizeType input_num,
  const std::string& ipat_str,
  char opat_char
)
{
  std::ostringstream buf;
  buf << input_num << ':'
      << opat_char << ':'
      << ipat_str;
  return buf.str();
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス CoverMgr
//////////////////////////////////////////////////////////////////////

// @brief パタン文字列からカバーを返す．
SizeType
CoverMgr::reg_cover(
  SizeType input_num,
  SizeType cube_num,
  const std::string& ipat_str,
  char opat_char
)
{
  // カバーを表す文字列を作る．
  auto key_str = key_func(input_num, ipat_str, opat_char);

  // すでに登録されているか調べる．
  if ( mCoverDict.count(key_str) > 0 ) {
    auto id = mCoverDict.at(key_str);
    return id;
  }

  // 新しいカバーを作る．
  auto id = new_cover(input_num, cube_num, ipat_str, opat_char);

  // そのカバーを登録する．
  mCoverDict.emplace(key_str, id);
  return id;
}

// @brief SopCover を作る．
SizeType
CoverMgr::new_cover(
  SizeType input_num,
  SizeType cube_num,
  const std::string& ipat_str,
  char opat
)
{
  std::vector<std::vector<Literal>> cube_list;
  cube_list.reserve(cube_num);
  for ( SizeType c = 0; c < cube_num; ++ c ) {
    string ipat = ipat_str.substr(c * input_num, (c + 1) * input_num);
    vector<Literal> cube;
    cube.reserve(input_num);
    for ( SizeType var = 0; var < input_num; ++ var ) {
      switch ( ipat[var] ) {
      case '0': cube.push_back(Literal{var, true}); break;
      case '1': cube.push_back(Literal{var, false}); break;
      case '-': break;
      default: ASSERT_NOT_REACHED; break;
      }
    }
    cube_list.push_back(cube);
  }

  auto id = mCoverList.size();
  mCoverList.push_back({SopCover(input_num, cube_list), opat});
  return id;
}

END_NAMESPACE_YM_BLIF
