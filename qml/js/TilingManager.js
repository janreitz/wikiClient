var tileCount = 1;
// Add ColumnLayout under ParentLayout
// Reassign existing Item to ColumnLayout
// Create new Item under ColumnLayout

function verticalSplit(parentLayout, existingItem, newItem, props)
{
    ++tileCount;
    var newLayoutId = "columnLayout" + tileCount
    props.objectName = "tile_" + tileCount

    createNewItem("qrc:/qml/components/MyColumnLayout.qml",
                  parentLayout,
                  {id: newLayoutId, objectName: newLayoutId},
                  function(obj) {
                      existingItem.parent = obj;
                      createNewItem(newItem, obj, props);
                  });
}

function horizontalSplit(parentLayout, existingItem, newItem, props)
{
    ++tileCount;
    var newLayoutId = "rowLayout" + tileCount

    createNewItem("qrc:/qml/components/MyRowLayout.qml",
                  parentLayout,
                  {id: newLayoutId, objectName: newLayoutId},
                  function(obj) {
                      existingItem.parent = obj;
                      createNewItem(newItem, obj, props);
                  });
}

function createNewItem(comp, parent, props, onCreateCallback = 0)
{
    comp = Qt.createComponent(comp);
    if (comp.status === Component.Ready) {
        finishCreation(comp, parent, props, onCreateCallback);
    }
    else if (comp.status === Component.Null) {
        console.log("TilingManager.js::createNewItem -> comp.status == Null");
    }
    else if (comp.status === Component.Error) {
        console.log("TilingManager.js::createNewItem -> comp.status == Error: " + comp.errorString());
    }
    else {
        comp.statusChanged.connect(function() {
        finishCreation(comp, parent, props, onCreateCallback)
        });
    }
}

function finishCreation(comp, parent, props, onCreateCallback = 0) {
    if (comp.status === Component.Ready) {
        var obj = comp.createObject(parent, props);
        if (obj === null) {
            // Error Handling
            console.log("Error creating object under " + parent);
        }
        else if (onCreateCallback){
            onCreateCallback(obj);
        }
    }
    else if (comp.status === Component.Error) {
            // Error Handling
            console.log("Error loading component:", comp.errorString());
    }
}
