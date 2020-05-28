var tileCount = 100; // account for initial population

function verticalSplit(parentLayout, existingTile, newTile, newTileProps = {})
{
    ++tileCount;
    var newLayout ="qrc:/qml/components/MyColumnLayout.qml"
    var newLayoutId = "tile_" + tileCount
    var newLayoutProps = {id: newLayoutId, objectName: newLayoutId}
    newTileProps.objectName = "t1le_" + tileCount + "-item_" + tileCount

    splitTile(parentLayout, existingTile, newLayout, newLayoutProps, newTile, newLayoutProps);
}

function horizontalSplit(parentLayout, existingTile, newTile, newTileProps = {})
{
    ++tileCount;
    var newLayout ="qrc:/qml/components/MyRowLayout.qml"
    var newLayoutId = "tile_" + tileCount
    var newLayoutProps = {id: newLayoutId, objectName: newLayoutId}
    newTileProps.objectName = "t1le_" + tileCount + "-item_" + tileCount

    splitTile(parentLayout, existingTile, newLayout, newLayoutProps, newTile, newLayoutProps);
}

function splitTile(parentLayout, existingTile, newLayout, newLayoutProps, newTile, newTileProps = {})
{
    // Add ColumnLayout under ParentLayout
    // Reassign existing Item to ColumnLayout
    // Create new Item under ColumnLayout

    createNewItem(newLayout,
                  parentLayout,
                  newLayoutProps,
                  function(obj) {
                      existingTile.parent = obj;
                      createNewItem(newTile, obj, newTileProps);
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
