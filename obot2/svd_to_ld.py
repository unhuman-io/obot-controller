#!/usr/bin/env python3
import xml.etree.ElementTree as ET

name = "STM32G474"
svd_file = name + ".svd"
linker_file = name + "_peripherals.ld"
module_file = name + "_peripherals.cppm.gen"

tree = ET.parse(svd_file)
root = tree.getroot()

with open(linker_file, "w") as lf, open(module_file, "w") as mf:
    for peripheral in root.findall(".//peripheral"):
        name = peripheral.find("name")
        derived = peripheral.get("derivedFrom")
        ptype = derived if derived is not None else name.text
        base = peripheral.find("baseAddress")
        if name is not None and base is not None:
            lf.write(f"PROVIDE( {name.text}_address = {base.text} );\n")
            mf.write(f"#undef {name.text}\n")
            mf.write(f"extern \"C\" {ptype}_Type {name.text}_address;\n")
            mf.write(f"constexpr {ptype}_Type * const {name.text} {{ &{name.text}_address }};\n")
            mf.write(f"using ::{ptype}_Type;\n")
            mf.write("\n")

    print(f"Linker file written to {linker_file}")
    print(f"Module file written to {module_file}")