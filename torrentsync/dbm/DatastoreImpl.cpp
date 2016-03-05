#include <torrentsync/dbm/DatastoreImpl.h>
#include <kcdb.h>

namespace torrentsync
{
namespace dbm
{

DatastoreImpl::~DatastoreImpl()
{
    db.close();
}

DatastoreImpl::DatastoreImpl(const std::string& path, bool readOnly)
{
    uint32_t flags = db.OTRYLOCK | db.OCREATE;
    if (!readOnly)
        flags = db.OWRITER;
    db.open(path, flags);
}

} /* dbm */
} /* torrentsync */
