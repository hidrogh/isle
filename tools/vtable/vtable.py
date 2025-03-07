#!/usr/bin/env python3

import os
import argparse
import logging
from typing import List
import colorama
from isledecomp.bin import Bin as IsleBin
from isledecomp.compare import Compare as IsleCompare
from isledecomp.utils import print_diff

# Ignore all compare-db messages.
logging.getLogger("isledecomp.compare").addHandler(logging.NullHandler())

colorama.init()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Comparing vtables.")
    parser.add_argument(
        "original", metavar="original-binary", help="The original binary"
    )
    parser.add_argument(
        "recompiled", metavar="recompiled-binary", help="The recompiled binary"
    )
    parser.add_argument(
        "pdb", metavar="recompiled-pdb", help="The PDB of the recompiled binary"
    )
    parser.add_argument(
        "decomp_dir", metavar="decomp-dir", help="The decompiled source tree"
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true", help="Show more detailed information"
    )
    parser.add_argument(
        "--no-color", "-n", action="store_true", help="Do not color the output"
    )

    (args, _) = parser.parse_known_args()

    if not os.path.isfile(args.original):
        parser.error(f"Original binary {args.original} does not exist")

    if not os.path.isfile(args.recompiled):
        parser.error(f"Recompiled binary {args.recompiled} does not exist")

    if not os.path.isfile(args.pdb):
        parser.error(f"Symbols PDB {args.pdb} does not exist")

    if not os.path.isdir(args.decomp_dir):
        parser.error(f"Source directory {args.decomp_dir} does not exist")

    return args


def show_vtable_diff(udiff: List[str], verbose: bool = False, plain: bool = False):
    lines = [
        line
        for line in udiff
        if verbose or line.startswith("+") or line.startswith("-")
    ]
    print_diff(lines, plain)


def print_summary(vtable_count: int, problem_count: int):
    if problem_count == 0:
        print(f"Vtables found: {vtable_count}.\n100% match.")
        return

    print(f"Vtables found: {vtable_count}.\nVtables not matching: {problem_count}.")


def main():
    args = parse_args()
    vtable_count = 0
    problem_count = 0

    with IsleBin(args.original) as orig_bin, IsleBin(args.recompiled) as recomp_bin:
        engine = IsleCompare(orig_bin, recomp_bin, args.pdb, args.decomp_dir)

        for tbl_match in engine.compare_vtables():
            vtable_count += 1
            if tbl_match.ratio < 1:
                problem_count += 1

                udiff = list(tbl_match.udiff)

                print(
                    tbl_match.name,
                    f": orig 0x{tbl_match.orig_addr:x}, recomp 0x{tbl_match.recomp_addr:x}",
                )
                show_vtable_diff(udiff, args.verbose, args.no_color)
                print()

        print_summary(vtable_count, problem_count)

    return 1 if problem_count > 0 else 0


if __name__ == "__main__":
    raise SystemExit(main())
