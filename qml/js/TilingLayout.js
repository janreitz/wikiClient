var tileCount = 100;
var dividerUrl = "qrc:/qml/components/TilingLayout/TileDivider.qml"
var tileUrl = "qrc:/qml/components/TilingLayout/Tile.qml"
var dividerThickness = 10;
var backgroundColor = "#2d2d2d"

function verticalSplit(existingTile, newContentItem) {

    if (!existingTile) { return; }

    var divider
    var newTileTop
    var newTileBottom

    var dividerComp = Qt.createComponent(dividerUrl);
    var newTileComp = Qt.createComponent(tileUrl);
    if (newTileComp.status === Component.Ready) {
        tileCount++
        newTileTop = finishCreation(newTileComp, existingTile, {
                                          color: backgroundColor,
                                          objectName: "tile_a_" + tileCount,
                                          isTop: true
                                      });
        tileCount++
        newTileBottom = finishCreation(newTileComp, existingTile, {
                                          color: backgroundColor,
                                          objectName: "tile_b_" + tileCount,
                                          isBottom:true
                                      });
    }

    var dividerProps = {
        //height: dividerThickness,
        isVDivider: true,
        objectName: "divider_v_"+(tileCount-1)+"_"+tileCount,
    }
    if (dividerComp.status === Component.Ready) {
        divider = finishCreation(dividerComp, existingTile, dividerProps);
    }
    if (divider && newTileTop && newTileBottom) {
        // Anchoring
        newTileTop.anchors.top = existingTile.top;
        newTileTop.anchors.right = existingTile.right;
        newTileTop.anchors.left = existingTile.left;
        newTileTop.anchors.bottom = divider.top;

        divider.anchors.left = existingTile.left;
        divider.anchors.right = existingTile.right
        divider.y = (existingTile.height - dividerThickness)/2

        newTileBottom.anchors.top = divider.bottom;
        newTileBottom.anchors.right = existingTile.right;
        newTileBottom.anchors.left = existingTile.left;
        newTileBottom.anchors.bottom = existingTile.bottom;

        // Reassign contentItem
        newTileTop.contentItem = existingTile.contentItem
        existingTile.contentItem = null
        newTileTop.contentItem.parent = newTileTop

        // Assign graph relations
        existingTile.firstChild = newTileTop
        existingTile.secondChild = newTileBottom
        newTileTop.sibling = newTileBottom;
        newTileBottom.sibling = newTileTop;
        newTileTop.divider = divider;
        newTileBottom.divider = divider;

        // Assign MetaData
        existingTile.isSplitVertically = true;

        if (newContentItem) {
            newTileBottom.contentItem = newContentItem
            newContentItem.parent = newTileBottom
        }
    }
}

function horizontalSplit(existingTile, newContentItem) {

    if (!existingTile) { return; }

    var divider
    var newTileLeft
    var newTileRight

    var dividerComp = Qt.createComponent(dividerUrl);
    var newTileComp = Qt.createComponent(tileUrl);

    if (newTileComp.status === Component.Ready) {
        tileCount++
        newTileLeft = finishCreation(newTileComp, existingTile, {
                                         color: backgroundColor,
                                         objectName: "tile_l_" + tileCount,
                                         isLeft:true
                                     });
        tileCount++
        newTileRight = finishCreation(newTileComp, existingTile, {
                                          color: backgroundColor,
                                          objectName: "tile_r_" + tileCount,
                                          isRight:true
                                      });
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
        // Anchoring
        newTileLeft.anchors.top = existingTile.top;
        newTileLeft.anchors.right = divider.left;
        newTileLeft.anchors.left = existingTile.left;
        newTileLeft.anchors.bottom = existingTile.bottom;
        existingTile.contentItem.parent = newTileLeft

        divider.anchors.top = existingTile.top;
        divider.anchors.bottom = existingTile.bottom
        divider.x = (existingTile.width - dividerThickness)/2

        newTileRight.anchors.top = existingTile.top;
        newTileRight.anchors.right = existingTile.right;
        newTileRight.anchors.left = divider.right;
        newTileRight.anchors.bottom = existingTile.bottom;

        // Reassign item
        newTileLeft.contentItem = existingTile.contentItem // JavaScript copies here
        existingTile.contentItem = null
        newTileLeft.contentItem.parent = newTileLeft

        // Assign graph relations
        existingTile.firstChild = newTileLeft
        existingTile.secondChild = newTileRight
        newTileLeft.sibling = newTileRight;
        newTileRight.sibling = newTileLeft;
        newTileLeft.divider = divider;
        newTileRight.divider = divider;

        // Assign MetaData
        existingTile.isSplitHorizontally = true;

        if (newContentItem) {
            newTileRight.contentItem = newContentItem
            newContentItem.parent = newTileRight
        }
    }
}

function undoSplit(existingTile) {
    if (!existingTile.isTile) {return;}
    if (!existingTile.parent.isTile) {return;}

    var _contentItem = existingTile.contentItem
    var _parent = existingTile.parent

    if (_parent.isSplitVertically || _parent.isSplitHorizontally) {
        _contentItem.parent = _parent
        _parent.contentItem = _contentItem
        existingTile.sibling.destroy()
        existingTile.divider.destroy()

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
