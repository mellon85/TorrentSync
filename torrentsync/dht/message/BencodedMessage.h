/*
 * BencodedMessage.h
 *
 *  Created on: 15/dic/2013
 *      Author: rustyrick
 */

#ifndef BENCODEDMESSAGE_H_
#define BENCODEDMESSAGE_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include "boost/variant.hpp"
#include "boost/shared_ptr.hpp"

typedef std::string BencodeMessageKey;
typedef std::vector<uint8_t> BencodeByteString;
typedef std::map<BencodeMessageKey,boost::recursive_wrapper<boost::recursive_variant_> > BencodeMap;
typedef std::list<boost::recursive_wrapper<boost::recursive_variant_> > BencodeList;
typedef boost::variant<std::string, BencodeByteString, BencodeList, BencodeMap > BencodeValue;

class BencodedMessage
{
public:
     BencodedMessage(boost::shared_ptr<BencodeValue> iMap = boost::shared_ptr<BencodeValue>(new BencodeValue())) : _root(iMap) {};
     virtual ~BencodedMessage() {};
     bool add_element(const std::string iMessage);
     bool add_element(const BencodeByteString& iByteString);
     bool add_element(const BencodeValue& iVal); // only if root is a list otherwise throw
     bool add_element(const BencodeMessageKey& iKey, const BencodeValue& iVal); // only if root is a map otherwise throw
     //... non so, inventati un interfaccia che sia utile per metterci dentro dati :)

    template <class Stream>
    Stream serialize( Stream out ); // per avere la rappresentazione in bencode

private:
    boost::shared_ptr<BencodeValue> _root;
};

#endif /* BENCODEDMESSAGE_H_ */
