# Copyright (c) 2015-2017, Chaos Software Ltd
#
# V-Ray For Houdini
#
# ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
#
# Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
#

import hou

# The node is passed in kwargs['node']
node = kwargs['node']

node_parm = node.parm("dimmer")
node_parm.setExpression("if(ch(\"./enabled\"), ch(\"./intensity\"), 0)")

cnode = node.createNode("sphere")
node_parm = cnode.parm("scale")
node_parm.setExpression("ch(\"../radius\")")

cnode.setRenderFlag(True)
cnode.setDisplayFlag(True)

node.layoutChildren()
node.setName("vraylightsphere")