
/// @file ModelImpl_test.cc
/// @brief ModelImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ModelImpl.h"
#include "ym/SopCover.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_BN

TEST( ModelImplTest, constructor1 )
{
  ModelImpl model;

  EXPECT_EQ( string{}, model.name() );
  EXPECT_EQ( string{}, model.comment() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.seq_num() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.func_num() );
}

TEST( ModelImplTest, set_library )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);

  ModelImpl model;

  model.set_library(library);

  EXPECT_EQ( library, model.library() );
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

TEST( ModelImplTest, set_output_name )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto oid = model.new_output(id1);
  string name{"abc"};

  model.set_output_name(oid, name);

  EXPECT_EQ( name, model.output_name(oid) );
}

TEST( ModelImplTest, alloc_node )
{
  ModelImpl model;

  auto id = model.alloc_node();
  EXPECT_EQ( 0, id );

  auto node = model.node_impl(id);
  EXPECT_EQ( nullptr, node );
}

TEST( ModelImplTest, new_input )
{
  ModelImpl model;

  auto id = model.new_input();
  auto node = model.node_impl(id);
  EXPECT_EQ( BnNodeType::INPUT, node->type() );
}

TEST( ModelImplTest, new_seq_output )
{
  ModelImpl model;

  auto seq_id = model.new_dff();
  auto id = model.new_seq_output(seq_id);
  auto node = model.node_impl(id);
  EXPECT_EQ( BnNodeType::SEQ_OUTPUT, node->type() );
}

TEST( ModelImplTest, new_output )
{
  ModelImpl model;

  auto id = model.new_input();

  auto oid = model.new_output(id);

  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( id, model.output_id(oid) );
}

TEST( ModelImplTest, new_primitive )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  PrimType type = PrimType::Xor;
  auto id3 = model.new_primitive(fanin_list, type);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::PRIMITIVE, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( type, node->primitive_type() );
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

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::AIG, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( inv1, node->fanin_inv(0) );
  EXPECT_EQ( inv2, node->fanin_inv(1) );
}

TEST( ModelImplTest, new_func )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  SizeType func_id = 3;
  auto id3 = model.new_func(fanin_list, func_id);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::FUNC, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( func_id, node->local_func_id() );
}

TEST( ModelImplTest, new_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18NAND2P005";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  vector<SizeType> fanin_list{id1, id2};
  auto id3 = model.new_cell(fanin_list, cell);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::CELL, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( cell, node->cell() );
}

TEST( ModelImplTest, new_dff )
{
  ModelImpl model;

  auto src_id = model.new_input();
  auto clock_id = model.new_input();
  auto clear_id = model.new_input();
  auto preset_id = model.new_input();

  char rsval = '1';
  auto id0 = model.new_dff(rsval);
  model.set_clock(id0, clock_id);
  model.set_clear(id0, clear_id);
  model.set_preset(id0, preset_id);
  model.set_data_src(id0, src_id);

  auto& seq = model.seq_impl(id0);
  EXPECT_EQ( BnSeqType::DFF, seq.type() );
  EXPECT_EQ( src_id, seq.data_src() );
  EXPECT_EQ( clock_id, seq.clock() );
  EXPECT_THROW( {seq.enable();}, std::invalid_argument );
  EXPECT_EQ( clear_id, seq.clear() );
  EXPECT_EQ( preset_id, seq.preset() );
  EXPECT_EQ( rsval, seq.rsval() );
}

TEST( ModelImplTest, new_latch )
{
  ModelImpl model;

  auto src_id = model.new_input();
  auto enable_id = model.new_input();
  auto clear_id = model.new_input();

  char rsval = 'X';
  auto id0 = model.new_latch(rsval);
  model.set_data_src(id0, src_id);
  model.set_enable(id0, enable_id);
  model.set_clear(id0, clear_id);

  auto& seq = model.seq_impl(id0);
  EXPECT_EQ( BnSeqType::LATCH, seq.type() );
  EXPECT_EQ( src_id, seq.data_src() );
  EXPECT_THROW( {seq.clock();}, std::invalid_argument );
  EXPECT_EQ( enable_id, seq.enable() );
  EXPECT_EQ( clear_id, seq.clear() );
  EXPECT_EQ( BAD_ID, seq.preset() );
  EXPECT_EQ( rsval, seq.rsval() );
}

