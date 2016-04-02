#pragma once

namespace torrentsync
{
namespace dbm
{

class DatastoreImpl;

class Schema
{
public:
    enum Version {
       V_1
    };

    Schema(DatastoreImpl& impl);

    Schema(DatastoreImpl& impl, Version version);

    ~Schema() = default;

    Schema(const Schema&) = delete;
    Schema& operator=(const Schema&) = delete;

    Schema(Schema&&) = default;
    Schema& operator=(Schema&&) = default;

private:
    void make_schema_v1() const;

    DatastoreImpl& impl;

    Version version;
};

} /* dbm */
} /* torrentsync */
