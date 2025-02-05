
/// @file BnModel_test.cc
/// @brief BnModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
#include "ym/AlgCover.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_BN

TEST( BnModelTest, constructor1 )
{
  BnModel model;

  EXPECT_FALSE( model.library().is_valid() );
  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.input_list().size() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.output_list().size() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.logic_list().size() );
  EXPECT_EQ( 0, model.seq_num() );
  EXPECT_EQ( 0, model.func_num() );
}

TEST( BnModelTest, input_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input(0);}, std::out_of_range );
}

TEST( BnModelTest, input_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input_name(0);}, std::out_of_range );
}

TEST( BnModelTest, output_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output(0);}, std::out_of_range );
}

TEST( BnModelTest, output_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output_name(0);}, std::out_of_range );
}

TEST( BnModelTest, logic_bad )
{
  BnModel model;

  EXPECT_THROW( {model.logic(0);}, std::out_of_range );
}

TEST( BnModelTest, seq_bad )
{
  BnModel model;

  EXPECT_THROW( {model.seq(0);}, std::out_of_range );
}

TEST( BnModelTest, func_bad )
{
  BnModel model;

  EXPECT_THROW( {model.func(0);}, std::out_of_range );
}

TEST( BnModelTest, reg_cover )
{
  BnModel model;

  Literal lit0{0, false};
  Literal lit1{1, false};
  auto func = model.reg_cover(2, {{lit0, lit1}}, '0');

  EXPECT_EQ( BnFuncType::COVER, func.type() );
  EXPECT_TRUE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( 2, func.input_num() );

  auto& sop = func.input_cover();
  ASSERT_EQ( 2, sop.variable_num() );
  ASSERT_EQ( 1, sop.cube_num() );
  EXPECT_EQ( AlgPat::_1, sop.get_pat(0, 0) );
  EXPECT_EQ( AlgPat::_1, sop.get_pat(0, 1) );

  EXPECT_EQ( '0', func.output_pat() );

  auto expr = func.expr();
  auto rep_str = expr.rep_string();
  EXPECT_EQ( "O2N0N1", rep_str );
}

