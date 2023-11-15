#! /usr/bin/env python3

""" BnModel のテストプログラム

:file: BnModel_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymbn import BnModel


def test_constructor1():
    model = BnModel()

    assert not model.library.is_valid()
    assert model.name == ""
    assert model.comment == ""
    assert model.node_num == 0
    assert model.input_num == 0
    assert len(model.input_list) == 0
    assert model.output_num == 0
    assert len(model.output_list) == 0
    assert model.logic_num == 0
    assert len(model.logic_list) == 0
    assert model.seq_num == 0
    assert model.func_num == 0

def test_constructor_bad():

    with pytest.raises(TypeError) as e:
        _ = BnModel(1)

def test_input_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.input(0)

def test_output_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.output(0)
    
def test_logic_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.logic(0)

def test_seq_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.input(0)

def test_func_bad():
    model = BnModel()

    with pytest.raises(ValueError) as e:
        _ = model.func(0)

