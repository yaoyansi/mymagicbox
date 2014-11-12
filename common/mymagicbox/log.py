
#
def log_fatal(format, *params):
    print("mymagicbox> Fatal> "+format % params);

def log_err(format, *params):
    print("mymagicbox> Err>   "+format % params);

def log_warn(format, *params):
    print("mymagicbox> Warn>  "+format % params);

def log_info(format, *params):
    print("mymagicbox> Info>  "+format % params);

def log_debug(format, *params):
    print("mymagicbox> Debug> "+format % params);

#
if __name__ == "__main__":
	log_fatal("msg: %s", "aaa");
	log_fatal("msg: %s", 123);
	log_err("msg: %s", "aaa");
	log_err("msg: %s", 123);
	log_warn("msg: %s", "aaa");
	log_warn("msg: %s", 123);
	log_info("msg: %s", "aaa");
	log_info("msg: %s", 123);
	log_debug("msg: %s", "aaa");
	log_debug("msg: %s", 123);

