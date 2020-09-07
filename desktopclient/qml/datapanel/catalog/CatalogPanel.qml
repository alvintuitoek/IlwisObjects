import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import CatalogModel 1.0
import MasterCatalogModel 1.0
import CatalogFilterModel 1.0
import TabModel 1.0
import "../../Global.js" as Global
import "../../controls" as Controls

Item {
    property int heightButtons :22
    property CatalogModel currentCatalog
    property TabModel tabmodel
    property string panelType : "catalog"
    property var createParameters : []
    id : catalogViews
    width : parent.width
    height : parent.height

    onTabmodelChanged: {
        if ( actionBar && tabmodel)
            actionBar.side = tabmodel.side
    }

    signal catalogChanged()

  

    Rectangle{
        id : infobar
        width : parent.width
        height : 20
        color : uicontext.genericBGColor
        Text {
            width : parent.width
            height : 16
            text : currentCatalog ? currentCatalog.url : ""
            anchors.centerIn: parent
            color : "#000099"
            opacity: 0.7
        }
    }

    SplitView {
        width : parent.width
        orientation: Qt.Vertical
        height : parent.height - infobar.height
        anchors.top : infobar.bottom
        id : catalogSplit

        property color backgroundCatalogColor : (tabmodel && tabmodel.side == "right" ? uicontext.workbenchBGColor : uicontext.genericBGColor)

        CatalogViews {
            id : catalogView
            height : parent.height - Global.actionBarMinHeight
        }
        CatalogManager{
            id : actionBar
        }
        handleDelegate: Rectangle{
            width : parent.width
            height : 1
            color : "lightgrey"
        }

    }

    Component.onDestruction: {
        currentCatalog.makeParent(null)
    }

  function showObject(objectids,options){
        var filter
        if ( objectids === -1){ // case for  .. one step back
            var container = currentCatalog.container
            filter = "container='" + container + "'"
            datapanesplit.changePanel(filter, "catalog", container)
        }else { // we want the object
            var newTab = null
            var ids = objectids.split("|")
            for ( var i=0; i < objectids.length; ++i) {
                var type = mastercatalog.id2type(ids[i])
                if ( !type) // unknow type, we can not show it
                    continue


                var resource = mastercatalog.id2Resource(ids[i],0)
                if ( resource.typeName === "catalog" || ((resource.typeName !== "rastercoverage" || options === undefined) && resource.hasExtendedType("catalog"))){ // object as container case // TODO: improve this
                    filter = "container='" + resource.url + "'"
                    newTab = datapanesplit.changePanel(filter, "catalog",resource.url)
                }else { // object as 'real' data case
                    filter = "itemid=" + resource.id
                    // try to find a suitable data pane for it 
                    if ( newTab && resource.typeName.indexOf("coverage")!== -1){
                        newTab.item.addDataSource(filter,resource.name,resource.typeName,options)
                    }else {
                        newTab = datapanesplit.newPanel(filter, resource.typeName,resource.url,"other",options)
                    }
                    if ( newTab == null){ // we dont have a seperate pane for it so it is an object with only metadata to show
                        mastercatalog.setSelectedObjects(ids[i])
                        bigthing.getWorkbenchPane("objectproperties","visible");
                    }
                }
                resource.suicide()
            }

        }
    }

    function addDataSource(filter, sourceName, sourceType){
        var url = sourceName
        if ( currentCatalog)
            currentCatalog.makeParent(0) 
        currentCatalog = mastercatalog.newCatalog(url,filter)
        if ( currentCatalog){
            currentCatalog.makeParent(catalogViews)
            createParameters = [filter, sourceName, sourceType]
            mastercatalog.add2history(url);
            return currentCatalog.id
        }
        return -1

    }

    function setSelected(objectid){
	    var currentSelection = mastercatalog.selectedIds()
        mastercatalog.setSelectedObjects("")
		
        var resources = currentCatalog.resources
        var sz = resources.length
        var selectedIds = ""
        if ( !uicontext.keyPressed(Qt.Key_Control) &&  !uicontext.keyPressed(Qt.Key_Shift))    {
            selectedIds = objectid
            for(var i = 0; i < sz; ++i){
                var resource = resources[i]
				if ( resource.id == objectid)
                resource.isSelected=resource.id == objectid
            }
        }else if ( uicontext.keyPressed(Qt.Key_Shift)){
            var startRange = false
            for(var j = 0; j < sz; ++j){
                if ( resources[j].isSelected){
                    startRange = !startRange;
                    selectedIds = selectedIds == "" ? resources[j].id : selectedIds + "|" + resources[j].id
                }else {
                    if ( startRange){
                        selectedIds = selectedIds + "|" + resources[j].id
                        resources[j].isSelected = true
                    }
                    else {
                        resources[j].isSelected=false
                    }
                }
            }
        } else if ( uicontext.keyPressed(Qt.Key_Control)){
		  var oldList = currentSelection.split("|")
		  selectedIds = currentSelection
          for(var k = 0; k < sz; ++k){
              if ( resources[k].isSelected){
			    var found = false
			    var id = resources[k].id
				for(var p=0; p < oldList.length && !found; ++p){
					if ( oldList[p] == id){
					    found = true
					}
				}
				if (!found){
					selectedIds = selectedIds == "" ? id : selectedIds + "|" + id
				}
              }
          }
        }
        mastercatalog.setSelectedObjects(selectedIds)
    }

    function setResources(){
        if ( currentCatalog){
            return currentCatalog.resources
		}
    }

    function iconsource(name) {
        if ( name.indexOf("/") !== -1)
            return name
        if ( name === "")
            name = "redbuttonr.png"

        var iconP = "../../images/" + name
        return iconP
    }
}
