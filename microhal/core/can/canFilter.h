/*
 * canFilter.h
 *
 *  Created on: Oct 23, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_CORE_CAN_CANFILTER_H_
#define SRC_MICROHAL_CORE_CAN_CANFILTER_H_

#include "canMessage.h"

namespace microhal {
namespace can {

namespace detail {
enum class Match { DataFrame = 0b001, RemoteFrame = 0b010, StandardIDAndExtendedID = 0b100 };

constexpr Match operator|(Match a, Match b) {
    using Type = std::underlying_type_t<Match>;
    return static_cast<Match>(static_cast<Type>(a) | static_cast<Type>(b));
}

constexpr bool operator&(Match a, Match b) {
    using Type = std::underlying_type_t<Match>;
    return static_cast<Type>(a) & static_cast<Type>(b);
}
}  // namespace detail

using detail::operator|;

class Filter {
 public:
    using Match = detail::Match;
    constexpr Filter(Message::ID id, Match match) : id(id), idMask(Message::ExtendedID{0x1FFF'FFFF}), matchType(match) {}
    constexpr Filter(Message::ID id, Message::ID idMask, Match match) : id(id), idMask(idMask), matchType(match) {}

    bool match(const Message &message) const {
        auto messageId = message.getID();

        if ((message.isDataFrame() && (matchType & Match::DataFrame)) || (message.isRemoteFrame() && (matchType & Match::RemoteFrame))) {
            if (messageId.isExtended() == id.isExtended() || messageId.isStandard() == id.isStandard()) {
                if ((messageId.getID() & idMask.getID()) == (id.getID() & idMask.getID())) {
                    return true;
                }
            } else if (matchType & Match::StandardIDAndExtendedID) {
                if (messageId.isExtended()) {
                    if (((messageId.getID() >> 17) & idMask.getID()) == (id.getID() & idMask.getID())) {
                        return true;
                    }
                }
                if (messageId.isStandard()) {
                    if (((messageId.getID() << 17) & idMask.getID()) == (id.getID() & idMask.getID())) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    Message::ID getID() const { return id; }
    Message::ID getIDMask() const { return idMask; }
    Match getMatchType() const { return matchType; }

    bool operator==(const Filter &b) const {
        if (id != b.id) return false;
        if (idMask != b.idMask) return false;
        if (matchType != b.matchType) return false;
        return true;
    }

 private:
    Message::ID id;
    Message::ID idMask;
    Match matchType;
};  // namespace can
}  // namespace can
}  // namespace microhal

#endif /* SRC_MICROHAL_CORE_CAN_CANFILTER_H_ */
