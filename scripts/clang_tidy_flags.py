#!/usr/bin/env python3
"""
Extract relevant compile flags for a single source file from compile_commands.json
Prints a space-separated list of -extra-arg=... tokens suitable for passing to clang-tidy.

Usage: scripts/clang_tidy_flags.py <source-file> <build-dir>
"""
import json
import os
import shlex
import sys
import subprocess


def main():
    if len(sys.argv) < 3:
        return
    source = os.path.abspath(sys.argv[1])
    build_dir = sys.argv[2]
    db = os.path.join(build_dir, 'compile_commands.json')
    if not os.path.exists(db):
        return
    try:
        data = json.load(open(db))
    except Exception:
        return

    for entry in data:
        path = entry.get('file') or entry.get('filename')
        if not path:
            continue
        if os.path.abspath(path) != source:
            continue

        cmd = entry.get('command') or ' '.join(entry.get('arguments', []))
        toks = shlex.split(cmd)
        flags = []
        i = 0
        takes_value = set(['-isysroot', '-arch', '-mmacosx-version-min', '-isystem', '-I', '-D', '-stdlib'])
        while i < len(toks):
            t = toks[i]
            if t == path or t == source:
                i += 1
                continue
            # combined forms like -I/dir or -DNAME=val or -std=...
            if any(t.startswith(prefix) for prefix in ('-I', '-D', '-isystem', '-std', '-stdlib', '-f')):
                flags.append(t)
                i += 1
                continue
            if t in takes_value:
                if i + 1 < len(toks):
                    flags.append(t)
                    flags.append(toks[i + 1])
                    i += 2
                    continue
                else:
                    flags.append(t)
                    i += 1
                    continue
            i += 1

        extras = ['-extra-arg={0}'.format(x) for x in flags]
        
        # On macOS, ensure clang-tidy uses Homebrew LLVM's libc++ and avoids system SDK conflicts
        if sys.platform == 'darwin':
            homebrew_llvm_cpp = '/opt/homebrew/opt/llvm/include/c++/v1'
            if os.path.isdir(homebrew_llvm_cpp):
                # Use Homebrew LLVM's stdlib and suppress system includes to avoid conflicts
                extras.append('-extra-arg=-stdlib=libc++')
                extras.append('-extra-arg=-nostdinc')
                extras.append('-extra-arg=-isystem')
                extras.append('-extra-arg={0}'.format(homebrew_llvm_cpp))
                extras.append('-extra-arg=-isystem')
                extras.append('-extra-arg=/opt/homebrew/opt/llvm/include')
        
        if extras:
            print(' '.join(extras))
        return


if __name__ == '__main__':
    main()
