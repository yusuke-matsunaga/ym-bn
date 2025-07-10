#ifndef MODELIMPL_H
#define MODELIMPL_H

/// @file ModelImpl.h
/// @brief ModelImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/BnNode.h"
#include "ym/BnFunc.h"
#include "ym/BddMgr.h"
#include "ym/JsonValue.h"
#include "NodeImpl.h"
#include "FuncMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class ModelImpl ModelImpl.h "ModelImpl.h"
/// @brief BnModel の内部情報を表すクラス
///
/// 関連する全てのオブジェクトの所有権を持つ．
//////////////////////////////////////////////////////////////////////
class ModelImpl
{
public:

  /// @brief コンストラクタ
  ModelImpl();

  /// @brief コピーコンストラクタ
  ModelImpl(
    const ModelImpl& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ModelImpl();

  /// @brief 複製を返す．
  ModelImpl*
  copy() const
  {
    return new ModelImpl{*this};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報を返す．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  std::string
  name() const
  {
    return mName;
  }

  /// @brief コメントを返す．
  const std::vector<std::string>&
  comment_list() const
  {
    return mCommentList;
  }

  /// @brief 入力名を返す．
  std::string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    auto key = _input_key(input_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return std::string{};
  }

  /// @brief 出力名を返す．
  std::string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    auto key = _output_key(output_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return std::string{};
  }

  /// @brief ラッチ名を返す．
  std::string
  dff_name(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    auto key = _dff_key(dff_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return std::string{};
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief ノードを取り出す．
  const NodeImpl&
  node_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    if ( id >= node_num() ) {
      throw std::out_of_range{"id is out of range"};
    }
    return *mNodeArray[id];
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputList.size();
  }

  /// @brief 入力のノード番号を返す．
  SizeType
  input_id(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    if ( input_id >= input_num() ) {
      throw std::out_of_range{"input_id is out of range"};
    }
    return mInputList[input_id];
  }

  /// @brief 入力のノード番号のリストを返す．
  const vector<SizeType>&
  input_id_list() const
  {
    return mInputList;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 出力のノード番号を返す．
  SizeType
  output_id(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    if ( output_id >= output_num() ) {
      throw std::out_of_range{"output_id is out of range"};
    }
    return mOutputList[output_id];
  }

  /// @brief 出力のノード番号のリストを返す．
  const vector<SizeType>&
  output_id_list() const
  {
    return mOutputList;
  }

  /// @brief DFF数を返す．
  SizeType
  dff_num() const
  {
    return mDffList.size();
  }

  /// @brief DFF出力のノード番号を返す．
  SizeType
  dff_output_id(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    if ( dff_id >= dff_num() ) {
      throw std::out_of_range{"dff_id is out of range"};
    }
    return mDffList[dff_id];
  }

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const
  {
    return mLogicList.size();
  }

  /// @brief 論理ノードのノード番号を返す．
  SizeType
  logic_id(
    SizeType pos ///< [in] 位置 ( 0 <= pos < logic_num() )
  ) const
  {
    if ( pos >= logic_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mLogicList[pos];
  }

  /// @brief 論理ノード番号のリストを返す．
  const vector<SizeType>&
  logic_id_list() const
  {
    return mLogicList;
  }

  /// @brief 関数の数を返す．
  SizeType
  func_num() const
  {
    return mFuncMgr.func_num();
  }

  /// @brief 関数情報を返す．
  const FuncImpl&
  func_impl(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const
  {
    return mFuncMgr.func(func_id);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 設定するオプション
  );

  /// @brief 名前を設定する．
  void
  set_name(
    const std::string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief コメントを追加する．
  void
  add_comment(
    const std::string& comment ///< [in] コメント
  )
  {
    mCommentList.push_back(comment);
  }

  /// @brief 入力名をセットする．
  void
  set_input_name(
    SizeType input_id,      ///< [in] 入力番号 ( 0 <= input_id < input_num() )
    const std::string& name ///< [in] 名前
  )
  {
    auto key = _input_key(input_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id,      ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    const std::string& name  ///< [in] 名前
  )
  {
    auto key = _output_key(output_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief DFF名をセットする．
  void
  set_dff_name(
    SizeType dff_id,        ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
    const std::string& name ///< [in] 名前
  )
  {
    auto key = _dff_key(dff_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 新しいノード用の番号を確保する．
  ///
  /// @return ID番号を返す．
  SizeType
  alloc_node()
  {
    auto id = mNodeArray.size();
    mNodeArray.push_back(unique_ptr<NodeImpl>{nullptr});
    return id;
  }

  /// @brief 対応するID番号に入力用の印を付ける．
  void
  set_input(
    SizeType id            ///< [in] ID番号
  );

  /// @brief 対応するID番号にDFFの出力用の印を付ける．
  void
  set_dff_output(
    SizeType id,    ///< [in] DFFの出力のノード番号
    SizeType src_id ///< [in] DFFの入力のノード番号
  );

  /// @brief 論理ノードの情報をセットする．
  void
  set_logic(
    SizeType id,                       ///< [in] ID番号
    SizeType func_id,                  ///< [in] 関数番号
    const vector<SizeType>& fanin_list ///< [in] 入力の識別子番号のリスト
  );

  /// @brief 新しい入力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_input()
  {
    auto id = alloc_node();
    set_input(id);
    return id;
  }

  /// @brief 新しい DFFの出力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_dff_output()
  {
    auto id = alloc_node();
    set_dff_output(id, BAD_ID);
    return id;
  }

  /// @brief 新しい出力ノードを作る．
  ///
  /// @return 出力番号を返す．
  SizeType
  new_output(
    SizeType src_id ///< [in] ソースのID番号
  )
  {
    auto oid = mOutputList.size();
    mOutputList.push_back(src_id);
    return oid;
  }

  /// @brief 新しい論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_logic(
    SizeType func_id,                  ///< [in] 関数番号
    const vector<SizeType>& fanin_list ///< [in] 入力の識別子番号のリスト
  )
  {
    auto id = alloc_node();
    set_logic(id, func_id, fanin_list);
    return id;
  }

  /// @brief DFFのソースをセットする．
  void
  set_dff_src(
    SizeType id,         ///< [in] ID番号
    SizeType src_id      ///< [in] ソースのID番号
  )
  {
    auto node = mNodeArray[id].get();
    node->set_dff_src(src_id);
  }

  /// @brief 論理ノードのリストを作る．
  void
  make_logic_list();

  /// @brief プリミティブを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_primitive(
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  )
  {
    return mFuncMgr.reg_primitive(input_num, primitive_type);
  }

  /// @brief カバーを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_cover(
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転属性
  )
  {
    return mFuncMgr.reg_cover(input_cover, output_inv);
  }

  /// @brief 論理式を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_expr(
    const Expr& expr ///< [in] 論理式
  )
  {
    return mFuncMgr.reg_expr(expr);
  }

  /// @brief 真理値表を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  )
  {
    return mFuncMgr.reg_tvfunc(func);
  }

  /// @brief BDDを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  )
  {
    return mFuncMgr.reg_bdd(bdd);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トポロジカルソートを行い mLogicList にセットする．
  void
  order_node(
    SizeType id,                  ///< [in] ID番号
    unordered_set<SizeType>& mark ///< [in] マーク
  );

  /// @brief SymbolDict の入力用のキーを作る．
  string
  _input_key(
    SizeType input_id
  ) const
  {
    ostringstream buf;
    buf << "I" << input_id;
    return buf.str();
  }

  /// @brief SymbolDict の出力用のキーを作る．
  string
  _output_key(
    SizeType output_id
  ) const
  {
    ostringstream buf;
    buf << "O" << output_id;
    return buf.str();
  }

  /// @brief SymbolDict のDFF用のキーを作る．
  string
  _dff_key(
    SizeType dff_id
  ) const
  {
    ostringstream buf;
    buf << "Q" << dff_id;
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  std::string mName;

  // コメントのりスト
  std::vector<std::string> mCommentList;

  // シンボルの辞書
  std::unordered_map<std::string, std::string> mSymbolDict;

  // NodeImplの配列
  // NodeImpl の所有権を持つ．
  std::vector<std::unique_ptr<NodeImpl>> mNodeArray;

  // 入力のノード番号のリスト
  std::vector<SizeType> mInputList;

  // DFF出力のノード番号のリスト
  std::vector<SizeType> mDffList;

  // 出力のノード番号のリスト
  std::vector<SizeType> mOutputList;

  // 論理ノード番号のリスト
  std::vector<SizeType> mLogicList;

  // 関数情報のマネージャ
  FuncMgr mFuncMgr;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H
