#pragma once

#include <string>

#include <memory>
#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <torrentsync/dht/NodeData.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

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

//! Abstract class representing every message
class Message
{
public:
    virtual ~Message() = default;

    Message( Message&& ) = default;
    
    Message( const Message& ) = default;
    
    /*! Parse a generic message and returns an instance of it.
     * This method must be used to parse messages.
     * @param istream the input stream to read from
     * @return a shared pointer with the message
     * @throw BEncodeDecoderException in case an error is encountered while
     *  parsing
     */
    static std::shared_ptr<Message> parseMessage(
        std::istream& istream );
    static std::shared_ptr<Message> parseMessage(
        const utils::Buffer& buffer );
    static std::shared_ptr<Message> parseMessage(
        const utils::Buffer& buffer,
        const size_t size );
        
    //! returns the type of the message
    //! @return a member of Type namespace
    //! @throw MalformedMessageException in case the field is not available.
    const utils::Buffer getType() const;
 
    //! Returns a buffer containing the transaction ID of the message.
    //! Must be reimplemented in every subclass
    //! @return transaction id
    const utils::Buffer getTransactionID() const;

    //! returns the node address of the remote node. Must be implemented
    //! by every subclass
    //! @return a NodeData instance
    //! @throw MalformedMessageException in case the data is not available or
    //! the message is an error (it's mandatory otherwise).
    const utils::Buffer getID() const;

    //! returns an optional buffer from the data map if found.
    const boost::optional<utils::Buffer> find(
        const std::string& key) const;
    
    //! converts the message to a human readable representation
    const std::string string() const;
    
    //! move assignment
    Message& operator=( Message&& ) = default;

protected:
    //! Default constructor
    Message() = default;

    //! Constructor 
    Message(const DataMap& data);

    //! Map containing all the data for the message
    DataMap _data;

    //! returns an optional buffer from the data map if found.
    static const boost::optional<utils::Buffer> find(
        const std::string& key,
        const DataMap& data);
};

} /* message */
} /* dht */
} /* torrentsync */

std::ostream& operator<<( std::ostream&, const torrentsync::dht::message::Message& );
