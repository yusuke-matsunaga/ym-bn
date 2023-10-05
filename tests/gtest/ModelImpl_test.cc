
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
  EXPECT_EQ( BnNodeType::None, node.type() );
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

TEST( ModelImplTest, set_input )
{
  ModelImpl model;

  auto id = model.new_node({});
  EXPECT_EQ( 0, id );

  model.set_input(id);

  auto& node = model.node(id);
  EXPECT_EQ( BnNodeType::Input, node.type() );
}

TEST( ModelImplTest, set_output )
{
  ModelImpl model;

  auto id = model.new_node({});
  EXPECT_EQ( 0, id );

  model.set_output(id);

  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( id, model.output(0) );
}

TEST( ModelImplTest, set_primitive )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  PrimType type = PrimType::Xor;
  model.set_primitive(id3, fanin_list, type);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Prim, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( type, node.primitive_type() );
}

TEST( ModelImplTest, set_aig )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  bool inv1 = true;
  bool inv2 = false;
  model.set_aig(id3, id1, id2, inv1, inv2);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Aig, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( inv1, node.fanin_inv(0) );
  EXPECT_EQ( inv2, node.fanin_inv(1) );
}

TEST( ModelImplTest, set_cover )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  SizeType cover_id = 3;
  model.set_cover(id3, fanin_list, cover_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Cover, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( cover_id, node.cover_id() );
}

TEST( ModelImplTest, set_expr )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  SizeType expr_id = 3;
  model.set_expr(id3, fanin_list, expr_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Expr, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( expr_id, node.expr_id() );
}

TEST( ModelImplTest, set_cell )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  SizeType cell_id = 3;
  model.set_cell(id3, fanin_list, cell_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Cell, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( cell_id, node.cell_id() );
}

TEST( ModelImplTest, set_func )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  SizeType func_id = 3;
  model.set_func(id3, fanin_list, func_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::TvFunc, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( func_id, node.func_id() );
}

TEST( ModelImplTest, set_bdd )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id2 = model.new_node({});
  EXPECT_EQ( 1, id2 );
  model.set_input(id2);

  auto id3 = model.new_node({});
  EXPECT_EQ( 2, id3 );

  vector<SizeType> fanin_list{id1, id2};
  SizeType bdd_id = 3;
  model.set_bdd(id3, fanin_list, bdd_id);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Bdd, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list() );
  EXPECT_EQ( bdd_id, node.bdd_id() );
}

TEST( ModelImplTest, set_dff )
{
  ModelImpl model;

  auto id1 = model.new_node({});
  EXPECT_EQ( 0, id1 );
  model.set_input(id1);

  auto id3 = model.new_node({});
  EXPECT_EQ( 1, id3 );

  char rval = '1';
  model.set_dff(id3, id1, rval);

  auto& node = model.node(id3);
  EXPECT_EQ( BnNodeType::Dff, node.type() );
  EXPECT_EQ( id1, node.dff_src() );
  EXPECT_EQ( rval, node.dff_rval() );
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
