#! /usr/bin/env python3

"""

:file: read_blif_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2023 Yusuke Matsunaga, All rights reserved.
"""

import pytest
import os
import os.path
from ymbnfe import BfModel


def test_read_blif():
    filename = "s5378.blif"
    DATAPATH = os.environ.get('TESTDATA_DIR')
    path = os.path.join(DATAPATH, filename)

    model = BfModel.read_blif(path)
    
