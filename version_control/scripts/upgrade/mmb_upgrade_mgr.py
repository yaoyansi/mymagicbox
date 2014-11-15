import maya.cmds as cmds

import mymagicbox.log		as log

###
class UpgradeBase(object):

	def upgrade(self, plugin_name):
		log.info('------------------------------------');
		log.info(' Trying To Upgrade ...              ');
		log.info('------------------------------------');

		self._upgrade(plugin_name);

		log.info('------------------------------------');
		log.info(' Upgrade Is Done                    ');
		log.info('------------------------------------');

	def upgrade_onIdle(self, plugin_name):
		log.info('------------------------------------');
		log.info(' Trying To Upgrade On Idle ...      ');
		log.info('------------------------------------');

		self._upgrade_onIdle(plugin_name);

		log.info('------------------------------------');
		log.info(' Upgrade On Idle Is Done            ');
		log.info('------------------------------------');
	
	# virtual 
	def _upgrade(self):
		pass

	# virtual 
	def _upgrade_onIdle(self):
		pass

	@staticmethod
	def versionStrToInt(versionStr):
	    versionDigital = versionStr.split('.');
	    versionDigital = [int(i) for i in versionDigital]# convert stirng list to int list
	    versionInt = versionDigital[0] * 10000 + versionDigital[1] * 100 + versionDigital[2];
	    return versionInt;

###
class Upgrade(UpgradeBase):

	def _upgrade(self, plugin_name):
		log.debug('This is Upgrade::_upgrade()');

	def _upgrade_onIdle(self, plugin_name):
		log.debug('This is Upgrade::_upgrade_onIdle()');

		# which nodes are going to be upgraded in this plugin?
		registedNodeTypes = cmds.pluginInfo(plugin_name, query=True, dependNode=True );
		log.debug("These node types will be upgraded: %s", registedNodeTypes);

		# current version
		currentVersionStr = cmds.pluginInfo(plugin_name, query=True, version=True );
		currentVersionInt = self.versionStrToInt(currentVersionStr);
		log.debug("The current version of this plugin: %s", currentVersionInt);

		# gather candidate nodes which will be upgraded
		candidateNodes = [];
		self.gather_candidate_nodes(registedNodeTypes, candidateNodes);
		log.debug('candidateNodes = %s', candidateNodes);

	def gather_candidate_nodes(self, nodeTypes, outNodes):
		for nodeType in nodeTypes:
			self.gather_node_type(nodeType, outNodes);

	def gather_node_type(self, nodeType, outNodes):
		outNodes.extend(cmds.ls(exactType=nodeType));









