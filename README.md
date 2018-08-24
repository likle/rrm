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

| name                                                       | type    | language    | os                  | author                                              |
|------------------------------------------------------------|---------|-------------|---------------------|-----------------------------------------------------|
| [trash-cli](https://github.com/andreafrancia/trash-cli)    | tool    | python      | linux               | [@andreafrancia](https://github.com/andreafrancia/) |
| [trashcan](https://github.com/FreeSlave/trashcan)          | library | D           | linux,macOS,windows | [@FreeSlave](https://github.com/FreeSlave/)         |
| [trash-cli](https://github.com/sindresorhus/trash-cli)     | tool    | javascript  | linux,macOS,windows | [@sindresorhus](https://github.com/sindresorhus/)   |
| [trash](https://github.com/sindresorhus/trash)             | library | javascript  | linux,macOS,windows | [@sindresorhus](https://github.com/sindresorhus/)   |
| [macos-trash](https://github.com/sindresorhus/macos-trash) | tool    | swift       | macOS               | [@sindresorhus](https://github.com/sindresorhus/)   |
| [brash](https://gist.github.com/dcparris/2508959)          | tool    | bash        | linux,macOS (?)     | [@dcparris](https://github.com/dcparris/)           |
| [safe-rm](https://github.com/kaelzhang/shell-safe-rm)      | tool    | shell       | linux,macOS         | [@kaelzhang](https://github.com/kaelzhang/)         |
| [rip](https://github.com/nivekuil/rip)                     | tool    | rust        | linux,macOS         | [@nivekuil](https://github.com/nivekuil/)           |
| [rmtrash](https://github.com/PhrozenByte/rmtrash)          | tool    | bash        | linux               | [@PhrozenByte](https://github.com/PhrozenByte/)     |
| [trash](https://github.com/macmade/trash)                  | tool    | swift       | macOS               | [@macmade](https://github.com/macmade/)             |
| [Trashd](https://github.com/FedeDP/Trashd)                 | service | C           | linux               | [@FedeDP](https://github.com/FedeDP/)               |
| [trash](https://github.com/jemtucker/trash)                | tool    | objective-c | macOS               | [@jemtucker](https://github.com/jemtucker/)         |
| [fish-trash](https://github.com/arbourd/fish-trash)        | tool    | shell       | macOS               | [@arbourd](https://github.com/arbourd/)             |
| [trash-cli](https://github.com/wicksome/trash-cli)         | tool    | shell       | linux,macOS         | [@wicksome](https://github.com/wicksome/)           |
| [dustbin-bin](https://github.com/Taguar258/dustbin-bin)    | tool    | shell       | linux,macOS (?)     | [@Taguar258](https://github.com/Taguar258/)         |
| [rubbish](https://github.com/jaxke/rubbish)                | tool    | python      | linux               | [@jaxke](https://github.com/jaxke/)                 |