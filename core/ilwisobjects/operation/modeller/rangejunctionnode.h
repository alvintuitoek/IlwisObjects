/*IlwisObjects is a framework for analysis, processing and visualization of remote sensing and gis data
Copyright (C) 2018  52n North

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef RANGEJUNCTIONNODE_H
#define RANGEJUNCTIONNODE_H

#include "kernel_global.h"

namespace Ilwis {

class KERNELSHARED_EXPORT RangeJunctionNode : public WorkFlowNode
{
public:
    RangeJunctionNode();
    RangeJunctionNode(quint64 nodeid);
    Ilwis::WorkFlowNode::NodeTypes type() const;
    int inputCount() const;
    void link2initialInput(SPWorkFlowNode trueNode, int parmIndex);
    void link2Input(SPWorkFlowNode falseNode, int parmIndex);
    void link2Output(Ilwis::SPWorkFlowNode node, int parmIndex);
    void link2FinalOutput(Ilwis::SPWorkFlowNode node, int parmIndex);
    void nodeId(quint64 id);
    void setFlow(const SPWorkFlowNode& fromNode, qint32 inParmIndex, qint32 outParmIndex, int attachRctIndxFrom, int attachRctIndxTo);
    bool isValid(const Ilwis::Workflow *workflow, WorkFlowNode::ValidityCheck) const;
	quint32 rangeId() const;

};

typedef std::shared_ptr<RangeJunctionNode> SPRangeJunctionNode;
}

#endif // RANGEJUNCTIONNODE_H
