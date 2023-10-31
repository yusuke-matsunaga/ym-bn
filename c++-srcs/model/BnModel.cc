
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnSeq.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief shared_ptr<ModelImpl> からのキャストコンストラクタ
BnModel::BnModel(
  const shared_ptr<ModelImpl>& impl
) : mImpl{impl}
{
}

// @brief コピーコンストラクタ
BnModel::BnModel(
  const BnModel& src
) : mImpl{src.mImpl}
{
}

// @brief ムーブコンストラクタ
BnModel::BnModel(
  BnModel&& src
) : mImpl{std::move(src.mImpl)}
{
  src.mImpl = nullptr;
}

// @brief コピー代入演算子
BnModel&
BnModel::operator=(
  const BnModel& src
)
{
  if ( this != &src ) {
    mImpl = src.mImpl;
  }
  return *this;
}

// @brief ムーブ代入演算子
BnModel&
BnModel::operator=(
  BnModel&& src
)
{
  std::swap(mImpl, src.mImpl);
  src.mImpl = nullptr;
  return *this;
}

// @brief コンストラクタ
BnModel::BnModel(
) : mImpl{shared_ptr<ModelImpl>{new ModelImpl}}
{
}

// @brief デストラクタ
BnModel::~BnModel()
{
}

// @brief 深いコピーを行う．
BnModel
BnModel::copy() const
{
  return BnModel{shared_ptr<ModelImpl>{new ModelImpl{*mImpl}}};
}

// @brief セルライブラリを返す．
ClibCellLibrary
BnModel::library() const
{
  return mImpl->library();
}

// @brief 名前を返す．
string
BnModel::name() const
{
  return mImpl->name();
}

// @brief コメントを返す．
string
BnModel::comment() const
{
  return mImpl->comment();
}

// @brief ノード数を返す．
SizeType
BnModel::node_num() const
{
  return mImpl->node_num();
}

// @brief 入力数を返す．
SizeType
BnModel::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::input(
  SizeType pos
) const
{
  return from_id(mImpl->input(pos));
}

// @brief 入力のノードのリストを返す．
vector<BnNode>
BnModel::input_list() const
{
  return from_id_list(mImpl->input_list());
}

