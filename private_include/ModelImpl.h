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
  string
  name() const
  {
    return mName;
  }

  /// @brief コメントを返す．
  string
  comment() const
  {
    return mComment;
  }

  /// @brief 入力名を返す．
  string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    auto key = _input_key(input_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief 出力名を返す．
  string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    auto key = _output_key(output_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief ラッチ名を返す．
  string
  seq_name(
    SizeType seq_id ///< [in] ラッチ番号 ( 0 <= seq_id < seq_num() )
  ) const
  {
    auto key = _seq_key(seq_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return string{};
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
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
    SizeType pos
  ) const
  {
    return mInputList[pos];
  }

  /// @brief 入力のノード番号のリストを返す．
  const vector<SizeType>&
  input_list() const
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
    SizeType pos
  ) const
  {
    return mOutputList[pos];
  }

  /// @brief 出力のノード番号のリストを返す．
  const vector<SizeType>&
  output_list() const
  {
    return mOutputList;
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
    SizeType pos ///< [in] 位置
  ) const
  {
    return mLogicList[pos];
  }

  /// @brief 論理ノード番号のリストを返す．
  const vector<SizeType>&
  logic_list() const
  {
    return mLogicList;
  }

  /// @brief ノードを取り出す．
  const NodeImpl*
  node_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    return mNodeArray[id].get();
  }

  /// @brief DFF数を返す．
  SizeType
  dff_num() const
  {
    return mDffArray.size();
  }

  /// @brief SEQノードを取り出す．
  const SeqImpl&
  seq_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    return *mSeqArray[id];
  }

  /// @brief 関数の数を返す．
  SizeType
  func_num() const
  {
    return mFuncArray.size();
  }

  /// @brief 関数情報を返す．
  const FuncImpl&
  func_impl(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const
  {
    return *mFuncArray[func_id];
  }

  /// @brief BnNode が自分のものか調べる．
  bool
  check(
    BnNode node
  ) const
  {
    return node.mModel.get() == this;
  }

  /// @brief BnFunc が自分のものか調べる．
  bool
  check(
    BnFunc func
  ) const
  {
    return func.mModel.get() == this;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // プロキシオブジェクトの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを生成する．
  static
  BnNode
  new_node(
    const shared_ptr<const ModelImpl>& model,
    SizeType id
  )
  {
    return BnNode(model, id);
  }

  /// @brief 論理関数を生成する．
  static
  BnFunc
  new_func(
    const shared_ptr<const ModelImpl>& model,
    SizeType id
  )
  {
    return BnFunc(model, id);
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
    const string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief コメントを設定する．
  void
  set_comment(
    const string& comment ///< [in] コメント
  )
  {
    mComment = comment;
  }

  /// @brief 入力名をセットする．
  void
  set_input_name(
    SizeType input_id, ///< [in] 入力番号 ( 0 <= input_id < input_num() )
    const string& name ///< [in] 名前
  )
  {
    auto key = _input_key(input_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id, ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    const string& name  ///< [in] 名前
  )
  {
    auto key = _output_key(output_id);
    mSymbolDict.emplace(key, name);
  }

  /// @brief DFF名をセットする．
  void
  set_dff_name(
    SizeType dff_id,   ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
    const string& name ///< [in] 名前
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
    SizeType id            ///< [in] ID番号
  );

  /// @brief 出力ノードをセットする．
  void
  set_output(
    SizeType output_id, ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    SizeType src_id     ///< [in] ソースのID番号
  )
  {
    mOutputList[output_id] = src_id;
  }

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
    set_dff_output(id);
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
  );

  /// @brief カバーを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_cover(
    const SopCover& input_cover ///< [in] 入力カバー
    bool output_inv             ///< [in] 出力の反転属性
  );

  /// @brief 論理式を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  );


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

  /// @brief SeqImpl を登録する．
  SizeType
  _reg_seq(
    SeqImpl* seq,
    const string& name
  )
  {
    auto id = mSeqArray.size();
    mSeqArray.push_back(unique_ptr<SeqImpl>{seq});
    if ( name != string{} ) {
      set_seq_name(id, name);
    }
    return id;
  }

  /// @brief SeqImpl を取り出す．
  SeqImpl&
  _seq(
    SizeType id,              ///< [in] ID番号
    const string& func_name,  ///< [in] 関数名
    const string& index_name  ///< [in] ID番号の変数名
  )
  {
    return *mSeqArray[id];
  }

  /// @brief FuncImpl を取り出す．
  FuncImpl&
  _func(
    SizeType id
  )
  {
    return *mFuncArray[id];
  }

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
    buf << "Q" << seq_id;
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr mBddMgr;

  // 名前
  std::string mName;

  // コメント
  std::string mComment;

  // シンボルの辞書
  std::unordered_map<std::string, std::string> mSymbolDict;

  // 入力のノード番号のリスト
  std::vector<SizeType> mInputList;

  // DFF出力のノード番号のリスト
  std::vector<SizeType> mDffList;

  // 出力のノード番号のリスト
  std::vector<SizeType> mOutputList;

  // 論理ノード番号のリスト
  std::vector<SizeType> mLogicList;

  // NodeImplの配列
  std::vector<std::unique_ptr<NodeImpl>> mNodeArray;

  // 関数情報のマネージャ
  FuncMgr mFuncMgr;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H
