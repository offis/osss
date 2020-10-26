#include "o3s/utils/osss_recon_types.h"
#include "o3s/utils/osss_user_process_descriptor.h"

namespace osss
{
namespace osssi
{

osss_context_base *
whichCurrentPermission(osss_user_process_descriptor * upd)
{
  return upd->m_current_permission;
}

osss_context_base *
whichLastContext(osss_user_process_descriptor * upd)
{
  return upd->m_context;
}

} // end namespace osssi
} // end namespace osss

// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
