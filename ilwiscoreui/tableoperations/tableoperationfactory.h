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

#ifndef TABLEOPERATIONFACTORY_H
#define TABLEOPERATIONFACTORY_H

#include "ilwiscoreui_global.h"

namespace Ilwis {
namespace Ui {

class TableOperation;

typedef std::function<TableOperation *()> TableOperationCreate;


class ILWISCOREUISHARED_EXPORT TableOperationFactory : public AbstractFactory
{
public:
    TableOperationFactory();
    void registerTableOperation(const QString& name, TableOperationCreate);
    TableOperationCreate * create(const QString& name);
    QList<TableOperation *> selectedOperations(TableModel *tblModel, const QVariantMap &parameters);
    template<class T=TableOperation> T *create(const QString& name)
    {
       auto iter = _creators.find(name);
       if ( iter == _creators.end()){
           return 0;
       }
       return static_cast<T *>((*iter).second());
    }

private:
     std::map<QString, TableOperationCreate> _creators;

};
}
}

#endif // TABLEOPERATIONFACTORY_H
