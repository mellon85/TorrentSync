#pragma once

class sqlite3;

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

    bool committed() const noexcept;

private:
    DatastoreImpl& impl;

    bool done;
};

} /* dbm */
} /* torrentsync */
