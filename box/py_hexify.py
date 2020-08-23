def display_as_hex(item):
    if isinstance(item, int):
        print(hex(item))
    else:
        print(repr(item))
import sys
sys.displayhook = display_as_hex
