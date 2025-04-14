#!/usr/bin/env python3

import sys
import re

with open(sys.argv[1], 'r') as f:
    data = f.read()
    data = re.sub(r'^([\s\w:]*)api\.add_api_variable\(\"(\w+)\", new (const )?(API[\w<>]+)\(([^\{\}]+?)\)\);\s*(\\)$',
                   r'\1api.add_api_variable<\3\4>("\2", \5);\6', data, flags=re.MULTILINE | re.DOTALL)

    data = re.sub(r'^([\s\w:]*)api\.add_api_variable\(\"(\w+)\", new (const )?(API[\w<>]+)\((.+?\}.+?)\)+;\s*(\\)$',
                   r'\1api.add_api_variable<\3\4>("\2", \5);\6', data, flags=re.MULTILINE | re.DOTALL)
    print(data)

    #match = re.findall(r'^(\s*)api.add_api_variable\(\"(\w+)\", new (const )?(API[\w<>]+)(.+?\{.+?\}.+?)\);\s*$', data, re.MULTILINE | re.DOTALL)
    # for m in match:
    #     print(m)
    # for m in match:ueuqeuqejj
    #    if m.group('lambda'):
    #         print(m)
        #print(f"{m[0]}static {m[2]}{m[3]} api_{m[1]}{m[4]};")
        #print(f"{m[0]}api.add_api_variable(\"{m[1]}\", &api_{m[1]});")
    
    #print(match)
    #print([m.groupdict() for m in match])