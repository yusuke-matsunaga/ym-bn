
/// @file AigParser.cc
/// @brief AigParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "AigParser.h"
#include "ym/BnModel.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_BNET

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BnMode
//////////////////////////////////////////////////////////////////////

// @brief aag ファイルの読み込みを行う．
BnModel
BnModel::read_aag(
  const string& filename
)
{
  BnModel model;

  AigParser parser;
  if ( !parser.read_aag(filename, model.mImpl) ) {
    ostringstream buf;
    buf << "BnModel::read_aag(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return model;
}

// @brief aig ファイルの読み込みを行う．
BnModel
BnModel::read_aig(
  const string& filename
)
{
  BnModel model;

  AigParser parser;
  if ( !parser.read_aig(filename, model.mImpl) ) {
    ostringstream buf;
    buf << "BnModel::read_aig(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return model;
}


//////////////////////////////////////////////////////////////////////
// クラス AigParser
//////////////////////////////////////////////////////////////////////

// @brief Ascii AIG フォーマットを読み込む．
bool
AigParser::read_aag(
  const string& filename,
  ModelImpl* model
)
{
  if ( !open(filename) ) {
    return true;
  }

  // ヘッダ行の読み込み
  SizeType M, I, L, O, A;
  if ( !read_aag_header(M, I, L, O, A) ) {
    return false;
  }

  initialize(M, model);

  // 入力行の読み込み
  for ( SizeType i = 0; i < I; ++ i ) {
    SizeType id;
    if ( !read_input(id) ) {
      return false;
    }
    if ( debug ) {
      cout << "I#" << i << ": " << id << endl;
    }
    model->set_input(id);
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    SizeType id;
    SizeType src;
    if ( !read_latch(id, src) ) {
      return false;
    }
    if ( debug ) {
      cout << "L#" << i << ": " << id << " " << src << endl;
    }
    model->set_dff(id, src, ' ');
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src;
    if ( !read_output(src) ) {
      return false;
    }
    if ( debug ) {
      cout << "O#" << i << ": " << src << endl;
    }
    mOutputList[i].mSrc = src;
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    if ( !getline(s, linebuf) ) {
      throw std::invalid_argument{"Unexpected EOF"};
    }
    SizeType id;
    SizeType src0, src1;
    if ( !read_and(id, src0, src1) ) {
      return false;
    }
    if ( debug ) {
      cout << "A#" << i << ": " << id << " " << src0 << " " << src1 << endl;
    }
    mAndList[i].mLiteral = lit;
    mAndList[i].mSrc1 = src0;
    mAndList[i].mSrc2 = src1;
  }

  // ラッチのソースが定義されているかチェック
  for ( SizeType i = 0; i < L; ++ i ) {
    auto src = latch_src(i);
    ostringstream buf;
    buf << "Latch#" << i  << "(" << latch(i) << ")";
    if ( !check_defined(src, buf.str()) ) {
      return false;
    }
  }
  // 出力のソースが定義されているかのチェック
  for ( SizeType i = 0; i < O; ++ i ) {
    auto src = output_src(i);
    ostringstream buf;
    buf < "Output#" << i;
    if ( !check_defined(src, buf.str()) ) {
      return false;
    }
  }
  // ANDの2つのソースが定義されているかのチェック
  for ( SizeType i = 0; i < A; ++ i ) {
    ostringstream buf;
    buf "And#" << i << "(" << and_node(i) << ")";
    auto src1 = and_src1(i);
    if ( !check_defined(src1, buf.str()) ) {
      return false;
    }
    auto src2 = and_src2(i);
    if ( !check_defined(src2, buf.str()) ) {
      return false;
    }
  }

  // シンボルテーブルとコメントの読み込みを行う．
  read_symbols(s);

  return true;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief AIG フォーマットを読み込む．
bool
AigParser::read_aig(
  const string& filename,
  ModelImpl* model
)
{
  if ( !open(filename) ) {
    return true;
  }

  // ヘッダ行の読み込み
  SizeType M, I, L, O, A;
  if ( !read_aig_header(M, I, L, O, A) ) {
    return false;
  }

  if ( debug ) {
    cout << "aig " << M
	 << " " << I
	 << " " << L
	 << " " << O
	 << " " << A << endl;
  }

  // M は捨てる．
  initialize(I, L, O, A);

  // 入力の情報を作る．
  for ( SizeType i = 0; i < I; ++ i ) {
    mInputList[i].mLiteral = (i + 1) * 2;
  }

  // ラッチ行の読み込み
  for ( SizeType i = 0; i < L; ++ i ) {
    SizeType src;
    if ( !read_src(src) ) {
      return false;
    }
    if ( debug ) {
      cout << "L#" << i << ": " << src << endl;
    }
    mLatchList[i] = LatchInfo{(i + I + 1) * 2, src};
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src;
    if ( !read_src(src) ) {
      return false;
    }
    if ( debug ) {
      cout << "O#" << i << ": " << src << endl;
    }
    mOutputList[i] = OutputInfo{src};
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType d0 = read_number();
    SizeType d1 = read_number();
    SizeType pos = i + I + L + 1;
    SizeType lhs = pos * 2;
    SizeType rhs0 = lhs - d0;
    SizeType rhs1 = rhs0 - d1;
    if ( debug ) {
      cout << "A#" << i << ": "
	   << d0 << " " << d1
	   << " -> " << rhs0 << " " << rhs1 << endl;
    }
    mAndList[i] = AndInfo{(i + I + L + 1) * 2, rhs0, rhs1};
  }

  // シンボルの読み込み
  read_symbols();

  return true;
}

// @brief aag ヘッダ行を読み込む．
bool
AigParser::read_aag_header(
  SizeType& M,
  SizeType& I,
  SizeType& L,
  SizeType& O,
  SizeType& A
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }
  if ( linebuf.substr(0, 3) != string{"aag"} ) {
    ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aag' expected.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  istringstream tmp{linebuf.substr(4, string::npos)};
  tmp >> M >> I >> L >> O >> A;

  if ( debug ) {
    cout << "aag " << M
	 << " " << I
	 << " " << L
	 << " " << O
	 << " " << A << endl;
  }
}

// @brief ファイルを開く
bool
AigParser::open(
  const string& filename
)
{
  mS = new ifstream{filename};
  if ( !*mS ) {
    ostringstream buf;
    buf << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__, MsgType::Failure, "AIG_PARSER", buf.str());
    delete mS;
    mS = nullptr;
    return false;
  }
  mFileInfo = FileInfo{filename};
  mLineNo = 0;
  mLastCol = 0;
  return true;
}

// @brief aig ヘッダ行を読み込む．
bool
AigParser::read_aig_header(
  SizeType& M,
  SizeType& I,
  SizeType& L,
  SizeType& O,
  SizeType& A
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }
  if ( linebuf.substr(0, 3) != string{"aig"} ) {
    ostringstream buf;
    buf << linebuf << ": Illegal header signature, 'aig' expected.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  istringstream tmp{linebuf.substr(4, string::npos)};
  tmp >> M >> I >> L >> O >> A;

  if ( M != (I + L + A) ) {
    ostringstream buf;
    buf << linebuf << ": wrong parameters";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }

  if ( debug ) {
    cout << "aag " << M
	 << " " << I
	 << " " << L
	 << " " << O
	 << " " << A << endl;
  }
}

// @brief 内容を初期化する．
void
AigParser::initialize(
  SizeType M,
  ModelImpl* model
)
{
  for ( SizeType i = 0; i < M; ++ i ) {
    model->new_node();
  }
  mDefined = vector<bool>(M, false);
}

// @brief aag の入力行の読み込み
bool
AigParser::read_input(
  SizeType& id
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  istringstream tmp{linebuf};
  tmp >> lit;

  if ( !set_defined(lit) ) {
    return false;
  }
  id = lit / 2;
  return true;
}

// @brief aag のラッチ行の読み込み
bool
AigParser::read_latch(
  SizeType& id,
  SizeTYpe& src
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  istringstream tmp{linebuf};
  SizeType lit;
  tmp >> lit >> src;

  if ( !set_defined(lit) ) {
    return false;
  }
  id = lit / 2;
  return true;
}

// @brief aag の出力行の読み込み
bool
AigParser::read_output(
  SizeType& src
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  istringstream tmp{linebuf};
  tmp >> src;
  return true;
}

// @brief aag のAND行の読み込み
bool
AigParser::read_and(
  SizeType& id,
  SizeType& src1,
  SizeType& src2
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  SizeType lit;
  tmp >> lit >> src0 >> src1;

  if ( !set_defined(lit) ) {
    return false;
  }

  id = lit / 2;
  return true;
}

// @brief 数字を一つ読み出す．
SizeType
AigParser::read_number()
{
  SizeType num = 0;
  for ( SizeType weight = 1; ; weight *= 128 ) {
    SizeType c = mS->get();
    num += (c & 127) * weight;
    if ( (c & 128) == 0 ) {
      break;
    }
  }
  return num;
}

// @brief シンボルテーブルとコメントを読み込む．
void
AigParser::read_symbols()
{
  string linebuf;
  bool symbol_mode = true;
  while ( getline(*s, linebuf) ) {
    if ( symbol_mode ) {
      if ( linebuf == "c" ) {
	symbol_mode = false;
      }
      else {
	auto p = linebuf.find_first_of(" ");
	auto pos_str = linebuf.substr(0, p);
	auto name = linebuf.substr(p + 1, string::npos);
	SizeType pos = atoi(pos_str.substr(1, string::npos).c_str());
	char prefix = pos_str[0];
	if ( prefix == 'i' ) {
	  set_input_symbol(pos, name);
	}
	else if ( prefix == 'l' ) {
	  set_latch_symbol(pos, name);
	}
	else if ( prefix == 'o' ) {
	  set_output_symbol(pos, name);
	}
	else {
	  ASSERT_NOT_REACHED;
	}
      }
    }
    else {
      mComment += linebuf + '\n';
    }
  }
}

// @brief 1行読み出す．
bool
AigParser::read_line(
  string& buf
)
{
  ++ mLineNo;
  if ( !getline(*mS, buf) ) {
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, 1},
		    MsgType::Error, "AIG_PARSER", "Unexpected EOF");
    return false;
  }
  mLastCol = buf.size();
  return true;
}

// @brief リテラルを定義済みにする．
bool
AigParser:: set_defined(
  SizeType lit
)
{
  if ( (lit % 2) == 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER",
		    "Positive Literal(even number) expected");
    return false;
  }
  if ( lit <= 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER",
		    "Unexpected constant literal");
    return false;
  }

  SizeType id = (lit / 2) - 1;
  if ( mDefined[id] ) {
    ostringstream buf;
    buf << id << " is already defined.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
    return false;
  }
  mDefined[id] = true;
  return true;
}

// @brief リテラルが定義済みか調べる．
bool
AigParser::check_defined(
  SizeType lit,
  const string& ref
)
{
  if ( lit <= 1 ) {
    // 定数リテラルは常に定義されている．
    return true;
  }
  auto id = (lit / 2) - 1;
  if ( !mDefined[id] ) {
    ostringstream buf;
    buf << lit << " is not defined required by " << ref << ".";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion{mFileInfo, mLineNo, 1, mLineNo, mLastCol},
		    MsgType::Error, "AIG_PARSER", buf.str());
  }
  return true;
}

END_NAMESPACE_YM_BNET
