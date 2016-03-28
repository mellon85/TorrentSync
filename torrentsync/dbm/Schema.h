#include <torrentsync/dbm/DatastoreImpl.h>

namespace torrentsync
{
namespace dbm
{

class Schema
{
public:
    enum Version {
       V_1
    };

    Schema(const DatastoreImpl& impl);

    Schema(const DatastoreImpl& impl, Version version);

    ~Schema() = default;

    Schema(const Schema&) = delete;
    Schema(Schema&&) = delete;

    Schema& operator=(Schema&&) = delete;
    Schema& operator=(const Schema&) = delete;

private:
    void make_schema_v1();

};

} /* dbm */
} /* torrentsync */
