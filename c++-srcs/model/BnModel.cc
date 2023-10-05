
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BNIR

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief ModelImpl からのキャストコンストラクタ
BnModel::BnModel(
  ModelImpl* impl
) : mImpl{impl}
{
}

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
  return BnModel{new ModelImpl{*mImpl}};
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

// @brief DFF数を返す．
SizeType
BnModel::dff_num() const
{
  return mImpl->dff_num();
}

// @brief DFFのノードを返す．
BnNode
BnModel::dff(
  SizeType pos
) const
{
  if ( pos < 0 || dff_num() <= pos ) {
    ostringstream buf;
    buf << "Error in BnModel::dff. "
	<< pos << " is out of range.";
    throw std::invalid_argument{buf.str()};
  }
  return from_id(mImpl->dff(pos));
}

// @brief DFFのノードのリストを返す．
vector<BnNode>
BnModel::dff_list() const
{
  return from_id_list(mImpl->dff_list());
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

// @brief 新しいノードを作る．
SizeType
BnModel::new_node(
  const string& name
)
{
  return mImpl->new_node(name);
}

// @brief 新しい入力ノードを作る．
SizeType
BnModel::new_input(
  const string& name
)
{
  return mImpl->new_input(name);
}

// @brief 新しいプリミティブ型の論理ノードを作る．
SizeType
BnModel::new_primitive(
  const vector<SizeType>& input_list,
  PrimType type,
  const string& name
)
{
  _check_ids(input_list, "new_primitive");
  return mImpl->new_primitive(input_list, type, name);
}

// @brief 新しいAIG型の論理ノードを作る．
SizeType
BnModel::new_aig(
  SizeType src0,
  SizeType src1,
  bool inv0,
  bool inv1,
  const string& name
)
{
  _check_ids(vector<SizeType>{src0, src1}, "new_aig");
  return mImpl->new_aig(src0, src1, inv0, inv1, name);
}

// @brief 新しいカバー型の論理ノードを作る．
SizeType
BnModel::new_cover(
  const vector<SizeType>& input_list,
  SizeType cover_id,
  const string& name
)
{
  _check_ids(input_list, "new_cover");
  _check_cover(cover_id, "new_cover");
  _check_cover_input(cover_id, input_list, "new_cover");
  return mImpl->new_cover(input_list, cover_id, name);
}

// @brief 論理式型のノードの情報をセットする．
SizeType
BnModel::new_expr(
  const vector<SizeType>& input_list,
  SizeType expr_id,
  const string& name
)
{
  _check_ids(input_list, "new_expr");
  _check_expr(expr_id, "new_expr");
  _check_expr_input(expr_id, input_list, "new_expr");
  return mImpl->new_expr(input_list, expr_id, name);
}

// @brief セル型のノードの情報をセットする．
SizeType
BnModel::new_cell(
  const vector<SizeType>& input_list,
  SizeType cell_id,
  const string& name
)
{
  _check_ids(input_list, "new_cell");
  return mImpl->new_cell(input_list, cell_id, name);
}

// @brief 真理値表型のノードの情報をセットする．
SizeType
BnModel::new_func(
  const vector<SizeType>& input_list,
  SizeType func_id,
  const string& name
)
{
  _check_ids(input_list, "new_func");
  _check_func(func_id, "new_func");
  _check_func_input(func_id, input_list, "new_func");
  return mImpl->new_func(input_list, func_id, name);
}

// @brief BDD型のノードの情報をセットする．
SizeType
BnModel::new_bdd(
  const vector<SizeType>& input_list,
  SizeType bdd_id,
  const string& name
)
{
  _check_ids(input_list, "new_bdd");
  _check_bdd(bdd_id, "new_bdd");
  _check_bdd_input(bdd_id, input_list, "new_bdd");
  return mImpl->new_bdd(input_list, bdd_id, name);
}

// @brief DFF型のノードの情報をセットする．
SizeType
BnModel::new_dff(
  SizeType src_id,
  char rval,
  const string& name
)
{
  _check_id(src_id, "new_dff");
  return mImpl->new_dff(src_id, rval, name);
}

// @brief ノードに名前をつける．
void
BnModel::set_node_name(
  SizeType id,
  const string& name
)
{
  _check_id(id, "set_node_name");
  mImpl->set_node_name(id, name);
}

// @brief 対応するID番号に入力用の印を付ける．
void
BnModel::set_input(
  SizeType id
)
{
  _check_id(id, "set_input");
  mImpl->set_input(id);
}

// @brief 対応するID番号に出力用の印をつける．
void
BnModel::set_output(
  SizeType id
)
{
  _check_id(id, "set_output");
  mImpl->set_output(id);
}

// @brief プリミティブ型のノードの情報をセットする．
void
BnModel::set_primitive(
  SizeType id,
  const vector<SizeType>& input_list,
  PrimType type
)
{
  _check_id(id, "set_primitive");
  _check_ids(input_list, "set_primitive");
  mImpl->set_primitive(id, input_list, type);
}

// @brief AIG型のノードの情報をセットする．
void
BnModel::set_aig(
  SizeType id,
  SizeType src0,
  SizeType src1,
  bool inv0,
  bool inv1
)
{
  _check_id(id, "set_aig");
  _check_id(src0, "set_aig");
  _check_id(src1, "set_aig");
  mImpl->set_aig(id, src0, src1, inv0, inv1);
}

// @brief カバー型のノードの情報をセットする．
void
BnModel::set_cover(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType cover_id
)
{
  _check_id(id, "set_cover");
  _check_cover(cover_id, "set_cover");
  _check_cover_input(cover_id, input_list, "set_cover");
  mImpl->set_cover(id, input_list, cover_id);
}

// @brief 論理式型のノードの情報をセットする．
void
BnModel::set_expr(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType expr_id
)
{
  _check_id(id, "set_expr");
  _check_expr(expr_id, "set_expr");
  _check_expr_input(expr_id, input_list, "set_expr");
  mImpl->set_expr(id, input_list, expr_id);
}

// @brief セル型のノードの情報をセットする．
void
BnModel::set_cell(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType cell_id
)
{
  _check_id(id, "set_cell");
  mImpl->set_cell(id, input_list, cell_id);
}

// @brief 真理値表型のノードの情報をセットする．
void
BnModel::set_func(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType func_id
)
{
  _check_id(id, "set_func");
  _check_func(func_id, "set_func");
  _check_func_input(func_id, input_list, "set_func");
  mImpl->set_func(id, input_list, func_id);
}

// @brief BDD型のノードの情報をセットする．
void
BnModel::set_bdd(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType bdd_id
)
{
  _check_id(id, "set_bdd");
  _check_bdd(bdd_id, "set_bdd");
  _check_bdd_input(bdd_id, input_list, "set_bdd");
  mImpl->set_bdd(id, input_list, bdd_id);
}

// @brief DFF型のノードの情報をセットする．
void
BnModel::set_dff(
  SizeType id,
  SizeType src_id,
  char rval
)
{
  _check_id(id, "set_dff");
  mImpl->set_dff(id, src_id, rval);
}

// @brief 論理ノードのリストを作る．
void
BnModel::make_logic_list()
{
  mImpl->make_logic_list();
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
    s << node_name(node)
      << ": INPUT#" << node.input_id() << "[" << input_name(i) << "]" << endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    s << "OUTPUT#" << i << "[" << output_name(i) << "]"
      << " = " << "Node#" << output(i).id() << endl;
  }
  for ( SizeType i = 0; i < dff_num(); ++ i ) {
    auto node = dff(i);
    s << node_name(node);
    s << ": DFF#" << i << "[" << node.name() << "]"
      << " src = " << "Node#" << node.dff_src().id();
    if ( node.dff_rval() != ' ' ) {
      s << ", rval = " << node.dff_rval();
    }
    s << endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << ": ";
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
      s << " " << inode.id();
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

// @brief 正しいノード番号かチェックする．
void
BnModel::_check_id(
  SizeType id,
  const string& funcname
) const
{
  if ( id < 0 || mImpl->node_num() <= id ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname
	<< ". " << id << " is not a valid node-id.";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 正しいノード番号かチェックする．
void
BnModel::_check_ids(
  const vector<SizeType>& id_list,
  const string& funcname
) const
{
  for ( auto id: id_list ) {
    _check_id(id, funcname);
  }
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
  const vector<SizeType>& input_list,
  const string& funcname
) const
{
  auto& cover = mImpl->cover(id);
  if ( cover.input_num() != input_list.size() ) {
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
  SizeType id,                        ///< [in] 論理式番号
  const vector<SizeType>& input_list, ///< [in] 入力番号のリスト
  const string& funcname              ///< [in] 関数名
) const
{
  auto& expr = mImpl->expr(id);
  if ( expr.input_size() != input_list.size() ) {
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
  const vector<SizeType>& input_list,
  const string& funcname
) const
{
  auto& func = mImpl->func(id);
  if ( func.input_num() != input_list.size() ) {
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
  const vector<SizeType>& input_list,
  const string& funcname
) const
{
  auto& bdd = mImpl->bdd(id);
  if ( bdd.support_size() != input_list.size() ) {
    ostringstream buf;
    buf << "Error in BnModel::" << funcname << ". "
	<< "Input size mismatch.";
      throw std::invalid_argument{buf.str()};
  }
}

END_NAMESPACE_YM_BNIR
