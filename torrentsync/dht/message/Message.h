#pragma once

#include <string>
#include <memory>

#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/Constants.h>

#include <boost/variant.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

class MessageException : public std::runtime_error
{
public:
    MessageException(
            const std::string& what,
            ErrorType::error_type error ) : std::runtime_error(what), _error(error) {}

    ErrorType::error_type errorType() const { return _error; }

private:
    ErrorType::error_type _error;
};

using namespace torrentsync;

//! Abstract class representing every message
class Message
{
public:
    ~Message() = default;
    Message(Message&&) = default;
    Message(const Message&) = default;

    //! returns the type of the message
    //! @return a member of Type namespace
    //! @throw MessageException in case the field is not available.
    const utils::Buffer getType() const;

    //! Returns a buffer containing the transaction ID of the message.
    //! Must be reimplemented in every subclass
    //! @return transaction id
    const utils::Buffer getTransactionID() const;

    //! returns the node address of the remote node. Must be implemented
    //! by every subclass
    //! @return a NodeData instance
    //! @throw MessageException in case the data is not available or
    //! the message is an error (it's mandatory otherwise).
    const utils::Buffer getID() const;

    //! returns an optional buffer from the data map if found.
    const boost::optional<utils::Buffer> find(
        const utils::Buffer& key) const;

    //! converts the message to a human readable representation
    const std::string string() const;

    //! move assignment
    Message& operator=( Message&& ) = default;
    Message& operator=( const Message& ) = default;

protected:
    //! Default constructor
    Message() = default;

    //! Constructor
    Message(const DataMap& data);

private:
    //! Map containing all the data for the message
    DataMap _data;
};

} /* message */
} /* dht */
} /* torrentsync */

std::ostream& operator<<( std::ostream&, const torrentsync::dht::message::Message& );
