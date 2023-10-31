
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mName{src.mName},
    mComment{src.mComment},
    mLibrary{src.mLibrary},
    mInputList{src.mInputList},
    mInputNameList{src.mInputNameList},
    mOutputList{src.mOutputList},
    mOutputNameList{src.mOutputNameList},
    mLogicList{src.mLogicList},
    mNodeArray{src.mNodeArray},
    mSeqArray{src.mSeqArray},
    mCoverArray{src.mCoverArray},
    mExprArray{src.mExprArray},
    mFuncArray{src.mFuncArray}
{
  for ( auto bdd: src.mBddArray ) {
    mBddArray.push_back(mBddMgr.copy(bdd));
  }
}

// @brief 名前を設定する．
void
ModelImpl::set_name(
  const string& name
)
{
  mName = name;
}

// @brief コメントを設定する．
void
ModelImpl::set_comment(
  const string& comment
)
{
  mComment = comment;
}

// @brief セルライブラリを設定する．
void
ModelImpl::set_library(
  ClibCellLibrary library
)
{
  if ( mLibrary.is_valid() && mLibrary != library ) {
    throw std::invalid_argument{"cell library mismatch"};
  }
  mLibrary = library;
}

// @brief 出力の名前を設定する．
void
ModelImpl::set_output_name(
  SizeType pos,
  const string& name
)
{
  if ( pos < 0 || pos >= output_num() ) {
    throw std::invalid_argument{"Error in ModelImpl::set_output_name(). pos is out of range"};
  }
  mOutputNameList[pos] = name;
}

// @brief 新しいノードを作る．
SizeType
ModelImpl::new_node(
  const string& name
)
{
  auto id = mNodeArray.size();
  mNodeArray.push_back({name});
  return id;
}

// @brief ノードに名前をつける．
void
ModelImpl::set_node_name(
  SizeType id,
  const string& name
)
{
  auto& node = _node(id, "set_node_name(id, name)", "id");
  node.set_name(name);
}

// @brief 対応するID番号に入力用の印を付ける．
void
ModelImpl::set_input(
  SizeType id
)
{
  auto& node = _node(id, "set_input(id", "id");
  auto iid = mInputList.size();
  mInputList.push_back(id);
  mInputNameList.push_back(node.name());
  node.set_input(iid);
}

// @brief 新しい出力ノードを作る．
SizeType
ModelImpl::new_output(
  SizeType id,
  const string& name
)
{
  auto& node = _node(id, "new_output(id, name)", "id");
  SizeType pos = mOutputList.size();
  mOutputList.push_back(id);
  if ( name != string{} ) {
    mOutputNameList.push_back(name);
  }
  else {
    mOutputNameList.push_back(node.name());
  }
  return pos;
}

// @brief プリミティブ型のノードの情報をセットする．
void
ModelImpl::set_primitive(
  SizeType id,
  const vector<SizeType>& input_list,
  PrimType type
)
{
  auto& node = _node(id, "set_primitive(id, input_list, type)", "id");
  node.set_primitive(input_list, type);
}

// @brief AIG型のノードの情報をセットする．
void
ModelImpl::set_aig(
  SizeType id,
  SizeType src0,
  SizeType src1,
  bool inv0,
  bool inv1
)
{
  auto& node = _node(id, "set_aig(id, src0, src1, inv0, inv1)", "id");
  node.set_aig(src0, src1, inv0, inv1);
}

// @brief カバー型のノードの情報をセットする．
void
ModelImpl::set_cover(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType cover_id
)
{
  auto& node = _node(id, "set_cover(id, input_list, cover_id)", "id");
  node.set_cover(input_list, cover_id);
}

// @brief 論理式型のノードの情報をセットする．
void
ModelImpl::set_expr(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType expr_id
)
{
  auto& node = _node(id, "set_expr(id, input_list, expr_id)", "id");
  node.set_expr(input_list, expr_id);
}

// @brief セル型のノードの情報をセットする．
void
ModelImpl::set_cell(
  SizeType id,
  const vector<SizeType>& input_list,
  ClibCell cell
)
{
  set_library(cell.library());
  auto& node = _node(id, "set_cell(id, input_list, cell)", "id");
  node.set_cell(input_list, cell.id());
}

// @brief 真理値表型のノードの情報をセットする．
void
ModelImpl::set_func(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType func_id
)
{
  auto& node = _node(id, "set_func(id, input_list, func_id)", "id");
  node.set_func(input_list, func_id);
}

