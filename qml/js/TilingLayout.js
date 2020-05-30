var tileCount = 100;
var dividerUrl = "qrc:/qml/components/TilingLayout/TileDivider.qml"
var tileUrl = "qrc:/qml/components/TilingLayout/Tile.qml"
var dividerThickness = 10;
var backgroundColor = "#2d2d2d"

function verticalSplit(existingTile, newContentItem = 0) {

    if (!existingTile) { return; }

    var divider
    var newTileAbove
    var newTileBelow

    var dividerComp = Qt.createComponent(dividerUrl);
    var newTileComp = Qt.createComponent(tileUrl);
    if (newTileComp.status === Component.Ready) {
        tileCount++
        newTileAbove = finishCreation(newTileComp, existingTile, {color: backgroundColor, objectName: "tile_a_" + tileCount});
        tileCount++
        newTileBelow = finishCreation(newTileComp, existingTile, {color: backgroundColor, objectName: "tile_b_" + tileCount});
    }

    var dividerProps = {
        //height: dividerThickness,
        isVDivider: true,
        objectName: "divider_v_"+(tileCount-1)+"_"+tileCount,
    }
    if (dividerComp.status === Component.Ready) {
        divider = finishCreation(dividerComp, existingTile, dividerProps);
    }
    if (divider && newTileAbove && newTileBelow) {
        newTileAbove.anchors.top = existingTile.top;
        newTileAbove.anchors.right = existingTile.right;
        newTileAbove.anchors.left = existingTile.left;
        newTileAbove.anchors.bottom = divider.top;
        // Reassign item
        newTileAbove.contentItem = existingTile.contentItem // JavaScript copies here
        existingTile.contentItem = null
        newTileAbove.contentItem.parent = newTileAbove

        divider.anchors.left = existingTile.left;
        divider.anchors.right = existingTile.right
        divider.y = (existingTile.height - dividerThickness)/2

        newTileBelow.anchors.top = divider.bottom;
        newTileBelow.anchors.right = existingTile.right;
        newTileBelow.anchors.left = existingTile.left;
        newTileBelow.anchors.bottom = existingTile.bottom;

        if (newContentItem) {
            newTileBelow.contentItem = newContentItem
            newContentItem.parent = newTileBelow
        }
    }
}

function horizontalSplit(existingTile, newContentItem = 0) {

    if (!existingTile) { return; }

    var divider
    var newTileLeft
    var newTileRight

    var dividerComp = Qt.createComponent(dividerUrl);
    var newTileComp = Qt.createComponent(tileUrl);

    if (newTileComp.status === Component.Ready) {
        tileCount++
        newTileLeft = finishCreation(newTileComp, existingTile, {color: backgroundColor, objectName: "tile_l_" + tileCount});
        tileCount++
        newTileRight = finishCreation(newTileComp, existingTile, {color: backgroundColor, objectName: "tile_r_" + tileCount});
    }

    var dividerProps = {
        //width: dividerThickness,
        isVDivider: false,
        objectName: "divider_h_"+(tileCount-1)+"_"+tileCount,
        //color: dividerColor
    }
    if (dividerComp.status === Component.Ready) {
        divider = finishCreation(dividerComp, existingTile, dividerProps);
    }

    if (divider && newTileLeft && newTileRight) {
        newTileLeft.anchors.top = existingTile.top;
        newTileLeft.anchors.right = divider.left;
        newTileLeft.anchors.left = existingTile.left;
        newTileLeft.anchors.bottom = existingTile.bottom;
        existingTile.contentItem.parent = newTileLeft
        // Reassign item
        newTileLeft.contentItem = existingTile.contentItem // JavaScript copies here
        existingTile.contentItem = null
        newTileLeft.contentItem.parent = newTileLeft

        divider.anchors.top = existingTile.top;
        divider.anchors.bottom = existingTile.bottom
        divider.x = (existingTile.width - dividerThickness)/2

        newTileRight.anchors.top = existingTile.top;
        newTileRight.anchors.right = existingTile.right;
        newTileRight.anchors.left = divider.right;
        newTileRight.anchors.bottom = existingTile.bottom;

        if (newContentItem) {
            newTileRight.contentItem = newContentItem
            newContentItem.parent = newTileRight
        }
    }
}

function finishCreation(comp, parent, props = {}, onCreateCallback = 0) {
    if (comp.status === Component.Ready) {
        var obj = comp.createObject(parent, props);
        if (obj === null) {
            // Error Handling
            console.log("Error creating object under " + parent);
            return null;
        }
        else if (onCreateCallback){
            onCreateCallback(obj);
            return obj;
        }
        return obj;
    }
    else if (comp.status === Component.Error) {
            // Error Handling
            console.log("Error loading component:", comp.errorString());
    }
}
