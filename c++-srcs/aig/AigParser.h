#ifndef AIGPARSER_H
#define AIGPARSER_H

/// @file AigParser.h
/// @brief AigParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class AigParser AigParser.h "AigParser.h"
/// @brief AIG 形式のファイルを読むためのクラス
//////////////////////////////////////////////////////////////////////
class AigParser
{
public:

  /// @brief コンストラクタ
  AigParser() = default;

  /// @brief デストラクタ
  ~AigParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ファイル入出力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief Ascii AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aag(
    const string& filename, ///< [in] ファイル名
    ModelImpl* model        ///< [out] 結果を格納するオブジェクト
  );

  /// @brief AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aig(
    const string& filename, ///< [in] ファイル名
    ModelImpl* model        ///< [out] 結果を格納するオブジェクト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  bool
  open(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aag ヘッダ行を読み込む．
  bool
  read_aag_header(
    SizeType& M,
    SizeType& I,
    SizeType& L,
    SizeType& O,
    SizeType& A
  );

  /// @brief aig ヘッダ行を読み込む．
  bool
  read_aig_header(
    SizeType& M,
    SizeType& I,
    SizeType& L,
    SizeType& O,
    SizeType& A
  );

  /// @brief 初期化する．
  void
  initialize(
    SizeType M,      ///< [in] 総ノード数
    ModelImpl* model ///< [in] 結果を格納するオブジェクト
  );

  /// @brief aag の入力行の読み込み
  bool
  read_input(
    SizeType& id ///< [out] 結果のIDを格納するオブジェクト
  );

  /// @brief aag のラッチ行の読み込み
  bool
  read_latch(
    SizeType& id,  ///< [out] 結果のIDを格納するオブジェクト
    SizeType& src  ///< [out] 結果のソースリテラルを格納するオブジェクト
  );

  /// @brief aag の出力行/aig のラッチ行の読み込み
  bool
  read_src(
    SizeType& src ///< [out] 結果のソースリテラルを格納するオブジェクト
  );

  /// @brief aag のAND行の読み込み
  bool
  read_and(
    SizeType& id,   ///< [out] 結果のIDを格納するオブジェクト
    SizeType& src0, ///< [out] 結果のソースリテラル0を格納するオブジェクト
    SizeType& src1  ///< [out] 結果のソースリテラル1を格納するオブジェクト
  );

  /// @brief aig の数字の読み込み
  SizeType
  read_number();

  /// @brief シンボルテーブルとコメントを読み込む．
  void
  read_symbols();

  /// @brief 1行読み出す．
  bool
  read_line(
    string& buf  ///< [out] 結果を格納するオブジェクト
  );

  /// @brief リテラルを定義済みにする．
  ///
  /// このリテラルは正のリテラルである必要がある．
  bool
  set_defined(
    SizeType lit ///< [in] リテラル
  );

  /// @brief リテラルが定義済みか調べる．
  bool
  check_defined(
    SizeType lit,     ///< [in] リテラル
    const string& ref ///< [in] このリテラルを参照しているオブジェクト名
  );

  /// @brief リテラルからノードの情報を得る．
  ///
  /// 通常は2で割って1引くだけだが，定数の場合定数ノードを返す．
  SizeType
  lit2node(
    SizeType lit, ///< [in] リテラル
    bool& inv     ///< [out] 反転属性
  );

  /// @brief リテラルからノードの情報を得る．
  ///
  /// こちらは反転属性を使わないバージョン
  SizeType
  lit2node(
    SizeType lit ///< [in] リテラル
  );

  /// @brief 定数0ノードを返す．
  SizeType
  const0();

  /// @brief 定数1ノードを返す．
  SizeType
  const1();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream* mS{nullptr};

  // ファイルの情報
  FileInfo mFileInfo;

  // 現在の行番号
  int mLineNo;

  // 右端のコラム
  int mLastCol;

  // 対象のモデル
  ModelImpl* mModel{nullptr};

  // 定義済みノード番号の印
  vector<bool> mDefined;

  // 定数0のノード番号
  SizeType mConst0;

  // 定数1のノード番号
  SizeType mConst1;

  // 反転出力の辞書
  unordered_map<SizeType, SizeType> mInvDict;

};

END_NAMESPACE_YM_BN

#endif // AIGPARSER_H
