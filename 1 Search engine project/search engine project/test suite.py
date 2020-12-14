def test(actual,expected):
    import sys
    linenum=sys._getframe(1).f_lineno
    if(expected==actual):
        msg="Test on line {0} passed".format(linenum)
    else:
        msg="Test on line {0} failed,expected {1} but got {2}".format(linenum,expected,actual)
    print(msg)
