
/// @file TruthReader.cc
/// @brief TruthReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "TruthReader.h"
#include "ym/BnModel.h"
#include "ym/TvFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////


// @brief .truth 形式のファイルを読み込む．
// @return ネットワークを返す．
BnModel
BnModel::read_truth(
  const string& filename ///< [in] ファイル名
)
{
  ifstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << filename << ": No such file";
    throw std::invalid_argument{buf.str()};
  }

  BnModel model;
  TruthReader reader;
  reader.read(s, model.mImpl.get());
  return model;
}


//////////////////////////////////////////////////////////////////////
// クラス TruthReader
//////////////////////////////////////////////////////////////////////

// @brief 真理値表形式のファイルを読み込む．
void
TruthReader::read(
  istream& s,
  ModelImpl* model
)
{
  vector<TvFunc> func_vect;
  {
    string line;
    while ( getline(s, line) ) {
      func_vect.push_back(TvFunc{line});
    }
  }

  SizeType no = func_vect.size();
  if ( no > 0 ) {
    // 入力数が全て等しいかチェック
    SizeType ni = func_vect[0].input_num();
    for ( SizeType i = 1; i < no; ++ i ) {
      auto& func = func_vect[i];
      if ( func.input_num() != ni ) {
	cout << "ni = " << ni << ", func_vect[" << i << "].input_num() = "
	     << func.input_num() << endl;
	throw std::invalid_argument{"the number of inputs should be the same for all outputs"};
      }
    }
    // 入力の生成
    vector<SizeType> input_list(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto id = model->new_input();
      input_list[i] = id;
    }
    // 論理ノードの生成
    // 注意が必要なのは .truth フォーマットでは最上位の変数が
    // 最後の変数だということ．
    // ファンインリストは input_list の逆順になる．
    vector<SizeType> fanin_list(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      fanin_list[i] = input_list[ni - i - 1];
    }
    for ( SizeType i = 0; i < no; ++ i ) {
      auto func_id = model->add_func(func_vect[i]);
      auto id = model->new_func(fanin_list, func_id);
      model->set_output(id);
    }
  }

  model->make_logic_list();
}

END_NAMESPACE_YM_BN
