
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
) : mImpl{new ModelImpl{*src.mImpl}}
{
}

// @brief ムーブコンストラクタ
BnModel::BnModel(
  BnModel&& src
) : mImpl{std::move(src.mImpl)}
{
  src.mImpl = nullptr;
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

// @brief セルライブラリを返す．
ClibCellLibrary
BnModel::library() const
{
  return mImpl->library();
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

// @brief オプション情報を表す JSON オブジェクトを返す．
JsonValue
BnModel::option() const
{
  return mImpl->option();
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

// @brief 入力名を返す．
string
BnModel::input_name(
  SizeType input_id
) const
{
  return mImpl->input_name(input_id);
}

// @brief 出力名を返す．
string
BnModel::output_name(
  SizeType output_id
) const
{
  return mImpl->output_name(output_id);
}

// @brief ラッチ名を返す．
string
BnModel::seq_name(
  SizeType seq_id
) const
{
  return mImpl->seq_name(seq_id);
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
    s << "SEQ#" << i << "[" << seq_name(i) << "]:"
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

END_NAMESPACE_YM_BN
