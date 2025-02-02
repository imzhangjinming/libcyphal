/// @copyright Copyright Amazon.com Inc. and its affiliates. All Rights Reserved.
/// @file
/// Cyphal Transport Interface Wrapper for sending message over a UDP bus in POSIX

#ifndef POSIX_LIBCYPHAL_WRAPPERS_CAN_SUBSCRIBER_HPP_INCLUDED
#define POSIX_LIBCYPHAL_WRAPPERS_CAN_SUBSCRIBER_HPP_INCLUDED

#include <libcyphal/transport/id_types.hpp>
#include <libcyphal/transport/listener.hpp>
#include <libcyphal/transport/metadata.hpp>
#include <libcyphal/types/status.hpp>
#include "posix/libcyphal/wrappers/can/base.hpp"

namespace libcyphal
{
namespace wrappers
{
namespace can
{

/// @brief CANSubscriber is a wrapper around the tasks needed to send messages
class Subscriber final : public Base
{
public:
    transport::Listener& listener_;

    /// @brief Constructor for CANSubscriber which is a wrapper around the tasks needed to send messages
    /// @param[in] can_interface CAN Interface name to use
    /// @param[in] node_id The desired NodeID of the Transport
    /// @param[in] listener Listener object providing custom triggers by the user
    Subscriber(const char*                       can_interface,
               const NodeID                      node_id,
               transport::Listener&              listener,
               cetl::pf17::pmr::memory_resource* resource)
        : Base(can_interface, node_id, resource)
        , listener_{listener}
    {
    }

    /// Destructor
    ~Subscriber() = default;

    /// @brief Initializes everything needed to send frames
    Status initialize() override
    {
        Status result{};
        result = interface_.initializeInput();
        if (result.isFailure())
        {
            return result;
        }
        return Base::initialize();
    }

    /// @brief Register a subject ID for receiving messages on
    /// @param[in] subject_id Subject ID to subscribe to
    Status registerSubjectId(const PortID subject_id) const noexcept
    {
        return can_->registerSubscription(subject_id, transport::TransferKindMessage);
    }

    /// @brief Receives any incoming frames and triggers the listener as needed
    Status receive() const
    {
        return can_->processIncomingTransfers(listener_);
    }
};

}  // namespace can
}  // namespace wrappers
}  // namespace libcyphal

#endif  // POSIX_LIBCYPHAL_WRAPPERS_CAN_SUBSCRIBER_HPP_INCLUDED
