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

#include <QtPlugin>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "coverage.h"
#include "symboltable.h"
#include "operationExpression.h"
#include "connectorinterface.h"
#include "abstractfactory.h"
#include "ilwisobjectfactory.h"
#include "osmobjectfactory.h"
#include "connectorfactory.h"
#include "osmconnectormodule.h"
#include "table.h"
#include "feature.h"
#include "domainitem.h"
#include "itemdomain.h"
#include "identifieritem.h"
#include "identifierrange.h"
#include "operationExpression.h"
#include "operationmetadata.h"
#include "operation.h"
#include "ilwisobjectconnector.h"
#include "osmrasterconnector.h"
#include "dataformat.h"
#include "ilwiscontext.h"


using namespace Ilwis;
using namespace OSM;

OSMConnectorModule::OSMConnectorModule(QObject *parent) : 
    Module(parent, "OSMonnectorModule", "iv40","1.0")
{
}

QString OSMConnectorModule::getInterfaceVersion() const
{
    return "iv40";

}

void OSMConnectorModule::prepare()
{
	QString server = ilwisconfig("users/" + Ilwis::context()->currentUser() + "/backgroundmap-server", QString(""));
	if (server == QString("")) {
		context()->configurationRef().addValue("users/" + Ilwis::context()->currentUser() + "/backgroundmap-server", "https://a.tile.openstreetmap.org/");
	}

	OSMObjectFactory *objfactory = new OSMObjectFactory();
	objfactory->prepare();
	kernel()->addFactory(objfactory);

	ConnectorFactory *factory = kernel()->factory<ConnectorFactory>("ilwis::ConnectorFactory");
	if (!factory)
		return;
//
	factory->addCreator(itRASTER, "osm",OSMRasterConnector::create);
	   

	factory->addCreator("osm", "osm", OSMRasterConnector::create);



	QFileInfo ilw = context()->ilwisFolder();
	QString path = ilw.canonicalFilePath() + "/extensions/osmconnector/resources";
	//DataFormat::setFormatInfo(path + "/ilwis4formats.config", "ilwis4");

	kernel()->issues()->log("Loaded OSM connector module", IssueObject::itMessage);
}


QString OSMConnectorModule::name() const
{
    return "OSM connector plugin";
}

QString OSMConnectorModule::version() const
{
    return "1.0";
}

IlwisTypes OSMConnectorModule::ilwisType(const QString &name) {
	return itUNKNOWN;
}






