
/// @file BnModel_test.cc
/// @brief BnModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnDff.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( BnModelTest, constructor1 )
{
  BnModel model;

  EXPECT_FALSE( model.library().is_valid() );
  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( vector<BnNode>{}, model.input_list() );
  EXPECT_EQ( vector<string>{}, model.input_name_list() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( vector<BnNode>{}, model.output_list() );
  EXPECT_EQ( vector<string>{}, model.output_name_list() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( vector<BnNode>{}, model.logic_list() );
  EXPECT_EQ( 0, model.dff_num() );
  EXPECT_EQ( vector<BnDff>{}, model.dff_list() );
  EXPECT_EQ( 0, model.cover_num() );
  EXPECT_EQ( 0, model.expr_num() );
  EXPECT_EQ( 0, model.func_num() );
  EXPECT_EQ( 0, model.bdd_num() );
}

TEST( BnModelTest, input_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input(0);}, std::invalid_argument );
}

TEST( BnModelTest, input_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input_name(0);}, std::invalid_argument );
}

TEST( BnModelTest, output_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output(0);}, std::invalid_argument );
}

TEST( BnModelTest, output_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output_name(0);}, std::invalid_argument );
}

TEST( BnModelTest, logic_bad )
{
  BnModel model;

  EXPECT_THROW( {model.logic(0);}, std::invalid_argument );
}

TEST( BnModelTest, dff_bad )
{
  BnModel model;

  EXPECT_THROW( {model.dff(0);}, std::invalid_argument );
}

TEST( BnModelTest, cover_bad )
{
  BnModel model;

  EXPECT_THROW( {model.cover(0);}, std::invalid_argument );
}

TEST( BnModelTest, expr_bad )
{
  BnModel model;

  EXPECT_THROW( {model.expr(0);}, std::invalid_argument );
}

TEST( BnModelTest, func_bad )
{
  BnModel model;

  EXPECT_THROW( {model.func(0);}, std::invalid_argument );
}

TEST( BnModelTest, bdd_bad )
{
  BnModel model;

  EXPECT_THROW( {model.bdd(0);}, std::invalid_argument );
}

TEST( BnModelTest, new_input )
{
  BnModel model;

  string name1{"input1"};
  auto node = model.new_input(name1);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_EQ( model, node.parent_model() );
  EXPECT_EQ( name1, node.name() );
}

