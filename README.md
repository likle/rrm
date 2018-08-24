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
   or: tsh [OPTION]... -l
   or: tsh [OPTION]... -u [DUMP]...
Moves files to the trash, recovers or lists dumps.

  -f, --force          ignore non-existent files and arguments, never prompt
  -r, -R, --recursive  remove directories and their contents recursively
  -d                   remove empty directories
  -u, --undo           recovers previously deleted dumps
  --output=DIR         specifies an output directory for recovery
  -l, --list           lists previously deleted dumps
  -v, --verbose        explain what is being done
  --help               display this help and exit
  --version            output version info and exit

Each delete is kept in a separate trash dump. The --list flag can be used to
receive DUMP information about previous delete operations operations. Use the
--undo flag to recover the files of a dump. If no --output is specified in
combination with --undo, the original file path will be used.
```

## Alternatives
There are many alternatives to ``tsh``. Here are some listed:
* The Python tool [trash-cli](https://github.com/andreafrancia/trash-cli) by @andreafrancia
* The D library [trashcan](https://github.com/FreeSlave/trashcan) by @FreeSlave
* The JavaScript tool [trash-cli](https://github.com/sindresorhus/trash-cli) by @sindresorhus
* The JavaScript library [trash](https://github.com/sindresorhus/trash) by @sindresorhus
* The Swift tool [macos-trash](https://github.com/sindresorhus/macos-trash) by @sindresorhus
* The Bash script [brash](https://gist.github.com/dcparris/2508959) by @dcparris
* The Shell script [safe-rm](https://github.com/kaelzhang/shell-safe-rm) by @kaelzhang
* The Rust tool [rip](https://github.com/nivekuil/rip) by nivekuil
* The Bash script [rmtrash](https://github.com/PhrozenByte/rmtrash) by PhrozenByte
* The Swift tool [trash](https://github.com/macmade/trash) by macmade
* The Linux Service [Trashd](https://github.com/FedeDP/Trashd) by @FedeDP
* The Objective-C tool [trash](https://github.com/jemtucker/trash) by @jemtucker
* The Python library [pytrash](https://github.com/nioperas06/pytrash) by @nioperas06
* The Shell tool [fish-trash](https://github.com/arbourd/fish-trash) by @arbourd
* The Shell tool [Silent-Trash](https://github.com/JayBrown/Silent-Trash) by @JayBrown
* The Shell tool [trash-cli](https://github.com/wicksome/trash-cli) by @wicksome
* The Shell tool [dustbin-bin](https://github.com/Taguar258/dustbin-bin) by @Taguar258
* The Python tool [rubbish](https://github.com/jaxke/rubbish) by @jaxke