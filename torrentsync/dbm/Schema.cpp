#include <torrentsync/dbm/Schema.h>
#include <torrentsync/dbm/Datastore.h>

namespace torrentsync
{
namespace dbm
{

Schema::Schema(const Datastore& _impl) : impl(_impl)
{
    Transaction t = impl.getTransaction();
    // TODO detect version
}

Schema::Schema(const Datastore& _impl, Version _version) :
    impl(_impl), version(_version)
{

}


} /* dbm */
} /* torrentsync */
