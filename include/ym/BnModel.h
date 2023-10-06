#ifndef BNMODEL_H
#define BNMODEL_H

/// @file BnModel.h
/// @brief BnModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/clib.h"
#include "ym/logic.h"


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
/// - DFFノードのリスト
/// - ラッチノードのリスト
/// - カバー(BlifCover)のリスト
/// - 論理式(Expr)のリスト
/// - 関数(TvFunc)のリスト
/// - BDD(Bdd)のリスト
///
/// - 論理ノードは以下の情報を持つ．
///   * タイプ(Primitive, Aig, Cover, Expr, Func, Bdd, Cell)
///   * タイプごとの補助情報
///   * ファンインノードのリスト
///
/// - Dffタイプは以下の情報を持つ．
///   * データ入力ノード
///   * クロック入力ノード
///   * リセット入力ノード
///   * プリセット入力ノード
///   * リセットとプリセットが共にオンの時の値('0', '1', 'X', or 'T')
///
/// - ラッチタイプは以下の情報を持つ．
///   * データ入力ノード
///   * イネーブル入力ノード
///   * リセット入力ノード
///   * プリセット入力ノード
///   * リセットとプリセットが共にオンの時の値('0', '1', 'X', or 'T')
///
/// - 実際には出力ノードという種類はなく，他のいずれかの
///   ノードとなっている．
/// - 論理ノードのリストは入力からのトポロジカル順
///   となっている．
///
/// - 扱えるファイルタイプは以下の通り
///   * blif(.blif)
///   * iscas89(.bench)
///   * aig(.aag, .aig)
///   * truth(IWLS2022)
///
/// - 読み込んだファイルによってはノードはユニークな名前を持つ．
/// - 入力名は入力ノード名と等しい
/// - blif/iscas89 の場合は出力名は出力として参照されているノード名と
///   等しい．
///   そのため入力ノードがそのまま出力となっている場合には
///   入力と出力に同じ名前を持つことになる．
//////////////////////////////////////////////////////////////////////
class BnModel
{
  friend BnNode;

private:

  /// @brief shared_ptr<ModelImpl> からのキャストコンストラクタ
  BnModel(
    const shared_ptr<ModelImpl>& impl ///< [in] 実装オブジェクト
  );


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

  /// @brief コピー代入演算子
  ///
  /// '浅い'コピーを行う．
  BnModel&
  operator=(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  BnModel&
  operator=(
    BnModel&& src ///< [in] ムーブ元のオブジェクト
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
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
  );

  /// @brief blif ファイルの読み込みを行う(セルライブラリ付き)．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_blif(
    const string& filename,              ///< [in] ファイル名
    const ClibCellLibrary& cell_library, ///< [in] セルライブラリ
    const string& clock_name = "clock",  ///< [in] クロック信号の名前
    const string& reset_name = "reset"   ///< [in] リセット信号の名前
  );

