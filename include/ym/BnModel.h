#ifndef BNMODEL_H
#define BNMODEL_H

/// @file BnModel.h
/// @brief BnModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/json.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnModel BnModel.h "BnModel.h"
/// @brief Boolean Network を表すクラス
///
/// 以下の情報を持つ．
/// - 入力ノードのリスト
/// - 出力ノードのリスト
/// - 論理ノードのリスト
///
/// 論理ノードは以下のいずれかの論理関数情報を持つ．
/// - プリミティブ(PrimType)
/// - カバー(SopCover)
/// - 論理式(Expr)
/// - 関数(TvFunc)
/// - BDD(Bdd)
/// 場合によっては複数の情報を持つ場合もある．
///
/// 出力ノードは実際にはノードではなく他のノードの参照である．
///
/// 純粋な Boolean Network は抽象化した組み合わせ論理回路を表すものであるが、
/// DFFの入力ノードと出力ノードをそれぞれ疑似外部出力、疑似外部入力とみなすことで
/// 同期式順序回路を表すことができる．
/// ただし、クロックや非同期リセットなどの情報は持てない．
///
/// - 扱えるファイルタイプは以下の通り
///   * blif(.blif)
///   * iscas89(.bench)
///   * aiger(.aag, .aig)
///   * truth(IWLS2022)
///
/// - 回路名，コメント，入出力名をオプションとして持つ．
/// - 本来，入出力名はユニークであるべきだが，blif/iscas89
///   の場合は出力名は出力として参照されているノード名と等しい．
///   そのため入力ノードがそのまま出力となっている場合には
///   入力と出力に同じ名前を持つことになる．
///
/// - 実装は本体の ModelImpl を指す共有ポインタのみを持つ．
///   ただし，ModelImpl は同一の BnModel に属する BnNode 内でしか共有しない．
//////////////////////////////////////////////////////////////////////
class BnModel
{
public:

  /// @brief 空のコンストラクタ
  BnModel();

  /// @brief コピーコンストラクタ
  ///
  /// '浅い'コピーを行う．
  BnModel(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  BnModel(
    BnModel&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief 代入演算子
  ///
  /// '浅い'コピーを行う．
  BnModel&
  operator=(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnModel();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 読み込みを行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_blif(
    const string& filename ///< [in] ファイル名
  );

  /// @brief iscas89(.bench) ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_iscas89(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aag ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aag(
    const string& filename ///< [in] ファイル名
  );

  /// @brief aig ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aig(
    const string& filename ///< [in] ファイル名
  );

  /// @brief truth ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_truth(
    const string& filename ///< [in] ファイル名
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を読み出す関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief '深い'コピーを作る．
  BnModel
  copy() const;

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 入力ノードのリストを返す．
  std::vector<BnNode>
  input_list() const;

  /// @brief 出力数を返す．
  SizeType
  output_num() const;

  /// @brief 出力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief 出力のノードのリストを返す．
  std::vector<BnNode>
  output_list() const;

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const;

  /// @brief 論理ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const;

  /// @brief 論理ノードのリストを返す．
  ///
  /// 入力からのトポロジカル順になっている．
  std::vector<BnNode>
  logic_list() const;

  /// @brief DFF数を返す．
  SizeType
  dff_num() const;

  /// @brief DFFの出力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  dff_output(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief DFFの入力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  dff_src(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief 関数情報の数を返す．
  SizeType
  func_num() const;

  /// @brief 関数情報を取り出す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnFunc
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator==(
    const BnModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator!=(
    const BnModel& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オプション情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報を表す JSON オブジェクトを返す．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief コメントを返す．
  string
  comment() const;

  /// @brief 入力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 出力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief DFF名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  string
  dff_name(
    SizeType dff_id ///< [in] ラッチ番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を設定する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 設定情報を確定する．
  void
  wrap_up();

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 追加するオプション
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを作る．
  /// @return 生成したノードを返す．
  BnNode
  new_input(
    const string& name = {} ///< [in] 名前
  );

  /// @brief 出力ノードを作る．
  /// @return 生成した出力ノードの出力番号を返す．
  ///
  /// - src は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  SizeType
  new_output(
    BnNode src,             ///< [in] ソースノード
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しいプリミティブ型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - input_list の要素は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_primitive(
    PrimType prim_type,              ///< [in] プリミティブ型
    const vector<BnNode>& input_list ///< [in] 入力の識別子番号のリスト
  );

  /// @brief 新しいAIG型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - src0, src1 は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_aig(
    BnNode src0,            ///< [in] ソース0のノード
    BnNode src1,            ///< [in] ソース1のノード
    bool inv0,              ///< [in] ソース0の反転属性
    bool inv1               ///< [in] ソース1の反転属性
  );

  /// @brief 新しい関数型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - local_func は事前に reg_cover(), reg_expr(), reg_tvfunc(), reg_bdd()
  ///   で登録したものを使う．
  /// - input_list の要素は同じ BnModel に属するノードでなければならない．
  /// - input_list の要素数は local_func の条件に合致しなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_func(
    BnFunc local_func,               ///< [in] 関数
    const vector<BnNode>& input_list ///< [in] 入力の識別子番号のリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理関数表現の登録
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを登録する．
  /// @return 登録結果(BnFunc)を返す．
  ///
  /// - cube_list 中に現れるリテラル番号は input_num 未満でなければならない．
  /// - 上記の条件に合わない場合は std::invalid_argument 例外を送出する．
  BnFunc
  reg_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    bool oinv                                 ///< [in] 反転属性
  );

  /// @brief 論理式を登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを登録する．
  /// @return 登録結果(BnFunc)を返す．
  BnFunc
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name SEQノードの生成/設定
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFを作る．
  /// @return 生成したSEQノードを返す．
  ///
  /// - rs_val は ' ', '0', '1', 'X', 'T' のいずれか
  /// - そうでなければ std::invalid_argument 例外を送出する．
  BnSeq
  new_dff(
    char rs_val = ' ',      ///< [in] リセットとプリセットが共にオンの時の値
    const string& name = {} ///< [in] 名前
  );

  /// @brief DFF/ラッチのソースノードをセットする．
  ///
  /// - seq は同じ BnModel に属していなければならない．
  /// - src は同じ BnModel に属していなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  void
  set_data_src(
    BnSeq seq,    ///< [in] SEQノード
    BnNode src    ///< [in] 入力ノード
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_node(
    const BnNode& node
  ) const;

  /// @brief BnFunc のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_func(
    const BnFunc& func
  ) const;

  /// @brief 入力番号のチェックを行う．
  void
  _check_input_id(
    SizeType input_id
  ) const;

  /// @brief 出力番号のチェックを行う．
  void
  _check_output_id(
    SizeType output_id
  ) const;

  /// @brief 論理ノード番号のチェックを行う．
  void
  _check_logic_id(
    SizeType logic_id
  ) const;

  /// @brief SDFF番号のチェックを行う．
  void
  _check_dff_id(
    SizeType dff_id
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  std::shared_ptr<ModelImpl> mImpl;

};

END_NAMESPACE_YM_BN

#endif // BNMODEL_H
