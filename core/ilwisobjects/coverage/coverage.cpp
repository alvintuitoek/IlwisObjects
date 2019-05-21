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

#include "coverage.h"
#include "table.h"

using namespace Ilwis;

Coverage::Coverage()
{
}

Coverage::Coverage(const Resource &resource) : IlwisObject(resource)
{
}

Coverage::~Coverage()
{
    _coordinateSystem.set(0);
}

bool Coverage::prepare(const IOOptions &options) {

    bool ok = IlwisObject::prepare(options);

    return ok;
}

ICoordinateSystem Coverage::coordinateSystem() const
{
    return _coordinateSystem;
}

void Coverage::coordinateSystem(const ICoordinateSystem &csy)
{
    if ( isReadOnly())
        return;
    changed(true);

    _coordinateSystem = csy;
    resourceRef().addProperty("coordinatesystem",coordinateSystem()->resourceRef().url(true).toString(), true);
}

Envelope Coverage::envelope(bool tolatlon) const
{
    if ( !coordinateSystem().isValid())
        return Envelope();

    if ( tolatlon){
        if ( _coordinateSystem->isLatLon())
            return _envelope;
        else if (_coordinateSystem->canConvertToLatLon()){
            auto env = Envelope(_coordinateSystem->coord2latlon(_envelope.min_corner()),
                                _coordinateSystem->coord2latlon(_envelope.max_corner()));
            return env;
        }else{
            return Envelope();
        }
    }
    return _envelope;
}

void Coverage::envelope(const Envelope &bnds)
{
    if ( isReadOnly())
        return;
    changed(true);

    _envelope = bnds;
}

NumericStatistics &Coverage::statistics(int  )
{
        return _statistics;
}

const NumericStatistics &Coverage::statistics(int) const
{
    return _statistics;
}

Resource Coverage::resource(int mode) const
{
    Resource resource = IlwisObject::resource(mode);
    if ( mode & IlwisObject::cmEXTENDED) {
        resource.addProperty("coordinatesystem", coordinateSystem()->resourceRef().url(true).toString(),true);
        resource.setExtendedType( resource.extendedType() | itCOORDSYSTEM);
    }
    return resource;
}

bool Coverage::canUse(const IlwisObject *obj, bool strict) const
{
    if ( IlwisObject::canUse(obj, strict))
        return true;

    if ( hasType(obj->ilwisType(), itCOORDSYSTEM )){
        if ( strict ){
            if ( coordinateSystem()->isCompatibleWith(obj))
                return true;
        }else
            return true;
    }
    return false;
}

void Coverage::name(const QString &nam)
{
    IlwisObject::name(nam)    ;
}

bool Coverage::hasAttributes() const
{
    return true;
}

void Coverage::copyTo(IlwisObject *obj)
{
    IlwisObject::copyTo(obj);
    Coverage *cov = static_cast<Coverage *>(obj);
    cov->_coordinateSystem = _coordinateSystem;
    cov->_envelope = _envelope;
    cov->_statistics = _statistics;
}


