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

#ifndef WorkflowCATALOGEXPLORER_H
#define WorkflowCATALOGEXPLORER_H

namespace Ilwis {
namespace WorkflowConnector {


class WorkflowCatalogExplorer : public CatalogExplorer
{
public:
    WorkflowCatalogExplorer(const Ilwis::Resource &resource,const IOOptions& options=IOOptions());

    std::vector<Ilwis::Resource> loadItems(const IOOptions &options=IOOptions());
    bool canUse(const Resource& resource) const;
    QString provider() const;
    virtual QFileInfo toLocalFile(const QUrl &datasource) const;

    static Ilwis::CatalogExplorer *create(const Resource &resource,const IOOptions& options=IOOptions());

    NEW_CATALOGEXPLORER(WorkflowCatalogExplorer);
};
}
}

#endif // WorkflowCATALOGEXPLORER_H
