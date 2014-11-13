
#
def fatal(format, *params):
    print("mymagicbox| Fat| "+format % params);

def err(format, *params):
    print("mymagicbox| Err| "+format % params);

def warn(format, *params):
    print("mymagicbox| Wrn| "+format % params);

def info(format, *params):
    print("mymagicbox| Inf| "+format % params);

def debug(format, *params):
    print("mymagicbox| Dbg| "+format % params);

#
if __name__ == "__main__":
	fatal("msg: %s", "aaa");
	fatal("msg: %s", 123);
	err("msg: %s", "aaa");
	err("msg: %s", 123);
	warn("msg: %s", "aaa");
	warn("msg: %s", 123);
	info("msg: %s", "aaa");
	info("msg: %s", 123);
	debug("msg: %s", "aaa");
	debug("msg: %s", 123);

