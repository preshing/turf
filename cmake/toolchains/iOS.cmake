set(CMAKE_SYSTEM_NAME Generic)
set(UNIX True)
set(APPLE True)
set(IOS True)

execute_process(COMMAND xcode-select -print-path OUTPUT_VARIABLE XCODE_SELECT OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT EXISTS ${XCODE_SELECT})
    message(FATAL_ERROR "xcode-select not found")
endif()

# Setting CMAKE_OSX_SYSROOT seems to do almost everything:
set(CMAKE_OSX_SYSROOT "${XCODE_SELECT}/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk")

# CMAKE_MACOSX_BUNDLE is needed to avoid the error "target specifies product type 'com.apple.product-type.tool',
# but there's no such product type for the 'iphoneos' platform" during CMake's compiler checks:
set(CMAKE_MACOSX_BUNDLE YES)

# CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED is needed to avoid the error "Code signing is required
# for product type 'Application' in SDK 'iOS 10.0'" during CMake's compiler checks:
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
