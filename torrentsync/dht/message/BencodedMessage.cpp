/*
 * BencodedMessage.cpp
 *
 *  Created on: 15/dic/2013
 *      Author: rustyrick
 */

#include "BencodedMessage.h"

bool BencodedMessage::add_element(const std::string iMessage) {
	return true;
}

bool BencodedMessage::add_element(const BencodeByteString& iByteString) {
	return true;
}

bool BencodedMessage::add_element(const BencodeValue& iVal) {
	return true;
}

bool BencodedMessage::add_element(const BencodeMessageKey& iKey, const BencodeValue& iVal) {
	return true;
}

