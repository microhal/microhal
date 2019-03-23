#include "scu.h"
#include "device/chip.h"

namespace microhal {
namespace lpc43xx {
SCU *scu_ptr = (SCU *)LPC_SCU_BASE;
SCU &scu = *scu_ptr;
}  // namespace lpc43xx
}  // namespace microhal
