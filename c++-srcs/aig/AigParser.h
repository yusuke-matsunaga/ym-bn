#ifndef AIGPARSER_H
#define AIGPARSER_H

/// @file AigParser.h
/// @brief AigParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bnet.h"


BEGIN_NAMESPACE_YM_BNET

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
    const string& filename ///< [in] ファイル名
    ModelImpl* model       ///< [out] 結果を格納するオブジェクト
  );

  /// @brief AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aig(
    const string& filename ///< [in] ファイル名
    ModelImpl* model       ///< [out] 結果を格納するオブジェクト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////

#if 0
public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号の最大値を返す．
  SizeType
  M() const
  {
    return I() + L() + A();
  }

  /// @brief 入力数を得る．
  SizeType
  I() const
  {
    return mInputList.size();
  }

  /// @brief ラッチ数を得る．
  SizeType
  L() const
  {
    return mLatchList.size();
  }

  /// @brief 出力数を得る．
  SizeType
  O() const
  {
    return mOutputList.size();
  }

  /// @brief ANDノード数を返す．
  SizeType
  A() const
  {
    return mAndList.size();
  }

  /// @brief 入力ノードのリテラルを得る．
  SizeType
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < I() );
    return mInputList[pos].mLiteral;
  }

  /// @brief ラッチノードのリテラルを得る．
  SizeType
  latch(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < L() );
    return mLatchList[pos].mLiteral;
  }

  /// @brief ラッチのソースリテラルを得る．
  SizeType
  latch_src(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < L() );
    return mLatchList[pos].mSrc;
  }

  /// @brief 出力のソースリテラルを得る．
  SizeType
  output_src(
    SizeType pos ///< [in] 出力番号 ( 0 <= pos < O() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < O() );
    return mOutputList[pos].mSrc;
  }

  /// @brief ANDノードのリテラルを得る．
  SizeType
  and_node(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < A() );
    return mAndList[pos].mLiteral;
  }

  /// @brief ANDノードのソース1のリテラルを得る．
  SizeType
  and_src1(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < A() );
    return mAndList[pos].mSrc1;
  }

  /// @brief ANDノードのソース2のリテラルを得る．
  SizeType
  and_src2(
    SizeType pos ///< [in] ANDノード番号 ( 0 <= pos < A() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < A() );
    return mAndList[pos].mSrc2;
  }

  /// @brief 入力のシンボルを得る．
  const string&
  input_symbol(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < I() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < I() );
    return mInputList[pos].mSymbol;
  }

  /// @brief ラッチのシンボルを得る．
  const string&
  latch_symbol(
    SizeType pos ///< [in] ラッチ番号 ( 0 <= pos < L() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < L() );
    return mLatchList[pos].mSymbol;
  }

  /// @brief 出力のシンボルを得る．
  const string&
  output_symbol(
    SizeType pos ///< [in] 出力番号 ( 0 <= pos < O() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < O() );
    return mOutputList[pos].mSymbol;
  }

  /// @brief コメントを得る．
  string
  comment() const
  {
    return mComment;
  }
#endif

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
    SizeTYpe& src  ///< [out] 結果のソースリテラルを格納するオブジェクト
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
    SizeType& src1, ///< [out] 結果のソースリテラル1を格納するオブジェクト
    SizeType& src2  ///< [out] 結果のソースリテラル1を格納するオブジェクト
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

#if 0
  /// @brief ラッチのソースリテラルを設定する．
  void
  set_latch_src(
    SizeType pos, ///< [in] ラッチ番号 ( 0 <= pos < L() )
    SizeType src ///< [in] ソースのリテラル
  )
  {
    ASSERT_COND( 0 <= pos && pos < L() );
    mLatchList[pos].mSrc = src;
  }

  /// @brief 出力のソースハンドルを設定する．
  void
  set_output_src(
    SizeType pos, ///< [in] 出力番号 ( 0 <= pos < O() )
    SizeType src ///< [in] ソースのリテラル
  )
  {
    ASSERT_COND( 0 <= pos && pos < O() );
    mOutputList[pos].mSrc = src;
  }

  /// @brief ANDノードのソースリテラルを設定する．
  void
  set_and_src(
    SizeType pos,   ///< [in] AND番号 ( 0 <= pos < A() )
    SizeType src1, ///< [in] ソース1のハンドル
    SizeType src2  ///< [in] ソース1のハンドル
  )
  {
    ASSERT_COND( 0 <= pos && pos < A() );
    mAndList[pos].mSrc1 = src1;
    mAndList[pos].mSrc2 = src2;
  }

  /// @brief 入力名を設定する．
  void
  set_input_symbol(
    SizeType pos,      ///< [in] 入力番号 ( 0 <= pos < I() )
    const string& name ///< [in] 名前
  )
  {
    ASSERT_COND( 0 <= pos && pos < I() );
    mInputList[pos].mSymbol = name;
  }

  /// @brief ラッチ名を設定する．
  void
  set_latch_symbol(
    SizeType pos,      ///< [in] ラッチ番号 ( 0 <= pos < L() )
    const string& name ///< [in] 名前
  )
  {
    ASSERT_COND( 0 <= pos && pos < L() );
    mLatchList[pos].mSymbol = name;
  }

  /// @brief 出力名を設定する．
  void
  set_output_symbol(
    SizeType pos,      ///< [in] 出力番号 ( 0 <= pos < O() )
    const string& name ///< [in] 名前
  )
  {
    ASSERT_COND( 0 <= pos && pos < O() );
    mOutputList[pos].mSymbol = name;
  }
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

#if 0
  // ノードの情報
  struct InputInfo
  {
    SizeType mLiteral; // リテラル
    string mSymbol;    // シンボル名
  };

  // ラッチノードの情報
  struct LatchInfo
  {
    SizeType mLiteral; // リテラル
    SizeType mSrc;     // ソースリテラル
    string mSymbol;    // シンボル名
  };

  // 出力ノードの情報
  struct OutputInfo
  {
    SizeType mSrc;     // ソースリテラル
    string mSymbol;    // シンボル名
  };

  // ANDノードの情報
  struct AndInfo
  {
    SizeType mLiteral; // リテラル
    SizeType mSrc1;    // ソース1リテラル
    SizeType mSrc2;    // ソース2リテラル
  };

  // 入力ノードのリテラルのリスト
  vector<InputInfo> mInputList;

  // ラッチノードのリスト
  vector<LatchInfo> mLatchList;

  // 出力ノードのリスト
  vector<OutputInfo> mOutputList;

  // ANDノードのソース１のリテラルのリスト
  vector<AndInfo> mAndList;

  // コメント
  string mComment;
#endif

  // 入力ストリーム
  istream* mS{nullptr};

  // ファイルの情報
  FileInfo mFileInfo;

  // 現在の行番号
  SizeType mLineNo;

  // 右端のコラム
  SizeType mLastCol;

  // 定義済みノード番号の印
  vector<bool> mDefined;

};

END_NAMESPACE_YM_BNET

#endif // AIGPARSER_H
