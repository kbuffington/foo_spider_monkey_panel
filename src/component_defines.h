#pragma once

#define JSP_NAME "JScript Panel"
#define JSP_WINDOW_CLASS_NAME "foo_jscript_panel_class"
#define JSP_DLL_NAME "foo_jscript_panel.dll"

// Remember to bump utils.version too
#define JSP_VERSION_NUMBER "2.1.3"
#define JSP_VERSION_TEST ""

#ifdef _DEBUG
#	define JSP_VERSION_DEBUG_SUFFIX " (Debug)"
#else
#	define JSP_VERSION_DEBUG_SUFFIX ""
#endif

#define JSP_VERSION JSP_VERSION_NUMBER JSP_VERSION_TEST JSP_VERSION_DEBUG_SUFFIX

constexpr GUID g_guid_jsp_ui_pref = { 0x19681d48, 0xd90e, 0x4cb6,{ 0xbb, 0x6, 0x69, 0x5f, 0x42, 0x57, 0xbe, 0x23 } };
constexpr GUID g_guid_jsp_window_cui = { 0x19646267, 0x86fc, 0x4676,{ 0xa9, 0x8a, 0x49, 0x27, 0x38, 0x52, 0xb0, 0x88 } };
constexpr GUID g_guid_jsp_window_dui = { 0xdaa5f4e7, 0x177d, 0x4489,{ 0x9a, 0xd9, 0x9f, 0x3a, 0x8e, 0xfa, 0x96, 0xab } };
constexpr GUID g_guid_jsp_prop_sets = { 0x7f6b71a1, 0x4f1a, 0x437d,{ 0x84, 0xcc, 0xd1, 0xad, 0xd6, 0x7a, 0xd9, 0x62 } };
constexpr GUID g_guid_jsp_metadb_index = { 0x835f0b63, 0xd96c, 0x447b,{ 0x9c, 0xcb, 0x71, 0x4f, 0xa8, 0x30, 0x49, 0x11 } };
constexpr GUID g_guid_jsp_mainmenu_group_id = { 0x7b23ce27, 0x7d37, 0x4a1f,{ 0x80, 0x5b, 0xe5, 0x89, 0x19, 0x5b, 0xbd, 0xd0 } };
constexpr GUID g_guid_jsp_menu_one = { 0xf56e5f2d, 0xf1a1, 0x4f54,{ 0x97, 0xf5, 0xe7, 0xc4, 0xbe, 0x47, 0x1a, 0xb3 } };
constexpr GUID g_guid_jsp_menu_two = { 0xc3bda2f5, 0xf011, 0x4f54,{ 0x99, 0xa, 0x77, 0xf9, 0xef, 0x27, 0xf1, 0xb } };
constexpr GUID g_guid_jsp_menu_three = { 0x9c610f78, 0x2eb7, 0x43b6,{ 0x89, 0x6d, 0x86, 0x9b, 0xd4, 0xec, 0xb9, 0xeb } };
constexpr GUID g_guid_jsp_menu_four = { 0x6123f3ee, 0xeb4f, 0x4170,{ 0x80, 0x49, 0x15, 0x18, 0xe6, 0xad, 0x8a, 0x62 } };
constexpr GUID g_guid_jsp_menu_five = { 0x59f48bd1, 0xa6a1, 0x4a8a,{ 0x93, 0x3c, 0x39, 0xcf, 0xef, 0x8, 0xee, 0x52 } };
constexpr GUID g_guid_jsp_menu_six = { 0x365377e0, 0x7a4b, 0x430f,{ 0x88, 0x3, 0xfd, 0xaf, 0x22, 0x60, 0xf6, 0xe1 } };
constexpr GUID g_guid_jsp_menu_seven = { 0x5e26ac8d, 0x38, 0x4743,{ 0x90, 0x72, 0x2c, 0x26, 0x56, 0x96, 0xcf, 0x14 } };
constexpr GUID g_guid_jsp_menu_eight = { 0x6b00b1c4, 0xa55, 0x46d8,{ 0x83, 0x2f, 0xfd, 0xd5, 0xd9, 0xde, 0x69, 0x43 } };
constexpr GUID g_guid_jsp_menu_nine = { 0xca840da4, 0xfc99, 0x44bc,{ 0x90, 0x71, 0xd2, 0xb0, 0x2b, 0x26, 0xd4, 0x35 } };
constexpr GUID g_guid_jsp_menu_ten = { 0xab05eee8, 0xbadc, 0x49ba,{ 0x80, 0x27, 0x84, 0x72, 0xa8, 0xbd, 0x49, 0xdb } };
