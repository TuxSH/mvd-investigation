#!/usr/bin/env python3
"""Compile actual ARM headers/sources and run IPC tests with mocked OS calls."""
import os
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
libctru = Path(os.environ.get('LIBCTRU', '/opt/devkitpro/libctru'))
devkit = Path(os.environ.get('DEVKITARM', '/opt/devkitpro/devkitARM'))
cc = devkit / 'bin/arm-none-eabi-gcc'
cxx = devkit / 'bin/arm-none-eabi-g++'
sources = sorted((root / 'source/services').glob('*.c'))
flags = ['-Wall', '-Wextra', '-Werror', '-march=armv6k', '-mtune=mpcore',
         '-mfloat-abi=hard', '-mtp=soft', '-I' + str(root / 'include'),
         '-I' + str(libctru / 'include'), '-fsyntax-only']
subprocess.run([cc, '-std=gnu11', *flags, *sources, root / 'tests/layout.c'], check=True)
subprocess.run([cxx, '-std=gnu++11', '-fshort-enums', *flags,
                '-x', 'c++', root / 'tests/layout.c'], check=True)

# Keep real libctru scalar types and Result macros; replace only OS/IPC entry points
headers = {
    'svc.h': 'u32* getThreadCommandBuffer(void); Result svcSendSyncRequest(Handle); Result svcCloseHandle(Handle);\n#define CUR_PROCESS_HANDLE 0xFFFF8001u\n',
    'srv.h': 'Result srvGetServiceHandle(Handle*, const char*);\n',
    'synchronization.h': 'static inline int AtomicPostIncrement(int* p) { return (*p)++; }\nstatic inline int AtomicDecrement(int* p) { return --*p; }\n',
    'os.h': 'u32 osConvertVirtToPhys(const void*); void* osConvertOldLINEARMemToNew(const void*);\n',
    'allocator/linear.h': 'void* linearAlloc(size_t); void linearFree(void*);\n',
}
with tempfile.TemporaryDirectory(prefix='mvd-ipc-tests-') as temporary:
    work = Path(temporary)
    for name, content in headers.items():
        path = work / '3ds' / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('#pragma once\n#include <3ds/types.h>\n' + content)
    # Copy the real descriptor/header implementation, replacing only its TLS dependency
    ipc = (libctru / 'include/3ds/ipc.h').read_text()
    (work / '3ds/ipc.h').write_text(ipc)
    (work / '3ds/thread.h').write_text('#pragma once\n#include <3ds/types.h>\nu32* getThreadCommandBuffer(void);\n')
    host_cc = os.environ.get('HOST_CC', 'clang')
    subprocess.run([host_cc, '-std=gnu11', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=address,undefined', '-g', '-I' + str(work),
                    '-I' + str(root / 'include'), '-I' + str(libctru / 'include'),
                    *sources, root / 'tests/wire.c', '-o', work / 'wire'], check=True)
    subprocess.run([work / 'wire'], check=True)
print('ARM C/C++ layout checks and sanitized host IPC tests passed')