TEST( ModelImplTest, new_seq_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18DFNP010";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto src_id = model.new_input();
  auto clock_id = model.new_input();
  auto output_id = model.new_input();

  auto id0 = model.new_seq_cell(cell);
  model.set_seq_pin(id0, 0, clock_id);
  model.set_seq_pin(id0, 1, src_id);
  model.set_seq_pin(id0, 2, output_id);

  auto& seq = model.seq_impl(id0);
  EXPECT_EQ( BnSeqType::CELL, seq.type() );
  EXPECT_THROW( {seq.data_src();}, std::invalid_argument );
  EXPECT_THROW( {seq.clock();}, std::invalid_argument );
  EXPECT_THROW( {seq.enable();}, std::invalid_argument );
  EXPECT_THROW( {seq.clear();}, std::invalid_argument );
  EXPECT_THROW( {seq.preset();}, std::invalid_argument );
  EXPECT_THROW( {seq.rsval();}, std::invalid_argument );
  EXPECT_EQ( clock_id, seq.cell_pin(0) );
  EXPECT_EQ( src_id, seq.cell_pin(1) );
  EXPECT_EQ( output_id, seq.cell_pin(2) );
}

TEST( ModelImplTest, set_input )
{
  ModelImpl model;

  auto id = model.alloc_node();
  model.set_input(id);
  auto node = model.node_impl(id);
  EXPECT_EQ( BnNodeType::INPUT, node->type() );
}

TEST( ModelImplTest, set_primitive )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  auto id3 = model.alloc_node();
  vector<SizeType> fanin_list{id1, id2};
  PrimType type = PrimType::Xor;
  model.set_primitive(id3, fanin_list, type);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::PRIMITIVE, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( type, node->primitive_type() );
}

TEST( ModelImplTest, set_aig )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  auto id3 = model.alloc_node();
  vector<SizeType> fanin_list{id1, id2};
  bool inv1 = true;
  bool inv2 = false;
  model.set_aig(id3, id1, id2, inv1, inv2);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::AIG, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( inv1, node->fanin_inv(0) );
  EXPECT_EQ( inv2, node->fanin_inv(1) );
}

TEST( ModelImplTest, set_func )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  auto id3 = model.alloc_node();
  vector<SizeType> fanin_list{id1, id2};
  SizeType func_id = 3;
  model.set_func(id3, fanin_list, func_id);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::FUNC, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( func_id, node->local_func_id() );
}

TEST( ModelImplTest, set_cell )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18NAND2P005";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  auto id3 = model.alloc_node();
  vector<SizeType> fanin_list{id1, id2};
  model.set_cell(id3, fanin_list, cell);

  auto node = model.node_impl(id3);
  EXPECT_EQ( BnNodeType::CELL, node->type() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
  EXPECT_EQ( cell, node->cell() );
}

TEST( ModelImplTest, set_seq_name )
{
  ModelImpl model;

  auto id = model.new_dff();
  string name{"abcd"};

  model.set_seq_name(id, name);

  EXPECT_EQ( name, model.seq_name(id) );
}

