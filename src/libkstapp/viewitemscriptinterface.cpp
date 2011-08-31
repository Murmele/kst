/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2011 Joshua Netterfield                               *
 *                   joshua.netterfield@gmail.com                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "viewitemscriptinterface.h"
#include "viewitem.h"
#include "lineedititem.h"
#include "buttonitem.h"

namespace Kst {

QByteArrayList LayoutTabSI::commands() {
    QByteArrayList ba;
    ba<<"setLayoutHorizontalMargin("<<"getLayoutHorizontalMargin()"<<"setLayoutVerticalMargin("<<
        "getLayoutVerticalMargin()"<<"setLayoutHorizontalSpacing("<<"getLayoutHorizontalSpacing()"<<
        "setLayoutVerticalSpacing("<<"getLayoutVerticalSpacing()";
    return ba;
}
QString LayoutTabSI::doCommand(QString x) {
    if(x.startsWith("getLayout")) {
        x.remove(0,9);
        QSizeF f;
        if(x.contains("Margin")) {
            f=vi->layoutMargins();
        } else if(x.contains("Spacing")) {
            f=vi->layoutSpacing();
        } else {
            return "";
        }
        if(x.contains("Horizontal")) {
            return QString::number(f.width());
        } else if(x.contains("Vertical")) {
            return QString::number(f.height());
        }
    } else if(x.startsWith("setLayout")) {
        x.remove(0,9);
        QSizeF f;
        if(x.contains("Margin")) {
            f=vi->layoutMargins();
        } else if(x.contains("Spacing")) {
            f=vi->layoutSpacing();
        } else {
            return "";
        }
        if(x.contains("Horizontal")) {
            x.remove(0,x.indexOf("("));
            x.remove(x.indexOf(")"),99999);
            f.setWidth(x.toInt());
        } else if(x.contains("Vertical")) {
            x.remove(0,x.indexOf("("));
            x.remove(x.indexOf(")"),99999);
            f.setWidth(x.toInt());
        } else {
            return "";
        }

        if(x.contains("Margin")) {
            vi->setLayoutMargins(f);
        } else if(x.contains("Spacing")) {
            vi->setLayoutSpacing(f);
        }
        return "Done";
    }
    return "";
}

QByteArrayList FillTabSI::commands() {
    QByteArrayList ba;
    ba<<"setFillColor("<<"setIndexOfFillStyle(";
    return ba;
}
QString FillTabSI::doCommand(QString x) {
    if(!x.startsWith("setFillColor")&&!x.startsWith("setIndexOfFillStyle(")) {
        return "";
    }
    QBrush b=item->brush();
    QColor this_color = (x.startsWith("setFillColor(")&&x!="setFillColor()") ? QColor(x.remove("setFillColor(").remove(")")) : b.color();
    Qt::BrushStyle this_style = b.style();
    if(x.startsWith("setIndexOfFillStyle(")) {
        x.remove("setIndexOfFillStyle(").remove(")");
        Qt::BrushStyle arr[]={Qt::NoBrush,Qt::SolidPattern,Qt::Dense1Pattern,Qt::Dense2Pattern,Qt::Dense3Pattern,Qt::Dense4Pattern,Qt::Dense5Pattern,
                              Qt::Dense6Pattern,Qt::Dense7Pattern,Qt::HorPattern,Qt::VerPattern,Qt::CrossPattern,Qt::BDiagPattern,Qt::FDiagPattern,
                              Qt::DiagCrossPattern};
        this_style=arr[x.toInt()];
    }
    b.setColor(this_color);
    b.setStyle(this_style);
    item->setBrush(b);
    return "Done";
}

QByteArrayList StrokeTabSI::commands() {
    QByteArrayList ba;
    ba<<"setIndexOfStrokeStyle("<<"setIndexOfStrokeBrushStyle("<<"setIndexOfStrokeJoinStyle("<<"setIndexOfStrokeCapStyle("<<
        "setStrokeWidth("<<"setStrokeBrushColor(";
    return ba;
}
QString StrokeTabSI::doCommand(QString x) {
    if(!x.startsWith("setIndexOfStrokeStyle")&&!x.startsWith("setIndexOfStrokeBrushStyle(")&&!x.startsWith("setIndexOfStrokeJoinStyle")&&
            !x.startsWith("setIndexOfStrokeCapStyle")&&!x.startsWith("setStrokeWidth")&&!x.startsWith("setStrokeBrushColor")) {
        return "";
    }

    QPen p=item->pen();
    QBrush b = p.brush();

    Qt::PenStyle this_style = p.style();
    qreal this_width = p.widthF();
    QColor this_brushColor = b.color();
    Qt::BrushStyle this_brushStyle = b.style();

    Qt::PenJoinStyle this_joinStyle = p.joinStyle();
    Qt::PenCapStyle this_capStyle = p.capStyle();

    if(x.startsWith("setIndexOfStrokeStyle(")) {
        x.remove("setIndexOfStrokeStyle(").remove(")");
        Qt::PenStyle arr[]={Qt::NoPen,Qt::SolidLine,Qt::DashLine,Qt::DotLine,Qt::DashDotLine,Qt::DashDotDotLine,Qt::CustomDashLine};
        this_style=arr[x.toInt()];
    } else if(x.startsWith("setIndexOfStrokeBrushStyle(")) {
        x.remove("setIndexOfStrokeBrushStyle(").remove(")");
        Qt::BrushStyle arr[]={Qt::NoBrush,Qt::SolidPattern,Qt::Dense1Pattern,Qt::Dense2Pattern,Qt::Dense3Pattern,Qt::Dense4Pattern,Qt::Dense5Pattern,
                              Qt::Dense6Pattern,Qt::Dense7Pattern,Qt::HorPattern,Qt::VerPattern,Qt::CrossPattern,Qt::BDiagPattern,Qt::FDiagPattern,
                              Qt::DiagCrossPattern};
        this_brushStyle=arr[x.toInt()];
    } else if(x.startsWith("setIndexOfStrokeJoinStyle(")) {
        x.remove("setIndexOfStrokeJoinStyle(").remove(")");
        Qt::PenJoinStyle arr[]={Qt::MiterJoin,Qt::BevelJoin,Qt::RoundJoin,Qt::SvgMiterJoin};
        this_joinStyle=arr[x.toInt()];
    } else if(x.startsWith("setIndexOfStrokeCapStyle(")) {
        x.remove("setIndexOfStrokeCapStyle(").remove(")");
        Qt::PenCapStyle arr[]={Qt::FlatCap,Qt::SquareCap,Qt::RoundCap};
        this_capStyle=arr[x.toInt()];
    } else if(x.startsWith("setStrokeWidth(")) {
        this_width=x.remove("setStrokeWidth(").remove(")").toFloat();
    } else if(x.startsWith("setStrokeBrushColor(")) {
        this_brushColor=QColor(x.remove("setStrokeBrushColor(").remove(")"));
    }

    p.setStyle(this_style);
    p.setWidthF(this_width);

    b.setColor(this_brushColor);
    b.setStyle(this_brushStyle);

    p.setJoinStyle(this_joinStyle);
    p.setCapStyle(this_capStyle);
    p.setBrush(b);
#ifdef Q_WS_WIN32
    if (p.isCosmetic()) {
        p.setWidth(1);
    }
#endif

    item->setPen(p);
    return "Done";
}

QByteArrayList DimensionTabSI::commands() {
    QByteArrayList ba;
    ba<<"setRotation("<<"getRotation()"<<"setGeoX("<<"getGeoX()"<<"setGeoY("<<"getGeoY()"<<
        "setPosX("<<"getPosX()"<<"setPosY("<<"getPosY()"<<"checkFixAspectRatio()"<<
        "uncheckFixAspectRatio()"<<"fixAspectRatioIsChecked()";
    return ba;
}

QString DimensionTabSI::doCommand(QString x) {
    if(x=="fixAspectRatioIsChecked()") {
        return item->lockAspectRatio()?"true":"false";
    }
    if(!x.startsWith("setGeo")&&!x.startsWith("setPos")&&!x.contains("checkFixAspect")&&!x.contains("setRotation")) {
        return "";
    }
    qreal parentWidth;
    qreal parentHeight;
    qreal parentX;
    qreal parentY;

    if (item->parentViewItem()) {
        parentWidth = item->parentViewItem()->width();
        parentHeight = item->parentViewItem()->height();
        parentX = item->parentViewItem()->rect().x();
        parentY = item->parentViewItem()->rect().y();
    } else if (item->view()) {
        parentWidth = item->view()->width();
        parentHeight = item->view()->height();
        parentX = item->view()->rect().x();
        parentY = item->view()->rect().y();
    } else {
        Q_ASSERT_X(false,"parent test", "item has no parentview item");
        parentWidth = parentHeight = 1.0;
        parentX = parentY = 0.0;
    }

    qreal aspectRatio;
    aspectRatio = qreal(item->rect().height()) / qreal(item->rect().width());

    qreal relativeWidth = x.startsWith("setGeoX(") ? x.remove("setGeoX(").remove(")").toDouble() :item->relativeWidth();
    qreal relativeHeight = x.startsWith("setGeoY(") ? x.remove("setGeoY(").remove(")").toDouble() :item->relativeHeight();
    bool fixedAspect = x.startsWith("checkFixAspectRatio()") ? true :item->lockAspectRatio();
    fixedAspect = x.startsWith("uncheckFixAspectRatio()") ? false :item->lockAspectRatio();

    qreal width = relativeWidth * parentWidth;
    qreal height;
    if (fixedAspect) {
        height = width * aspectRatio;
        item->setLockAspectRatio(true);
    } else {
        height = relativeHeight * parentHeight;
        item->setLockAspectRatio(false);
    }

    item->setPos(parentX + (x.startsWith("setPosX(")?x.remove("setPosX(").remove(")").toDouble():item->relativeCenter().x())*parentWidth,
                 parentY + (x.startsWith("setPosY(")?x.remove("setPosY(").remove(")").toDouble():item->relativeCenter().y())*parentHeight);

    item->setViewRect(-width/2, -height/2, width, height);

    qreal rotation = x.startsWith("setRotation(") ? x.remove("setRotation(").remove(")").toDouble() :item->rotationAngle();

    QTransform transform;
    transform.rotate(rotation);

    item->setTransform(transform);
    item->updateRelativeSize();
    return "Done.";
}

ViewItemSI::ViewItemSI(ViewItem *it) : layout(new LayoutTabSI), dim(new DimensionTabSI), fill(new FillTabSI), stroke(new StrokeTabSI) {
    layout->vi=it;
    dim->item=it;
    fill->item=it;
    stroke->item=it;
}

QByteArrayList ViewItemSI::commands() {
    return layout->commands()<<dim->commands()<<fill->commands()<<stroke->commands()<<(dynamic_cast<LineEditItem*>(layout->vi)||dynamic_cast<ButtonItem*>(layout->vi)?"setText(":"");
}

QString ViewItemSI::doCommand(QString x) {
    QString v=layout->doCommand(x);
    if(v.isEmpty()) {
        v=dim->doCommand(x);
    }
    if(v.isEmpty()) {
        v=fill->doCommand(x);
    }
    if(v.isEmpty()) {
        v=stroke->doCommand(x);
    }
    if(v.isEmpty()&&x.startsWith("setText(")) {
        if(dynamic_cast<ButtonItem*>(layout->vi)) {
            dynamic_cast<ButtonItem*>(layout->vi)->setText(x.remove("setText(").remove(")"));
            v="Done";
        } else if(dynamic_cast<LineEditItem*>(layout->vi)) {
            dynamic_cast<LineEditItem*>(layout->vi)->setText(x.remove("setText(").remove(")"));
            v="Done";
        }
    }
    return v.isEmpty()?"No command":v;
}

bool ViewItemSI::isValid() {
    return dim->item;
}

QByteArray ViewItemSI::getHandle() {
    return ((QString)("Finished editing "%dim->item->Name())).toAscii();
}

}
