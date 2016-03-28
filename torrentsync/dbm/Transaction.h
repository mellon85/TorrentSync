#pragma once

#include <torrentsync/dbm/DatastoreImpl.h>

namespace torrentsync
{
namespace dbm
{

class DatastoreImpl;

class Transaction
{
public:
    Transaction(DatastoreImpl& impl);

    ~Transaction();

    Transaction(Transaction&& impl) = default;
    Transaction& operator=(Transaction&&) = default;

    Transaction(const Transaction& impl) = delete;
    Transaction& operator=(const Transaction&) = delete;

    void commit();

private:
    DatastoreImpl& impl;
};

} /* dbm */
} /* torrentsync */
