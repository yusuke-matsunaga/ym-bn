#ifndef BLIFPARSER_H
#define BLIFPARSER_H

/// @file BlifParser.h
/// @brief BlifParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/ClibCellLibrary.h"
#include "BlifScanner.h"
#include "CoverMgr.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BlifParser BlifParser.h "ym/BlifParser.h"
/// @brief blif形式のファイルを読み込むパーサークラス
//////////////////////////////////////////////////////////////////////
class BlifParser
{
public:

  /// @brief コンストラクタ
  BlifParser(
    ModelImpl* model ///< [in] 結果を格納するオブジェクト
  );

  /// @brief デストラクタ
  ~BlifParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込みを行う(セルライブラリ付き)．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(
    const string& filename,              ///< [in] ファイル名
    const ClibCellLibrary& cell_library, ///< [in] セルライブラリ
    const string& clock_name,            ///< [in] クロック信号の名前
    const string& reset_name             ///< [in] リセット信号の名前
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 識別子番号に関連する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前に対応するID番号を取り出す．
  /// @return 対応するID番号を返す．
  ///
  /// 未登録の場合には新たに作る．
  SizeType
  find_id(
    const string& name,   ///< [in] 名前
    const FileRegion& loc ///< [in] name の位置
  )
  {
    if ( mIdDict.count(name) > 0 ) {
      return mIdDict.at(name);
    }
    mRefLocArray.push_back(loc);
    auto id = mModel->alloc_node();
    mIdDict.emplace(name, id);
    mNameDict.emplace(id, name);
    return id;
  }

  /// @brief ID番号から文字列を得る．
  const string&
  id2str(
    SizeType id ///< [in] ID番号
  )
  {
    return mNameDict.at(id);
  }

  /// @brief 参照位置を返す．
  const FileRegion&
  ref_loc(
    SizeType id ///< [in] ID番号
  )
  {
    ASSERT_COND( 0 <= id && id < mRefLocArray.size() );
    return mRefLocArray[id];
  }

  /// @brief 定義済みの印をつける．
  void
  set_defined(
    SizeType id,          ///< [in] 識別子番号
    const FileRegion& loc ///< [in] 定義位置
  )
  {
    mDefLocDict.emplace(id, loc);
  }

  /// @brief 対応する識別子がすでに定義済みか調べる．
  /// @retval true 定義済み
  /// @retval false 未定義
  bool
  is_defined(
    SizeType id ///< [in] 識別子番号
  ) const
  {
    return mDefLocDict.count(id) > 0;
  }

  /// @brief 定義位置を返す．
  const FileRegion&
  def_loc(
    SizeType id ///< [in] ID番号
  )
  {
    ASSERT_COND( is_defined(id) );
    return mDefLocDict.at(id);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief .model 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_model();

  /// @brief .inputs 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_inputs();

  /// @brief .outputs 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_outputs();

  /// @brief .names 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_names();

  /// @brief .gate 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_gate();

  /// @brief .latch 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_latch();

  /// @brief .exdc 文の読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_exdc();

  /// @brief ダミーの１行読み込みを行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起こった．
  bool
  read_dummy1();

  /// @brief 次のトークンを読み出す．
  void
  next_token();

  /// @brief 直前に読み出したトークンを返す．
  BlifToken
  cur_token() const;

  /// @brief 直前に読み出したトークンが文字列の場合にその文字列を返す．
  string
  cur_string() const;

  /// @brief 直前に読み出したトークンの位置を返す．
  FileRegion
  cur_loc() const;

  /// @brief クロック入力の識別子を返す．
  SizeType
  get_clock_id();

  /// @brief リセット入力の識別子を返す．
  SizeType
  get_reset_id();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  ClibCellLibrary mCellLibrary;

  // 字句解析器
  // この変数は read() 内でのみ有効
  BlifScanner* mScanner;

  // BlifCover を管理するオブジェクト
  CoverMgr mCoverMgr;

  // 結果を格納するオブジェクト
  ModelImpl* mModel;

  // クロック入力の名前
  string mClockName;

  // クロック入力のノード番号
  SizeType mClockId;

  // リセット入力の名前
  string mResetName;

  // リセット入力のノード番号
  SizeType mResetId;

  // 現在のトークン
  BlifToken mCurToken;

  // 現在のトークンの位置
  FileRegion mCurLoc;

  // モデル名
  string mModelName;

  // 名前をキーにしたノード番号の辞書
  unordered_map<string, SizeType> mIdDict;

  // ID番号をキーにした名前の辞書
  unordered_map<SizeType, string> mNameDict;

  // ノードを参照している箇所の配列
  vector<FileRegion> mRefLocArray;

  // ノードを定義している箇所の辞書
  unordered_map<SizeType, FileRegion> mDefLocDict;

};

END_NAMESPACE_YM_BN

#endif // BLIFPARSER_H
