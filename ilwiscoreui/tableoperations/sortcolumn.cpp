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

#include "kernel.h"
#include "ilwisdata.h"
#include "datadefinition.h"
#include "columndefinition.h"
#include "table.h"
#include "models/tablemodel.h"
#include "tableoperation.h"
#include "sortcolumn.h"

using namespace Ilwis;
using namespace Ui;

SortColumn::SortColumn() : TableOperation("Column Sort", QUrl("SortColumn.qml"),TR("Sorts columns in ascending or decending order"))
{

}

bool SortColumn::canUse(TableModel *tblModel, const QVariantMap& parameter) const
{
    bool ok;
    quint32 colIndex = parameter["columnindex"].toInt(&ok);
    if ( !ok || colIndex >= tblModel->getColumnCount())
        return false;
    IDomain dom = tblModel->table()->columndefinition(colIndex).datadef().domain();

    return hasType(dom->valueType(), itNUMBER | itSTRING | itDOMAINITEM);

}

template<typename T> struct OrderedPair{
    OrderedPair(){}
    OrderedPair(const T& value, quint32 order) : _sortableValue(value), _order(order){}
    T _sortableValue;
    quint32 _order;

};

void SortColumn::execute(const QVariantMap& parameters )
{
    bool ok;
    quint32 colIndex = parameters["columnindex"].toInt(&ok);
    if ( !ok || colIndex >= tableModel()->getColumnCount())
        return ;
    QString direction = parameters["order"].toString();
    if ( direction != "ascending" && direction != "decending")
        return ;

    IlwisTypes valueType = table()->columndefinition(colIndex).datadef().domain()->valueType();
    std::vector<QVariant> values = table()->column(colIndex);
    std::vector<quint32> order(values.size());
    quint32 index = 0;

    if ( hasType(valueType, itNUMBER)){
        std::vector<OrderedPair<double>> numbers(values.size());
        for(auto& opair : numbers){
            opair = OrderedPair<double>(values[index].toDouble(), index);
            index++;
        }
        std::sort(numbers.begin(), numbers.end(),[](const OrderedPair<double>& opair1,const OrderedPair<double>& opair2)->bool { return opair1._sortableValue < opair2._sortableValue;});
        index = 0;
        for(const auto& opair : numbers)
            order[index++] = opair._order;
    }
    if ( hasType(valueType, itSTRING)){
        std::vector<OrderedPair<QString>> strings(values.size());
        for(auto& opair : strings){
            opair = OrderedPair<QString>(values[index].toString(), index);
            index++;
        }
        index = 0;
        std::sort(strings.begin(), strings.end(),[](const OrderedPair<QString>& opair1,const OrderedPair<QString>& opair2)->bool { return opair1._sortableValue < opair2._sortableValue;});
        for(const auto& opair : strings)
            order[index++] = opair._order;
    }
    if ( hasType(valueType, itDOMAINITEM)){
        std::vector<OrderedPair<QString>> strings(values.size());
        IDomain dom = table()->columndefinition(colIndex).datadef().domain();
        for(auto& opair : strings){
            opair = OrderedPair<QString>(dom->impliedValue(values[index]).toString(), index);
            index++;
        }
        index = 0;
        std::sort(strings.begin(), strings.end(),[](const OrderedPair<QString>& opair1,const OrderedPair<QString>& opair2)->bool { return opair1._sortableValue < opair2._sortableValue;});
        for(const auto& opair : strings)
            order[index++] = opair._order;

    }
    tableModel()->order(order);
    tableModel()->update();
}

TableOperation *SortColumn::create()
{
    return new SortColumn();
}
