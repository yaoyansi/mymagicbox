import mymagicbox.log		as log

###
class UpgradeBase(object):

	def upgrade(self):
		log.info('------------------------------------');
		log.info(' Trying To Upgrade ...              ');
		log.info('------------------------------------');

		self._upgrade();

		log.info('------------------------------------');
		log.info(' Upgrade Is Done                    ');
		log.info('------------------------------------');

	def upgrade_onIdle(self):
		log.info('------------------------------------');
		log.info(' Trying To Upgrade On Idle ...      ');
		log.info('------------------------------------');

		self._upgrade_onIdle();

		log.info('------------------------------------');
		log.info(' Upgrade On Idle Is Done            ');
		log.info('------------------------------------');
	
	# virtual 
	def _upgrade(self):
		pass

	# virtual 
	def _upgrade_onIdle(self):
		pass

###
class Upgrade(UpgradeBase):

	def _upgrade(self):
		log.debug('This is Upgrade::_upgrade()');

	def _upgrade_onIdle(self):
		log.debug('This is Upgrade::_upgrade_onIdle()');
