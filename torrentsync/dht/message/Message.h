#pragma once

#include <string>

#include <boost/shared_ptr.hpp>
#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <torrentsync/dht/NodeData.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

namespace Type
{
    extern const std::string Query;
    extern const std::string Reply;
    extern const std::string Error;
};

namespace Field
{
    // general
    extern const std::string PeerID;
    extern const std::string TransactionID;
    extern const std::string Type;
    extern const std::string Query;
    extern const std::string Reply;
    extern const std::string ErrorType;
    extern const std::string Arguments;

    // find_node
    extern const std::string Target;
    extern const std::string Nodes;

};

namespace Messages
{
    extern const std::string Ping;
    extern const std::string FindNode;
};

class MalformedMessageException : public std::runtime_error
{
public:
    MalformedMessageException( const std::string& what ) : std::runtime_error(what) {}
};

class Ping;

//! Abstract class representing every message
class Message
{
public:
    virtual ~Message() {}

    /*! Parse a generic message and returns an instance of it.
     * This method must be used to parse messages.
     * @param istream the input stream to read from
     * @return a shared pointer with the message
     * @throw BEncodeDecoderException in case an error is encountered while
     *  parsing
     */
    static boost::shared_ptr<Message> parseMessage( std::istream& istream );
    static boost::shared_ptr<Message> parseMessage( const torrentsync::utils::Buffer& buffer );
    static boost::shared_ptr<Message> parseMessage( const torrentsync::utils::Buffer& buffer, const size_t size );

    //! Returns the message type. In this way you can cast to the correct
    //! object.
    //! @return a member of the Messages namespace
    //! @throw MalformedMessageException in case the field is not available.
    const std::string getMessageType() const;

    //! returns the type of the message
    //! @return a member of Type namespace
    //! @throw MalformedMessageException in case the field is not available.
    const std::string getType() const;
 
    //! Returns a buffer containing the transaction ID of the message.
    //! Must be reimplemented in every subclass
    //! @return transaction id
    torrentsync::utils::Buffer getTransactionID() const;

    //! returns the node address of the remote node. Must be implemented
    //! by every subclass
    //! @return a NodeData instance
    //! @throw MalformedMessageException in case the data is not available or
    //! the message is an error (it's mandatory otherwise).
    torrentsync::utils::Buffer getID() const;

protected:
    //! inline constructor
    inline Message() {}

    //! Copy constructor
    inline Message(const DataMap& data) : data(data) {}

    //! Map containing all the data for the message
    DataMap data;

    //! returns an optional buffer from the data map if found.
    static boost::optional<torrentsync::utils::Buffer> find(
        const std::string& key,
        const DataMap& data);
};

} /* message */
} /* dht */
} /* torrentsync */

