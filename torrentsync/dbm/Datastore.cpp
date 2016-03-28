#include <torrentsync/dbm/Datastore.h>
#include <torrentsync/dbm/DatastoreImpl.h>

namespace torrentsync
{
namespace dbm
{

Datastore::~Datastore()
{
}

Datastore::Datastore(const boost::filesystem::path& path,
                     bool readOnly)
{
    DatastoreImpl *ptr = nullptr;
    try
    {
        ptr = new DatastoreImpl(path, readOnly);
    } catch (std::exception e)
    {
        if (ptr != nullptr)
            delete ptr;
        throw e;
    }
    impl.reset(ptr);
}

} /* dbm */
} /* torrentsync */
