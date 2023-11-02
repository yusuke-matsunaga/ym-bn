
/// @file AigParser.cc
/// @brief AigParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "AigParser.h"
#include "ModelImpl.h"
#include "ym/BnModel.h"
#include "ym/MsgMgr.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_BN

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief aag ファイルの読み込みを行う．
BnModel
BnModel::read_aag(
  const string& filename,
  const string& clock_name,
  const string& reset_name
)
{
  BnModel model;

  AigParser parser;
  if ( !parser.read_aag(filename, clock_name, reset_name, model.mImpl.get()) ) {
    ostringstream buf;
    buf << "BnModel::read_aag(\"" << filename << "\") failed.";
    throw std::invalid_argument{buf.str()};
  }

  return model;
}

// @brief aig ファイルの読み込みを行う．
BnModel
BnModel::read_aig(
  const string& filename,
  const string& clock_name,
  const string& reset_name
)
{
  BnModel model;

  AigParser parser;
  if ( !parser.read_aig(filename, clock_name, reset_name, model.mImpl.get()) ) {
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
  const string& clock_name,
  const string& reset_name,
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

  initialize(M, I, O, model);

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
  SizeType clock_id = BAD_ID;
  SizeType reset_id = BAD_ID;
  if ( L > 0 ) {
    clock_id = model->new_input();
    reset_id = model->new_input();
  }
  for ( SizeType i = 0; i < L; ++ i ) {
    SizeType oid;
    SizeType src_lit;
    if ( !read_latch(oid, src_lit) ) {
      return false;
    }
    if ( debug ) {
      cout << "L#" << i << ": " << oid << " " << src_lit << endl;
    }
    SizeType dff_id = mModel->new_dff(' ', oid);
    auto src_id = lit2node(src_lit);
    model->set_data_src(dff_id, src_id);
    model->set_clock(dff_id, clock_id);
    model->set_clear(dff_id, reset_id);
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    if ( debug ) {
      cout << "O#" << i << ": " << src_lit << endl;
    }
    auto src_id = lit2node(src_lit);
    mModel->new_output(src_id);
  }

  // AND行の読み込み
  for ( SizeType i = 0; i < A; ++ i ) {
    SizeType id;
    SizeType src0_lit, src1_lit;
    if ( !read_and(id, src0_lit, src1_lit) ) {
      return false;
    }
    if ( debug ) {
      cout << "A#" << i << ": " << id << " " << src0_lit << " " << src1_lit << endl;
    }
    bool src0_inv;
    auto src0_id = lit2node(src0_lit, src0_inv);
    bool src1_inv;
    auto src1_id = lit2node(src1_lit, src1_inv);
    mModel->set_aig(id, src0_id, src1_id, src0_inv, src1_inv);
  }

  // ラッチのソースが定義されているかチェック
  for ( SizeType i = 0; i < mModel->seq_num(); ++ i ) {
    auto& seq = mModel->seq_node(i);
    auto src = seq.data_src();
    ostringstream buf;
    buf << "Latch#" << i << "(Node#" << seq.data_output() << ")";
    if ( !check_defined(src, buf.str()) ) {
      return false;
    }
  }
  // 出力のソースが定義されているかのチェック
  for ( SizeType i = 0; i < mModel->output_num(); ++ i ) {
    auto id = mModel->output(i);
    ostringstream buf;
    buf << "Output#" << i;
    if ( !check_defined(id, buf.str()) ) {
      return false;
    }
  }
  // 論理ノードのソースが定義されているかのチェック
  for ( auto id: mModel->logic_list() ) {
    auto& node = mModel->node(id);
    ostringstream buf;
    buf << "Node#" << id;
    for ( auto iid: node.fanin_list() ) {
      if ( !check_defined(iid, buf.str()) ) {
	return false;
      }
    }
  }

  model->make_logic_list();

  // シンボルテーブルとコメントの読み込みを行う．
  read_symbols();

  return true;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief AIG フォーマットを読み込む．
bool
AigParser::read_aig(
  const string& filename,
  const string& clock_name,
  const string& reset_name,
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

  initialize(M, I, O, model);

  // ラッチ行の読み込み
  SizeType clock_id = BAD_ID;
  SizeType reset_id = BAD_ID;
  if ( L > 0 ) {
    clock_id = model->new_input();
    reset_id = model->new_input();
  }
  for ( SizeType i = 0; i < L; ++ i ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    if ( debug ) {
      cout << "L#" << i << ": " << src_lit << endl;
    }
    auto oid = i + I;
    auto dff_id = mModel->new_dff(' ', oid);
    bool src_inv;
    auto src_id = lit2node(src_lit, src_inv);
    mModel->set_data_src(dff_id, src_id);
    mModel->set_clock(dff_id, clock_id);
    mModel->set_clear(dff_id, reset_id);
  }

  // 出力行の読み込み
  for ( SizeType i = 0; i < O; ++ i ) {
    SizeType src_lit;
    if ( !read_src(src_lit) ) {
      return false;
    }
    if ( debug ) {
      cout << "O#" << i << ": " << src_lit << endl;
    }
    auto src_id = lit2node(src_lit);
    mModel->set_output(i, src_id);
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
    auto id = i + I + L;
    bool src0_inv;
    auto src0_id = lit2node(rhs0, src0_inv);
    bool src1_inv;
    auto src1_id = lit2node(rhs1, src1_inv);
    mModel->set_aig(id, src0_id, src1_id, src0_inv, src1_inv);
  }

  model->make_logic_list();

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

  return true;
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

  return true;
}

// @brief 内容を初期化する．
void
AigParser::initialize(
  SizeType M,
  SizeType I,
  SizeType O,
  ModelImpl* model
)
{
  for ( SizeType i = 0; i < M; ++ i ) {
    model->new_node();
  }
  mModel = model;
  mDefined = vector<bool>(M, false);
  mConst0 = -1;
  mConst1 = -1;
  mInvDict.clear();
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
  SizeType lit;
  tmp >> lit;

  if ( !set_defined(lit) ) {
    return false;
  }
  bool dummy;
  id = lit2node(lit, dummy);
  return true;
}

// @brief aag のラッチ行の読み込み
bool
AigParser::read_latch(
  SizeType& id,
  SizeType& src
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
  bool dummy;
  id = lit2node(lit, dummy);
  return true;
}

// @brief aag の出力行の読み込み
bool
AigParser::read_src(
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
  SizeType& src0,
  SizeType& src1
)
{
  string linebuf;
  if ( !read_line(linebuf) ) {
    return false;
  }

  istringstream tmp{linebuf};
  SizeType lit;
  tmp >> lit >> src0 >> src1;

  if ( !set_defined(lit) ) {
    return false;
  }
  bool dummy;
  id = lit2node(lit, dummy);
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
  string comment;
  bool symbol_mode = true;
  while ( getline(*mS, linebuf) ) {
    if ( symbol_mode ) {
      if ( linebuf == "c" ) {
	symbol_mode = false;
      }
      else {
	auto p = linebuf.find_first_of(" ");
	auto pos_str = linebuf.substr(0, p);
	auto name = linebuf.substr(p + 1, string::npos);
	SizeType pos = atoi(pos_str.substr(1, string::npos).c_str());
	ostringstream buf;
	buf << pos;
	char prefix = pos_str[0];
	if ( prefix == 'i' ) {
	  mModel->set_input_name(pos, name);
	}
	else if ( prefix == 'l' ) {
	  mModel->set_output_name(pos, name);
	}
	else if ( prefix == 'o' ) {
	  mModel->set_seq_name(pos, name);
	}
	else {
	  ostringstream buf;
	  buf << prefix << ": Illegal prefix";
	  throw std::invalid_argument{buf.str()};
	}
      }
    }
    else {
      comment += linebuf + '\n';
    }
  }
  mModel->set_comment(comment);
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

// @brief リテラルからノードの情報を得る．
SizeType
AigParser::lit2node(
  SizeType lit,
  bool& inv
)
{
  inv = static_cast<bool>(lit & 1);
  auto id = lit / 2;
  if ( id == 0 ) {
    // 定数0
    return const0();
  }
  -- id;
  return id;
}

// @brief リテラルからノードの情報を得る．
SizeType
AigParser::lit2node(
  SizeType lit
)
{
  auto id = lit / 2;
  bool inv = static_cast<bool>(lit & 1);
  if ( id == 0 ) {
    if ( inv ) {
      // 定数1
      return const1();
    }
    else {
      // 定数0
      return const0();
    }
  }
  -- id;
  if ( inv ) {
    if ( mInvDict.count(id) > 0 ) {
      return mInvDict.at(id);
    }
    auto inv_id = mModel->new_node();
    mModel->set_primitive(inv_id, {id}, PrimType::Not);
    return inv_id;
  }
  return id;
}

// @brief 定数0ノードを返す．
SizeType
AigParser::const0()
{
  if ( mConst0 == -1 ) {
    mConst0 = mModel->new_node();
    mModel->set_primitive(mConst0, {}, PrimType::C0);
  }
  return mConst0;
}

// @brief 定数1ノードを返す．
SizeType
AigParser::const1()
{
  if ( mConst1 == -1 ) {
    mConst1 = mModel->new_node();
    mModel->set_primitive(mConst0, {}, PrimType::C1);
  }
  return mConst1;
}

END_NAMESPACE_YM_BN
