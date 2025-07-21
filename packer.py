#!/bin/env python

import argparse
import os
import re
import sys
from typing import override

Version = "0.1"
DefaultOutputFile = "packedrs.h"
CVarTypes = [
    "static const unsigned char",
    "const unsigned char",
]

CNameTypes = ["{varname}_DATA[{varname}_SIZE]", "{varname}[{varname}_SIZE]"]

FileTemplate = """\
/**************************************
* Exported with BpPacker.py v{version}
**************************************/

#ifndef {output}_H
#define {output}_H

// Data Sizes
{datasize_defines}

// Data Arrays
{data_arrays}

#endif //{output}_H
"""

ItemSizeTemplate = "#define {varname}_SIZE {datasize}"

ItemDataTemplate = """
// Embedded file : {filename} ({fsize} bytes)
{vartype} {varname} = {{
{data}
}};
"""


def get_vartype(ctype: int | None) -> str:
    if ctype is None:
        ctype = 1
    if ctype <= 0 or ctype >= len(CVarTypes):
        ctype = 1
    return CVarTypes[ctype - 1]


def get_nametype(nametype: int | None) -> str:
    if nametype is None:
        nametype = 1
    if nametype <= 0 or nametype >= len(CNameTypes):
        nametype = 1

    return CNameTypes[nametype - 1]


class FileData:
    fname: str = ""
    filepath: str = ""
    fdata: bytes = bytes(0)
    fsize: int = 0

    def __init__(self) -> None:
        return

    def set_filename(self, name: str) -> None:
        self.fname = sanitize_varname(name)

    @override
    def __str__(self) -> str:
        return f"{self.fname} {self.fsize}"


def check_paths(paths: list[str]) -> bool:
    for p in paths:
        if not os.path.exists(p):
            print(f"Error: File '{p}' does not exist!")
            return False
    return True


def sanitize_varname(name: str):
    name = re.sub(r"\W+", "_", name)
    if re.match(r"^\d", name):
        name = "_" + name
    return name


def make_template_string(size_str: list[str], data_str: list[str], header: str) -> str:
    sizes = "\n".join(size_str)
    datas = "\n".join(data_str)

    return FileTemplate.format(
        output=header.upper(),
        datasize_defines=sizes,
        data_arrays=datas,
        version=Version,
    )


def make_size_string(data: FileData) -> str:
    return ItemSizeTemplate.format(varname=data.fname.upper(), datasize=data.fsize)


def make_name_string(data: FileData, nametype: int) -> str:
    namestr = get_nametype(nametype)
    return namestr.format(
        varname=data.fname.upper(),
    )


def make_data_string(data: FileData, ctype: int, nametype: int) -> str:
    filename = os.path.basename(data.filepath)
    vartype = get_vartype(ctype)
    varname = make_name_string(data, nametype)

    hexes = [f"0x{b:02X}" for b in data.fdata]
    lines: list[str] = []
    width = 12
    for ih in range(0, len(hexes), width):
        ln = (" " * 4) + ", ".join(hexes[ih : ih + width]) + ","
        lines.append(ln)

    line = "\n".join(lines)

    result = ItemDataTemplate.format(
        filename=filename,
        vartype=vartype,
        varname=varname,
        fsize=data.fsize,
        data=line,
    )

    return result


def make(
    finput: list[str],
    output: str,
    namefmt: int,
    typefmt: int,
    verbose: bool,
) -> str:

    if not check_paths(finput):
        print("Fatal Error Occured. Exiting...")
        sys.exit(1)

    header = os.path.basename(output).split(".")[0]

    datastr: list[str] = []
    sizestr: list[str] = []

    for fnp in finput:
        fd = FileData()
        fd.filepath = fnp
        fd.set_filename(os.path.basename(fnp))

        with open(fnp, "rb") as f:
            data = f.read()
            fd.fdata = data

        fd.fsize = len(fd.fdata)

        # filedatas.append(fd)
        datastr.append(make_data_string(fd, typefmt, namefmt))
        sizestr.append(make_size_string(fd))

    result = make_template_string(sizestr, datastr, header)

    with open(output, "w") as f:
        _ = f.write(result)

    return ""


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Resource Packer for BauriPixel v" + Version
    )

    _ = parser.add_argument(
        "input",
        type=str,
        nargs="+",
        help="Resource filepaths (ie. *.jpg *.png)",
    )

    _ = parser.add_argument("-o", "--output", type=str, help="Output file name")

    _ = parser.add_argument(
        "-f",
        "--format",
        type=int,
        help="""
            Variable name format 
            [1 <FILENAME>_data[] = {...}] 
            [2 <FILENAME>[] = {...}]""",
    )
    _ = parser.add_argument(
        "-t",
        "--ctype",
        type=int,
        help="""
            C Variable Type = 
            [1 'static const unsigned char'] 
            [2 'static const char'] 
            [3 'const unsigned char'] 
            [4 'const char']""",
    )
    _ = parser.add_argument(
        "-v",
        "--verbose",
        help="Increase output verbosity",
        action="store_true",
    )

    args = parser.parse_args()
    _input: list[str] = args.input
    _output: str = ""
    if args.output:
        _output = args.output
    else:
        _output = DefaultOutputFile
    _var_format: int = args.format
    _var_ctype: int = args.ctype
    verbose: bool = args.verbose

    make(_input, _output, _var_format, _var_ctype, verbose)


if __name__ == "__main__":
    main()
