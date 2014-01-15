journal
=======

The UNIX user's personal log and diary.

## About

The `journal` project is (will be) a proper command line utility for
UNIX systems to produce static HTML weblogs (aka "_blogs_") from
[Markdown](http://daringfireball.net/projects/markdown/) formatted text 
documents.  All this while staying true to proper UNIX (not GNU) 
design-conventions.

## Installation

**Don't** even try, it's not even finished yet.

However, when it is finished:

`make install clean`

That's it, you don't even have to run clean if you don't want to.

### Prerequistites

* A UNIX system, such as FreeBSD or OS X (probably works in various
  flavours of linux too, I haven't tested).

* `gmake` because it's ubiquitus and I have some foo in the `Makefile`
  that isn't cross-make compatible right now.  I'll fix it, I promise.

* A decent compiler.  I recommend `clang` from `llvm`, but it probably
  compiles under `gcc` or BSD `cc` just fine.

## Usage

To be determined.  It will probably be something along the lines of:

```
usage: journal [-options] <command> [<argument> ...]
```

## Copyright

Copyright (c) 2014, xles, see [LICENSE](LICENSE) for details.