// @brief 入力名を返す．
string
BnModel::input_name(
  SizeType pos
) const
{
  if ( pos < 0 || input_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::input_name. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return mImpl->input_name(pos);
}

// @brief 入力名のリストを返す．
vector<string>
BnModel::input_name_list() const
{
  return mImpl->input_name_list();
}

// @brief 入力数を返す．
SizeType
BnModel::output_num() const
{
  return mImpl->output_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::output(
  SizeType pos
) const
{
  if ( pos < 0 || output_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::output. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return from_id(mImpl->output(pos));
}

// @brief 出力のノードのリストを返す．
vector<BnNode>
BnModel::output_list() const
{
  return from_id_list(mImpl->output_list());
}

// @brief 出力名を返す．
string
BnModel::output_name(
  SizeType pos
) const
{
  if ( pos < 0 || output_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::output_name. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return mImpl->output_name(pos);
}

// @brief 出力名のリストを返す．
vector<string>
BnModel::output_name_list() const
{
  return mImpl->output_name_list();
}

// @brief 論理ノード数を返す．
SizeType
BnModel::logic_num() const
{
  return mImpl->logic_num();
}

// @brief 論理ノードを返す．
BnNode
BnModel::logic(
  SizeType pos
) const
{
  if ( pos < 0 || logic_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::logic. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return from_id(mImpl->logic(pos));
}

// @brief 論理ノードのリストを返す．
vector<BnNode>
BnModel::logic_list() const
{
  return from_id_list(mImpl->logic_list());
}

// @brief SEQノード数を返す．
SizeType
BnModel::seq_num() const
{
  return mImpl->seq_num();
}

// @brief SEQノードを返す．
BnSeq
BnModel::seq_node(
  SizeType pos
) const
{
  if ( pos < 0 || seq_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::seq_node(pos). "
  	    << pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return BnSeq{mImpl, pos};
}

// @brief SEQノードのリストを返す．
vector<BnSeq>
BnModel::seq_node_list() const
{
  vector<BnSeq> ans_list(seq_num());
  for ( SizeType i = 0; i < seq_num(); ++ i ) {
    ans_list[i] = BnSeq{mImpl, i};
  }
  return ans_list;
}

// @brief カバーの種類の数を返す．
SizeType
BnModel::cover_num() const
{
  return mImpl->cover_num();
}

// @brief カバーを取り出す．
const BnCover&
BnModel::cover(
  SizeType cover_id
) const
{
  _check_cover(cover_id, "cover");
  return mImpl->cover(cover_id);
}

// @brief 論理式の数を返す．
SizeType
BnModel::expr_num() const
{
  return mImpl->expr_num();
}

// @brief 論理式を返す．
const Expr&
BnModel::expr(
  SizeType expr_id
) const
{
  _check_expr(expr_id, "expr");
  return mImpl->expr(expr_id);
}

// @brief 真理値表型の関数の数を返す．
SizeType
BnModel::func_num() const
{
  return mImpl->func_num();
}

// @brief 真理値表型の関数を返す．
const TvFunc&
BnModel::func(
  SizeType func_id
) const
{
  _check_func(func_id, "func");
  return mImpl->func(func_id);
}

// @brief BDDの数を返す．
SizeType
BnModel::bdd_num() const
{
  return mImpl->bdd_num();
}

// @brief BDDを返す．
Bdd
BnModel::bdd(
  SizeType bdd_id
) const
{
  _check_bdd(bdd_id, "bdd");
  return mImpl->bdd(bdd_id);
}

// @brief セルライブラリを設定する．
void
BnModel::set_celllibrary(
  ClibCellLibrary lib
)
{
  mImpl->set_library(lib);
}

// @brief 名前を設定する．
void
BnModel::set_name(
  const string& name
)
{
  mImpl->set_name(name);
}

// @brief コメントを設定する．
void
BnModel::set_comment(
  const string& comment
)
{
  mImpl->set_comment(comment);
}

// @brief 出力の名前を設定する．
void
BnModel::set_output_name(
  SizeType pos,
  const string& name
)
{
  if ( pos < 0 || output_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::output. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  mImpl->set_output_name(pos, name);
}

// @brief 新しい入力ノードを作る．
BnNode
BnModel::new_input(
  const string& name
)
{
  return BnNode{mImpl, mImpl->new_input(name)};
}

// @brief 新しい出力ノードを作る．
SizeType
BnModel::new_output(
  BnNode src,
  const string& name
)
{
  return mImpl->new_output(src.id(), name);
}

// @brief 新しいプリミティブ型の論理ノードを作る．
BnNode
BnModel::new_primitive(
  const vector<BnNode>& input_list,
  PrimType type,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_primitive");
  return BnNode{mImpl, mImpl->new_primitive(input_id_list, type, name)};
}

// @brief 新しいAIG型の論理ノードを作る．
BnNode
BnModel::new_aig(
  BnNode src0,
  BnNode src1,
  bool inv0,
  bool inv1,
  const string& name
)
{
  auto src0_id = to_id(src0, "new_aig");
  auto src1_id = to_id(src1, "new_aig");
  return BnNode{mImpl, mImpl->new_aig(src0_id, src1_id, inv0, inv1, name)};
}

// @brief 新しいカバー型の論理ノードを作る．
BnNode
BnModel::new_cover(
  const vector<BnNode>& input_list,
  SizeType cover_id,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_cover");
  _check_cover(cover_id, "new_cover");
  _check_cover_input(cover_id, input_list.size(), "new_cover");
  return BnNode{mImpl, mImpl->new_cover(input_id_list, cover_id, name)};
}

// @brief 論理式型の論理ノードを作る．
BnNode
BnModel::new_expr(
  const vector<BnNode>& input_list,
  SizeType expr_id,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_expr");
  _check_expr(expr_id, "new_expr");
  _check_expr_input(expr_id, input_list.size(), "new_expr");
  return BnNode{mImpl, mImpl->new_expr(input_id_list, expr_id, name)};
}

// @brief セル型の論理ノードを作る．
BnNode
BnModel::new_cell(
  const vector<BnNode>& input_list,
  ClibCell cell,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_cell");
  return BnNode{mImpl, mImpl->new_cell(input_id_list, cell, name)};
}

// @brief 真理値表型の論理ノードを作る．
BnNode
BnModel::new_func(
  const vector<BnNode>& input_list,
  SizeType func_id,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_func");
  _check_func(func_id, "new_func");
  _check_func_input(func_id, input_list.size(), "new_func");
  return BnNode{mImpl, mImpl->new_func(input_id_list, func_id, name)};
}

// @brief BDD型の論理ノードを作る．
BnNode
BnModel::new_bdd(
  const vector<BnNode>& input_list,
  SizeType bdd_id,
  const string& name
)
{
  auto input_id_list = to_id_list(input_list, "new_bdd");
  _check_bdd(bdd_id, "new_bdd");
  _check_bdd_input(bdd_id, input_list.size(), "new_bdd");
  return BnNode{mImpl, mImpl->new_bdd(input_id_list, bdd_id, name)};
}

// @brief DFFを作る．
BnSeq
BnModel::new_dff(
  char rs_val,
  const string& name
)
{
  auto id = mImpl->new_dff(rs_val, BAD_ID, name);
  return BnSeq{mImpl, id};
}

// @brief ラッチを作る．
BnSeq
BnModel::new_latch(
  char rs_val,
  const string& name
)
{
  auto id = mImpl->new_latch(rs_val, BAD_ID, name);
  return BnSeq{mImpl, id};
}

// @brief セルタイプのSEQノードを作る．
BnSeq
BnModel::new_seq_cell(
  ClibCell cell,
  const string& name
)
{
  auto id = mImpl->new_seq_cell(cell, name);
  return BnSeq{mImpl, id};
}

// @brief DFFのソースノードをセットする．
void
BnModel::set_data_src(
  BnSeq dff,
  BnNode src
)
{
  mImpl->set_data_src(dff.id(), src.id());
}

// @brief DFFのクロック入力をセットする．
void
BnModel::set_clock(
  BnSeq dff,
  BnNode clock
)
{
  mImpl->set_clock(dff.id(), clock.id());
}

// @brief ラッチのイネーブル入力をセットする．
void
BnModel::set_enable(
  BnSeq latch,
  BnNode enable
)
{
  mImpl->set_enable(latch.id(), enable.id());
}

// @brief DFFのクリア入力をセットする．
void
BnModel::set_clear(
  BnSeq dff,
  BnNode clear
)
{
  mImpl->set_clear(dff.id(), clear.id());
}

// @brief DFFのプリセット入力をセットする．
void
BnModel::set_preset(
  BnSeq dff,
  BnNode preset
)
{
  mImpl->set_preset(dff.id(), preset.id());
}

// @brief ピンに対応するノードをセットする．
void
BnModel::set_seq_pin(
  BnSeq dff,
  SizeType pos,
  BnNode node
)
{
  mImpl->set_seq_pin(dff.id(), pos, node.id());
}

// @brief カバーを追加する．
SizeType
BnModel::add_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  return mImpl->add_cover(input_num, cube_list, opat);
}

// @brief 論理式を追加する．
SizeType
BnModel::add_expr(
  const Expr& expr
)
{
  return mImpl->add_expr(expr);
}

// @brief 真理値表を追加する．
SizeType
BnModel::add_func(
  const TvFunc& func
)
{
  return mImpl->add_func(func);
}

// @brief BDDを追加する．
SizeType
BnModel::add_bdd(
  const Bdd& bdd
)
{
  return mImpl->add_bdd(bdd);
}


BEGIN_NONAMESPACE

inline
string
node_name(
  const BnNode& node
)
{
  ostringstream buf;
  buf << "Node#" << node.id();
  if ( node.name() != string{} ) {
    buf << "(" << node.name() << ")";
  }
  return buf.str();
}

END_NONAMESPACE

// @brief 内容を出力する．
void
BnModel::print(
  ostream& s
) const
{
  if ( name() != string{} ) {
    s << "Name: " << name() << endl;
  }
  if ( comment() != string{} ) {
    s << "Comment: " << comment() << endl;
  }
  for ( SizeType i = 0;i < input_num(); ++ i ) {
    auto node = input(i);
    s << "INPUT#" << i << "[" << input_name(i) << "]"
      << " = " << node_name(node) << endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    s << "OUTPUT#" << i << "[" << output_name(i) << "]"
      << " = " << node_name(output(i)) << endl;
  }
  for ( SizeType i = 0; i < seq_num(); ++ i ) {
    auto seq = seq_node(i);
    s << "SEQ#" << i << "[" << seq.name() << "]:"
      << " type = " << seq.type()
      << " output = " << node_name(seq.data_output())
      << " src = " << node_name(seq.data_src())
      << " clock = " << node_name(seq.clock());
    if ( seq.clear().is_valid() ) {
      s << " clear = " << node_name(seq.clear());
    }
    if ( seq.preset().is_valid() ) {
      s << " preset = " << node_name(seq.preset());
    }
    if ( seq.rsval() != ' ' ) {
      s << ", rsval = " << seq.rsval();
    }
    s << endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << " = ";
    if ( node.is_primitive() ) {
      s << node.primitive_type();
    }
    else if ( node.is_aig() ) {
      s << "AIG[" << node.fanin_inv(0) << node.fanin_inv(1) << "]";
    }
    else if ( node.is_cover() ) {
      s << "Cover#" << node.cover_id();
    }
    else if ( node.is_expr() ) {
      s << "Expr#" << node.expr_id();
    }
    else if ( node.is_cell() ) {
      s << "Cell#" << node.cell_id();
    }
    else if ( node.is_func() ) {
      s << "Func#" << node.func_id();
    }
    else if ( node.is_bdd() ) {
      s << "Bdd#" << node.bdd_id();
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << " (";
    for ( auto inode: node.fanin_list() ) {
      s << " " << node_name(inode);
    }
    s << ")" << endl;
  }
  if ( cover_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < cover_num(); ++ id ) {
      s << "Cover#" << id << ":" << endl;
      auto& cover = this->cover(id);
      cover.print(s);
    }
  }
  if ( expr_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < expr_num(); ++ id ) {
      s << "Expr#" << id << ":" << endl
	<< expr(id) << endl;
    }
  }
  if ( func_num() > 0 ) {
    s << endl;
    for ( SizeType id = 0; id < func_num(); ++ id ) {
      s << "Func#" << id << ":" << endl
	<< func(id) << endl;
    }
  }
}

// @brief ID 番号から BnNode を作る．
BnNode
BnModel::from_id(
  SizeType id
) const
{
  return BnNode{mImpl, id};
}

// @brief ID 番号のリストから vector<BnNode> を作る．
vector<BnNode>
BnModel::from_id_list(
  const vector<SizeType>& id_list
) const
{
  vector<BnNode> node_list;
  for ( auto id: id_list ) {
    node_list.push_back(from_id(id));
  }
  return node_list;
}

// @brief BnNode から ID 番号を取り出す．
SizeType
BnModel::to_id(
  const BnNode& node,
  const string& funcname
) const
{
  if ( node.is_valid() ) {
    if ( node.parent_model() != *this ) {
      ostringstream buf;
      buf << "Error in BnModel::" << funcname
	  << ".  "
	  << "Illega node with different model.";
      throw std::invalid_argument{buf.str()};
    }
    return node.id();
  }
  return BAD_ID;
}

// @brief vector<BnNode> から ID番号のリストを取り出す．
vector<SizeType>
BnModel::to_id_list(
  const vector<BnNode>& node_list,
  const string& funcname
) const
{
  vector<SizeType> id_list;
  id_list.reserve(node_list.size());
  for ( auto& node: node_list ) {
    auto id = to_id(node, funcname);
    id_list.push_back(id);
  }
  return id_list;
}

// @brief 正しいカバー番号かチェックする．
void
BnModel::_check_cover(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->cover_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid cover-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief カバーと入力のサイズが合っているか調べる．
void
BnModel::_check_cover_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& cover = mImpl->cover(id);
  if ( cover.input_num() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しい論理式番号かチェックする．
void
BnModel::_check_expr(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->expr_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid expr-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 論理式と入力のサイズが合っているか調べる．
void
BnModel::_check_expr_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& expr = mImpl->expr(id);
  if ( expr.input_size() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しい関数番号かチェックする．
void
BnModel::_check_func(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->func_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid func-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 関数と入力のサイズが合っているか調べる．
void
BnModel::_check_func_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& func = mImpl->func(id);
  if ( func.input_num() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しいBDD番号かチェックする．
void
BnModel::_check_bdd(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->bdd_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< id << " is not a valid bdd-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief BDDと入力のサイズが合っているか調べる．
void
BnModel::_check_bdd_input(
  SizeType id,
  SizeType size,
  const string& funcname
) const
{
  auto& bdd = mImpl->bdd(id);
  if ( bdd.support_size() != size ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

// @brief 本体に登録されているライブラリとセルが属するライブラリが等しいか調べる．
void
BnModel::_check_library(
  ClibCell cell,
  const string& funcname
) const
{
  if ( library() != cell.library() ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "cell library mismatch";
    throw std::invalid_argument{buf.str()};
  }
}

END_NAMESPACE_YM_BN
