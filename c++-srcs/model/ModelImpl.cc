
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"
#include "NodeImpl_sub.h"
#include "SeqImpl_sub.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

// @brief コンストラクタ
ModelImpl::ModelImpl()
{
}

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mName{src.mName},
    mComment{src.mComment},
    mSymbolDict{src.mSymbolDict},
    mInputList{src.mInputList},
    mOutputList{src.mOutputList},
    mLogicList{src.mLogicList},
    mNodeArray(src.mNodeArray.size()),
    mFuncArray(src.mFuncArray.size())
{
  for ( SizeType i = 0; i < src.mNodeArray.size(); ++ i ) {
    mNodeArray[i] = src.mNodeArray[i]->copy();
  }
  for ( SizeType i = 0; i < src.mFuncArray.size(); ++ i ) {
    mFuncArray[i] = src.mFuncArray[i]->copy(mBddMgr);
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
  std::unordered_map<std::string, JsonValue> src_dict;
  if ( name() != std::string{} ) {
    src_dict.emplace("name", JsonValue{name()});
  }
  if ( comment() != std::string{} ) {
    src_dict.emplace("comment", JsonValue{comment()});
  }
  if ( !mSymbolDict.empty() ) {
    std::unordered_map<std::string, JsonValue> symbol_dict;
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
  mName = std::string{};
  mComment = std::string{};
  mSymbolDict.clear();
  mInputList.clear();
  mOutputList.clear();
  mLogicList.clear();
  mNodeArray.clear();
  mFuncArray.clear();
  // BddMgr に関しては完全な初期化は行えない．
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

// @brief 対応するID番号に入力用の印を付ける．
void
ModelImpl::set_input(
  SizeType id
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    trhow std::invalid_argument{"id has already been used"};
  }
  auto iid = mInputList.size();
  auto node = new NodeImpl_Input(iid);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  mInputList.push_back(id);
}

// @brief 対応するID番号にDFFの出力用の印を付ける．
void
ModelImpl::set_dff_output(
  SizeType id
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    trhow std::invalid_argument{"id has already been used"};
  }
  auto dff_id = mDffList.size();
  auto node = new NodeImpl_DffOutput(dff_id);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  mDffList.push_back(id);
}

// @brief 論理ノードの情報をセットする．
void
ModelImpl::set_logic(
  SizeType id,
  const vector<SizeType>& input_list,
  SizeType func_id
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    trhow std::invalid_argument{"id has already been used"};
  }
  auto node = new NodeImpl_Logic(func_id, fanin_list);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  // mLogicList には追加しない．
}

// @brief 論理ノードのリストを作る．
void
ModelImpl::make_logic_list()
{
  std::unordered_set<SizeType> mark;

  // 入力ノードに印をつける．
  for ( auto id: mInputList ) {
    mark.emplace(id);
  }

  // DFFの出力に印を作る．
  for ( auto id: mDffList ) {
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
  for ( auto id: mDffList ) {
    auto node = mNodeArray[id].get();
    auto src_id = node->dff_src();
    order_node(src_id, mark);
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
  auto node = mNodeArray[id].get();
  if ( !node->is_logic() ) {
    throw std::logic_error{"node->is_logic() == false"};
  }
  for ( auto iid: node->fanin_list() ) {
    order_node(iid, mark);
  }
  mLogicList.push_back(id);
  mark.emplace(id);
}

// @brief プリミティブを登録する．
SizeType
ModelImpl::reg_primitive(
  SizeType input_num,
  PrimType primitive_type
)
{
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
