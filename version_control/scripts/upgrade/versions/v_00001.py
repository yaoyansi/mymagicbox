import maya.cmds as cmds

import mymagicbox.log		as log

class Upgrade(object):

	def __init__(self):
		self.thisVersion = 00001;

	def do(self, nodes):
		log.info('------------------------------------');
		log.info(' Trying To Upgrade to %s ...        ', self.thisVersion);

		for node in nodes:
			if cmds.getAttr(node+'.mmbversion') >= self.thisVersion:
				continue;# skip the newer node

			nodeType = cmds.nodeType(node);
			if nodeType == 'testNodeA':
				self.on_testNodeA(node);
			elif nodeType == 'testNodeB':
				self.on_testNodeB(node);

			# update $node.mmbversion to thisVersion
			cmds.setAttr(node+'.mmbversion', self.thisVersion);

		log.info('\n\n\n');

	def on_testNodeA(self, node):
		log.debug('upgrading %s to %s', node, self.thisVersion);

	def on_testNodeB(self, node):
		log.debug('upgrading %s to %s', node, self.thisVersion);