// @brief BDD型のノードの情報をセットする．
void
ModelImpl::set_bdd(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType bdd_id
)
{
  auto& node = _node(id, "set_bdd(id, input_list, bdd_id)", "id");
  node.set_bdd(input_list, bdd_id);
}

// @brief SEQノードの名前をセットする．
void
ModelImpl::set_seq_name(
  SizeType id,         ///< [in] ID番号
  const string& name   ///< [in] 名前
)
{
  auto& seq = _seq(id, "set_seq_name(id, name)", "id");
  seq.set_name(name);
}

// @brief DFF型のノードのソースをセットする．
void
ModelImpl::set_data_src(
  SizeType id,
  SizeType src_id
)
{
  auto& seq = _seq(id, "set_data_src(id, src_id)", "id");
  seq.set_data_src(src_id);
}

// @brief DFF型のノードのクロック入力をセットする．
void
ModelImpl::set_clock(
  SizeType id,
  SizeType clock_id
)
{
  auto& seq= _seq(id, "set_clock(id, check_id)", "id");
  seq.set_clock(clock_id);
}

// @brief ラッチ型のノードのイネーブル入力をセットする．
void
ModelImpl::set_enable(
  SizeType id,
  SizeType enable_id
)
{
  auto& seq = _seq(id, "set_enable(id, enable_id)", "id");
  seq.set_enable(enable_id);
}

// @brief DFF型のノードのクリア入力をセットする．
void
ModelImpl::set_clear(
  SizeType id,
  SizeType clear_id
)
{
  auto& seq = _seq(id, "set_clear(id, clear_id", "id");
  seq.set_clear(clear_id);
}

// @brief DFF型のノードのプリセット入力をセットする．
void
ModelImpl::set_preset(
  SizeType id,
  SizeType preset_id
)
{
  auto& seq = _seq(id, "set_preset(id, preset_id)", "id");
  seq.set_preset(preset_id);
}

// @brief DFFセルのピンのノードをセットする．
void
ModelImpl::set_seq_pin(
  SizeType id,
  SizeType pos,
  SizeType node_id
)
{
  auto& seq = _seq(id, "set_dff_pin(id, pos, node_id)", "id");
  seq.set_cell_pin(pos, node_id);
}

// @brief 論理ノードのリストを作る．
void
ModelImpl::make_logic_list()
{
  unordered_set<SizeType> mark;

  // 入力ノードに印をつける．
  for ( auto id: mInputList ) {
    mark.emplace(id);
  }

  // DFFの出力に印を作る．
  for ( auto& seq: mSeqArray ) {
    auto id = seq.data_output();
    mark.emplace(id);
  }

  // 出力ノードからファンインをたどり
  // post-order で番号をつける．
  // 結果としてノードは入力からのトポロジカル順
  // に整列される．
  for ( auto id: mOutputList ) {
    order_node(id, mark);
  }

  // DFFのファンインに番号をつける．
  for ( auto& seq: mSeqArray ) {
    order_node(seq.data_src(), mark);
  }
}

// @brief トポロジカルソートを行い mLogicList にセットする．
void
ModelImpl::order_node(
  SizeType id,
  unordered_set<SizeType>& mark
)
{
  if ( mark.count(id) > 0 ) {
    return;
  }
  auto& node = _node(id, "", "");
  ASSERT_COND( node.is_logic() );
  for ( auto iid: node.fanin_list() ) {
    order_node(iid, mark);
  }
  mLogicList.push_back(id);
  mark.emplace(id);
}

// @brief カバーを追加する．
SizeType
ModelImpl::add_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  auto id = cover_num();
  mCoverArray.push_back(BnCover{input_num, cube_list, opat});
  return id;
}

// @brief 論理式を追加する．
SizeType
ModelImpl::add_expr(
  const Expr& expr
)
{
  auto id = expr_num();
  mExprArray.push_back(expr);
  return id;
}

// @brief 真理値表を追加する．
SizeType
ModelImpl::add_func(
  const TvFunc& func
)
{
  auto id = func_num();
  mFuncArray.push_back(func);
  return id;
}

// @brief BDDを追加する．
SizeType
ModelImpl::add_bdd(
  const Bdd& bdd
)
{
  auto id = bdd_num();
  // mBddMgr に属する BDD に変換しておく．
  mBddArray.push_back(mBddMgr.copy(bdd));
  return id;
}

END_NAMESPACE_YM_BN
