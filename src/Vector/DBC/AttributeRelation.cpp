/*
 * Copyright (C) 2013-2019 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file is part of Tobias Lorenz's Toolkit.
 *
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Tobias Lorenz.
 *
 * GNU General Public License 3.0 Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#include <Vector/DBC/AttributeRelation.h>

namespace Vector {
namespace DBC {

AttributeRelation::AttributeRelation() :
    Attribute(),
    nodeName(),
    environmentVariableName(),
    messageId(0),
    signalName()
{
    /* nothing to do here */
}

bool AttributeRelation::operator<(const AttributeRelation & rhs) const
{
    /* compare name */
    if (name == rhs.name) {

        /* compare relationType */
        if (objectType == rhs.objectType) {

            /* relationType based optimizations */
            switch(objectType) {
            case AttributeObjectType::ControlUnitEnvironmentVariable:
                /* only compare nodeName, environmentVariableName */
                if (nodeName == rhs.nodeName) {
                    return environmentVariableName < rhs.environmentVariableName;
                } else {
                    return nodeName < rhs.nodeName;
                }
                break;

            case AttributeObjectType::NodeTxMessage:
                /* only compare nodeName, messageId */
                if (nodeName == rhs.nodeName) {
                    return messageId < rhs.messageId;
                } else {
                    return nodeName < rhs.nodeName;
                }
                break;

            case AttributeObjectType::NodeMappedRxSignal:
                /* only compare nodeName, messageId, signalName */
                if (nodeName == rhs.nodeName) {
                    if (messageId == rhs.messageId) {
                        return signalName < rhs.signalName;
                    } else {
                        return messageId < rhs.messageId;
                    }
                } else {
                    return nodeName < rhs.nodeName;
                }
                break;
            }
        } else {
            return objectType < rhs.objectType;
        }
    } else {
        return name < rhs.name;
    }

    return false;
}

}
}
