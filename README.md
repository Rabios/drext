# drext

C Extensions i made for [DragonRuby](https://dragonruby.org)

> NOTE: DragonRuby Pro required for C Extensions.

## List

| Name   | Description               | Platforms                     |
|--------|---------------------------|-------------------------------|
| drbat  | Battery information library | Windows, OSX, Linux         |
| drclip | native clipboard support library | Windows, OSX, iOS      |
| drcpu  | CPU information library | Windows, OSX, iOS, Linux, PS4   |
| drram  | RAM information library | Windows, OSX, iOS, Linux        |

## API

```rb
# drbat (Windows, OSX, Linux)

drbat.is_charging   # Is Battery charging?
drbat.sys_state     # Internal system state of Battery, Differs depending on OS
drbat.percent       # Percentage of Battery (From 0 to 100)
drbat.life_time     # Lifetime of Battery

# drclip (Windows, OSX, iOS)

drclip_init         # Initializes Clipboard, Returns 1 on success or 0 on error
drclip_get          # Returns string from Clipboard
drclip_set(text)    # Sets Clipboard string to text
drclip_clear        # Clears Clipboard
drclip_uninit       # Deinitializes the Clipboard, Returns 1 on success or 0 on error

# drcpu (Windows, OSX, iOS, Linux, PS4)

drcpu.cores         # Returns CPU cores
drcpu.name          # Returns CPU name
drcpu_free(stats)   # Frees stats, You get it by simply call drcpu

# drram (Windows, OSX, iOS, Linux)

drram.total         # Returns total RAM available (In bytes)
drram.free          # Returns free RAM available (In bytes)
```

> NOTE: The examples provide full usage of the API, Check out `examples` to know how to work with the extensions!

## Building

For each C file in `src` simply do:

```sh
clang -shared <file>.c --sysroot=<your_mingw64_path> --target=<target> -fuse-ld=lld -isystem <dragonruby_pro_include_path> -I. -o <shared_library>
```

> NOTE 1: Each C file has thing you need to link, Check out the C file before compilation...
> NOTE 2: It's encouraged to check out DragonRuby Pro's C extensions guide to know how to compile the extension correctly...

## License (Public Domain)

```
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
```
