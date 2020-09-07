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


namespace Ilwis {
	class RasterCoverage;
	typedef IlwisData<RasterCoverage> IRasterCoverage;

	namespace Sentinel {

		class SentinelRasterConnector : public IlwisObjectConnector
		{
		public:
			SentinelRasterConnector(const Ilwis::Resource &resource, bool load, const IOOptions& options = IOOptions());
			IlwisObject *create() const;

			bool loadMetaData(IlwisObject*obj, const IOOptions & options);
			static ConnectorInterface *create(const Ilwis::Resource &resource, bool load, const IOOptions& options = IOOptions());
			bool loadData(IlwisObject *, const IOOptions &options = IOOptions());
			QString provider() const;

		private:
			quint32 _version;
			IRasterCoverage _dataRaster;

		};
	}
}


