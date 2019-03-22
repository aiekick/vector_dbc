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

#include <algorithm>
#include <cmath>

#include <Vector/DBC/Signal.h>

namespace Vector {
namespace DBC {

Signal::Signal() :
    name(),

    /* multiplexer indicator */
    multiplexor(Multiplexor::NoMultiplexor),
    multiplexerSwitchValue(0),

    /* position */
    startBit(0),
    bitSize(0),
    byteOrder(ByteOrder::BigEndian),
    valueType(ValueType::Unsigned),

    /* raw/physical conversion */
    factor(0.0),
    offset(0.0),
    minimum(0.0),
    maximum(0.0),

    /* unit */
    unit(),

    /* receivers */
    receivers(),

    /* value type and description */
    extendedValueType(Signal::ExtendedValueType::Undefined),
    valueDescriptions(),
    type(),

    /* comments and attributes */
    comment(),
    attributeValues(),

    /* extended multiplexors */
    extendedMultiplexors()
{
    /* nothing to do here */
}

double Signal::rawToPhysicalValue(double rawValue)
{
    /* physicalValue = rawValue * factor + offset */
    return rawValue * factor + offset;
}

double Signal::physicalToRawValue(double physicalValue)
{
    /* rawValue = (physicalValue - offset) / factor */
    return (physicalValue - offset) / factor;
}

double Signal::minimumRawValue()
{
    /* calculate minimum raw value */
    double minimumRawValue = 0.0;
    switch (extendedValueType) {
    case ExtendedValueType::Undefined:
    case ExtendedValueType::Integer:
        if (valueType == ValueType::Signed) {
            minimumRawValue = -(2 << (bitSize - 2)); // bitSize-- because shift instead of pow
        } else {
            minimumRawValue = 0.0;
        }
        break;

    case ExtendedValueType::Float:
        minimumRawValue = 3.4e-38;
        break;

    case ExtendedValueType::Double:
        minimumRawValue = 1.7e-308;
        break;
    }
    return minimumRawValue;
}

double Signal::maximumRawValue()
{
    /* calculate maximum raw value */
    double maximumRawValue = 0.0;
    switch (extendedValueType) {
    case ExtendedValueType::Undefined:
    case ExtendedValueType::Integer:
        if (valueType == ValueType::Signed) {
            maximumRawValue = (2 << (bitSize - 2)) - 1; // bitSize-- because shift instead of pow
        } else {
            maximumRawValue = (2 << (bitSize - 1)) - 1; // bitSize-- because shift instead of pow
        }
        break;

    case ExtendedValueType::Float:
        maximumRawValue = 3.4e38;
        break;

    case ExtendedValueType::Double:
        maximumRawValue = 1.7e308;
        break;
    }
    return maximumRawValue;
}

uint64_t Signal::decode(std::vector<uint8_t> & data)
{
    /* safety check */
    if (bitSize == 0) {
        return 0;
    }

    /* copy bits */
    uint64_t retVal = 0;
    if (byteOrder == ByteOrder::BigEndian) {
        /* preparation */
        const unsigned int bitMask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
        unsigned int lowerBitSize = startBit % 8;
        unsigned int upperBitSize = 8 - lowerBitSize;

        /* init counters */
        unsigned int srcBit = startBit;
        unsigned int dstBit = 0;
        unsigned int remainingBits = bitSize;

        /* start with LSB */
        while (remainingBits > 0) {
            /* copy upper part */
            if ((upperBitSize > 0) && (remainingBits > 0)) {
                if (remainingBits < upperBitSize) {
                    upperBitSize = remainingBits;
                }
                uint64_t part = (data[srcBit / 8] >> lowerBitSize) & bitMask[upperBitSize];
                retVal |= part << dstBit;
                srcBit -= 8 + lowerBitSize;
                dstBit += upperBitSize;
                remainingBits -= upperBitSize;
            }

            /* copy lower part */
            if ((lowerBitSize >> 0) && (remainingBits > 0)) {
                if (remainingBits < lowerBitSize) {
                    lowerBitSize = remainingBits;
                }
                uint64_t part = (data[srcBit / 8]) & bitMask[lowerBitSize];
                retVal |= part << dstBit;
                srcBit += upperBitSize;
                dstBit += lowerBitSize;
                remainingBits -= lowerBitSize;
            }
        }
    } else {
        /* preparation */
        const unsigned int bitMask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
        unsigned int lowerBitSize = startBit % 8;
        unsigned int upperBitSize = 8 - lowerBitSize;

        /* init counters */
        unsigned int srcBit = startBit;
        unsigned int dstBit = 0;
        unsigned int remainingBits = bitSize;

        /* start with LSB */
        while (remainingBits > 0) {
            /* copy upper part */
            if ((upperBitSize > 0) && (remainingBits > 0)) {
                if (remainingBits < upperBitSize) {
                    upperBitSize = remainingBits;
                }
                uint64_t part = (data[srcBit / 8] >> lowerBitSize) & bitMask[upperBitSize];
                retVal |= part << dstBit;
                srcBit += upperBitSize;
                dstBit += upperBitSize;
                remainingBits -= upperBitSize;
            }

            /* copy lower part */
            if ((lowerBitSize >> 0) && (remainingBits > 0)) {
                if (remainingBits < lowerBitSize) {
                    lowerBitSize = remainingBits;
                }
                uint64_t part = (data[srcBit / 8]) & bitMask[lowerBitSize];
                retVal |= part << dstBit;
                srcBit += lowerBitSize;
                dstBit += lowerBitSize;
                remainingBits -= lowerBitSize;
            }
        }
    }

    /* if signed, then fill all bits above MSB with 1 */
    if (valueType == ValueType::Signed) {
        if (retVal & (1 << (bitSize - 1))) {
            for (unsigned int i = bitSize; i < 8 * sizeof(retVal); ++i) {
                retVal |= (1ULL << i);
            }
        }
    }

    return retVal;
}

void Signal::encode(std::vector<uint8_t> & data, uint64_t rawValue)
{
    /* safety check */
    if (bitSize == 0) {
        return;
    }

    /* copy bits */
    if (byteOrder == ByteOrder::BigEndian) {
        /* start with MSB */
        unsigned int srcBit = startBit;
        unsigned int dstBit = bitSize - 1;
        for (unsigned int i = 0; i < bitSize; ++i) {
            /* copy bit */
            if (rawValue & (1ULL << dstBit)) {
                data[srcBit / 8] |= (1 << (srcBit % 8));
            } else {
                data[srcBit / 8] &= ~(1 << (srcBit % 8));
            }

            /* calculate next position */
            if ((srcBit % 8) == 0) {
                srcBit += 15;
            } else {
                --srcBit;
            }
            --dstBit;
        }
    } else {
        /* start with LSB */
        unsigned int srcBit = startBit;
        unsigned int dstBit = 0;
        for (unsigned int i = 0; i < bitSize; ++i) {
            /* copy bit */
            if (rawValue & (1ULL << dstBit)) {
                data[srcBit / 8] |= (1 << (srcBit % 8));
            } else {
                data[srcBit / 8] &= ~(1 << (srcBit % 8));
            }

            /* calculate next position */
            ++srcBit;
            ++dstBit;
        }
    }
}

std::ostream & operator<<(std::ostream & os, const Signal & signal)
{
    /* Name */
    os << " SG_ " << signal.name << ' ';

    /* Multiplexed Signal, Multiplexor Switch/Signal */
    switch (signal.multiplexor) {
    case Signal::Multiplexor::NoMultiplexor:
        os << ' ';
        break;
    case Signal::Multiplexor::MultiplexedSignal:
        os << 'm' << signal.multiplexerSwitchValue;
        break;
    case Signal::Multiplexor::MultiplexorSwitch:
        os << 'M';
        break;
    }
    os << ": ";

    /* Start Bit, Size, Byte Order, Value Type */
    os << signal.startBit << '|' << signal.bitSize << '@' << char(signal.byteOrder) << char(signal.valueType);

    /* Factor, Offset */
    os << " (" << signal.factor << ',' << signal.offset << ')';

    /* Minimum, Maximum */
    os << " [" << signal.minimum << '|' << signal.maximum << ']';

    /* Unit */
    os << " \"" << signal.unit << "\" ";

    /* Receivers */
    if (signal.receivers.empty()) {
        os << "Vector__XXX";
    } else {
        for (auto & receiver : signal.receivers) {
            os << " " << receiver;
        }
    }
    os << endl;

    return os;
}

}
}
