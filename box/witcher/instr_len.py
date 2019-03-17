import gdb

def instn_length(addr_expr):
    t = gdb.execute('x/2i ' + addr_expr, to_string=True)
    return int(gdb.parse_and_eval('$_')) - int(gdb.parse_and_eval(addr_expr))

class InstnLength(gdb.Function):
    def __init__(self):
        super(InstnLength, self).__init__('instn_length')

    def invoke(self, addr):
        return instn_length(str(int(addr)))

InstnLength()
