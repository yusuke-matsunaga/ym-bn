#ifndef COVERMGR_H
#define COVERMGR_H

/// @file CoverMgr.h
/// @brief CoverMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class CoverMgr CoverMgr.h "CoverMgr.h"
/// @brief カバーを管理するクラス
//////////////////////////////////////////////////////////////////////
class CoverMgr
{
public:

  /// @brief コンストラクタ
  CoverMgr() = default;

  /// @brief デストラクタ
  ~CoverMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを登録してカバー番号を返す．
  SizeType
  reg_cover(
    SizeType input_num,          ///< [in] 入力数
    SizeType cube_num,           ///< [in] キューブ数
    const std::string& ipat_str, ///< [in] 入力パタン文字列
    char opat_char               ///< [in] 出力パタン
  );

  /// @brief 登録されているカバー数を返す。
  SizeType
  cover_num() const
  {
    return mCoverList.size();

  }
  /// @brief カバーを返す．
  const SopCover&
  cover(
    SizeType cover_id ///< [in] カバー番号 ( 0 <= cover_id < cover_num() )
  ) const
  {
    if ( cover_id >= mCoverList.size() ) {
      throw std::out_of_range{"cover_id is out of range"};
    }
    return mCoverList[cover_id].cover;
  }

  /// @brief 出力パタンを返す．
  char
  opat(
    SizeType cover_id ///< [in] カバー番号 ( 0 <= cover_id < cover_num() )
  ) const
  {
    if ( cover_id >= mCoverList.size() ) {
      throw std::out_of_range{"cover_id is out of range"};
    }
    return mCoverList[cover_id].opat;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを作る．
  /// @return 作成されたカバーの ID 番号を返す．
  SizeType
  new_cover(
    SizeType input_num,          ///< [in] 入力数
    SizeType cube_num,           ///< [in] キューブ数
    const std::string& ipat_str, ///< [in] 入力パタン文字列
    char opat_char               ///< [in] 出力パタン
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カバーの情報を表すクラス
  struct CoverInfo {
    SopCover cover; ///< カバー
    char opat;      ///< 出力パタン('1' or '0')
  };

  // カバーのリスト
  std::vector<CoverInfo> mCoverList;

  // カバーを表す文字列をキーにしてカバー番号を納める辞書
  std::unordered_map<std::string, SizeType> mCoverDict;

};

END_NAMESPACE_YM_BN

#endif // COVERMGR_H
