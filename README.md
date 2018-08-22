[![Build Status](https://travis-ci.org/likle/tsh.svg?branch=master)](https://travis-ci.org/likle/tsh) 
[![codecov](https://codecov.io/gh/likle/tsh/branch/master/graph/badge.svg)](https://codecov.io/gh/likle/tsh)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://choosealicense.com/licenses/mit/)

# tsh - trash utility
The ``tsh`` utility is a simple tool which can be used as a replacement for rm.
While rm permanently deletes all files, ``tsh`` will just move them it to a trash.

## Warning
This is a work in progress and the tool is not yet ready to use.

## Examples
A couple of examples:
```bash
# delete all files from the working directory
tsh *

# delete all files and directories
tsh -r *

# delete an empty folder
tsh -d myfolder
```