TEST( BnModelTest, new_primitive)
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  PrimType type = PrimType::And;
  auto node = model.new_primitive(fanin_list, type);

  EXPECT_EQ( BnNodeType::PRIMITIVE, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_TRUE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
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
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
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
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( 2, node.fanin_num() );
  EXPECT_EQ( input1, node.fanin(0) );
  EXPECT_EQ( input2, node.fanin(1) );
  vector<BnNode> fanin_node_list{input1, input2};
  EXPECT_EQ( fanin_node_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_EQ( inv0, node.fanin_inv(0) );
  EXPECT_EQ( inv1, node.fanin_inv(1) );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cover();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnModelTest, new_cover )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  Literal lit1{0, false};
  Literal lit2{1, false};
  vector<vector<Literal>> cube_list{{lit1}, {lit2}};
  SizeType cover_id = model.add_cover(2, cube_list, '0');
  auto node = model.new_cover(fanin_list, cover_id);

  EXPECT_EQ( BnNodeType::COVER, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_TRUE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_EQ( cover_id, node.cover_id() );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr();}, std::invalid_argument );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnModelTest, new_expr )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  vector<BnNode> fanin_list{input1, input2};
  auto lit1 = Expr::make_posi_literal(0);
  auto lit2 = Expr::make_posi_literal(1);
  auto expr = lit1 | lit2;
  SizeType expr_id = model.add_expr(expr);
  auto node = model.new_expr(fanin_list, expr_id);

  EXPECT_EQ( BnNodeType::EXPR, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_TRUE( node.is_expr() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_THROW( {node.input_id();}, std::invalid_argument );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_EQ( expr_id, node.expr_id() );
  EXPECT_EQ( expr, node.expr() );
  EXPECT_THROW( {node.cell_id();}, std::invalid_argument );
  EXPECT_THROW( {node.cell();}, std::invalid_argument );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
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
  auto node = model.new_cell(fanin_list, cell);

  EXPECT_EQ( BnNodeType::CELL, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_TRUE( node.is_logic() );
  EXPECT_FALSE( node.is_primitive() );
  EXPECT_FALSE( node.is_aig() );
  EXPECT_FALSE( node.is_cover() );
  EXPECT_FALSE( node.is_expr() );
  EXPECT_FALSE( node.is_func() );
  EXPECT_FALSE( node.is_bdd() );
  EXPECT_TRUE( node.is_cell() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < node.fanin_num(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_THROW( {node.primitive_type();}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(0);}, std::invalid_argument );
  EXPECT_THROW( {node.fanin_inv(1);}, std::invalid_argument );
  EXPECT_THROW( {node.cover_id();}, std::invalid_argument );
  EXPECT_THROW( {node.expr_id();}, std::invalid_argument );
  EXPECT_EQ( cell.id(), node.cell_id() );
  EXPECT_EQ( cell, node.cell() );
  EXPECT_THROW( {node.func_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.func(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.bdd(); }, std::invalid_argument );
}

TEST( BnModelTest, new_dff)
{
  BnModel model;

  char rsval = '1';
  auto name = "abc";
  auto node = model.new_dff(rsval, name);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( model, node.parent_model() );
  EXPECT_EQ( name, node.name() );
  EXPECT_EQ( BnDffType::DFF, node.type() );
  EXPECT_TRUE( node.is_dff() );
  EXPECT_FALSE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.data_src().is_valid() );
  EXPECT_FALSE( node.clock().is_valid() );
  EXPECT_THROW( {node.enable(); }, std::invalid_argument );
  EXPECT_FALSE( node.clear().is_valid() );
  EXPECT_FALSE( node.preset().is_valid() );
  EXPECT_EQ( rsval, node.rsval() );
  EXPECT_TRUE( node.data_output().is_valid() );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );
}

TEST( BnModelTest, new_latch)
{
  BnModel model;

  auto node = model.new_latch();

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( model, node.parent_model() );
  EXPECT_EQ( string{}, node.name() );
  EXPECT_EQ( BnDffType::LATCH, node.type() );
  EXPECT_FALSE( node.is_dff() );
  EXPECT_TRUE( node.is_latch() );
  EXPECT_FALSE( node.is_cell() );
  EXPECT_FALSE( node.data_src().is_valid() );
  EXPECT_THROW( {node.clock();}, std::invalid_argument );
  EXPECT_FALSE( node.enable().is_valid() );
  EXPECT_FALSE( node.clear().is_valid() );
  EXPECT_FALSE( node.preset().is_valid() );
  EXPECT_EQ( ' ', node.rsval() );
  EXPECT_TRUE( node.data_output().is_valid() );
  EXPECT_THROW( {node.cell_id(); }, std::invalid_argument );
  EXPECT_THROW( {node.cell(); }, std::invalid_argument );
}

TEST( BnModelTest, new_dff_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18DFNP010";
  auto cell = library.cell(CELL_NAME);

  BnModel model;
  auto name = "xyz";
  auto node = model.new_dff_cell(cell, name);

  EXPECT_TRUE( node.is_valid() );
  EXPECT_EQ( model, node.parent_model() );
  EXPECT_EQ( name, node.name() );
  EXPECT_EQ( BnDffType::CELL, node.type() );
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
  EXPECT_EQ( cell.id(), node.cell_id() );
  EXPECT_EQ( cell, node.cell() );
  EXPECT_FALSE( node.cell_pin(0).is_valid() );
  EXPECT_FALSE( node.cell_pin(1).is_valid() );
  EXPECT_FALSE( node.cell_pin(2).is_valid() );
}

END_NAMESPACE_YM_BN
