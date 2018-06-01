import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import QtGraphicalEffects 1.0
import QtQuick 2.5
import QtCharts 2.0
import ChartModel 1.0
import DataseriesModel 1.0

Rectangle {
    id : chartView
    width : parent.width
    height : parent.height - 270
    color : "blue"
	property ChartModel chart : chartspane.chart
    property var updateChart : chart ? chart.updateSeries : 0

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
		tickCount : chart ? chart.tickCountX : 5
        labelFormat : chart ? chart.formatXAxis : "%.3f"
	}

	ValueAxis {
		id : yas
		min : chart != null  ? chart.minY : 0
		max : chart != null  ? chart.maxY : 5
		tickCount : chart ? chart.tickCountY : 5
        labelFormat : chart ? chart.formatYAxis : "%.3f"
	}

  LinearGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#EBF0EC" }
            GradientStop { position: 1.0; color: "white" }
        }
    }

	ChartView {
		id : visibleGraphs
		title: "Line"
		anchors.fill: parent
		antialiasing: true
        backgroundColor : "transparent"
        theme : ChartView.ChartThemeBlueIcy
        dropShadowEnabled : true
    }

  
	function loadGraphs() {
        if ( !chart)
            return
		for (var i = 0; i < chart.seriesCount; i++) {
			var smodel = chart.getSeries(i);
            var series = createSeries(chart.chartType, smodel.name, xas, yas)
			series.pointsVisible = false;
			series.color = chart.seriesColor(i);
			var points = smodel.points
			var pointsCount = points.length;
			for (var j = 0; j < pointsCount; j++) {
				series.append(points[j].x, points[j].y);
			}
		}
	}

    function createSeries(ctype, name, xas, yas){
        if ( ctype == "line"){
            return visibleGraphs.createSeries(ChartView.SeriesTypeLine, name, xas, yas);
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