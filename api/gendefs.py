#! /usr/bin/env python

import os
import re
import sys
import optparse
import fnmatch

from mpi.module import Module
from mpi.defswriter import Writer

for arg in sys.argv[1:]:
    mod = Module.from_xml(arg)
    Writer(sys.stdout).write(mod)