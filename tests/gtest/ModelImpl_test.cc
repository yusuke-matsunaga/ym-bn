
/// @file ModelImpl_test.cc
/// @brief ModelImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( ModelImplTest, constructor1 )
{
  ModelImpl model;

  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.dff_num() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.cover_num() );
  EXPECT_EQ( 0, model.expr_num() );
}

TEST( ModelImplTest, set_name )
{
  ModelImpl model;

  string name{"abcd"};
  model.set_name(name);

  EXPECT_EQ( name, model.name() );
}

TEST( ModelImplTest, set_comment )
{
  ModelImpl model;

  string comment{"abcd"};
  model.set_comment(comment);

  EXPECT_EQ( comment, model.comment() );
}

TEST( ModelImplTest, new_node )
{
  ModelImpl model;

  auto id = model.new_node({});
  EXPECT_EQ( 0, id );

  auto& node = model.node(id);
  EXPECT_EQ( BnNodeType::NONE, node.type() );
}

TEST( ModelImplTest, set_node_name )
{
  ModelImpl model;

  auto id = model.new_node({});
  EXPECT_EQ( 0, id );

  string name{"xyz"};
  model.set_node_name(id, name);

  auto& node = model.node(id);
  EXPECT_EQ( name, node.name() );
}

TEST( ModelImplTest, new_input )
{
  ModelImpl model;

  auto id = model.new_input({});
  auto& node = model.node(id);
  EXPECT_EQ( BnNodeType::INPUT, node.type() );
}

TEST( ModelImplTest, set_output )
{
  ModelImpl model;

  auto id = model.new_node({});
  EXPECT_EQ( 0, id );

  model.new_output(id);

  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( id, model.output(0) );
}

TEST( ModelImplTest, new_primitive )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  PrimType type = PrimType::Xor;
  auto id3 = model.new_primitive(fanin_list, type);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::PRIMITIVE, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( type, node.primitive_type() );
}

TEST( ModelImplTest, new_aig )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  bool inv1 = true;
  bool inv2 = false;
  auto id3 = model.new_aig(id1, id2, inv1, inv2);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::AIG, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( inv1, node.fanin_inv(0) );
  EXPECT_EQ( inv2, node.fanin_inv(1) );
}

TEST( ModelImplTest, new_cover )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  SizeType cover_id = 3;
  auto id3 = model.new_cover(fanin_list, cover_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::COVER, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( cover_id, node.cover_id() );
}

TEST( ModelImplTest, new_expr )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  SizeType expr_id = 3;
  auto id3 = model.new_expr(fanin_list, expr_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::EXPR, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( expr_id, node.expr_id() );
}

TEST( ModelImplTest, new_func )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  SizeType func_id = 3;
  auto id3 = model.new_func(fanin_list, func_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::TVFUNC, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( func_id, node.func_id() );
}

TEST( ModelImplTest, new_bdd )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  SizeType bdd_id = 3;
  auto id3 = model.new_bdd(fanin_list, bdd_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::BDD, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( bdd_id, node.bdd_id() );
}

TEST( ModelImplTest, new_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18NAND2P005";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto id1 = model.new_input({});
  auto id2 = model.new_input({});
  vector<SizeType> fanin_list{id1, id2};
  auto id3 = model.new_cell(fanin_list, cell);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::CELL, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( cell.id(), node.cell_id() );
}

TEST( ModelImplTest, new_dff )
{
  ModelImpl model;

  auto src_id = model.new_node({});
  auto clock_id = model.new_node({});
  auto clear_id = model.new_node({});
  auto preset_id = model.new_node({});

  char rsval = '1';
  auto id0 = model.new_dff(rsval);
  model.set_data_src(id0, src_id);
  model.set_clock(id0, clock_id);
  model.set_clear(id0, clear_id);
  model.set_preset(id0, preset_id);

  auto& dff = model.dff(id0);
  EXPECT_EQ( BnDffType::DFF, dff.type() );
  EXPECT_EQ( src_id, dff.data_src() );
  EXPECT_EQ( clock_id, dff.clock() );
  EXPECT_THROW( {dff.enable();}, std::invalid_argument );
  EXPECT_EQ( clear_id, dff.clear() );
  EXPECT_EQ( preset_id, dff.preset() );
  EXPECT_EQ( rsval, dff.rsval() );
}

