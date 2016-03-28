#pragma once

namespace torrentsync
{
namespace dbm
{

class Datastore;

class Schema
{
public:
    enum Version {
       V_1
    };

    Schema(const Datastore& impl);

    Schema(const Datastore& impl, Version version);

    ~Schema() = default;

    Schema(const Schema&) = delete;
    Schema& operator=(const Schema&) = delete;

    Schema(Schema&&) = default;
    Schema& operator=(Schema&&) = default;

private:
    void make_schema_v1() const;

    const Datastore& impl;

    Version version;
};

} /* dbm */
} /* torrentsync */
