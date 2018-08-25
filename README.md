[![build status](https://travis-ci.org/likle/rrm.svg?branch=master)](https://travis-ci.org/likle/rrm) 
[![codecov](https://codecov.io/gh/likle/rrm/branch/master/graph/badge.svg)](https://codecov.io/gh/likle/rrm)
![status](https://img.shields.io/badge/status-not_released-red.svg?longCache=true&style=flat)

# rrm - trash utility
The ``rrm`` (**r**ecoverable **r**e**m**ove) utility is a simple tool which can be 
used as a replacement for rm. While rm permanently deletes all files, ``rrm`` 
will be able to recover those files.
    
## WARNING
This is a work in progress and not yet ready to use.

## Usage
```
Usage: rrm [OPTION]... [FILE]...
   or: rrm [OPTION]... -l
   or: rrm [OPTION]... -u [DUMP]...
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
There are many alternatives. Pick the one you like most.

| name                                                       | type    | language    | os                  | author                                              | commit activity                                                                                  |
|------------------------------------------------------------|---------|-------------|---------------------|-----------------------------------------------------|--------------------------------------------------------------------------------------------------|
| [trash-cli](https://github.com/andreafrancia/trash-cli)    | tool    | python      | linux               | [@andreafrancia](https://github.com/andreafrancia/) | ![commit activity](https://img.shields.io/github/commit-activity/y/andreafrancia/trash-cli.svg)  |
| [trashcan](https://github.com/FreeSlave/trashcan)          | library | D           | linux,macOS,windows | [@FreeSlave](https://github.com/FreeSlave/)         | ![commit activity](https://img.shields.io/github/commit-activity/y/FreeSlave/trashcan.svg)       |
| [trash-cli](https://github.com/sindresorhus/trash-cli)     | tool    | javascript  | linux,macOS,windows | [@sindresorhus](https://github.com/sindresorhus/)   | ![commit activity](https://img.shields.io/github/commit-activity/y/sindresorhus/trash-cli.svg)   |
| [trash](https://github.com/sindresorhus/trash)             | library | javascript  | linux,macOS,windows | [@sindresorhus](https://github.com/sindresorhus/)   | ![commit activity](https://img.shields.io/github/commit-activity/y/sindresorhus/trash.svg)       |
| [macos-trash](https://github.com/sindresorhus/macos-trash) | tool    | swift       | macOS               | [@sindresorhus](https://github.com/sindresorhus/)   | ![commit activity](https://img.shields.io/github/commit-activity/y/sindresorhus/macos-trash.svg) |
| [safe-rm](https://github.com/kaelzhang/shell-safe-rm)      | tool    | shell       | linux,macOS         | [@kaelzhang](https://github.com/kaelzhang/)         | ![commit activity](https://img.shields.io/github/commit-activity/y/kaelzhang/shell-safe-rm.svg)  |
| [rip](https://github.com/nivekuil/rip)                     | tool    | rust        | linux,macOS         | [@nivekuil](https://github.com/nivekuil/)           | ![commit activity](https://img.shields.io/github/commit-activity/y/nivekuil/rip.svg)             |
| [rmtrash](https://github.com/PhrozenByte/rmtrash)          | tool    | bash        | linux               | [@PhrozenByte](https://github.com/PhrozenByte/)     | ![commit activity](https://img.shields.io/github/commit-activity/y/PhrozenByte/rmtrash.svg)      |
| [trash](https://github.com/macmade/trash)                  | tool    | swift       | macOS               | [@macmade](https://github.com/macmade/)             | ![commit activity](https://img.shields.io/github/commit-activity/y/macmade/trash.svg)            |
| [Trashd](https://github.com/FedeDP/Trashd)                 | service | C           | linux               | [@FedeDP](https://github.com/FedeDP/)               | ![commit activity](https://img.shields.io/github/commit-activity/y/FedeDP/Trashd.svg)            |
| [trash](https://github.com/jemtucker/trash)                | tool    | objective-c | macOS               | [@jemtucker](https://github.com/jemtucker/)         | ![commit activity](https://img.shields.io/github/commit-activity/y/jemtucker/trash.svg)          |
| [fish-trash](https://github.com/arbourd/fish-trash)        | tool    | shell       | macOS               | [@arbourd](https://github.com/arbourd/)             | ![commit activity](https://img.shields.io/github/commit-activity/y/arbourd/fish-trash.svg)       |
| [trash-cli](https://github.com/wicksome/trash-cli)         | tool    | shell       | linux,macOS         | [@wicksome](https://github.com/wicksome/)           | ![commit activity](https://img.shields.io/github/commit-activity/y/wicksome/trash-cli.svg)       |
| [dustbin-bin](https://github.com/Taguar258/dustbin-bin)    | tool    | shell       | linux,macOS (?)     | [@Taguar258](https://github.com/Taguar258/)         | ![commit activity](https://img.shields.io/github/commit-activity/y/Taguar258/dustbin-bin.svg)    |
| [rubbish](https://github.com/jaxke/rubbish)                | tool    | Python      | linux               | [@jaxke](https://github.com/jaxke/)                 | ![commit activity](https://img.shields.io/github/commit-activity/y/jaxke/rubbish.svg)            |
| [softrm](https://github.com/danielmanesku/softrm)          | tool    | Go          | linux               | [@danielmanesku](https://github.com/danielmanesku/) | ![commit activity](https://img.shields.io/github/commit-activity/y/danielmanesku/softrm.svg)     |
| [brash](https://gist.github.com/dcparris/2508959)          | tool    | bash        | linux,macOS (?)     | [@dcparris](https://github.com/dcparris/)           | ![commit activity](https://img.shields.io/badge/commit%20activity-none-red.svg)                  |