  /// @brief iscas89(.bench) ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_iscas89(
    const string& filename,            ///< [in] ファイル名
    const string& clock_name = "clock" ///< [in] クロック信号の名前
  );

  /// @brief aag ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aag(
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
  );

  /// @brief aig ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_aig(
    const string& filename,             ///< [in] ファイル名
    const string& clock_name = "clock", ///< [in] クロック信号の名前
    const string& reset_name = "reset"  ///< [in] リセット信号の名前
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

  /// @brief 深いコピーを行う．
  BnModel
  copy() const;

  /// @brief 名前を返す．
  ///
  /// 空の場合もある．
  string
  name() const;

  /// @brief コメントを返す．
  ///
  /// 空の場合もある．
  string
  comment() const;

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力のノードを返す．
  BnNode
  input(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力のノードのリストを返す．
  vector<BnNode>
  input_list() const;

  /// @brief 入力名を返す．
  string
  input_name(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief 入力名のリストを返す．
  vector<string>
  input_name_list() const;

  /// @brief 出力数を返す．
  SizeType
  output_num() const;

  /// @brief 入力のノードを返す．
  BnNode
  output(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力のノードのリストを返す．
  vector<BnNode>
  output_list() const;

  /// @brief 出力名を返す．
  string
  output_name(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() )
  ) const;

  /// @brief 出力名のリストを返す．
  vector<string>
  output_name_list() const;

  /// @brief DFF数を返す．
  SizeType
  dff_num() const;

  /// @brief DFFのノードを返す．
  BnNode
  dff(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < dff_num() )
  ) const;

  /// @brief DFFのノードのリストを返す．
  vector<BnNode>
  dff_list() const;

  /// @brief ラッチ数を返す．
  SizeType
  latch_num() const;

  /// @brief ラッチのノードを返す．
  BnNode
  latch(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < latch_num() )
  ) const;

  /// @brief ラッチのノードのリストを返す．
  vector<BnNode>
  latch_list() const;

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const;

  /// @brief 論理ノードを返す．
  BnNode
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const;

  /// @brief 論理ノードのリストを返す．
  vector<BnNode>
  logic_list() const;

  /// @brief カバーの種類の数を返す．
  SizeType
  cover_num() const;

  /// @brief カバーを取り出す．
  const BnCover&
  cover(
    SizeType cover_id ///< [in] カバー番号
  ) const;

  /// @brief 論理式の数を返す．
  SizeType
  expr_num() const;

  /// @brief 論理式を返す．
  const Expr&
  expr(
    SizeType expr_id ///< [in] 論理式番号 ( 0 <= expr_id < expr_num() )
  ) const;

  /// @brief 真理値表型の関数の数を返す．
  SizeType
  func_num() const;

  /// @brief 真理値表型の関数を返す．
  const TvFunc&
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const;

  /// @brief BDDの数を返す．
  SizeType
  bdd_num() const;

  /// @brief BDDを返す．
  Bdd
  bdd(
    SizeType bdd_id ///< [in] BDD番号 ( 0 <= bdd_id < bdd_num() )
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
  /// @name 内容を設定する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  void
  set_name(
    const string& name ///< [in] 名前
  );

  /// @brief コメントを設定する．
  void
  set_comment(
    const string& comment ///< [in] コメント
  );

  /// @brief 出力の名前を設定する．
  void
  set_output_name(
    SizeType pos,      ///< [in] 位置番号 ( 0 <= pos < output_num() )
    const string& name ///< [in] 名前
  );

  /// @brief 新しい入力ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_input(
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しいプリミティブ型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_primitive(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    PrimType type,                    ///< [in] プリミティブタイプ
    const string& name = {}           ///< [in] 名前
  );

  /// @brief 新しいAIG型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_aig(
    BnNode src0,            ///< [in] ソース0のID番号
    BnNode src1,            ///< [in] ソース1のID番号
    bool inv0,              ///< [in] ソース0の反転属性
    bool inv1,              ///< [in] ソース1の反転属性
    const string& name = {} ///< [in] 名前
  );

  /// @brief 新しいカバー型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_cover(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType cover_id,                ///< [in] カバー番号
    const string& name = {}           ///< [in] 名前
  );

  /// @brief 論理式型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_expr(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType expr_id,                 ///< [in] 論理式番号
    const string& name = {}           ///< [in] 名前
  );

  /// @brief セル型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_cell(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType cell_id,                 ///< [in] セル番号
    const string& name = {}           ///< [in] 名前
  );

  /// @brief 真理値表型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_func(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType func_id,                 ///< [in] 関数番号
    const string& name = {}           ///< [in] 名前
  );

  /// @brief BDD型の論理ノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_bdd(
    const vector<BnNode>& input_list, ///< [in] 入力の識別子番号のリスト
    SizeType bdd_id,                  ///< [in] BDD番号
    const string& name = {}           ///< [in] 名前
  );

  /// @brief DFF型のノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_dff(
    BnNode src,             ///< [in] 入力の識別子番号
    BnNode clock,           ///< [in] クロック入力の識別子番号
    BnNode reset,           ///< [in] リセット入力の識別子番号
    BnNode preset,          ///< [in] プリセット入力の識別子番号
    char rs_val = ' ',      ///< [in] リセットとプリセットが共にオンの時の値
    const string& name = {} ///< [in] 名前
  );

  /// @brief ラッチ型のノードを作る．
  ///
  /// @return 生成したノードを返す．
  BnNode
  new_latch(
    BnNode src,             ///< [in] 入力の識別子番号
    BnNode enable,          ///< [in] イネーブル入力の識別子番号
    BnNode reset,           ///< [in] リセット入力の識別子番号
    BnNode preset,          ///< [in] プリセット入力の識別子番号
    char rs_val = ' ',      ///< [in] リセットとプリセットが共にオンの時の値
    const string& name = {} ///< [in] 名前
  );

  /// @brief 論理ノードのリストを作る．
  void
  make_logic_list();

  /// @brief カバーを追加する．
  /// @return カバー番号を返す．
  SizeType
  add_cover(
    SizeType input_num,                       ///< [in] 入力数
    const vector<vector<Literal>>& cube_list, ///< [in] キューブのリスト
    char opat                                 ///< [in] 出力パタン ( '1' or '0' )
  );

  /// @brief 論理式を追加する．
  /// @return 論理式番号を返す．
  SizeType
  add_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表を追加する．
  /// @return 関数番号を返す．
  SizeType
  add_func(
    const TvFunc& func ///< [in] 真理値表型の関数
  );

  /// @brief BDDを追加する．
  /// @return BDD番号を返す．
  SizeType
  add_bdd(
    const Bdd& bdd ///< [in] BDD
  );

  /// @}
  //////////////////////////////////////////////////////////////////////

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID 番号から BnNode を作る．
  BnNode
  from_id(
    SizeType id
  ) const;

  /// @brief ID 番号のリストから vector<BnNode> を作る．
  vector<BnNode>
  from_id_list(
    const vector<SizeType>& id_list
  ) const;

  /// @brief BnNode から ID 番号を取り出す．
  ///
  /// 同じ Model に属していないとエラーとなる．
  SizeType
  to_id(
    const BnNode& node,
    const string& funcname
  ) const;

  /// @brief vector<BnNode> から ID番号のリストを取り出す．
  ///
  /// 同じ Model に属していないとエラーとなる．
  vector<SizeType>
  to_id_list(
    const vector<BnNode>& node_list,
    const string& funcname
  ) const;

  /// @brief 正しいカバー番号かチェックする．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_cover(
    SizeType id,           ///< [in] カバー番号
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief カバーと入力のサイズが合っているか調べる．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_cover_input(
    SizeType id,           ///< [in] カバー番号
    SizeType size,         ///< [in] 入力のサイズ
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief 正しい論理式番号かチェックする．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_expr(
    SizeType id,           ///< [in] 論理式番号
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief 論理式と入力のサイズが合っているか調べる．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_expr_input(
    SizeType id,           ///< [in] 論理式番号
    SizeType size,         ///< [in] 入力のサイズ
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief 正しい関数番号かチェックする．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_func(
    SizeType id,           ///< [in] 関数番号
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief 関数と入力のサイズが合っているか調べる．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_func_input(
    SizeType id,           ///< [in] 関数番号
    SizeType size,         ///< [in] 入力のサイズ
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief 正しいBDD番号かチェックする．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_bdd(
    SizeType id,           ///< [in] BDD番号
    const string& funcname ///< [in] 関数名
  ) const;

  /// @brief BDDと入力のサイズが合っているか調べる．
  ///
  /// 正しくない場合には std::invalid_argument 例外を送出する．
  void
  _check_bdd_input(
    SizeType id,           ///< [in] BDD番号
    SizeType size,         ///< [in] 入力のサイズ
    const string& funcname ///< [in] 関数名
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  shared_ptr<ModelImpl> mImpl;

};

END_NAMESPACE_YM_BN

#endif // BNMODEL_H
