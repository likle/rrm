[![build status](https://travis-ci.org/likle/tsh.svg?branch=master)](https://travis-ci.org/likle/tsh) 
[![codecov](https://codecov.io/gh/likle/tsh/branch/master/graph/badge.svg)](https://codecov.io/gh/likle/tsh)
![status](https://img.shields.io/badge/status-not_released-red.svg?longCache=true&style=flat)

# tsh - trash utility
The ``tsh`` utility is a simple tool which can be used as a replacement for rm.
While rm permanently deletes all files, ``tsh`` will just move them it to a trash.

## WARNING
This is a work in progress and not yet ready to use.

## Examples
A couple of examples:
```bash
# delete all files from the working directory
tsh *
```

```bash
# delete all files and directories
tsh -r *
```

```bash
# delete an empty folder
tsh -d myfolder
```


## Usage
```
Usage: tsh [OPTION]... [FILE]...
Moves the files to the trash.

  -f, --force          ignore non-existent files and arguments, never prompt
  -r, -R, --recursive  remove directories and their contents recursively
  -d                   remove empty directories
  --version            output version info and exit
  --help               display this help and exit

By default, tsh does not remove directories. Use the --recursive
option to remove each listed directory too, along with all of its
```