/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/logging.h"
#include "xenia/emulator.h"
#include "xenia/kernel/kernel_flags.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xam/xam_private.h"
#include "xenia/xbox.h"

namespace xe {
namespace kernel {
namespace xam {

static uint32_t dash_context_ = 0;  // don't know what this does right now

void XamSetDashContext_entry(dword_t context) {
  dash_context_ = context.value();
}
DECLARE_XAM_EXPORT1(XamSetDashContext, kMisc, kStub);

dword_result_t XamIsSystemTitleId_entry(dword_t title_id) {
  if (title_id == 0) {
    return true;
  }
  if ((title_id & 0xFF000000) == 0x58000000u) {
    return (title_id & 0xFF0000) != 0x410000;  // if 'X' but not 'XA' (XBLA)
  }
  return (title_id >> 16) == 0xFFFE;  // FFFExxxx are always system apps
}
DECLARE_XAM_EXPORT1(XamIsSystemTitleId, kMisc, kImplemented);

dword_result_t XamIsXbox1TitleId_entry(dword_t title_id) {
  if (title_id == 0xFFFE0000) {
    return true;  // Xbox OG dashboard ID?
  }
  if (title_id == 0 || (title_id & 0xFF000000) == 0xFF000000) {
    return false;  // X360 system apps
  }
  return (title_id & 0x7FFF) < 0x7D0;  // lower 15 bits smaller than 2000
}
DECLARE_XAM_EXPORT1(XamIsXbox1TitleId, kMisc, kImplemented);

dword_result_t XamIsSystemExperienceTitleId_entry(dword_t title_id) {
  if ((title_id >> 16) == 0x584A) {  // 'XJ'
    return true;
  }
  if ((title_id >> 16) == 0x5848) {  // 'XH'
    return true;
  }
  return title_id == 0x584E07D2 || title_id == 0x584E07D1;  // XN-2002 / XN-2001
}
DECLARE_XAM_EXPORT1(XamIsSystemExperienceTitleId, kMisc, kImplemented);

dword_result_t XamGetDashContext_entry() { return dash_context_; }
DECLARE_XAM_EXPORT1(XamGetDashContext, kMisc, kStub);

dword_result_t XamFitnessClearBodyProfileRecords_entry(
    unknown_t r3, unknown_t r4, unknown_t r5, unknown_t r6, unknown_t r7,
    unknown_t r8, unknown_t r9) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessClearBodyProfileRecords, kMisc, kStub);
dword_result_t XamSetLastActiveUserData_entry(unknown_t r3, unknown_t r4,
                                              unknown_t r5, unknown_t r6,
                                              unknown_t r7, unknown_t r8,
                                              unknown_t r9) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamSetLastActiveUserData, kMisc, kStub);
dword_result_t XamGetLastActiveUserData_entry(unknown_t r3, unknown_t r4,
                                              unknown_t r5, unknown_t r6,
                                              unknown_t r7, unknown_t r8,
                                              unknown_t r9) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamGetLastActiveUserData, kMisc, kStub);
dword_result_t XamPngDecode_entry(unknown_t r3, unknown_t r4, unknown_t r5,
                                  unknown_t r6, unknown_t r7, unknown_t r8,
                                  unknown_t r9) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamPngDecode, kMisc, kStub);
dword_result_t XamPackageManagerGetExperienceMode_entry(
    unknown_t r3, unknown_t r4, unknown_t r5, unknown_t r6, unknown_t r7,
    unknown_t r8, unknown_t r9) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamPackageManagerGetExperienceMode, kMisc, kStub);
dword_result_t XamGetLiveHiveValueW_entry(lpstring_t name, lpstring_t value,
                                          dword_t ch_value, dword_t unk,
                                          lpvoid_t overlapped_ptr) {
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamGetLiveHiveValueW, kMisc, kStub);

// https://github.com/jogolden/testdev/blob/master/xkelib/xam/_xamext.h
typedef enum {
  XAM_DEFAULT_IMAGE_SYSTEM = 0x0,    // "xam"     "defaultsystemimage.png"
  XAM_DEFAULT_IMAGE_DASHICON = 0x1,  // "xam"     "dashicon.png"
  XAM_DEFAULT_IMAGE_SETTINGS = 0x2,  // "shrdres"   "ico_64x_licensestore.png"
} XAM_DEFAULT_IMAGE_ID;

X_HRESULT xeXGetDefaultImage(dword_t /*XAM_DEFAULT_IMAGE_ID*/ index,
                             lpvoid_t image_source, lpdword_t image_len) {
  XELOGD("Stubbed");
  return X_ERROR_FUNCTION_FAILED;
}

dword_result_t XamGetDefaultSystemImage_entry(lpvoid_t image_source,
                                              lpdword_t image_len){  // Gets "Defaultsystemimage.png"
  return xeXGetDefaultImage(0, image_source, image_len);
}
DECLARE_XAM_EXPORT1(XamGetDefaultSystemImage, kMisc, kStub);

dword_result_t XamGetDefaultImage_entry(dword_t index, lpvoid_t image_source,
                                        lpdword_t image_len){
  return xeXGetDefaultImage(index, image_source, image_len);
}
DECLARE_XAM_EXPORT1(XamGetDefaultImage, kMisc, kStub);

}  // namespace xam
}  // namespace kernel
}  // namespace xe