TEST( BnModelTest, reg_expr )
{
  BnModel model;

  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto expr = v0 | ~v1;
  auto func = model.reg_expr(expr);

  EXPECT_EQ( BnFuncType::EXPR, func.type() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_TRUE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( 2, func.input_num() );
  EXPECT_THROW( {func.input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func.output_pat();}, std::invalid_argument );
  EXPECT_EQ( "O2P0N1", func.expr().rep_string() );
  EXPECT_THROW( {func.tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func.bdd();}, std::invalid_argument );
}

TEST( BnModelTest, reg_tvfunc )
{
  auto v0 = TvFunc::posi_literal(3, 0);
  auto v1 = TvFunc::posi_literal(3, 1);
  auto v2 = TvFunc::posi_literal(3, 2);
  auto tvfunc = v0 | v1 | v2;

  BnModel model;

  auto func = model.reg_tvfunc(tvfunc);

  EXPECT_EQ( BnFuncType::TVFUNC, func.type() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_TRUE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( 3, func.input_num() );
  EXPECT_THROW( {func.input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func.output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func.expr();}, std::invalid_argument );
  EXPECT_EQ( "11111110", func.tvfunc().str() );
  EXPECT_THROW( {func.bdd();}, std::invalid_argument );
}

TEST( BnModelTest, reg_bdd )
{
  BnModel model;

  BddMgr mgr;
  auto var0 = mgr.variable(0);
  auto var1 = mgr.variable(1);
  auto bdd = var0 & ~var1;
  auto func = model.reg_bdd(bdd);

  EXPECT_EQ( BnFuncType::BDD, func.type() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_TRUE( func.is_bdd() );
  EXPECT_EQ( 2, func.input_num() );
  EXPECT_THROW( {func.input_cover();}, std::invalid_argument );
  EXPECT_THROW( {func.output_pat();}, std::invalid_argument );
  EXPECT_THROW( {func.expr();}, std::invalid_argument );
  EXPECT_THROW( {func.tvfunc();}, std::invalid_argument );
  EXPECT_EQ( bdd, mgr.copy(func.bdd()) );
}

TEST( BnModelTest, new_input )
{
  BnModel model;

  auto node = model.new_input();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_TRUE( node.is_input() );
}

TEST( BnModelTest, new_primitive)
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  PrimType type = PrimType::And;
  auto node = model.new_primitive(type, fanin_list);

  EXPECT_EQ( BnNodeType::PRIMITIVE, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_TRUE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( type, node.primitive_type() );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.local_func(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_aig )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  bool inv0 = false;
  bool inv1 = true;
  auto node = model.new_aig(input1, input2, inv0, inv1);

  EXPECT_EQ( BnNodeType::AIG, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_TRUE( node.is_aig() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( 2, node.fanin_num() );
  EXPECT_EQ( input1, node.fanin(0) );
  EXPECT_EQ( input2, node.fanin(1) );
  vector<BnNode> fanin_node_list{input1, input2};
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node.fanin_inv(0) );
  EXPECT_EQ( inv1, node.fanin_inv(1) );
  EXPECT_THROW( {node.local_func(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_func_cover )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  Literal lit1{0, false};
  Literal lit2{1, false};
  vector<vector<Literal>> cube_list{{lit1}, {lit2}};
  auto func = model.reg_cover(2, cube_list, '0');
  auto node = model.new_func(func, fanin_list);

  EXPECT_EQ( BnNodeType::FUNC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( func, node.local_func() );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_func_expr )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  auto lit1 = Expr::posi_literal(0);
  auto lit2 = Expr::posi_literal(1);
  auto expr = lit1 | lit2;
  auto func = model.reg_expr(expr);
  auto node = model.new_func(func, fanin_list);

  EXPECT_EQ( BnNodeType::FUNC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( func, node.local_func() );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_func_tvfunc )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};

  auto v0 = TvFunc::posi_literal(2, 0);
  auto v1 = TvFunc::posi_literal(2, 1);
  auto tvfunc = v0 | v1;
  auto func = model.reg_tvfunc(tvfunc);
  auto node = model.new_func(func, fanin_list);

  EXPECT_EQ( BnNodeType::FUNC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( func, node.local_func() );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_func_bdd )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};

  BddMgr mgr;
  auto var0 = mgr.variable(0);
  auto var1 = mgr.variable(1);
  auto bdd = var0 | var1;
  auto func = model.reg_bdd(bdd);
  auto node = model.new_func(func, fanin_list);

  EXPECT_EQ( BnNodeType::FUNC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_func() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( func, node.local_func() );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
}

TEST( BnModelTest, new_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18NAND2P005";
  auto cell = library.cell(CELL_NAME);

  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  auto node = model.new_cell(cell, fanin_list);

  EXPECT_EQ( BnNodeType::CELL, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.local_func(); }, std::invalid_argument );
  EXPECT_EQ( cell, node.cell() );
}

TEST( BnModelTest, new_dff)
{
  BnModel model;

  char rsval = '1';
  auto node = model.new_dff(rsval);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnSeqType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.data_src().is_valid() );
  EXPECT_FALSE( node.clock().is_valid() );
  EXPECT_THROW( {node.enable(); }, std::invalid_argument );
  EXPECT_FALSE( node.clear().is_valid() );
  EXPECT_FALSE( node.preset().is_valid() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );

  auto onode = node.data_output();
  EXPECT_FALSE( onode.is_valid() );
}

TEST( BnModelTest, new_latch)
{
  BnModel model;

  auto node = model.new_latch();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnSeqType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.data_src().is_valid() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_FALSE( node.enable().is_valid() );
  EXPECT_FALSE( node.clear().is_valid() );
  EXPECT_FALSE( node.preset().is_valid() );
  EXPECT_EQ( ' ', node.rsval() );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );

  auto onode = node.data_output();
  EXPECT_FALSE( onode.is_valid() );
}

TEST( BnModelTest, new_seq_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18DFNP010";
  auto cell = library.cell(CELL_NAME);

  BnModel model;
  auto node = model.new_seq_cell(cell);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( BnSeqType::CELL, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_THROW( {node.data_src();}, std::invalid_argument );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_THROW( {node.enable();}, std::invalid_argument );
  EXPECT_THROW( {node.clear();}, std::invalid_argument );
  EXPECT_THROW( {node.preset();}, std::invalid_argument );
  EXPECT_THROW( {node.rsval();}, std::invalid_argument );
  EXPECT_THROW( {node.data_output();}, std::invalid_argument );
  EXPECT_EQ( cell, node.cell() );
  EXPECT_FALSE( node.cell_pin(0).is_valid() );
  EXPECT_FALSE( node.cell_pin(1).is_valid() );
  EXPECT_FALSE( node.cell_pin(2).is_valid() );
}

TEST( BnModelTest, clear )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  PrimType type = PrimType::And;
  auto node = model.new_primitive(type, fanin_list);
  auto output = model.new_output(node);

  model.wrap_up();

  EXPECT_EQ( 2, model.input_num() );
  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( 1, model.logic_num() );

  model.clear();

  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.logic_num() );
}

END_NAMESPACE_YM_BN
