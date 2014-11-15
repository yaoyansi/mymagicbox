import mymagicbox.log		as log
import mymagicbox.mmb_flog	as flog

@flog.trace
def upgrade():
	log.info("this is upgrade()");

@flog.trace
def upgrade_onIdle():
	log.info("this is upgrade_onIdle()");
