
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

// @brief コンストラクタ
ModelImpl::ModelImpl()
{
}

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mLibrary{src.mLibrary},
    mName{src.mName},
    mComment{src.mComment},
    mSymbolDict{src.mSymbolDict},
    mInputList{src.mInputList},
    mOutputList{src.mOutputList},
    mLogicList{src.mLogicList},
    mNodeArray{src.mNodeArray},
    mSeqArray{src.mSeqArray},
    mFuncArray(src.mFuncArray.size())
{
  mFuncArray.reserve(src.mFuncArray.size());
  for ( auto& func_ptr: src.mFuncArray ) {
    auto new_func = func_ptr->copy(mBddMgr);
    mFuncArray.push_back(unique_ptr<FuncImpl>(new_func));
  }
}

// @brief デストラクタ
ModelImpl::~ModelImpl()
{
}

// @brief オプション情報を返す．
JsonValue
ModelImpl::option() const
{
  unordered_map<string, JsonValue> src_dict;
  if ( name() != string{} ) {
    src_dict.emplace("name", JsonValue{name()});
  }
  if ( comment() != string{} ) {
    src_dict.emplace("comment", JsonValue{comment()});
  }
  if ( !mSymbolDict.empty() ) {
    unordered_map<string, JsonValue> symbol_dict;
    for ( auto& p: mSymbolDict ) {
      auto key = p.first;
      auto value = p.second;
      symbol_dict.emplace(key, JsonValue{value});
    }
    src_dict.emplace("symbol_dict", JsonValue{symbol_dict});
  }
  return JsonValue{src_dict};
}

// @brief 内容をクリアする．
void
ModelImpl::clear()
{
  mLibrary = ClibCellLibrary{};
  mName = string{};
  mComment = string{};
  mSymbolDict.clear();
  mInputList.clear();
  mOutputList.clear();
  mLogicList.clear();
  mNodeArray.clear();
  mSeqArray.clear();
  mFuncArray.clear();
  // BddMgr に関しては完全な初期化は行えない．
}

// @brief セルライブラリを設定する．
void
ModelImpl::set_library(
  ClibCellLibrary library
)
{
  mLibrary = library;
}

// @brief オプション情報をセットする．
void
ModelImpl::set_option(
  const JsonValue& option
)
{
  if ( option.has_key("name") ) {
    set_name(option.at("name").get_string());
  }
  if ( option.has_key("comment") ) {
    set_comment(option.at("comment").get_string());
  }
  if ( option.has_key("symbol_dict") ) {
    for ( auto& p: option.item_list() ) {
      auto key = p.first;
      auto value = p.second.get_string();
      mSymbolDict.emplace(key, value);
    }
  }
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

// @brief カバーを登録する．
SizeType
ModelImpl::reg_cover(
  SizeType input_num,
  const vector<vector<Literal>>& cube_list,
  char opat
)
{
  auto id = func_num();
  auto func = FuncImpl::new_cover(input_num, cube_list, opat);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief 論理式を登録する．
SizeType
ModelImpl::reg_expr(
  const Expr& expr
)
{
  auto id = func_num();
  auto func = FuncImpl::new_expr(expr);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief 真理値表を登録する．
SizeType
ModelImpl::reg_tvfunc(
  const TvFunc& tvfunc
)
{
  auto id = func_num();
  auto func = FuncImpl::new_tvfunc(tvfunc);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

// @brief BDDを登録する．
SizeType
ModelImpl::reg_bdd(
  const Bdd& bdd
)
{
  auto id = func_num();
  // mBddMgr に属する BDD に変換しておく．
  auto my_bdd = mBddMgr.copy(bdd);
  auto func = FuncImpl::new_bdd(my_bdd);
  mFuncArray.push_back(unique_ptr<FuncImpl>(func));
  return id;
}

END_NAMESPACE_YM_BN
