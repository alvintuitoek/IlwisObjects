import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import UIContextModel 1.0
import LayerManager 1.0
import ModelRegistry 1.0
import "../../workbench/propertyform" as MetaData
import "../../controls" as Controls
import "../../Global.js" as Global
import "../.." as Base

Item {
    id : metatdata
    width : 210
    height : parent.height
    property alias coverage : overview
    property alias mouseActions : mouseActions
    property bool drawerActive : false
    property string iconName : "../images/metadata"
    property LayerManager manager
    objectName: uicontext.uniqueName() + "_metadata_mappanel"

    signal zoomEnded(string envelope)

  

    Connections {
        target: mouseActions
        onZoomEnded :{
            layersmeta.endZoom(envelope)
        }
    }


    Row {
        width: parent.width
        height : parent.height
        spacing : 3
        Rectangle{
            width : parent.width - 8
            height : metatdata.height
            color : Global.alternatecolor3



            MetaDataSpatialInfo{
                id : mdspatialinfo
                anchors.leftMargin: 2
                width : 350
                height : parent.height
            }
            Rectangle {
                id : overLabel
                width : overv.width
                height : 18
                anchors.left: mdspatialinfo.right
                anchors.top : parent.top
                color : uicontext.paleColor
                Text{
                    text : qsTr("Overview")
                    font.bold : true
                    x : 5
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Item {
                id : overv
                anchors.left : mdspatialinfo.right
                anchors.top : overLabel.bottom
                anchors.right: parent.right
                height : parent.height - overLabel.height

                OverviewExtentToolbar{
                    id : buttons
                    anchors.right: viewcontainer.left
                    height : parent.height
                }

                function entireMap() {
                   if ( layersmeta.currentIndex == 2){
                       metatdata.manager.addCommand("setviewextent("+ overview.viewid + ",entiremap)");
                       metatdata.manager.refresh()
                    }
                }

                Rectangle{
                    id : viewcontainer
                    width :parent.width  - buttons.width
                    height : parent.height
                    border.width: 1
                    border.color: "lightgrey"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 3
                    function entireMap() {
                        if ( layersmeta.currentIndex == 2){
                            metatdata.manager.addCommand("setviewextent("+ overview.viewid + ",entiremap)");
                            metatdata.manager.refresh()
                        }
                    }

                    onWidthChanged: {
                        entireMap()
                    }

                    onVisibleChanged: {
                        if (viewcontainer.visible)
                            entireMap()
                    }
                    onHeightChanged: {
                        entireMap()
                    }


                    Controls.DummySpatial {
		                id: overview
                        anchors.fill: parent
                        objectName : "overview_mainui_"  + uicontext.uniqueName()
                        layermanager : manager

                        Connections {
                            target: mouseActions
                            onZoomEnded :{
                                layerview.zoomFromOverview(envelope)
                                //layerview.manager.addCommand("setviewextent("+ layerview.manager.viewid + "," + envelope + ")");
                            }
                        }

                        Controls.LayerExtentMouseActions{
                            id : mouseActions
                            layerManager: manager
                            zoomToExtents: false
			                hasPermanence: true
			                showInfo: false
			                selectiondrawerColor: "basic"
                        }
                    }
                }
            }
        }
    }
    function iconSource(name) {
        if ( name === "")
            name = "redbuttonr.png"
        var iconP = "../../images/" + name
        return iconP

    }

    function addDataSource(filter, sourceName, sourceType, options){
     if ( coverage && layerview.showManager){
            if (! metatdata.manager){
                metatdata.manager = models.createLayerManager(metadata,overview)
                overview.layermanager = metatdata.manager
				layerview.activeLayerManager().setAssociatedLayerManager(metatdata.manager)
            }
            var cmd = "adddrawer(" + manager.viewid + ",\"\"," + filter + "," + sourceType + ",true,\"" + (options !== undefined ? options : "") +  "\")"
            metatdata.manager.addCommand(cmd)
            var layer = manager.topLevelLayers[1];
            var expr = "setactiveattribute(" + manager.viewid + "," + layer.nodeId + ",1)"
            metatdata.manager.addCommand(expr)
            metatdata.manager.refresh()
        }
    }

    function transfer(datapanel){
        var layers = datapanel.manager.layers;
       for(var i =1; i < layers.length; i++){  // start at 1 because the first layer is always the global layer, is there by default so we skip it
            var expr = "adddrawer(" + overview.layermanager.viewid + ","+ layers[i].name + ",\"itemid=" + layers[i].id + "\"," + layers[i].typeName + ",true,\"\")"
            overview.layermanager.addCommand(expr)
       }
    }

    function newZoomExtent(newenvelope){
        mouseActions.setRectangle(newenvelope)
    }

    function changeSubPanel() {
        // dummy function
        return false
    }
}
