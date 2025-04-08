#!/usr/bin/env python3

import sys
import re

with open(sys.argv[1], 'r') as f:
    data = f.read()
    match = re.findall(r'^\s*api.add_api_variable\(\"(\w+)\", new (const )?(.+?)\);\s*$', data, re.MULTILINE | re.DOTALL)
    for m in match:
        print(m)
    for m in match:
        print(f"        static {m[1]}api_{m[0]} {m[2]};")
        print(f"        api.add_api_variable(\"{m[0]}\", &api_{m[0]});")
    