TEST( ModelImplTest, set_data_src_dff )
{
  ModelImpl model;

  auto id = model.new_dff();
  auto src_id = model.new_input();
  model.set_data_src(id, src_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( src_id, seq.data_src() );
}

TEST( ModelImplTest, set_clock )
{
  ModelImpl model;

  auto id = model.new_dff();
  auto clock_id = model.new_input();
  model.set_clock(id, clock_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( clock_id, seq.clock() );
}

TEST( ModelImplTest, set_enable )
{
  ModelImpl model;

  auto id = model.new_latch();
  auto enable_id = model.new_input();
  model.set_enable(id, enable_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( enable_id, seq.enable() );
}

TEST( ModelImplTest, set_clear_dff )
{
  ModelImpl model;

  auto id = model.new_dff();
  auto clear_id = model.new_input();
  model.set_clear(id, clear_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( clear_id, seq.clear() );
}

TEST( ModelImplTest, set_clear_latch )
{
  ModelImpl model;

  auto id = model.new_latch();
  auto clear_id = model.new_input();
  model.set_clear(id, clear_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( clear_id, seq.clear() );
}

TEST( ModelImplTest, set_preset_dff )
{
  ModelImpl model;

  auto id = model.new_dff();
  auto preset_id = model.new_input();
  model.set_preset(id, preset_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( preset_id, seq.preset() );
}

TEST( ModelImplTest, set_preset_latch )
{
  ModelImpl model;

  auto id = model.new_latch();
  auto preset_id = model.new_input();
  model.set_preset(id, preset_id);
  auto& seq = model.seq_impl(id);
  EXPECT_EQ( preset_id, seq.preset() );
}

TEST( ModelImplTest, set_seq_pin )
{
  auto FILENAME = string{DATAPATH} + string{"/HIT018.typ.snp"};
  auto library = ClibCellLibrary::read_liberty(FILENAME);
  auto CELL_NAME = "HIT18DFNP010";
  auto cell = library.cell(CELL_NAME);

  ModelImpl model;

  auto id = model.new_seq_cell(cell);
  auto clock = model.new_input();
  auto src = model.new_input();
  auto output = model.new_input();
  model.set_seq_pin(id, 0, clock);
  model.set_seq_pin(id, 1, src);
  model.set_seq_pin(id, 2, output);

  auto& seq = model.seq_impl(id);
  EXPECT_EQ( clock, seq.cell_pin(0) );
  EXPECT_EQ( src, seq.cell_pin(1) );
  EXPECT_EQ( output, seq.cell_pin(2) );
}

TEST( ModelImplTest, reg_cover )
{
  ModelImpl model;

  SizeType ni = 2;
  Literal lit0{0, false};
  Literal lit1{1, false};

  vector<vector<Literal>> cube_list{{lit0}, {lit1}};
  char opat = '1';
  auto id1 = model.reg_cover(ni, cube_list, opat);

  auto& func = model.func_impl(id1);
  EXPECT_EQ( ni, func.input_num() );
  auto& cover = func.input_cover();
  EXPECT_EQ( cube_list.size(), cover.cube_num() );
  EXPECT_EQ( SopPat::_1, cover.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_X, cover.get_pat(0, 1) );
  EXPECT_EQ( SopPat::_X, cover.get_pat(1, 0) );
  EXPECT_EQ( SopPat::_1, cover.get_pat(1, 1) );
  EXPECT_EQ( opat, func.output_pat() );

  vector<vector<Literal>> cube_list2{{lit0, lit1}};
  char opat2 = '0';
  auto id2 = model.reg_cover(ni, cube_list2, opat2);

  EXPECT_NE( id1, id2 );

  auto& func2 = model.func_impl(id2);
  EXPECT_EQ( ni, func2.input_num() );
  auto& cover2 = func2.input_cover();
  EXPECT_EQ( cube_list2.size(), cover2.cube_num() );
  EXPECT_EQ( SopPat::_1, cover2.get_pat(0, 0) );
  EXPECT_EQ( SopPat::_1, cover2.get_pat(0, 1) );
  EXPECT_EQ( opat2, func2.output_pat() );
}

TEST( ModelImplTest, reg_expr )
{
  ModelImpl model;

  auto expr = Expr::from_rep_string("A2P0N1");
  auto id = model.reg_expr(expr);

  EXPECT_EQ( expr, model.func_impl(id).expr() );
}

TEST( ModelImplTest, reg_tvfunc )
{
  ModelImpl model;

  auto func1 = TvFunc::make_zero(4);
  auto id1 = model.reg_tvfunc(func1);
  auto func2 = TvFunc{2, vector<int>{0, 1, 1, 0}};
  auto id2 = model.reg_tvfunc(func2);

  EXPECT_EQ( func1, model.func_impl(id1).tvfunc() );
  EXPECT_EQ( func2, model.func_impl(id2).tvfunc() );
}

END_NAMESPACE_YM_BN
