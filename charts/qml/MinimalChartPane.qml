import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import QtGraphicalEffects 1.0
import QtQuick 2.5
import QtCharts 2.0
import ModelRegistry 1.0
import ChartModel 1.0
import DataseriesModel 1.0
import "../../../../qml/controls" as Controls

Rectangle {
    id : chartView
    width : parent.width
   // height : parent.height - 270
	property ChartModel chart : models.createChartModel(chartView) 
    property var updateChart : chart ? chart.updateSeries : 0
    property alias margins : visibleGraphs.margins
    property alias xmin : xas.min
    property alias xmax : xas.max

    onUpdateChartChanged : {
        visibleGraphs.removeAllSeries()
        loadGraphs()    
    }

	onChartChanged : {
		loadGraphs();
	}

	ValueAxis {
		id : xas
		min : chart != null ? chart.minX : 0
		max : chart != null  ? chart.maxX : 5
		tickCount : 5
        labelFormat : chart ? chart.formatXAxis : "%.3f"
        visible : false
	}

	ValueAxis {
		id : yas
		min : chart != null  ? chart.minYLeft : 0
		max : chart != null  ? chart.maxYLeft  : 5
		tickCount : 5
        labelFormat : chart ? chart.formatYAxisLeft : "%.3f"
        visible : true
        labelsVisible : false
	}

    Controls.FloatingRectangle {
        id : chartInfo

    }

	ChartView {
		id : visibleGraphs
		anchors.fill: parent
        anchors.margins : 2
		legend.visible: false
        backgroundColor : "transparent"
        theme : ChartView.ChartThemeBlueIcy
        dropShadowEnabled : true

        MouseArea {
            id : chartMouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton

            onReleased: {
                chartInfo.enabled = false
                chartInfo.opacity = 0
                chartInfo.x = 0
                chartInfo.y = 0
            }        
        }
    }

    function linehover(pnt, state) {
        if (state) {    // true: mouse is on the line
            var pos = visibleGraphs.mapToPosition(pnt);
            chartInfo.enabled = true
            chartInfo.opacity = 1
            chartInfo.x = pos.x
            chartInfo.y = pos.y
            chartInfo.text = pnt.y.toPrecision(4)
        }
    }

	function loadGraphs() {
        if ( !chart)
            return
		for (var i = 0; i < chart.seriesCount; i++) {
			var smodel = chart.getSeries(i);
            var series = createSeries(chart.chartType, smodel.name, xas, yas)
			series.pointsVisible = false;
			series.color = chart.seriesColor(i);
			smodel.setData(series)
		}
	}

    function createSeries(ctype, name, xas, yas){
        if ( ctype == "line") {
            var series = visibleGraphs.createSeries(ChartView.SeriesTypeLine, name, xas, yas);
            series.hovered.connect(linehover)
            return series;
        }
        if (ctype == "spline"){
            return visibleGraphs.createSeries(ChartView.SeriesTypeSpline, name, xas, yas);
        }
        if (ctype == "bar")
            return visibleGraphs.createSeries(ChartView.SeriesTypeBar, name, xas, yas);
        if ( ctype == "pie")
            return visibleGraphs.createSeries(ChartView.SeriesTypePie, name, xas, yas);
        if ( ctype == "points"){
            var series =  visibleGraphs.createSeries(ChartView.SeriesTypeScatter, name, xas, yas);
            series.markerSize = 10
            return series
        }
        if ( ctype == "polar")
            return visibleGraphs.createSeries(ChartView.SeriesTypeScatter, name, xas, yas);
        if ( ctype == "area")
            return visibleGraphs.createSeries(ChartView.SeriesTypeArea, name, xas, yas);
        return visibleGraphs.createSeries(ChartView.SeriesTypeLine, name, xas, yas);
    }
}