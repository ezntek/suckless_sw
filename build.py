#!/usr/bin/env python3

import subprocess

APPS = [
    "dwm",
    "st",
]

ROOT_CMD = "doas"
RESET_CONFIG_CMD = "rm -f config.h"
get_build_cmd = lambda dirname: f"{RESET_CONFIG_CMD}; doas make cc=clang -C {dirname} clean install"

for dir in APPS:
    subprocess.run(get_build_cmd(dir), shell=True)
