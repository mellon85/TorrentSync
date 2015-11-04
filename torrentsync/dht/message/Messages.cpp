#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include <boost/variant/apply_visitor.hpp>

#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/dht/message/Messages.h>

namespace bio = boost::iostreams;

typedef bio::stream<bio::array_source> array_stream;

namespace torrentsync {
namespace dht {
namespace message {

AnyMessage parseMessage(const utils::Buffer &buffer) {
  return parseMessage(buffer, buffer.size());
}

AnyMessage parseMessage(const utils::Buffer &buffer, const size_t length) {
  bio::array_source source(reinterpret_cast<const char *>(buffer.data()),
                           length);
  bio::stream<bio::array_source> in(source);
  return parseMessage(in);
}

AnyMessage parseMessage(std::istream &istream) {
  BEncodeDecoder decoder;
  try {
    decoder.parseMessage(istream);
  } catch (const BEncodeException &e) {
    std::stringstream ss;
    ss << "Couldn't parse message: " << e.what();
    throw MessageException(ss.str(), ErrorType::protocolError);
  }

  const auto type = decoder.find(Field::Type);
  if (!type)
    throw MessageException("Couldn't find message type",
                           ErrorType::protocolError);

  if (*type == Type::Query) {
    const auto msgType = decoder.find(Field::Query);
    if (!msgType)
      throw MessageException("Couldn't find message name",
                             ErrorType::protocolError);
    LOG(DEBUG, "Received a message: " << pretty_print(*msgType));

    if (*msgType == Messages::Ping) {
      return AnyMessage(query::Query(query::Ping(decoder.getData())));
    } else if (*msgType == Messages::FindNode) {
      return AnyMessage(query::Query(query::FindNode(decoder.getData())));
    } else if (*msgType == Messages::GetPeers) {
      return AnyMessage(query::Query(query::GetPeers(decoder.getData())));
    } else // @TODO must send methodUnknown error message back
           // not a malformed message.
    {
      throw MessageException("Unknown message name",
                             ErrorType::methodUnknownError);
    }
  } else if (*type == Type::Reply) {
    if (reply::isGetPeers(decoder)) {
      LOG(DEBUG, "Found GetPeers");
      return AnyMessage(reply::Reply(reply::GetPeers(decoder.getData())));
    }
    if (reply::isFindNode(decoder)) {
      LOG(DEBUG, "Found FindNode");
      return AnyMessage(reply::Reply(reply::FindNode(decoder.getData())));
    }
    LOG(WARN, "Found Ping/Announce");
    // ping and announce are indistinguisahable at this point
    // it's up to the receiver to know what are they waiting for
    return AnyMessage(reply::Reply(reply::Ping(decoder.getData())));
  } else if (*type == Type::Error) {
    return AnyMessage(reply::Reply(reply::Error(decoder.getData())));
  }

  throw MessageException("Unknown message type", ErrorType::protocolError);
}

namespace detail {

//! generic accessor to obtain the transaction id of the message
typedef struct __getTransactionID__T
    : public boost::static_visitor<utils::Buffer> {

  utils::Buffer operator()(const reply::Reply &r) const {
    return boost::apply_visitor(*this, r);
  }

  utils::Buffer operator()(const query::Query &q) const {
    return boost::apply_visitor(*this, q);
  }

  template <class T> utils::Buffer operator()(T &&m) const {
    return m.getTransactionID();
  }

} getTransactionID;

//! generic accessor to obtain the id of the message
typedef struct __getID__T : public boost::static_visitor<utils::Buffer> {

  utils::Buffer operator()(const reply::Reply &r) const {
    return boost::apply_visitor(*this, r);
  }

  utils::Buffer operator()(const query::Query &q) const {
    return boost::apply_visitor(*this, q);
  }

  template <class T> utils::Buffer operator()(T &&m) const { return m.getID(); }

} getID;

//! generic accessor to obtain the id of the message
typedef struct __string__T : public boost::static_visitor<std::string> {

  std::string operator()(const reply::Reply &r) const {
    return boost::apply_visitor(*this, r);
  }

  std::string operator()(const query::Query &q) const {
    return boost::apply_visitor(*this, q);
  }

  template <class T> std::string operator()(T &&m) const { return m.string(); }

} getString;

//! generic accessor to obtain the transaction id of the message
typedef struct __getType__T : public boost::static_visitor<utils::Buffer> {

  utils::Buffer operator()(const reply::Reply &r) const {
    return boost::apply_visitor(*this, r);
  }

  utils::Buffer operator()(const query::Query &q) const {
    return boost::apply_visitor(*this, q);
  }

  template <class T> utils::Buffer operator()(T &&m) const {
    return m.getType();
  }

} getType;

}; // detail

utils::Buffer getTransactionID(const AnyMessage &m) {
  return boost::apply_visitor(detail::getTransactionID(), m);
}

utils::Buffer getID(const AnyMessage &m) {
  return boost::apply_visitor(detail::getID(), m);
}

utils::Buffer getType(const AnyMessage &m) {
  return boost::apply_visitor(detail::getType(), m);
}

std::string getString(const AnyMessage &m) {
  return boost::apply_visitor(detail::getString(), m);
}

}; // message
}; // dht
}; // torrentsync