TEST( ModelImplTest, new_latch )
{
  ModelImpl model;

  auto src_id = model.new_node({});
  auto enable_id = model.new_node({});
  auto clear_id = model.new_node({});

  char rsval = 'X';
  auto id0 = model.new_latch(rsval);
  model.set_data_src(id0, src_id);
  model.set_enable(id0, enable_id);
  model.set_clear(id0, clear_id);

  auto& dff = model.dff(id0);
  EXPECT_EQ( BnDffType::LATCH, dff.type() );
  EXPECT_EQ( src_id, dff.data_src() );
  EXPECT_THROW( {dff.clock();}, std::invalid_argument );
  EXPECT_EQ( enable_id, dff.enable() );
  EXPECT_EQ( clear_id, dff.clear() );
  EXPECT_EQ( BAD_ID, dff.preset() );
  EXPECT_EQ( rsval, dff.rsval() );
}

TEST( ModelImplTest, new_dff_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18DFNP010";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto src_id = model.new_node({});
  auto clock_id = model.new_node({});
  auto output_id = model.new_input({});

  auto id0 = model.new_dff_cell(cell);
  model.set_dff_pin(id0, 0, clock_id);
  model.set_dff_pin(id0, 1, src_id);
  model.set_dff_pin(id0, 2, output_id);

  auto& dff = model.dff(id0);
  EXPECT_EQ( BnDffType::CELL, dff.type() );
  EXPECT_THROW( {dff.data_src();}, std::invalid_argument );
  EXPECT_THROW( {dff.clock();}, std::invalid_argument );
  EXPECT_THROW( {dff.enable();}, std::invalid_argument );
  EXPECT_THROW( {dff.clear();}, std::invalid_argument );
  EXPECT_THROW( {dff.preset();}, std::invalid_argument );
  EXPECT_THROW( {dff.rsval();}, std::invalid_argument );
  EXPECT_EQ( clock_id, dff.cell_pin(0) );
  EXPECT_EQ( src_id, dff.cell_pin(1) );
  EXPECT_EQ( output_id, dff.cell_pin(2) );
}

TEST( ModelImplTest, add_cover )
{
  ModelImpl model;

  SizeType ni = 2;
  Literal lit0{0, false};
  Literal lit1{1, false};

  vector<vector<Literal>> cube_list{{lit0}, {lit1}};
  char opat = '1';
  auto id1 = model.add_cover(ni, cube_list, opat);

  auto& cover = model.cover(id1);
  EXPECT_EQ( ni, cover.input_num() );
  EXPECT_EQ( cube_list.size(), cover.cube_num() );
  EXPECT_EQ( SopPat::_1, cover.input_pat(0, 0) );
  EXPECT_EQ( SopPat::_X, cover.input_pat(0, 1) );
  EXPECT_EQ( SopPat::_X, cover.input_pat(1, 0) );
  EXPECT_EQ( SopPat::_1, cover.input_pat(1, 1) );
  EXPECT_EQ( opat, cover.output_pat() );

  vector<vector<Literal>> cube_list2{{lit0, lit1}};
  char opat2 = '0';
  auto id2 = model.add_cover(ni, cube_list2, opat2);

  EXPECT_NE( id1, id2 );

  auto& cover2 = model.cover(id2);
  EXPECT_EQ( ni, cover2.input_num() );
  EXPECT_EQ( cube_list2.size(), cover2.cube_num() );
  EXPECT_EQ( SopPat::_1, cover2.input_pat(0, 0) );
  EXPECT_EQ( SopPat::_1, cover2.input_pat(0, 1) );
  EXPECT_EQ( opat2, cover2.output_pat() );
}

TEST( ModelImplTest, add_expr )
{
  ModelImpl model;

  auto expr = Expr::from_rep_string("A2P0N1");
  auto id = model.add_expr(expr);

  EXPECT_EQ( expr, model.expr(id) );
}

TEST( ModelImplTest, add_func )
{
  ModelImpl model;

  auto func1 = TvFunc::make_zero(4);
  auto id1 = model.add_func(func1);
  auto func2 = TvFunc{2, vector<int>{0, 1, 1, 0}};
  auto id2 = model.add_func(func2);

  EXPECT_EQ( func1, model.func(id1) );
  EXPECT_EQ( func2, model.func(id2) );
}

END_NAMESPACE_YM_BN
