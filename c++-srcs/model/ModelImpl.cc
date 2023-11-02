
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief コンストラクタ
ModelImpl::ModelImpl() : mOption{JsonValue::Object()}
{
}

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mLibrary{src.mLibrary},
    mOption{src.mOption},
    mInputList{src.mInputList},
    mOutputList{src.mOutputList},
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

// @brief 入力名をセットする．
void
ModelImpl::set_input_name(
  SizeType input_id,
  const string& name
)
{
  if ( input_id < 0 || input_id >= input_num() ) {
    throw std::invalid_argument{"Error in ModelImpl::set_input_name(input_id, name). input_id is out of range"};
  }
  if ( !mOption.has_key("symbol_dict") ) {
    auto symbol_dict = JsonValue::Object();
    mOption.emplace("symbol_dict", symbol_dict);
  }
  auto symbol_dict = mOption.at("symbol_dict");
  ostringstream buf;
  buf << "I" << input_id;
  symbol_dict.emplace(buf.str(), name);
}

// @brief 出力名をセットする．
void
ModelImpl::set_output_name(
  SizeType output_id,
  const string& name
)
{
  if ( output_id < 0 || output_id >= output_num() ) {
    throw std::invalid_argument{"Error in ModelImpl::set_output_name(output_id, name). output_id is out of range"};
  }
  if ( !mOption.has_key("symbol_dict") ) {
    auto symbol_dict = JsonValue::Object();
    mOption.emplace("symbol_dict", symbol_dict);
  }
  auto symbol_dict = mOption.at("symbol_dict");
  ostringstream buf;
  buf << "O" << output_id;
  symbol_dict.emplace(buf.str(), name);
}

// @brief SEQ名をセットする．
void
ModelImpl::set_seq_name(
  SizeType seq_id,
  const string& name
)
{
  if ( seq_id < 0 || seq_id >= seq_num() ) {
    throw std::invalid_argument{"Error in ModelImpl::set_seq_name(seq_id, name). seq_id is out of range"};
  }
  if ( !mOption.has_key("symbol_dict") ) {
    auto symbol_dict = JsonValue::Object();
    mOption.emplace("symbol_dict", symbol_dict);
  }
  auto symbol_dict = mOption.at("symbol_dict");
  ostringstream buf;
  buf << "Q" << seq_id;
  symbol_dict.emplace(buf.str(), name);
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
